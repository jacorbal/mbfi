/**
 * @file mbfread.c
 *
 * @brief Read file or from stdin implementation
 *
 * @version 1.0.4
 * @author J. A. Corbal <jacorbal@gmail.com>
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

/* System includes */
#include <errno.h>  /* errno */
#include <stdio.h>  /* FILE, perror, stdin, stdout */
#include <stdlib.h> /* malloc, free, NULL */

/* Local includes */
#include <mbfread.h>


/* Read a file and store its contents in a null-terminated buffer */
int mbfi_read_file_to_buffer(const char *path, char **buffer)
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


/* Read a stream and store its contents in a null-terminated buffer */
int mbfi_read_stream_to_buffer(FILE *fp, char **buffer)
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

    *buffer = buf;
    return 0;
}
