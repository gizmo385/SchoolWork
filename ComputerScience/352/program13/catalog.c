#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "processOpts.h"
#include "utils.h"
#include "parser.h"
#include "catalog.h"

void printTitle( char *title );

int main( int argc, char *argv[] ) {
    /*setDebuggingLevel( E_ALL );*/

    char *progName = argv[0];
    int flags[ NUMBER_OF_FLAGS ];
    processOpts( argv, argc, progName, flags );

    // Output the current flag settings as a debug message
    debug( E_DEBUG, "------------------------------\n" );
    debug( E_DEBUG, "Sort order: %s\n", flags[ ORDER_INDEX ] == NORMAL_ORDER ?
            "Normal" : "Reverse" );
    debug( E_DEBUG, "Case sensitivity: %s\n", flags[ CASE_INDEX ] == CASE_SENSITIVE ?
            "Case sensitive" : "Case insensitive" );
    debug( E_DEBUG, "------------------------------\n" );

    argc -= optind;
    argv += optind;

    ComparisonFunction comparisonFunction;

    // Determine which comparison function to use
    if( flags[ CASE_INDEX ] == CASE_SENSITIVE ) {
        comparisonFunction = (ComparisonFunction) strcmp;
    } else {
        comparisonFunction = (ComparisonFunction) strcasecmp;
    }

    if( argc < 1 ) {
        fprintf( stderr, "Missing file name\n" );
        usage( progName, stderr );
    } else {
        for( int i = 0; i < argc; i++ ) {
            debug( E_INFO, "File argument: %s\n", argv[i] );

            // Create the intial list and load the titles from the .txt file
            BST *titles = newBST( (ComparisonFunction) comparisonFunction );
            loadTitlesFromFile( argv[i], titles );

            bstInOrder( titles, (ElementConsumer) printTitle );

            // Create save file -_-
            char *saveFilename = calloc( strlen(argv[i]) + strlen(".save") + 1, sizeof(char) );
            strcpy( saveFilename, argv[i] );
            strcat( saveFilename, ".save" );
            FILE *thisIsPointless = fopen( saveFilename, "w" );
            fclose( thisIsPointless );

            free( saveFilename );

            // Parse the commands file
            parseFile( argv[i], flags, titles );

            bstFree(titles);
        }
    }
}

void loadTitlesFromFile( char *filename, BST *titles ) {
    char *titlesFilename = calloc( strlen(filename) + strlen(".txt") + 1, sizeof(char) );
    strcpy( titlesFilename, filename );
    strcat( titlesFilename, ".txt\0" );

    FILE *file = fopen( titlesFilename, "r" );

    if( file == NULL ) {
        debug( E_ERROR, "Error openning \'%s\' for reading!\n", titlesFilename);
    } else {
        int prefixLength = strlen( "Title:  " );

        while( !feof(file) ) {
            char *nextTitle = processLine(file);
            char *actualTitle = strdup( nextTitle + prefixLength );

            debug( E_DEBUG, "Inserting '%s' into titles\n", actualTitle );

            if( bstFind(titles, actualTitle) ) {
                free( actualTitle );
                free( nextTitle );
                continue;
            }

            bstInsert( titles, actualTitle );
            free( nextTitle );
        }
    }

    fclose( file );
    free( titlesFilename );
}

void printTitle( char *title ) {
    if( title && strlen(title) != 0 ) {
        debug( E_DEBUG, "Title in list: '%s'\n", title );
    }
}
