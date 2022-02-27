#include "parser.h"
#include "util.h"

int parse_command_line(int argc, char** argv)
{
    int operation = OPTION_RAWSTRING;

    if (argc < 2)
    {
        return -1;
    }

    // Parse option flags
    for (int i = argc-1; i > 0; i--)
    {
        if (argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
                case 'r':
                case 'R':
                {
                    operation = OPTION_RAWSTRING;
                    break;
                }
                case 'c':
                case 'C':
                {
                    operation = OPTION_C_STRING;
                    break;
                }
                default:
                    __usage_error(2, "unkown option", argv[0]);
                    break;
            }
        }
    }

    return operation;
}
