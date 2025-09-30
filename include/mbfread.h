/**
 * @file mbfread.h
 *
 * @brief Read file or from stdin functions declaration
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

#ifndef MBFREAD_H
#define MBFREAD_H

#include <stdio.h>  /* FILE */


/* Public interface */
/**
 * @brief Read a file and store its contents in a null-terminated buffer
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
 *       where @a ftell returns a 32-bit long
 * @note The buffer is null-terminated
 *
 * @warning The buffer must be freed by the caller
 */
int mbfi_read_file_to_buffer(const char *path, char **buffer);

/**
 * @brief Read a stream and store its contents in a null-terminated
 *        buffer
 *
 * Reads all data available from the provided file stream (starting at
 * the current file position) into a dynamically-allocated buffer and
 * returns it via @p buffer.
 *
 * @param fp     Open file stream to read from (must not be @c NULL)
 * @param buffer Pointer that will receive the allocated buffer
 *
 * @return 0 if success and @p buffer will point to a @a malloc'd,
 *         null‑terminated; on failure returns a non‑zero value and
 *         leaves @e errno set to indicate the error
 *
 * @note The buffer is null-terminated
 *
 * @warning The buffer must be freed by the caller
 */
int mbfi_read_stream_to_buffer(FILE *fp, char **buffer);


#endif  /* ! MBFREAD_H */
