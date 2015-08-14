#include <stdio.h>
#include <stdlib.h>

int main( int argc, char *argv[] ) {

    if( argc < 3 ) {
        printf( "Usage: boxes <number> <fillCharacter>\n" );
        exit(1);
    }

    int boxWidth = atoi(argv[1]);
    char boxChar = argv[2][0];

    if( boxWidth < 3 ) {
        boxWidth = 3;
    } else if( boxWidth > 80 ) {
        boxWidth = 80;
    }

    // Print the top of the box
    int i;
    for( i = 0; i < boxWidth; i++ ) {
        printf("*");
    }

    for( i = 0; i < boxWidth - 2; i++ ) {
        printf("\n*");
        int j;

        for( j = 0; j < boxWidth - 2; j++ ) {
            printf("%c", boxChar);
        }

        printf("*");
    }

    printf("\n");

    for( i = 0; i < boxWidth; i++ ) {
        printf("*");
    }

    printf("\n");

    return 0;
}
