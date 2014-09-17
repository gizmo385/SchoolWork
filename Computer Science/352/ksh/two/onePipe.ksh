#!/bin/ksh

current_file_name=`basename $0`

# Verify that the number of arguments i valid
if [ $(( $# % 2 )) -eq 1 ] ; then
    echo Arguments must be paired >&2
    echo Usage: $current_file_name cmd1 cmd2 [cmd1 cmd2] ... >&2
    exit 1
fi

starting_args=$#

for (( i = 1; i <= $starting_args; i += 2 )) do
    first="${1}"
    second="${2}"
    skip="false"

    # Construct the piped command
    constructed_command="${first} | ${second}"
    echo Executing: $constructed_command

    # Execute the command and print a newline
    #eval "${constructed_command} 2> /dev/null"
    ($first | $second) 2>".out.err" 1>".out.out"
    cat .out.out
    echo

    # Create some temporary files and write stdout and stderr to files
    ($first) 2>".first.err" 1>".first.out"

    # Print stderr if it is present, or say nothing was printed
    if [[ -s ".first.err" ]] ; then
        echo ${first} produced the following error output:
        cat ".first.err"
        skip="true"
    else
        echo ${first} produced nothing on standard error
    fi

    # Create some temporary files and write stdout and stderr to files
    ($first | $second) 2>".second.err" 1>".second.out"
    exit_status=$?

    # Print stderr if it is present, or say nothing was printed
    if [[ -s ".second.err" && "$skip" != "true" ]] ; then
        echo ${second} produced the following error output:
        cat ".second.err"
    else
        echo ${second} produced nothing on standard error
    fi

    # Remove temporary files
    rm .second.err .second.out .first.err .first.out

    echo Exit status of ${second}: $exit_status
    echo
    echo --------------------
    echo

    shift
    shift
done
