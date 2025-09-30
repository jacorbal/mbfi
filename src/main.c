/**
 * @file main.c
 *
 * @brief Minimal Brainfuck interpreter
 *
 * @version 1.0.3
 * @author J. A. Corbal <jacorbal@gmail.com>
 * @copyright Copyright (c) 2018-2025, J. A. Corbal.
              Licensed under the MIT license; read the file for more info.
 */
/*
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <errno.h>  /* errno */
#include <stdio.h>  /* FILE, perror, stdout */
#include <stdlib.h> /* malloc, free, NULL */

#include <mbfi.h>


#define PROG_DESC "Minimal Brainfuck interpreter"
#define PROG_NAME "mbfi"
#define PROG_VERSION "1.0.3"


/**
 * @brief Read a file and store its contents in a buffer
 *
 * Opens the file specified by @p path in binary read mode and reads its
 * entire contents into a newly-allocated, null-terminated buffer which
 * is returned via @p buffer.  The implementation obtains the file size
 * by seeking to the end and using @a ftell, allocates exactly (@e size
 * + 1) bytes, rewinds to the start, and reads @e size bytes with
 * a single @a fread call.
 *
 * @param path   Path where to open the file
 * @param buffer Buffer where to store file contents
 *
 * @return 0 if success and @p buffer will point to a @a malloc'd,
 *         null‑terminated; on failure returns a non‑zero value and
 *         leaves @e errno set to indicate the error
 *
 * @note Assumes the target file is seekable and regular: it is not
 *       suitable for non-seekable streams (pipes, sockets, or some procfs
 *       entries)
 * @note It can overflow or misbehave for very large files on platforms
 *       where ftell returns a 32-bit long
 */
static int s_read_file_to_buffer(const char *path, char **buffer)
{
    FILE *file;
    long pos;
    size_t size;
    char *buf;
    size_t nread;
    int rc = 1;

    if (buffer == NULL || path == NULL) {
        errno = EINVAL;
        return 1;
    }

    file = fopen(path, "rb");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }

    if (fseek(file, 0L, SEEK_END) != 0) {
        fclose(file);
        perror("fseek");
        return 1;
    }

    pos = ftell(file);
    if (pos < 0) {
        fclose(file);
        perror("ftell");
        return 1;
    }

    size = (size_t)pos;

    if (fseek(file, 0L, SEEK_SET) != 0) {
        fclose(file);
        perror("fseek");
        return 1;
    }

    buf = (char *) malloc(size + 1);
    if (buf == NULL) {
        fclose(file);
        perror("malloc");
        return 1;
    }

    if (size > 0) {
        nread = fread(buf, 1, size, file);
        if (nread != size) {
            free(buf);
            fclose(file);
            perror("fread");
            return 1;
        }
    } else {
        /* Empty file: still provide empty string */
    }

    buf[size] = '\0';
    *buffer = buf;
    rc = 0;

    if (fclose(file) != 0) {
        /* Non-fatal: report but keep loaded buffer */
        perror("fclose");
    }

    return rc;
}


/**
 * @brief Read a stream and store its contents in a null-terminated
 *        buffer
 *
 * Reads all data available from the provided file stream (starting at
 * the current file position) into a dynamically-allocated buffer and
 * returns it via @p out.
 *
 * @param fp  Open FILE* stream to read from (must not be @c NULL)
 * @param out Pointer to a char* that will receive the allocated buffer
 *
 * @return 0 on success, and @p *out will point to a null-terminated
 *         buffer; on error returns -1 and leaves @a errno set to
 *         indicate the failure
 *
 * @note The buffer is null-terminated and must be freed by the caller
 */
static int s_read_stream_to_buffer(FILE *fp, char **out)
{
    size_t cap = 4096;
    size_t len = 0;
    char *buf = malloc(cap);
    char *tmp;

    if (buf == NULL) {
        return -1;
    }

    while (1) {
        size_t n = fread(buf + len, 1, cap - len, fp);
        len += n;
        if (feof(fp)) {
            break;
        }
        if (ferror(fp)) {
            free(buf);
            errno = EIO;
            return -1;
        }
        cap *= 2;
        tmp = realloc(buf, cap);
        if (tmp == NULL) {
            free(buf);
            perror("realloc");
            return -1;
        }
        buf = tmp;
    }

    /* Ensure terminator */
    tmp = realloc(buf, len + 1);
    if (tmp == NULL) {
        free(buf);
        perror("realloc");
        return -1;
    }
    buf = tmp;
    buf[len] = '\0';

    *out = buf;
    return 0;
}

/**
 * @brief Shows help information
 *
 * @param fp File pointer where to print help
 */
static void s_show_help(FILE *fp)
{
    fprintf(fp, "%s -- version %s\n", PROG_DESC, PROG_VERSION);
    fprintf(fp, "Usage: %s <source.bf>\n", PROG_NAME);
    fprintf(fp, "Use '-' to read from 'stdin'\n");
}


/* Main entry */
int main(int argc, char **argv)
{
    char * buffer = NULL;
    int retval;

    if (argc < 2) {
        s_show_help(stderr);
        return -1;
    }

    /* Display help to 'stdout' on '-h' (don't want to use 'strcmp' here) */
    if (argv[1][0] == '-' && argv[1][1] == 'h' && argv[1][2] == '\0') {
        s_show_help(stdout);
        return 0;
    }

    if (argv[1][0] == '-' && argv[1][1] == '\0') {  /* Read from 'stdin' */
        if (s_read_stream_to_buffer(stdin, &buffer) != 0) {
            perror("Error while reading stdin");
            return -2;
        }
    } else {                                        /* Read from file */
        if (s_read_file_to_buffer(argv[1], &buffer) != 0) {
            perror("Error while loading file");
            return -2;
        }
    }

    retval = mbfi_eval(buffer, stdout);
    free(buffer);

    return retval;
}
