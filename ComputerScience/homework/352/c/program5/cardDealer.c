#include <stdio.h>
#include <stdlib.h>

#include "processOpts.h"

int main( int argc, char *argv[] ) {
    char *progName = argv[0];

    int flags[ NUMBER_OF_FLAGS ];
    processOpts( argv, argc, progName, flags );

    debug( "Current Flags:\n" );
    debug( "--------------\n" );
    debug( "Seed: %d\n", flags[SEED_INDEX] );
    debug( "Poker: %s\n", flags[ POKER_FLAG ] ? "yes" : "no" );
    debug( "Pinochle: %s\n", flags[ PINOCHLE_FLAG ] ? "yes" : "no" );
    debug( "Hearts: %s\n", flags[ HEARTS_FLAG ] ? "yes" : "no" );
    debug( "Hand size: %d\n", flags[HAND_SIZE_INDEX]);
    debug( "--------------\n" );
}
