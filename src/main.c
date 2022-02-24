/**
 * Creator:    VPR
 * Created:    February 24th, 2022
 * Updated:    February 24th, 2022
 *
 * Description:
 *     Takes in a given file and returns a specified format of either
 *     a c-style string array or a raw backslash escaped hex string.
**/

/*#include "parser.h"*/ // TODO: create command line parser
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>

#ifndef BUFSIZ
#define BUFSIZ 8192
#endif /* BUFSIZ */

#define SIZE_CONSTANT 4

char* convert_to_rawstring(char* buffer, size_t size)
{
    char* raw_string = (char *)calloc(sizeof(uint8_t), size * SIZE_CONSTANT * sizeof(uint8_t));
    for (size_t i = 0, j = 0; i < size; i++, j += SIZE_CONSTANT)
    {
        snprintf(raw_string + j, SIZE_CONSTANT + 1, "\\x%02x", buffer[i]);
    }
    raw_string[size * SIZE_CONSTANT] = '\0';

    return raw_string;
}

void print_formatted_string(int format, char* string)
{
    switch (format)
    {
        case 0:
        {
            putc('\"', stdout);
            fprintf(stdout, "%s", string);
            putc('\"', stdout);
            break;
        }
        default:
            break;
    }
}

int main(int argc, char** argv)
{
    char   file_name[256];
    char   buffer[1024];
    size_t buffer_size;
    char*  raw_string;
    int    format;
    FILE*  fp;

    // Handle command-line
    if (argc < 2)
    {
        perror("FATAL"); // TODO: replace with custom message
        return -1;
    }

    // Get file_name
    snprintf(file_name, sizeof(file_name), "%s", argv[1]);
#ifndef NDEBUG
    puts(file_name);
#endif /* NDEBUG */

    // Attempt to open file
    if (!(fp = fopen(file_name, "rb")))
    {
        perror("FATAL"); // TODO: replace with custom message
        return -1;
    }

    // Read contents of file into buffer
    memset(buffer, 0, sizeof(buffer));
    if (!(fread(buffer, sizeof(uint8_t), sizeof(buffer), fp)))
    {
        perror("FATAL"); // TODO: replace with custom message
        return -1;
    }
#ifndef NDEBUG
    puts(buffer);
#endif /* NDEBUG */

    // Get file-size
    fseek(fp, 0, SEEK_END);
    buffer_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Format contents of buffer into rawstring
    raw_string = convert_to_rawstring(buffer, buffer_size);

    // Display
    format = 0;
    print_formatted_string(format, raw_string);

    // Cleanup
    /*free(raw_string);*/
    fclose(fp);

    return 0;
}
