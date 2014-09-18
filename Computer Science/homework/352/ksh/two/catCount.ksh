#!/bin/ksh

# Options
w_enabled="false"
c_enabled="false"

# Iterate over all of the command line arguments
starting_args=$#

# Terminal exit status to return
exit_status=0

for (( i=0; i < $starting_args; i += 1 )) ; do
    # Check if the first or second arguments are '-w' or '-c'
    if (( i == 0 )) || (( i == 1 )) ; then
        if [ "$1" = "-w" ]; then
            w_enabled="true"
            shift
            continue
        fi

        if [ "$1" = "-c" ]; then
            c_enabled="true"
            shift
            continue
        fi
    fi

    # Check that the file exists
    if [ -f "${1}" ] ; then
        cat_output=$(cat ${1})

        # Check enabled options
        if "$c_enabled" == "true" ; then
            echo -e "${cat_output}"

            if "$w_enabled" == "true" ; then
                echo -n "${cat_output}" | wc ${1}
            fi
        else
            if "$w_enabled" == "true" ; then
                echo -n "${cat_output}" | wc ${1}
            fi
        fi
    else
        echo -e File \'${1}\' not found >&2
        exit_status=1
    fi

    if (( $i < $starting_args )) ; then
        shift
    fi

done

exit ${exit_status}
