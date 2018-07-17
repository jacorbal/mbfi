/**
 * @file mbfi.c
 *
 * @brief Implements Brainfuck interpreter
 *
 * @version 1.0.1
 *
 * @author J. A. Corbal <jacorbal@gmail.com>
 *
 * @note Minimal Brainfuck interpreter
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

#include <stdio.h>  /* FILE */
#include <stdlib.h> /* calloc, free */

#include <mbfi.h>


/* Evaluates Brainfuck code and prints the output to a file stream */
void evaluate_bf(char *code, FILE *output)
{
    char *buffer = calloc(MAX_BUFFER, sizeof(char));
    char *bufferptr = buffer;

    for (; *code != 0; ++code) {
        switch (*code) {
            case BF_ADD:
                (*bufferptr)++;
                break;

            case BF_SUB:
                (*bufferptr)--;
                break;

            case BF_ALE:
                bufferptr--;
                break;

            case BF_ARI:
                bufferptr++;
                break;

            case BF_BLE:
                if (*bufferptr == 0) {
                    code++;
                    for (int i = 1; i > 0; ++code) {
                        if (*code == '[') {
                            i++;
                        } else if (*code == ']') {
                            i--;
                        }
                    }
                    code--;
                }
                break;

            case BF_BRI:
                if (*bufferptr != 0) {
                    code--;
                    for (int i = 1; i != 0; --code) {
                        if (*code == '[') {
                            i--;
                        } else if (*code == ']') {
                            i++;
                        }
                    }
                    code++;
                }
                break;

            case BF_PRINT_CH:
                fprintf(output, "%c", *bufferptr);
                fflush(output);
                break;

            case BF_GET_CH:
                *bufferptr = getchar();
                break;

            default:
                /* Ignore any other characters */
                break;
        }
    }

    free(buffer);
}

