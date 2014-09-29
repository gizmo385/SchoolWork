#include <stdio.h>
#include <stdlib.h>

int main( int argc, char *argv[] ) {
    // Validate that there is at least one command-line argument
    if( argc < 2 ) {
        printf("Usage: skipArgs <number> [arg ...]\n");
        exit(1);
    }

    // Read and validate the first command-line argument
    int divisor = atoi(argv[1]);
    if( divisor < 1 ) {
        divisor = 1;
    }

    int i;
    // Print the left-justifed strings
    printf("Left-justified strings:\n");

    for( i = 2; i < argc; i++ ) {
        if( (i - 1) % divisor != 0 ) {
            printf("%4d:|%-15s|%-15s|\n", i - 1, argv[i], "");
        } else {
            printf("%4d:|%-15s|%-15s|\n", i - 1, "", argv[i]);
        }
    }


    // Print the right-justifed strings
    printf("Right-justified strings:\n");

    for( i = 2; i < argc; i++ ) {
        if( (i - 1) % divisor != 0 ) {
            printf("%4d:|%15s|%15s|\n", i - 1, argv[i], "");
        } else {
            printf("%4d:|%15s|%15s|\n", i - 1, "", argv[i]);
        }
    }

    return 0;
}
