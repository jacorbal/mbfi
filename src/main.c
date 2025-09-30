/**
 * @file main.c
 *
 * @brief Minimal Brainfuck interpreter
 *
 * @version 1.0.2
 *
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


#define PROG_NAME "mbfi"


/**
 * @brief Reads a file and store its contents in a buffer
 *
 * @param buffer Buffer where to store file contents
 * @param path   Path where to open the file
 *
 * @return 0 if success, or otherwise
 */
static int read_file(char **buffer, const char *path)
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

    buf = (char *)malloc(size + 1);
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
 * @brief Shows help information
 *
 * @param fp File pointer where to print help
 */
static void show_help(FILE *fp)
{
    fprintf(fp, "Usage: %s <source.bf>\n", PROG_NAME);
}


/* Main entry */
int main(int argc, char **argv)
{
    char * buffer;
    int retval;

    if (argc < 2) {
        show_help(stderr);
        return -1;
    }

    buffer = NULL;
    if (read_file(&buffer, argv[1]) != 0) {
        perror("Error while loading file");
        return -2;
    }

    retval = mbfi_eval(buffer, stdout);
    free(buffer);

    return retval;
}
