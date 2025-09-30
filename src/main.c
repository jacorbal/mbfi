/**
 * @file main.c
 *
 * @brief Minimal Brainfuck interpreter
 *
 * @version 1.0.4
 * @author J. A. Corbal <jacorbal@gmail.com>
 * @copyright Copyright (c) 2018-2025, J. A. Corbal.
 *            Licensed under the MIT license; read `LICENSE` for more info.
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
#include <stdio.h>  /* FILE, perror, stdin, stdout */
#include <stdlib.h> /* free, NULL */

/* Project includes */
#include <mbfi.h>
#include <mbfread.h>


#define PROG_DESC "Minimal Brainfuck interpreter"
#define PROG_NAME "mbfi"
#define PROG_VERSION "1.0.4"



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
    char *buffer = NULL;
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
        if (mbfi_read_stream_to_buffer(stdin, &buffer) != 0) {
            perror("Error while reading stdin");
            return -2;
        }
    } else {                                        /* Read from file */
        if (mbfi_read_file_to_buffer(argv[1], &buffer) != 0) {
            perror("Error while loading file");
            return -2;
        }
    }

    retval = mbfi_eval(buffer, stdout);
    free(buffer);

    return retval;
}
