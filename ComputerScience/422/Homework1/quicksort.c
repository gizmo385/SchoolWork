#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

void quicksort( char *words[], int length ) {
    if( length <= 1 ) {
        return;
    } else {
        char *pivot = words[ length / 2 ];
        int leftIndex = 0,
            rightIndex = length - 1;

        for(;; leftIndex++, rightIndex--){
            while( strcmp( words[leftIndex], pivot ) < 0 ) {
                leftIndex++;
            }

            while( strcmp( words[rightIndex], pivot ) > 0 ) {
                rightIndex--;
            }

            if( leftIndex >= rightIndex ) {
                break;
            } else {
                char *temp = words[leftIndex];
                words[leftIndex] = words[rightIndex];
                words[rightIndex] = temp;
            }
        }

        quicksort(words, leftIndex);
        quicksort(words + leftIndex, length - leftIndex );
    }
}

static inline int min( int a, int b ) {
    return a < b ? a : b;
}

char **merge( char *first[], int firstLength, char *second[], int secondLength ) {
    char **result = calloc( sizeof(char *), firstLength + secondLength );
    int firstIndex = 0, secondIndex = 0, resultIndex = 0;

    while( firstIndex < firstLength && secondIndex < secondLength ) {
        if( strcmp(first[firstIndex], second[secondIndex]) <= 0 ) {
            result[resultIndex] = first[firstIndex];
            resultIndex++;
            firstIndex++;
        } else {
            result[resultIndex] = second[secondIndex];
            resultIndex++;
            secondIndex++;
        }
    }

    while( firstIndex < firstLength ) {
        result[resultIndex] = first[firstIndex];
        resultIndex++;
        firstIndex++;
    }

    while( secondIndex < secondLength ) {
        result[resultIndex] = second[secondIndex];
        resultIndex++;
        secondIndex++;
    }

    return result;
}

#if 0
char **merge( char *first[], int firstLength, char *second[], int secondLength ) {
    char **result = calloc( sizeof(char *), firstLength + secondLength );
    int firstIndex = 0, secondIndex = 0;

    for( int i = 0; i < firstLength + secondLength; i++ ) {
        if( i < firstLength && i < secondLength ) {
            if( strcmp(first[firstIndex], second[secondIndex]) < 0 ) {
                result[i] = first[firstIndex];
                firstIndex++;
            } else {
                result[i] = second[secondIndex];
                secondIndex++;
            }
        } else if( i < firstLength ) {
            result[i] = first[firstIndex];
            firstIndex++;
        } else if( i < secondLength ) {
            result[i] = second[secondIndex];
            secondIndex++;
        }
    }

    return result;
}
#endif
