/**
 * @file main.c
 *
 * @brief Minimal Brainfuck interpreter
 *
 * @version 1.0.1
 *
 * @author J. A. Corbal <jacorbal@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>

#include <mbfi.h>


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
    FILE * file;
    size_t size;

    file = fopen(path, "rb");
    if (file == NULL) {
        return 1;
    }

    fseek(file, 0L, SEEK_END);
    size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    *buffer = malloc(size + 1);
    fread(*buffer, 1, size, file);
    (*buffer)[size] = 0;

    fclose(file);

    return 0;
}


/**
 * @brief Shows help information
 *
 * @param fp File pointer where to print help
 */
static void show_help(FILE *fp)
{
    fprintf(fp, "Usage: mbfc <source.bf>\n");
}


/* Main entry */
int main(int argc, char **argv)
{
    char * buffer;

    if (argc < 2) {
        show_help(stderr);
        return 1;
    }

    if (read_file(&buffer, argv[1]) == 0) {
        evaluate_bf(buffer);
    } else {
        perror("Error while loading file");
        return 2;
    }

    return 0;
}

