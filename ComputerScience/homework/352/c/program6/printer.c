#include <stdlib.h>
#include <stdio.h>

#include "processOpts.h"
#include "list.h"
#include "printer.h"

void recursive_backwards_helper( FILE *file, int skipValue, int currentSkipCounter );

/*
 * Prints the file based on the options supplied to the program
 *
 * Arguments:
 * filename -- The file that will be openned and printed
 * flags    -- Optional flags passed to the program that determine how the input/output is handled
 */
void print_file( char *filename, int *flags ) {
    if( flags[ PRINTING_DIRECTION ] == PRINT_FORWARDS  ) {
        iterative_forwards( filename, flags );
    } else {
        if( flags[ PRINTING_METHOD ] == ITERATIVE ) {
            iterative_backwards( filename, flags );
        } else {
            recursive_backwards( filename, flags );
        }
    }
}

/*
 * Traverses through the file and prints lines as they are encountered
 *
 * Arguments:
 * filename -- The file that will be openned and printed
 * flags    -- Optional flags passed to the program that determine how the input/output is handled
 */
void iterative_forwards( char *filename, int *flags ) {
    FILE *file = fopen( filename, "r" );
    int currentSkipCounter = 1;
    int skipValue = flags[LINE_SKIP];

    while( !feof(file) ) {
        char buffer[120];
        fgets( buffer, 120, file );

        if( currentSkipCounter == skipValue || skipValue < 1 ) {
            printf( "%s", buffer );
            currentSkipCounter = 1;
        } else {
            currentSkipCounter++;
        }
    }

    fclose( file );
}

/*
 * Creates a stack of lines in the file and prints them
 *
 * Arguments:
 * filename -- The file that will be openned and printed
 * flags    -- Optional flags passed to the program that determine how the input/output is handled
 */
void iterative_backwards( char *filename, int *flags ) {
    FILE *file = fopen( filename, "r" );
    struct node *list = createList();

    while( !feof(file) ) {
        char buffer[120];
        fgets( buffer, 120, file );

        push( buffer, list );
    }

    fclose( file );
    int currentSkipCounter = 1;
    int skipValue = flags[ LINE_SKIP ];

    // Traverse and print the nodes
    while( peek( list ) != NULL ) {
        char *word = pop(list);

        if( currentSkipCounter == skipValue ) {
            printf( "%s\n", word );
            currentSkipCounter = 1;
        } else {
            currentSkipCounter++;
        }

        /*free( word );*/
    }
}

/*
 * Recursively reads the file and prints the lines
 *
 * Arguments:
 * filename -- The file that will be openned and printed
 * flags    -- Optional flags passed to the program that determine how the input/output is handled
 */
void recursive_backwards( char *filename, int *flags ) {
    FILE *file = fopen( filename, "r" );
    recursive_backwards_helper( file, flags[ LINE_SKIP ], 0 );
    fclose( file );
}

/*
 * A helper function for the recursive solution to printing a file backwards
 *
 * Arguments:
 * filename  -- The file that will be openned and printed
 * skipValue -- The number of lines that should be printed
 */
void recursive_backwards_helper( FILE *file, int skipValue, int currentSkipCounter ) {

    if( ! feof(file) ) {
        char buffer[120];
        fgets( buffer, 120, file );

        if( currentSkipCounter == skipValue || skipValue < 1 ) {
            recursive_backwards_helper( file, skipValue, 1 );
            printf( "%s", buffer );
        } else {
            recursive_backwards_helper( file, skipValue, currentSkipCounter + 1 );
        }
    } else {
        return;
    }
}
