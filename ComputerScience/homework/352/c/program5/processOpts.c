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
    flags[ HAND_SIZE_INDEX ]    = 4;
    flags[ POKER_FLAG ]         = 0;
    flags[ PINOCHLE_FLAG ]      = 0;
    flags[ HEARTS_FLAG ]        = 0;

    struct option longopts[] = {
        // User facing options
        {"usage",       no_argument,        NULL, 'u'},
        {"seed",        required_argument,  NULL, 's'},
        {"poker",       no_argument,        NULL, 'p'},
        {"pinochle",    no_argument,        NULL, 'P'},
        {"hearts",      no_argument,        NULL, 'h'},
        {"hands",       optional_argument,  NULL, 'H'},

        // Transparent options
        {"debug",       no_argument,    NULL, 'd'},
        {NULL,          0,              NULL,  0 }
    };

    char ch = -1;
    int seedEncountered = 0;

    while( (ch = getopt_long(argc, argv, "us::pPhH:d", longopts, NULL)) != -1 ) {
        switch(ch) {
            case 's':
                seedEncountered = 1;

                // Parse the required argument for this option
                if( optarg == NULL ) {
                    usage( progName, stderr );
                } else {
                    if( *optarg == '=' ) {
                        optarg++;
                    }

                    flags[ SEED_INDEX ] = atoi(optarg);
                    debug( "Seed set to: %d\n", flags[ SEED_INDEX ] );
                }
                break;
            case 'p':
                debug( "Dealing for Poker\n" );
                flags[ POKER_FLAG ] = 1;
                break;
            case 'P':
                debug( "Dealing for Pinochle\n" );
                flags[ PINOCHLE_FLAG ] = 1;
                break;
            case 'h':
                debug( "Dealing for Hearts\n" );
                flags[ HEARTS_FLAG ] = 1;
                break;
            case 'H':
                // Handle the optional argument for this option
                if( optarg == NULL ) {
                    flags[ HAND_SIZE_INDEX ] = DEFAULT_HAND_SIZE;
                } else {
                    if( *optarg == '=' ) {
                        optarg++;
                    }

                    flags[ HAND_SIZE_INDEX ] = atoi(optarg);
                }

                debug( "Dealing %d hands!\n", flags[ HAND_SIZE_INDEX ] );
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

    // Ensure that we encountered a seed
    if( ! seedEncountered ) {
        usage( progName, stderr );
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
    fprintf( file, "Usage: %s -s=# [-p] [-P] [-h] [-H[=#]]\n", progName );
    fprintf( file, "--usage       print this information and exit\n" );
    fprintf( file, "-s --seed     initialize random number generator (required argument)\n" );
    fprintf( file, "-p --poker    deal 5-card poker hands\n" );
    fprintf( file, "-P --pinochle deal pinochle hands\n" );
    fprintf( file, "-h --hearts   deal cards for hearts\n" );
    fprintf( file, "-H --hands    number of hands to deal  (optional argument)\n" );
    fprintf( file, "              Number of hands defaults to 4\n" );

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
