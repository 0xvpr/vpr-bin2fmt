/**
 * Creator:    VPR
 * Created:    February 24th, 2022
 * Updated:    June 5th, 2022
 *
 * Description:
 *     Takes in a given file and returns a specified format of either
 *     a c-style string array or a raw backslash escaped hex string.
**/

#include <stdlib.h>
#include <stdio.h>

static FILE* out_stream;
static char* program;
static char* target;
static int format;

static inline void __attribute((noreturn)) __usage_error(int error_code, char* restrict message)
{
    fprintf(stderr,
        "Error %d: %s.\n" // error_code, message
        "\n"
        "Usage:\n"
        "  %s <path/to/executable> [ optional switches ]\n" // program
        "\n"
        "positional arguments:\n"
        "  path/to/executable\n"
        "\n"
        "optional switches:\n"
        "  -c,-C\t\toutput as c-style char array\n"
        "  -s,-S\t\toutput as double quoted string\n"
        "example :\n"
        "  %s path/to/executable\n" // program
        ,error_code, message, program, program
    );

    exit(error_code);
}

static __attribute__((hot)) void handle_command_line(int argc, char** argv)
{
    if (argc < 2)
    {
        __usage_error(1, "Missing positional arguments");
    }

    for (int i = 2; i < argc; i++)
    {
        switch (argv[i][0])
        {
            case '-':
            {
                switch (argv[i][1])
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
                        __usage_error(2, "Unknown option supplied");
                    }
                }
            }
        }
    }

    if (!format)
    {
        format = 'c';
    }

    if (!out_stream)
    {
        out_stream = stdout;
    }
}

static __attribute__((hot)) void verify_target_exists(void)
{
    FILE* fp;
    if (!(fp = fopen(target, "rb")))
    {
        char buffer[256] = { 0 };
        snprintf(buffer, sizeof(buffer), "Failed to find executable '%s'", target);
        __usage_error(3, buffer);
    }
    fclose(fp);
}

static __attribute((hot)) char* find_last(char c, char* file_name)
{
    char* return_addr = file_name;

    int position = 0;
    for (int i = 0; *(return_addr + i) != '\0'; ++i)
    {
        if (*(return_addr + i) == c)
        {
            position = i + 1;
        }
    }

    return return_addr + position;
}

static __attribute__((hot)) void extract_file_name(void* restrict dst, void* restrict src, size_t size)
{
    src = find_last('/', src);

    for (size_t i = 0; i < size; ++i)
    {
        char new_char = *((char *)src + i);
        if ( (new_char >= 'A' && new_char <= 'Z') ||
             (new_char >= 'a' && new_char <= 'z') ||
             (new_char >= '0' && new_char <= '9')
           )
        {
            *((char *)dst + i) = new_char | 0x20;
        }
        else if (new_char == '\0')
        {
            return;
        }
        else
        {
            *((char *)dst + i) = '_';
        }
    }
}

int main(int argc, char** argv)
{
    FILE*  fp;

    program = argv[0];
    handle_command_line(argc, argv);

    for (int i = 1; i < argc; ++i)
    {
        target = argv[i];
        if (target[0] == '-')
        {
            continue;
        }

        if (!(fp = fopen(target, "rb")))
        {
            fprintf(stderr, "Failed to open %s.\n", target);
            continue;
        }

        verify_target_exists();
        switch (format)
        {
            case 'c':
            {
                long int file_size = 0;
                fseek(fp, 0, SEEK_END);
                file_size = ftell(fp);
                fseek(fp, 0, SEEK_SET);
                
                char file_name_buffer[256] = { 0 };
                extract_file_name(file_name_buffer, target, sizeof(file_name_buffer));

                fprintf(out_stream, "unsigned char %s[%ld] = {", file_name_buffer, file_size);
                for (int position = 0, c = 0; (c = fgetc(fp)) != EOF; ++position)
                {
                    fprintf(out_stream, "%s0x%02X%c", (position % 10 == 0 && position < file_size - 1 ? "\n    " : " "), c, (position < file_size - 1? ',' : '\0'));
                }
                fprintf(out_stream, "\n};\n");
                fflush(out_stream);
                break;
            }
            case 's':
            {
                fputc('"', out_stream);
                for (int c = 0; (c = fgetc(fp)) != EOF;)
                {
                    fprintf(out_stream, "\\x%02X", c);
                }
                fputc('"', out_stream);
                fflush(out_stream);
                break;
            }
        }

        // Cleanup
        fclose(fp);
    }

    return 0;
}
