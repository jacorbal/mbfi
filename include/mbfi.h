/**
 * @file mbfc.h
 *
 * @brief Brainfuck interpreter declaration
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

#ifndef MBFI_H
#define MBFI_H


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


/* Public interface */
/**
 * @brief Evaluates Brainfuck code
 *
 * @param code   Input code to evaluate
 * @param output Output file pointer
 */
void evaluate_bf(char *code, FILE *output);


#endif /* ! MBFI_H */

