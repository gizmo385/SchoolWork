#include <stdio.h>
#include <stdlib.h>

int main( int argc, char *argv[] ) {

    unsigned long sum = 0;

    printf("%20s %10s\n", "number", "line");

    char ch;
    int currentNumber = 0;
    int currentlyParsingNumber = 0;
    int currentLineNumber = 1;
    while( ( ch = getchar() ) != EOF ) {
        if( ch >= '0' && ch <= '9' ) {
            currentlyParsingNumber = 1;
            currentNumber = (currentNumber * 10) + ((int)ch - 48);
        } else {
            if( currentlyParsingNumber ) {
                printf("%20d %10d\n", currentNumber, currentLineNumber);
                sum += currentNumber;
                currentNumber = 0;
                currentlyParsingNumber = 0;
            }

            if( ch == '\n' ) {
                currentLineNumber++;
            }
        }
    }

    sum += currentNumber;

    printf("\nThe sum of all the integers: %lu\n", sum);

    return 0;
}
