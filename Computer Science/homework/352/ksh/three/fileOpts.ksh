#!/bin/ksh

# Command line argument flags
c="disabled"
l="disabled"
f="disabled"
r="disabled"
r_arg="disabled"
p="disabled"
p_arg="disabled"

# Verify that there are command line options/arguments
if (( $# == 0 )); then
    exit 1
fi

# Parse command line arguments
while getopts "clfr:p#" optchar ; do
    case ${optchar} in

        c)  c="enabled"
            ;;

        l)  l="enabled"
            ;;

        f)  f="enabled"
            ;;

        r)  r="enabled"
            r_arg=${OPTARG}
            ;;

        p)  p="enabled"
            p_arg=${OPTARG}
            ;;

        \?) #echo "fileOpts.ksh: -h: unknown option" >&2
            echo "Usage: ksh fileOpts.ksh [-c] [-l] [-p #] [-f]  [-r filename] [ file ... ]" >&2
            #return 1
            ;;
    esac
done

# Shift the command line arguments away
shift $(( ${OPTIND} - 1 ))

# Verify that there are command line arguments after the options
if (( $# == 0 )); then
    exit 1
fi

# Debugging arguments
#echo c=$c
#echo l=$l
#echo f=$f
#echo r=$r
#echo r_arg=$r_arg
#echo p=$p
#echo p_arg=$p_arg

#for filename in "$@"; do
    #echo $filename
#done

# Truncate output file
if [[ $r == "enabled" ]] ; then
    > $r_arg
fi

starting_args=$#

#for filename in "$@" ; do
for (( i = 1; i <= $starting_args; i+= 1 )) do
    filename="$1"

    if [[ -e $filename || -d $filename ]] ; then
        ;
    else
        exit 1
    fi


    if [[ $r == "enabled" ]] ; then
        # Send output to file
        echo Processing file: $filename
        echo
        #echo Processing file: $filename >> $r_arg
        #echo >> $r_arg

        if [[ $c == "enabled" ]]; then
            wc "$filename" >> $r_arg
        fi

        if [[ $l == "enabled" ]]; then
            ls -l "$filename" >> $r_arg
        fi

        if [[ $p == "enabled" ]]; then
            if (( $p_arg < 0 )); then
                tail -n $(( $p_arg * -1 )) $filename >> $r_arg
            elif (( $p_arg == 0 )); then
                cat $filename >> $r_arg
            else
                head -n $p_arg $filename >> $r_arg
            fi
        fi

        if [[ $f == "enabled" ]]; then
            file "$filename" >> $r_arg
        fi


        if (( $i != $starting_args )) ; then
            echo >> $r_arg
            echo -------------------- >> $r_arg
            echo >> $r_arg
        fi
    else
        if [[ -e $filename || -d $filename ]] ; then
            ;
        else
            exit 1
        fi

        # Send output to stdout
        echo Processing file: $filename
        echo

        if [[ $c == "enabled" ]]; then
            wc "$filename"
        fi

        if [[ $l == "enabled" ]]; then
            ls -l "$filename"
        fi

        if [[ $p == "enabled" ]]; then
            if (( $p_arg < 0 )) ; then
                tail -n $(( $p_arg * -1 )) $filename
            elif (( $p_arg == 0 )); then
                cat $filename
            else
                head -n $p_arg $filename
            fi
        fi

        if [[ $f == "enabled" ]]; then
            file "$filename"
        fi


        if (( $i != $starting_args )) ; then
            echo
            echo --------------------
            echo
        fi
    fi

    shift
done
