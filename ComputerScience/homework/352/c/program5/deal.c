#include <string.h>
#include "processOpts.h"
#include "shuffle.h"
#include "deal.h"

/*
 * Deals the appropriate hands that are desired by the user
 *
 * Arguments:
 * flags -- The list of flags for the program
 */
void deal( int *flags ) {
   // initialize the random number generator
    initShuffle( flags[SEED_INDEX] );
    int numberOfHands = flags[ NUMBER_OF_HANDS ];

    if ( flags[ POKER_FLAG ] ) {
        dealPoker(numberOfHands);
    }

    if ( flags[ HEARTS_FLAG ] ) {
        dealHearts(numberOfHands);
    }

    if ( flags[ PINOCHLE_FLAG ] ) {
        dealPinochle(numberOfHands);
    }
}

/*
 * Prints out the cards so that cards are placed in columns under their respective hands like as
 * shown below:
 *
 *      Hand 1            Hand 2            Hand 3            Hand 4
 *  Ace of Spades      Ten of Spades      Six of Spades      Two of Spades
 *  King of Hearts     Nine of Hearts     Five of Hearts     Ace of Hearts
 *  Queen of Diamonds  Eight of Diamonds  Four of Diamonds   King of Diamonds
 *  Jack of Clubs      Seven of Clubs     Three of Clubs     Queen of Clubs
 *
 *   Hand 5            Hand 6
 *  Jack of Spades     Seven of Spades
 *  Ten of Hearts      Six of Hearts
 *  Nine of Diamonds   Five of Diamonds
 *  Eight of Clubs     Four of Clubs
 *
 *  Arguments:
 *  faces         -- An array of strings is the card's face values (King, Queen, etc).
 *  numFaces      -- The number of card faces
 *  handSize      -- The size of each player's hand
 *  numberOfHands -- The number of hands that were dealt
 *  hands         -- An array of (numberOfHands) hands, each with (handSize) cards
 */
void printCards( char *faces[], int numFaces, int handSize, int numberOfHands,
        int hands[numberOfHands][handSize]) {

    for( int handBlock = 0; handBlock < numberOfHands; handBlock += 4 ) {
        // Print out the hand titles
        for( int hand = 0; hand < 4; hand++ ) {
            if( (handBlock + hand) >= numberOfHands ) {
                break;
            }

            char handText[20];
            sprintf(handText, "Hand %d", handBlock + hand + 1 );
            printf("%5s%s%7s", "", handText, "");

        }

        printf("\n");

        // Prints out the number of hands
        for( int cardInHand = 0; cardInHand < handSize; cardInHand++ ) {
            for( int hand = 0; hand < 4; hand++ ) {
                if( (hand + handBlock) >= numberOfHands ) {
                    break;
                }

                int cardValue = hands[ hand + handBlock ][ cardInHand ];
                char card[20];

                sprintf( card, "%s of %s", faces[ cardValue % numFaces ], getSuit( cardValue ));
                /*strcpy( card, faces[cardValue % numFaces] );*/
                /*strcat( card, " of " );*/
                /*strcat( card, getSuit( cardValue ) );*/
                printf( "%-17s ", card );
            }

            printf( "\n" );
        }

        if( handBlock + 4 < numberOfHands ) {
            printf("\n");
        }
    }

}

/*
 * Deals the specified number of Poker hands from a shuffled deck of 52 cards
 *
 * Arguments:
 * numberOfHands -- The number of hands to deal
 */
void dealPoker( int numberOfHands ) {
    numberOfHands = numberOfHands < 9 ? numberOfHands : 8;
    debug( "Dealing %d %s of Poker.\n", numberOfHands, numberOfHands == 1 ? "hand" : "hands" );

    // Initialize and shuffle our card deck
    char *faces[] = { "Ace",    "King",   "Queen",    "Jack",
                      "Ten",    "Nine",   "Eight",    "Seven",
                      "Six",    "Five",   "Four",     "Three",
                      "Two" };

    int deck[POKER_DECK_SIZE];
    debug("BEFORE SHUFFLE\n");
    for( int i = 0; i < POKER_DECK_SIZE; i++ ) {
        debug( "In Poker deck - (%d) %s of %s\n", i, faces[ i % 13 ], getSuit(i) );
        deck[i] = i;
    }

    shuffleCards( deck, POKER_DECK_SIZE );
    debug("AFTER SHUFFLE\n");

    for( int i = 0; i < POKER_DECK_SIZE; i++ ) {
        int cardValue = deck[i];
        debug( "In Poker deck - (%d) %s of %s\n", i, faces[ cardValue % 13 ], getSuit( cardValue ) );
    }


    int hands[ numberOfHands ][5]; // A poker hand has 5 cards in it
    int currentCard = 0;

    // Deal the cards
    for( int currentCardInHand = 0; currentCardInHand < 5; currentCardInHand++ ) {
        for( int currentHand = 0; currentHand < numberOfHands; currentHand++ ) {
            hands[ currentHand ][ currentCardInHand ] = deck[currentCard];
            currentCard++;
        }
    }

    // Print out the cards
    printf("----------\n");
    printf("| Poker: |\n");
    printf("----------\n\n");
    printCards( faces, 13, 5, numberOfHands, hands );
}


/*
 * Deals the specified number of Hearts hands from a shuffled deck of 52 cards
 *
 * Arguments:
 * numberOfHands -- The number of hands to deal
 */
void dealHearts( int numberOfHands ) {
    debug( "Dealing %d %s of Hearts\n.", numberOfHands, numberOfHands == 1 ? "hand" : "hands" );

    // Initialize and shuffle our card deck
    char *faces[] = { "Ace",    "King",   "Queen",    "Jack",
                      "Ten",    "Nine",   "Eight",    "Seven",
                      "Six",    "Five",   "Four",     "Three",
                      "Two" };

    int cards[HEARTS_DECK_SIZE];
    for( int i = 0; i < HEARTS_DECK_SIZE; i++ ) {
        debug( "In Hearts deck - (%d) %s of %s\n", i, faces[ i % 13 ], getSuit(i) );
        cards[i] = i;
    }

    shuffleCards( cards, HEARTS_DECK_SIZE );

    int hands[numberOfHands + 1][ HEARTS_DECK_SIZE / numberOfHands ];
    int currentCard = 0;

    // Deal the cards
    for( int currentHand = 0; currentHand < numberOfHands; currentHand++ ) {
        if( (currentCard + numberOfHands) >= HEARTS_DECK_SIZE ) {
            break;
        }

        for( int currentCardInHand = 0; currentCardInHand < 5; currentCardInHand++ ) {
            hands[ currentHand ][ currentCardInHand ] = cards[currentCard];
            currentCard++;
        }
    }

    printf("-----------\n");
    printf("| Hearts: |\n");
    printf("-----------\n\n");
    printCards( faces, 13, HEARTS_DECK_SIZE / numberOfHands, numberOfHands, hands );
}


/*
 * Deals the specified number of Pinochle hands from a shuffled deck of 48 cards
 *
 * Arguments:
 * numberOfHands -- The number of hands to deal
 */
void dealPinochle( int numberOfHands ) {
    debug( "Dealing %d %s of Pinochle.\n", numberOfHands, numberOfHands == 1 ? "hand" : "hands" );

    // Initialize and shuffle our card deck
    /*char *suits[] = { "Spades", "Hearts", "Diamonds", "Clubs" };*/
    /*char *faces[] = { "Ace", "King", "Queen", "Jack", "Ten", "Nine"};*/

    int cards[PINOCHLE_DECK_SIZE];
    for( int i = 0; i < PINOCHLE_DECK_SIZE; i++ ) {
        cards[i] = i;
    }

    shuffleCards( cards, PINOCHLE_DECK_SIZE );
    /*printCards( faces, suits, 0, numberOfHands, hands );*/
}

char *getSuit( int cardValue ) {
    if( cardValue < 13 ) {
        return "Spades";
    } else if( cardValue < 26 ) {
        return "Hearts";
    } else if( cardValue < 39 ) {
        return "Clubs";
    } else {
        return "Diamonds";
    }
}
