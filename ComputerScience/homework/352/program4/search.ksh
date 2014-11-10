#! /bin/ksh

# usage: search.ksh [ options ] pattern [file ...]
#   -e, --egrep     use egrep for searching
#   -f, --fgrep     use fgrep for searching
#   -w, --wc        pipe output through wc
#   -h, --head[=lines]
#                 pipe the output through head. The option value may be
#                 omitted. The default value is 10.
#   -t, --tail[=lines]
#                 pipe the output through tail. The option value may be
#                 omitted. The default value is 10.
#   -o, --output=filename
#                 re-direct output to filename

USAGE="[+NAME?search.ksh -- uses fgrep or egrep to search files]"

USAGE+="[+DESCRIPTION?The search.ksh script will use either or both of egrep and fgrep to search "
USAGE+="for a pattern in one or more files. If neither -e nor -f is specified, then "
USAGE+="the program defaults to using fgrep.]"

USAGE+="[+?The results of the search(es) are printed to stdout by default. The results "
USAGE+="can be piped to wc by using -w. The results can also be piped through head or "
USAGE+="tail, and the maximum number of output lines that head or tail will print can "
USAGE+="be specified.]"

USAGE+="[+?Output can be directed into a file by using the -o option. In this case, no "
USAGE+="output goes to the screen.]"

USAGE+="[-version?search.ksh, version 1.0]"
USAGE+="[-author?Christopher A. Chapline]"
USAGE+="[-copyright?Copyright (c) September 26, 2014]"
USAGE+="[e:egrep?use egrep for searching]"
USAGE+="[f:fgrep?use fgrep for searching]"
USAGE+="[w:wc?pipe output through wc]"
USAGE+="[h:head]#?[lines:=10?pipe the output through head.]"
USAGE+="[t:tail]#?[lines:=10?pipe the output through tail.]"
USAGE+="[o:output?re-direct output to filename]:[filename]"
USAGE+=$'\n\npattern [file ...]\n\n'
USAGE+=$'[+SEE ALSO?egrep(1), fgrep(1), head(1), tail(1), wc(1)]'

# Default argument values
num_lines=10
egrep_enabled=0
fgrep_enabled=0
head_or_tail=
head_or_tail_enabled=0
wc_enabled=0
output_file_enabled=0
output_file=

# Parse the command line options
while getopts "$USAGE" optchar ; do
    case $optchar in
    e)  egrep_enabled=1
        ;;
    f)  fgrep_enabled=1
        ;;
    w)  wc_enabled=1
        ;;
    h)  head_or_tail="head"
        head_or_tail_enabled=1
        num_lines=${OPTARG:-10}
        ;;
    t)  head_or_tail="tail"
        head_or_tail_enabled=1
        num_lines=${OPTARG:-10}
        ;;
    o)  output_file_enabled=1
        output_file=${OPTARG:-"filename"}
        ;;
    esac
done

# Shifts the command so that the first argument is the pattern
shift $(( $OPTIND - 1 ))

# Get the pattern argument
pattern="$1"
shift

# Debug output
#echo ----------------------------------------------
#echo pattern=$pattern
#echo wc enabled=$wc_enabled
#echo head_or_tail_enabled=$head_or_tail_enabled
#echo head_or_tail=$head_or_tail
#echo num lines=$num_lines
#echo search_method=$search_method
#echo output file enabled=$output_file_enabled
#echo output_file = $output_file
#echo ----------------------------------------------
#echo

# Truncate the output file if one is present
if (( $output_file_enabled == 1 )) ; then
    > $output_file
fi

# Get the number of filenames
num_files=$#

# Iterate over filename arguments
for (( i = 1; i <= num_files; i+= 1 )) do
    filename="$1"

    if (( $egrep_enabled == 1 )) ; then
        # Build the beginning of the command
        command_to_execute="egrep -e '${pattern}'"
        command_to_execute+=" ${filename}"

        # Check if head or tail is enabled
        if (( $head_or_tail_enabled == 1 )) ; then
            command_to_execute+=" | $head_or_tail -n ${num_lines}"
        fi

        # Check if word count is enabled
        if (( $wc_enabled == 1 )) ; then
            command_to_execute+=" | wc"
        fi

        # Debug output
        #echo Executing: $command_to_execute

        # Check if output is being piped to a file
        if (( output_file_enabled == 1 )) ; then
            eval $command_to_execute >> $output_file
        else
            eval $command_to_execute
        fi
    fi

    if (( $fgrep_enabled == 1 )) ; then
        # Build the beginning of the command
        command_to_execute="fgrep -e '${pattern}'"
        command_to_execute+=" ${filename}"

        # Check if head or tail is enabled
        if (( $head_or_tail_enabled == 1 )) ; then
            command_to_execute+=" | $head_or_tail -n ${num_lines}"
        fi

        # Check if word count is enabled
        if (( $wc_enabled == 1 )) ; then
            command_to_execute+=" | wc"
        fi

        # Debug output
        #echo Executing: $command_to_execute

        # Check if output is being piped to a file
        if (( output_file_enabled == 1 )) ; then
            eval $command_to_execute >> $output_file
        else
            eval $command_to_execute
        fi
    fi

    shift
done

