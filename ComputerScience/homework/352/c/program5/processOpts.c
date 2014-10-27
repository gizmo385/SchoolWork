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
 * argv  -- The list of command line arguments provided to main
 * argc  -- The number of command line arguments provided to main
 * flags -- The destination array for the flags to be set in
 */
void processOpts( char *argv[], int argc, int *flags ) {
    debug( "Testing debug!" );
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
