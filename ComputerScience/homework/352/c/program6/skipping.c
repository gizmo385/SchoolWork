#include <stdio.h>
#include <stdlib.h>

#include "processOpts.h"

int main( int argc, char *argv[] ) {
    char *progName = argv[0];

    int flags[NUMBER_OF_FLAGS];
    debug( "Testing\n");
    processOpts( argv, argc, progName, flags );
}
