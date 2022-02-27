#!/bin/sh

# Creator:    VPR
# Created:    February 24th, 2022
# Updated:    February 24th, 2022

# Description:
#     Tests my terrible code

set -e pipefail
set -e errexit
set -e nounset
set -e xtrace

__target="bin/vpr-bin2fmt"
__test_file="file.txt"

if ! [ -f ${__target} ]:
then
    make
fi

if ! [ -f ${__test_file} ]:
then
    make file
fi

exec ${__target} ${__test_file}
