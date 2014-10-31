/* CONSTANTS */
#define HEARTS_DECK_SIZE   52
#define POKER_DECK_SIZE    52
#define PINOCHLE_DECK_SIZE 48

/**
 * Deals the hands that have been enabled in the flags. The flags follow the specification found in
 * processOpts
 *
 * Arguments:
 * flags -- The flags that tell the program what types of hands to deal and how many cards to deal
 *          in each hand.
 */
extern void deal( int *flags );

/*
 * Deals the specified number of Poker hands from a shuffled deck of 52 cards
 *
 * Arguments:
 * numberOfHands -- The number of hands to deal
 */
extern void dealPoker( int numberOfHands );

/*
 * Deals the specified number of Hearts hands from a shuffled deck of 52 cards
 *
 * Arguments:
 * numberOfHands -- The number of hands to deal
 */
extern void dealHearts( int numberOfHands );

/*
 * Deals the specified number of Pinochle hands from a shuffled deck of 48 cards
 *
 * Arguments:
 * numberOfHands -- The number of hands to deal
 */
extern void dealPinochle( int numberOfHands );

extern char *getSuit( int cardValue );
