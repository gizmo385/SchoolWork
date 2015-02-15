#include <stdlib.h>
#include <time.h>

void swap( char *words[], int x, int y ) {
    char *temp = words[x];
    words[x] = words[y];
    words[y] = temp;
}

void quicksort( char *words[], int length ) {
    if( length > 1 ) {
        return;
    } else {
        char *pivot = words[ length / 2 ];
        int leftIndex = 0,
            rightIndex = 0;

        while( start < end ) {
            while( strcmp( words[leftIndex], pivot ) < 0 ) {
                leftIndex++;
            }

            while( strcmp( pivot, words[rightIndex] ) < 0 ) {
                rightIndex--;
            }

            if( leftIndex >= rightIndex ) {
                break;
            } else {
                swap( words, leftIndex, rightIndex );
            }
        }

        quicksort(words, leftIndex);
        quicksort(words + leftIndex, length - leftIndex );
    }
}
