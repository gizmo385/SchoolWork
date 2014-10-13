#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char *usage = "Usage: shuffle -b|-f <num>\nwhere <num> is greater than 0\n";
    char printingMode = 0;
    int numberOfPairs = 0;

    // Parse arguments, handle errors
    if( argc != 3 ) {
        fprintf(stderr, "%s", usage);
        return 1;
    } else {
        if( strcmp(argv[1], "-b") == 0 ) {
            printingMode = 'b';
        } else if ( strcmp(argv[1], "-f") == 0 ) {
            printingMode = 'f';
        } else {
            fprintf(stderr, "%s", usage);
            return 1;
        }

        numberOfPairs = atoi(argv[2]);
        if( numberOfPairs <= 0 ) {
            fprintf(stderr, "%s", usage);
            return 1;
        }
    }

    // Read in the input
    for( int i = 0; i < numberOfPairs; i++ ) {
        char firstLine[1000];
        char secondLine[1000];
        char c = -1;

        // Read in first line
        int firstLineIndex = 0;
        while( (c = getchar()) != EOF ) {
            firstLine[firstLineIndex] = c;
            firstLineIndex++;
        }

        // Read in second line
        int secondLineIndex = 0;
        while( (c = getchar()) != 0 ) {
            secondLine[secondLineIndex] = c;
            secondLineIndex++;
        }

        int shuffleIndex = 0;
        while( shuffleIndex < firstLineIndex && shuffleIndex < secondLineIndex ) {
            if( printingMode == 'f' ) {
                printf("%c", firstLine[shuffleIndex]);
                printf("%c", secondLine[shuffleIndex]);
            } else {
                printf("%c", secondLine[shuffleIndex]);
                printf("%c", firstLine[shuffleIndex]);
            }

            shuffleIndex += 2;
        }
    }

    return 0;
}
