/**
 * @file mbfi.c
 *
 * @brief Implements Brainfuck interpreter
 *
 * @version 1.0.2
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

/* System includes */
#include <stdio.h>  /* FILE */
#include <stdlib.h> /* calloc, free */
#include <string.h> /* strlen */

/* Local includes */
#include <mbfi.h>


/**
 * @brief Build a jump table for Brainfuck loops in a source string
 *
 * @param code           Null-terminated Brainfuck source string
 * @param jump_table_out Pointer to receive newly allocated jump table
 * @param length_out     Pointer to receive length of the source
 *
 * @pre code != NULL
 * @pre jump_table_out != NULL
 * @pre length_out != NULL
 *
 * @return 0 on success, or -1 otherwise
 *
 * @note Complexity: @e O(n), where @e n is the source length
 */
static int s_build_jump_table_raw(const char *code,
        int **jump_table_out, int *length_out)
{
    int len;
    int *table;
    int *stack;
    int sp;
    int i;

    if (!code || !jump_table_out || !length_out) {
        return -1;
    }

    len = (int) strlen(code);
    table = calloc((size_t) len, sizeof(int));
    if (!table) return -1;

    stack = calloc((size_t) len, sizeof(int));
    if (!stack) {
        free(table);
        return -1;
    }

    sp = 0;
    for (i = 0; i < len; ++i) {
        if ((int) code[i] == BF_BLE) {
            stack[sp++] = i;
        } else if ((int) code[i] == BF_BRI) {
            int j;
            if (sp == 0) {
                free(stack);
                free(table);
                return -1;  /* unmatched ']' */
            }
            j = stack[--sp];
            table[i] = j;
            table[j] = i;
        }
    }

    if (sp != 0) {  /* unmatched '[' */
        free(stack);
        free(table);
        return -1;
    }

    free(stack);
    *jump_table_out = table;
    *length_out = len;

    return 0;    
}


/* Evaluates Brainfuck code and prints the output to a file stream */
int mbfi_eval(char *code, FILE *output)
{
    unsigned char *tape;
    unsigned char *ptr;
    unsigned char *tape_end;
    int *jump_table;
    int code_len;
    int ip;
    int error;
    int c;
    int n;

    if (code == NULL || output == NULL) {
        return 1;
    }

    tape = calloc((size_t) MAX_BUFFER, sizeof(unsigned char));
    if (!tape) {
        return 2;
    }

    jump_table = NULL;
    code_len = 0;
    if (s_build_jump_table_raw(code, &jump_table, &code_len) != 0) {
        /* Unmatched brackets or memory allocation failure */
        free(tape);
        return 3;
    }

    ptr = tape;
    tape_end = tape + MAX_BUFFER;
    error = 0;

    for (ip = 0; ip < code_len && !error; ++ip) {
        switch (code[ip]) {
            case BF_ADD:
                (*ptr)++;
                break;

            case BF_SUB:
                (*ptr)--;
                break;

            case BF_ALE:
                if (ptr == tape) {
                    /* tape underflow */
                    error = 4;
                } else {
                    ptr--;
                }
                break;

            case BF_ARI:
                if (ptr + 1 == tape_end) {
                    /* tape overflow */
                    error = 5;
                } else {
                    ptr++;
                }
                break;

            case BF_BLE:
                if (*ptr == 0) {
                    ip = jump_table[ip];
                }
                break;

            case BF_BRI:
                if (*ptr != 0) {
                    ip = jump_table[ip];
                }
                break;

            case BF_PRINT_CH:
                n = fprintf(output, "%c", (unsigned char) *ptr);
                if (n < 0) {
                    error = 6;
                }
                /* don't fflush here */
                break;

            case BF_GET_CH:
                c = getchar();
                if (c == EOF) {
                    *ptr = 0;
                } else {
                    *ptr = (unsigned char) c;
                }
                break;

            default:
                /* ignore other characters (comments, whitespace) */
                break;
        }
    }

    if (!error) {
        if (fflush(output) != 0) {
            error = 7;
        }
    }

    free(jump_table);
    free(tape);

    return error;
}
