#include <stdio.h>
#include <stdlib.h>

#include "processOpts.h"
#include "deal.h"

int main( int argc, char *argv[] ) {
    char *progName = argv[0];

    int flags[ NUMBER_OF_FLAGS ];
    processOpts( argv, argc, progName, flags );

    debug( "\nCurrent Flags:\n" );
    debug( "--------------\n" );
    debug( "Seed: %d\n", flags[SEED_INDEX] );
    debug( "Poker: %s\n", flags[ POKER_FLAG ] ? "yes" : "no" );
    debug( "Pinochle: %s\n", flags[ PINOCHLE_FLAG ] ? "yes" : "no" );
    debug( "Hearts: %s\n", flags[ HEARTS_FLAG ] ? "yes" : "no" );
    debug( "Number of hands: %d\n", flags[ NUMBER_OF_HANDS ]);
    debug( "--------------\n\n" );

    deal( flags );
}
