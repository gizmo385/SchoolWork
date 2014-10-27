#include "cardDealer.h"

void dealPoker( /* appropriate formal parameters here */ );
void dealHearts( /* appropriate formal parameters here */ );
void dealPinochle( /* appropriate formal parameters here */ );


void
deal( /* put here the formal parameters for deal, they will be the arrays for
       * the cards and the flags from processOpts that tell what to do */)
{
   // initialize the random number generator
   initShuffle( seed );
   
   if ( pokerFlag ) {
      dealPoker( /* appropriate actual parameters here */ );
   }

   if ( heartsFlag ) {
      dealHearts( /* appropriate actual parameters here */ );
   }
   
   if ( pinochleFlag ) {
      dealPinochle( /* appropriate actual parameters here */ );
   }

} /* deal */

void
dealPoker( /* appropriate formal parameters here */ )
{
} /* dealPoker */


void
dealHearts( /* appropriate formal parameters here */ )
{
} /* dealHearts */
   

void
dealPinochle( /* appropriate formal parameters here */ )
{
} /* dealPinochle */
