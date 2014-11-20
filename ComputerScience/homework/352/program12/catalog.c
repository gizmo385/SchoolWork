#include <stdlib.h>
#include <stdio.h>

#include "processOpts.h"
#include "utils.h"
#include "parser.h"

int main( int argc, char *argv[] ) {
    char *progName = argv[0];
    int flags[ NUMBER_OF_FLAGS ];
    setDebugReporting( E_ALL );
    processOpts( argv, argc, progName, flags );

    // Output the current flag settings as a debug message
    debug( E_DEBUG, "------------------------------\n" );
    debug( E_DEBUG, "Sort order: %s\n", flags[ ORDER_INDEX ] == NORMAL_ORDER ?
            "Normal" : "Reverse" );
    debug( E_DEBUG, "Case sensitivity: %s\n", flags[ CASE_INDEX ] == CASE_SENSITIVE ?
            "Case sensitive" : "Case insensitive" );
    debug( E_DEBUG, "------------------------------\n" );

    argc -= optind;
    argv += optind;

    if( argc < 1 ) {
        fprintf( stderr, "Missing file name\n" );
        usage( progName, stderr );
    } else {
        for( int i = 0; i < argc; i++ ) {
            debug( E_INFO, "File argument: %s\n", argv[i] );
            parseFile( argv[i], flags );
        }
    }
}
