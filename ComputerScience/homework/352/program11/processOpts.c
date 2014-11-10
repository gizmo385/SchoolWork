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
     flags[ R_FORMAT_INDEX ] = 0;
     flags[ I_FORMAT_INDEX ] = 0;
     flags[ J_FORMAT_INDEX ] = 0;

    struct option longopts[] = {
        // User facing options
        {"usage",       no_argument,        NULL, 'u'},
        {"Rformat",     no_argument,        NULL, 'r'},
        {"Iformat",     no_argument,        NULL, 'i'},
        {"Jformat",     no_argument,        NULL, 'j'},
        {"all",         no_argument,        NULL, 'a'},


        // Transparent options
        {"debug",       no_argument,    NULL, 'd'},
        {NULL,          0,              NULL,  0 }
    };

    char ch = -1;

    while( (ch = getopt_long(argc, argv, "durija", longopts, NULL)) != -1 ) {
        switch(ch) {
            case 'r':
                debug( "Enabling conversion of R-Format instructions\n");
                flags[ R_FORMAT_INDEX ] = 1;
                break;
            case 'i':
                debug( "Enabling conversion of I-Format instructions\n");
                flags[ I_FORMAT_INDEX ] = 1;
                break;
            case 'j':
                flags[ J_FORMAT_INDEX ] = 1;
                debug( "Enabling conversion of J-Format instructions\n");
                break;
            case 'a':
                debug( "Enabling conversion of ALL instructions\n");
                flags[ R_FORMAT_INDEX ] = 1;
                flags[ I_FORMAT_INDEX ] = 1;
                flags[ J_FORMAT_INDEX ] = 1;
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
    fprintf( file,"Usage: %s [-r] [-i] [-j] [-a] file [file ...]\n", progName );
    fprintf( file,"--usage       print this information and exit\n" );
    fprintf( file,"-r --Rformat  disassemble R-format instructions\n" );
    fprintf( file,"-i --Iformat  disassemble I-format instructions\n" );
    fprintf( file,"-j --Jformat  disassemble J-format instructions\n" );
    fprintf( file,"-a --all      disassemble all instructions\n" );

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
