#include <stdio.h>
#include <stdlib.h>

#include "processOpts.h"
#include "printer.h"

int main( int argc, char *argv[] ) {
    char *progName = argv[0];

    int flags[NUMBER_OF_FLAGS];
    processOpts( argv, argc, progName, flags );

    debug( "\nCurrent Flags:\n" );
    debug( "--------------\n" );
    debug( "Printing method: %s\n", flags[PRINTING_METHOD] == RECURSIVE ? "Recusive" : "Iterative" );
    debug( "Printing method: %s\n", flags[PRINTING_DIRECTION] == PRINT_FORWARDS ? "Forwards" : "Backwards" );
    debug( "Line skip: %d\n", flags[LINE_SKIP] );
    debug( "--------------\n\n" );

    argc -= optind;
    argv += optind;

    for( int i = 0; i < argc; i++ ) {
        debug( "File argument: %s\n", argv[i] );
        print_file( argv[i], flags );
    }
}
