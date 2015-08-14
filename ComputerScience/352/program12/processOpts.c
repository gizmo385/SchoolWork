#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>
#include <getopt.h>

#include "processOpts.h"
#include "utils.h"

int debugEnabled = 0;


/*
 * Processes the command line arguments and returns an array of flags that represent the various
 * settings for the program. The organization of the flags follows the constants defined in
 * processOpts.h
 *
 * Arguments:
 * argv     -- The list of command line arguments provided to main
 * argc     -- The number of command line arguments provided to main
 * progName -- The name of the program
 * flags    -- The destination array for the flags to be set
 */
void processOpts( char *argv[], int argc, char *progName, int *flags ) {

    flags[ CASE_INDEX ]  = CASE_SENSITIVE;
    flags[ ORDER_INDEX ] = NORMAL_ORDER;

    struct option longopts[] = {
        // User facing options
        {"usage",       no_argument,        NULL, 'u'},
        {"reverse",     no_argument,        NULL, 'r'},
        {"insensitive", no_argument,        NULL, 'i'},

        // Transparent options
        {NULL,          0,              NULL,  0 }
    };

    char ch = -1;

    while( (ch = getopt_long(argc, argv, "uri", longopts, NULL)) != -1 ) {
        switch(ch) {
            case 'r':
                debug( E_DEBUG, "Now sorting titles in reverse\n");
                flags[ ORDER_INDEX ] = REVERSE_ORDER;
                break;
            case 'i':
                debug( E_DEBUG, "Now sorting titles in a case insensitive manner\n");
                flags[ CASE_INDEX ] = CASE_INSENSITIVE;
                break;
            case 'u':
                usage( progName, stdout );
                break;
            case '?':
                usage( progName, stderr );
                break;
            default:
                fprintf( stderr, "main: reached default\n");
                break;
        }

    }
}

/*
 * Prints the usage information for this program to the specified file
 * descriptor.
 *
 * Arguments:
 * progName -- The name of the program being executed
 * file     -- The file descriptor to write usage information to
 *
 * Exit Status:
 * 1 -- If the file descriptor is stderr, exit status will be 1
 * 0 -- If the file descriptor is stdout, exit status will be 0
 */
void usage( char *progName, FILE *file) {
    fprintf( file, "Usage: %s [-r] [-i] file [file ...]\n", progName );
    fprintf( file, "  --usage           print this information and exit\n" );
    fprintf( file, "  -r --reverse      reverse alphabetic order\n" );
    fprintf( file, "  -i --insensitive  case insensitive order\n" );
    fprintf( file, "Default is case-sensitive alphabetic ordering\n" );

    // Exit from the usage information
    file == stdout ?
        exit(0) :
        exit(1);
}
