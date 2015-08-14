#include <stdlib.h>
#include <stdio.h>

#include "processOpts.h"
#include "parser.h"

int main( int argc, char *argv[] ) {
    char *progName = argv[0];
    int flags[ NUMBER_OF_FLAGS ];
    processOpts( argv, argc, progName, flags );

    debug( "\nCurrent Flags:\n" );
    debug( "--------------\n" );
    debug( "Processing R-Format Instructions: %s\n", flags[R_FORMAT_INDEX] == 1 ? "Yes" : "No" );
    debug( "Processing J-Format Instructions: %s\n", flags[J_FORMAT_INDEX] == 1 ? "Yes" : "No" );
    debug( "Processing I-Format Instructions: %s\n", flags[I_FORMAT_INDEX] == 1 ? "Yes" : "No" );
    debug( "--------------\n\n" );

    argc -= optind;
    argv += optind;

    if( argc == 0 ) {
        fprintf( stderr, "Missing file name\n" );
        usage( progName, stderr );
    }

    for( int i = 0; i < argc; i++ ) {
        printf( "File \'%s\' contains:\n", argv[i] );
        parseFile( progName, argv[i], flags );
    }
}
