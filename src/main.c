/**
 * Creator:    VPR
 * Created:    February 24th, 2022
 * Updated:    June 18th, 2023
 *
 * Description:
 *     Takes in a given file and returns a specified format of either
 *     a c-style string array or a raw backslash escaped hex string.
**/

#include <stdlib.h>
#include <stdio.h>

static FILE* out_stream;
static FILE* in_stream;
static char* program;
static char* target;
static int format;

static __attribute__((noreturn)) 
void __usage_error(int error_code, char* restrict message)
{
    fprintf(stderr,
        "Error %d: %s.\n" // error_code, message
        "\n"
        "Usage:\n"
        "  %s <path/to/file> [ optional switches ]\n" // program
        "\n"
        "positional arguments:\n"
        "  path/to/file\n"
        "\n"
        "optional switches:\n"
        "  -c,-C\t\toutput as c-style char array\n"
        "  -s,-S\t\toutput as double quoted string\n"
        "example :\n"
        "  %s path/to/file-1 path/to/file2 -s\n" // program
        ,error_code, message, program, program
    );

    exit(error_code);
}

static __attribute((hot))
const char* find_last(char c, const char* restrict file_name)
{
    const char* return_addr = file_name;

    int position = 0;
    for (int i = 0; return_addr[i] != '\0'; ++i)
    {
        if (return_addr[i] == c)
        {
            position = i + 1;
        }
    }

    return return_addr + position;
}

static __attribute__((hot))
void extract_file_name(char* restrict dst, const char* restrict src, size_t size)
{
    src = find_last('/', src);

    for (size_t i = 0; i < size; ++i)
    {
        char new_char = src[i];
        if ( (new_char >= 'A' && new_char <= 'Z') ||
             (new_char >= 'a' && new_char <= 'z') ||
             (new_char >= '0' && new_char <= '9')
           )
        {
            dst[i] = new_char | 0x20;
        }
        else if (new_char == '\0')
        {
            return;
        }
        else
        {
            dst[i] = '_';
        }
    }
}

int main(int argc, char** argv)
{
    program = argv[0];
    format = 'c';
    out_stream = stdout;

    if (argc < 2)
    {
        __usage_error(-1, "No arguments supplied.");
    }

    for (int i = 1; i < argc; ++i)
    {
        target = argv[i];
        if (target[0] == '-' && target[1])
        {
            switch (target[1])
            {
                case 'C':
                case 'c':
                {
                    format = 'c';
                    break;
                }
                case 'S':
                case 's':
                {
                    format = 's';
                    break;
                }
                default:
                {
                    fprintf(stderr, "Unknown option supplied: %c\n\n", target[1]);
                    break;
                }
            }
            continue;
        }

        if (!(in_stream = fopen(target, "rb")))
        {
            fprintf(stderr, "Failed to find file: '%s'\n\n", target);
            continue;
        }

        switch (format)
        {
            case 'c':
            {
                long int file_size = 0;
                fseek(in_stream, 0, SEEK_END);
                file_size = ftell(in_stream);
                fseek(in_stream, 0, SEEK_SET);
                
                char file_name_buffer[256] = { 0 };
                extract_file_name(file_name_buffer, target, sizeof(file_name_buffer));

                fprintf(out_stream, "unsigned char %s[%ld] = {", file_name_buffer, file_size);
                for (int position = 0, c = 0; (c = fgetc(in_stream)) != EOF; ++position)
                {
                    fprintf(out_stream, "%s0x%02X%c", (position % 10 == 0 && position < file_size - 1 ? "\n    " : " "), c, (position < file_size - 1? ',' : '\0'));
                }
                fprintf(out_stream, "\n};\n\n");
                fflush(out_stream);
                break;
            }
            case 's':
            {
                fputc('"', out_stream);
                for (int c = 0; (c = fgetc(in_stream)) != EOF;)
                {
                    fprintf(out_stream, "\\x%02X", c);
                }
                fprintf(out_stream, "%s", "\"\n\n");
                fflush(out_stream);
                break;
            }
            default:
            {
                break;
            }
        }

        fclose(in_stream);
    }

    return 0;
}
