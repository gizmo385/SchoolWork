#include "cardDealer.h"
#include "processOpts.h"
#include "shuffle.h"

/* Function prototypes */
void dealPoker();
void dealHearts();
void dealPinochle();

/*
 * Deals the appropriate hands that are desired by the user
 *
 * Arguments:
 * flags -- The list of flags for the program
 */
void deal( int *flags ) {
   // initialize the random number generator
   initShuffle( flags[SEED_INDEX] );

   if ( flags[ POKER_FLAG ] ) {
      dealPoker();
   }

   if ( flags[ HEARTS_FLAG ] ) {
      dealHearts();
   }

   if ( flags[ PINOCHLE_FLAG ] ) {
      dealPinochle();
   }

}

void dealPoker( /* appropriate formal parameters here */ ) {

}


void dealHearts( /* appropriate formal parameters here */ ) {

}


void dealPinochle( /* appropriate formal parameters here */ ) {

}
