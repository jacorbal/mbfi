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

#include <stdlib.h>
#include <stdio.h>
//#include <string.h>

#include <mbfi.h>


#define MAX_BUFFER 30000


/**
 * @enum commands
 *
 * @brief The eight language commands each consist of a single character
 */
enum {
    BF_ADD = '+',       /* add */
    BF_SUB = '-',       /* subtract */
    BF_ALE = '<',       /* arrow left */
    BF_ARI = '>',       /* arrow right */
    BF_BLE = '[',       /* bracket left */
    BF_BRI = ']',       /* bracket right */
    BF_PRINT_CH = '.',  /* print character */
    BF_GET_CH = ','     /* read character */
};


/**
 * @brief Evaluates Brainfuck code
 *
 * @param code Code to interpret
 */
void evaluate_bf(char *code)
{
    FILE *output;
    int i;
    char *buffer = calloc(MAX_BUFFER, sizeof(char));
    char *bufferptr = buffer;

//    memset(buffer, 0, MAX_BUFFER);

    output = stdout;

    for (; *code != 0; code++) {
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
                    for (i = 1; i > 0; code++) {
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
                    for (i = 1; i != 0; code--) {
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
    fclose(output);
}

