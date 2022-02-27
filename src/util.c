#include "util.h"

#include <stdlib.h>

_Noreturn void __usage_error(int error, char* msg, char* argv_0)
{
    fprintf(stderr,
            "Error message: %s.\n" // msg
            "\n"
            "usage: %s path/to/file(s) [ <optional arguments> ]\n" // argv_0
            "\n"
            "optional arguments:\n"
            "  -r, -R\t\tOutput as raw string\n"
            "  -c, -C\t\tOutput as C-style string\n"
            "\n"
            "examples:\n"
            "  %s -r file1.txt file2.txt\n" // argv_0
            "  %s file1.txt file2.txt -C\n" // argv_0
            , msg
            , argv_0
            , argv_0
            , argv_0
    );

    exit(error);
}
