#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


char* timeTense( int occurences ) {
    if( occurences != 1 ) {
        return "times";
    } else {
        return "time";
    }
}

int main( int argc, char *argv[] ) {

    int count = 0;
    scanf("%d", &count);

    if( count < 0 ) {
        count = 0;
    } else if( count > 100000 ) {
        count = 100000;
    }

    if( count == 0 ) {
        return 0;
    }

    int numbers[count];

    int largest = INT_MIN;
    int smallest = INT_MAX;
    int secondLargest = INT_MIN;
    int secondSmallest = INT_MAX;

    int occurencesOfLargest = 0;
    int occurencesOfSmallest = 0;
    int occurencesOfSecondLargest = 0;
    int occurencesOfSecondSmallest = 0;


    for (int i = 0; i < count; i++) {
        scanf("%d", &numbers[i]);

        int number = numbers[i];


        /*
         * Check if we've found a new  largest or second largest
         */
        if( number > largest ) {
            secondLargest = largest;
            largest = number;

            occurencesOfSecondLargest = occurencesOfLargest;
            occurencesOfLargest = 0;
        } else if ( number > secondLargest ) {
            if( number != largest ) {
                secondLargest = number;
                occurencesOfSecondLargest = 0;
            }
        }

        /*
         * Check if we've found a new  smallest or second smallest
         */
        if( number < smallest ) {
            secondSmallest = smallest;
            smallest = number;

            occurencesOfSecondSmallest = occurencesOfSmallest;
            occurencesOfSmallest = 0;
        } else if( number < secondSmallest ) {
            if( number != smallest ) {
                secondSmallest = number;
                occurencesOfSecondSmallest = 0;
            }
        }

        /*
         * Determine if we've found new occurences of largest numbers
         */
        if( number == largest ) {
            occurencesOfLargest++;
        } else if ( number == secondLargest ) {
            occurencesOfSecondLargest++;
        }

        if ( number == secondSmallest ) {
            occurencesOfSecondSmallest++;
        } else if ( number == smallest ) {
            occurencesOfSmallest++;
        }
    }

    printf("There are %d numbers in the array\n\n", count);

    if( largest == smallest ) {
        printf( "The largest and smallest number is %d and appears %d %s.\n",
                largest, occurencesOfLargest,
                timeTense( occurencesOfLargest ));
    } else {
        // Print largest and second largest
        printf( "The largest number is %d and appears %d %s.\n",
            largest, occurencesOfLargest, timeTense( occurencesOfLargest ));
        printf( "The 2nd largest number is %d and appears %d %s.\n\n",
            secondLargest, occurencesOfSecondLargest,
            timeTense( occurencesOfSecondLargest ));

        // Print smallest and second smallest
        printf( "The smallest number is %d and appears %d %s.\n",
            smallest, occurencesOfSmallest, timeTense( occurencesOfSmallest ));
        printf( "The 2nd smallest number is %d and appears %d %s.\n\n",
            secondSmallest, occurencesOfSecondSmallest,
            timeTense( occurencesOfSecondSmallest ));


        int remaining = count - occurencesOfLargest - occurencesOfSmallest;
        remaining -= (occurencesOfSecondLargest + occurencesOfSecondSmallest);

        // Print remaining numbers
        if( remaining != 0 ) {
            printf("The other %d numbers in the array are:\n", remaining);

            int totalPrinted = 0;
            int printedInRow = 0;
            for( int i = 0; i < count; i += 1 ) {
                if( totalPrinted >= remaining ) {
                    continue;
                }

                int number = numbers[i];
                if( number != largest && number != smallest &&
                        number != secondLargest && number != secondSmallest ) {
                    printf("%12d", number);
                    printedInRow++;
                    totalPrinted++;
                }

                if( printedInRow == 5 ) {
                    printf("\n");
                    printedInRow = 0;
                }
            }
        }

        printf("\n");
    }

    return 0;
}
