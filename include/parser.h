#ifndef PARSER_HEADER
#define PARSER_HEADER

#include "stddef.h"

#ifndef OPTION_RAWSTRING
#define OPTION_RAWSTRING 0x01 
#endif /* OPTION_RAWSTRING */

#ifndef OPTION_C_STRING
#define OPTION_C_STRING 0x02
#endif /* OPTION_C_STRING */

/**
 * Determines the operation that should be performed on a given  file.
 *
 * @param:     argc         number of arguments.
 * @param:     argv         array of arguments.
 *
 * @return:    operation to be performed.
**/
int parse_command_line(int, char**);

#endif /* PARSER_HEADER */
