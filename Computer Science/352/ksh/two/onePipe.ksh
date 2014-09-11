#!/bin/ksh

if [ $(( $# % 2 )) -eq 1 ] ; then
    echo "The number of arguments must be divisible by 2!" >&2
    exit 1
fi

for ((i = 0; i < $#; i += 2 )) do
    first=${1}
    second=${2}

    constructed_command="${first} | ${second}"
    echo $constructed_command

    shift 2
done
