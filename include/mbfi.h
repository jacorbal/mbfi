/**
 * @file mbfc.h
 *
 * @brief Brainfuck interpreter declaration
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

#ifndef MBFI_H
#define MBFI_H

/* System includes */
#include <stdio.h>  /* FILE */


#define MBFI_MAX_BUFFER 30000


/**
 * @enum commands
 *
 * @brief The eight Brainfuck commands that consist of a single
 *        character each
 */
enum {
    BF_ADD = '+',       /* add */
    BF_SUB = '-',       /* subtract */
    BF_ALE = '<',       /* arrow left */
    BF_ARI = '>',       /* arrow right */
    BF_BLE = '[',       /* bracket left */
    BF_BRI = ']',       /* bracket right */
    BF_PUTCH = '.',     /* print character */
    BF_GETCH = ','      /* read character */
};


/* Public interface */
/**
 * @brief Evaluates Brainfuck code to an output stream
 *
 * @param code   Input code to evaluate
 * @param output Output stream file pointer
 *
 * @return Status of the operation
 * @retval  0 success
 * @retval  1 invalid arguments
 * @retval  2 @a calloc failed
 * @retval  3 bracket table build error
 * @retval  4 tape underflow
 * @retval  5 tape overflow
 * @retval  6 write error (@a fputc)
 * @retval  7 @a fflush error
 */
int mbfi_eval(char *code, FILE *output);


#endif  /* ! MBFI_H */
