#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>
#include <getopt.h>

#include "processOpts.h"

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
    // Set defaults for flags
    flags[PRINTING_DIRECTION] = PRINT_FORWARDS;
    flags[PRINTING_METHOD] = RECURSIVE;
    flags[LINE_SKIP] = 0;

    struct option longopts[] = {
        // User facing options
        {"usage",       no_argument,        NULL, 'u'},
        {"forwards",    no_argument,        NULL, 'f'},
        {"backwards",   optional_argument,  NULL, 'b'},
        {"skip",        required_argument,  NULL, 's'},


        // Transparent options
        {"debug",       no_argument,    NULL, 'd'},
        {NULL,          0,              NULL,  0 }
    };

    char ch = -1;
    int forwardSpecified = 0;

    while( (ch = getopt_long(argc, argv, "us:b::df", longopts, NULL)) != -1 ) {
        switch(ch) {
            case 'f':
                forwardSpecified = 1;
                debug( "Forward printing specified\n" );
                break;
            case 'b':
                if( forwardSpecified == 0) {
                    flags[ PRINTING_DIRECTION ] = PRINT_BACKWARDS;

                    if( optarg != NULL ) {
                        if( *optarg == '=' ) {
                            optarg++;
                        }

                        if( strcmp( optarg, "iterative" ) == 0 ) {
                            flags[ PRINTING_METHOD ] = ITERATIVE;
                            debug( "Iteratively printing backwards!\n" );
                        } else {
                            debug( "Recursively printing backwards!\n" );
                        }
                    } else {
                        char *possibleArgument = argv[optind];

                        if( optind < argc) {
                            if( strcmp( possibleArgument, "iterative" ) == 0 ) {
                                flags[ PRINTING_METHOD ] = ITERATIVE;
                                debug( "Iteratively printing backwards!\n" );
                                optind++;
                            } else if( strcmp( possibleArgument, "recursive" ) == 0 ) {
                                flags[ PRINTING_METHOD ] = RECURSIVE;
                                debug( "Recursively printing backwards!\n" );
                                optind++;
                            }
                        }
                    }
                }
                break;
            case 's':
                if( optarg == NULL ) {
                    usage( progName, stderr );
                } else {
                    if( *optarg == '=' ) {
                        optarg++;
                    }

                    flags[ LINE_SKIP ] = atoi( optarg );

                    debug( "Line skip set to %d\n", flags[ LINE_SKIP ] );
                }
                break;
            case 'd':
                debugEnabled = DEBUG;
                debug( "Debug output enabled!\n" );
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

    // If both forwards and backwards are specified, ensure that the text is printed
    // forwards
    if( forwardSpecified == 1 ) {
        flags[PRINTING_DIRECTION] = PRINT_FORWARDS;
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
    fprintf( file, "Usage: %s [options] filename [filename ...]\n", progName);
    fprintf( file, "OPTIONS\n" );
    fprintf( file, "--usage            print this information and exit\n" );
    fprintf( file, "-f, --forwards     prints the lines of the file in order\n" );
    fprintf( file, "      The default is to print forwards\n" );
    fprintf( file, "-b, --backwards=[iterative | recursive]\n" );
    fprintf( file, "                   prints the lines of the file in reverse order\n" );
    fprintf( file, "      iterative    uses an iterative loop to print each file\nin reverse order\n" );
    fprintf( file, "      recursive    uses recursion to print each file in reverse order\n" );
    fprintf( file, "      The default is to use recursion\n" );
    fprintf( file, "-s, --skip=value   prints every <value> line beginning with the\n<value> line\n" );

    // Exit from the usage information
    file == stdout ?
        exit(0) :
        exit(1);
}

/*
 * Prints debug information if the debug flag is enabled and if the option has been
 * passed into the command line using either --debug or -d.
 *
 * Arguments:
 * format   -- The format for printing. This follows the same rules as printf
 * VA_ARGS  -- The items that will be printed by printf
 */
void debug( const char *format, ... ) {
    if( DEBUG && debugEnabled ) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}
