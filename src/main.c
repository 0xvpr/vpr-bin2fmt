/**
 * Creator:    VPR
 * Created:    February 24th, 2022
 * Updated:    February 27th, 2022
 *
 * Description:
 *     Takes in a given file and returns a specified format of either
 *     a c-style string array or a raw backslash escaped hex string.
**/

#include "parser.h"
#include "util.h"

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>

#ifndef BUFSIZ
#define BUFSIZ 8192
#endif /* BUFSIZ */

#define RAW_SIZE_CONSTANT   0x04
#define C_SIZE_CONSTANT     0x06

#define CLEAN_NAME(clean_name, file_name)               \
    memset(clean_name, 0, sizeof(clean_name));          \
    memcpy(clean_name, file_name, strlen(file_name));   \
    for (size_t i = 0; i < strlen(file_name); i++)      \
    {                                                   \
        if (!isalnum(clean_name[i]))                    \
        {                                               \
            clean_name[i] = '_';                        \
        }                                               \
    }

char* convert_to_rawstring(char* buffer, size_t size)
{
    char* raw_string = (char *)calloc(sizeof(uint8_t), size * RAW_SIZE_CONSTANT * sizeof(uint8_t));
    for (size_t i = 0, j = 0; i < size; i++, j += RAW_SIZE_CONSTANT)
    {
        snprintf(raw_string + j, RAW_SIZE_CONSTANT + 1, "\\x%02X", buffer[i]);
    }
    raw_string[size * RAW_SIZE_CONSTANT] = '\0';

    return raw_string;
}

char* convert_to_c_style_string(char* buffer, size_t size)
{
    char* c_style_string = (char *)calloc(sizeof(uint8_t), size + size * C_SIZE_CONSTANT * sizeof(uint8_t));
    size_t last_chars;

    size_t i = 0, j = 0, breakline = 10;
    for (i = 0, j = 0; i < size; i++, j += C_SIZE_CONSTANT)
    {
        if (i % breakline == 0)
        {
            snprintf(c_style_string + j, C_SIZE_CONSTANT + 8, "\n    0x%02X, ", buffer[i]);
            j += 5;
        }
        else
        {
            snprintf(c_style_string + j, C_SIZE_CONSTANT + 3, "0x%02X, ", buffer[i]);
        }
    }

    last_chars = strlen(c_style_string)-2;
    memset(c_style_string + last_chars, 0, 2);

    return c_style_string;
}

void print_raw_string(char* const file_name, char* raw_string)
{
    char clean_name[256];
    CLEAN_NAME(clean_name, file_name);

    fprintf(stdout,
            "char* const %s = \"%s\"\n" // clean_name, raw_string
            , clean_name
            , raw_string
    );
}

void print_c_string(char* const file_name, char* c_style_string, size_t buffer_size)
{
    char clean_name[256];
    CLEAN_NAME(clean_name, file_name);

    fprintf(stdout,
            "unsigned char %s[%lu] = {" // clean_name, buffer_size
            "%s\n" // c_style_string
            "};\n"
            , clean_name
            , buffer_size
            , c_style_string
    );
}

int main(int argc, char** argv)
{
    char   file_name[256];
    char   buffer[BUFSIZ];
    size_t buffer_size;
    char*  raw_string;
    char*  c_style_string;
    int    format;
    FILE*  fp;

    // Handle command-line
    if ((format = parse_command_line(argc, argv)) == -1)
    {
        __usage_error(1, "no arguments supplied", argv[0]);
        return -1;
    }

    // Iterate arguments
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            continue;
        }

        // Get file_name
        memset(file_name, 0, sizeof(file_name));
        snprintf(file_name, sizeof(file_name), "%s", argv[i]);

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
            fclose(fp);
            return -1;
        }

        // Get file-size
        fseek(fp, 0, SEEK_END);
        buffer_size = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        // Display
        fprintf(stdout, "File contents of %s:\n", file_name);
        switch (format)
        {
            case OPTION_RAWSTRING:
            {
                raw_string = convert_to_rawstring(buffer, buffer_size);
                print_raw_string(file_name, raw_string);
                break;
            }
            case OPTION_C_STRING:
            {
                c_style_string = convert_to_c_style_string(buffer, buffer_size);
                print_c_string(file_name, c_style_string, buffer_size);
                break;
            }
            default:
                break;
        }

        // Cleanup
        fclose(fp);
    }

    return 0;
}
