#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "processOpts.h"
#include "utils.h"
#include "parser.h"
#include "catalog.h"

int main( int argc, char *argv[] ) {
    // If debugging has been enabled,
#ifdef DEBUG
    setDebugReporting( E_ALL );
#endif

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

    int (*comparisonFunction)(const char *, const char *);

    // Determine which comparison function to use
    if( flags[ CASE_INDEX ] == CASE_SENSITIVE ) {
        comparisonFunction = strcmp;
    } else {
        comparisonFunction = strcasecmp;
    }

    if( argc < 1 ) {
        fprintf( stderr, "Missing file name\n" );
        usage( progName, stderr );
    } else {
        for( int i = 0; i < argc; i++ ) {
            debug( E_INFO, "File argument: %s\n", argv[i] );

            // Create the intial list and load the titles from the .txt file
            List *titles = createList( comparisonFunction, flags[ORDER_INDEX] );
            loadTitlesFromFile( argv[i], titles );

            Node *current = titles->head;
            while( current->next != NULL ) {
                debug( E_INFO, "Title in list: '%s'\n", current->title );
                current = current->next;
            }

            // Create save file -_-
            char *saveFilename = calloc( strlen(argv[i]) + strlen(".save"), sizeof(char) );
            strcpy( saveFilename, argv[i] );
            strcat( saveFilename, ".save" );
            FILE *thisIsPointless = fopen( saveFilename, "w" );
            fclose( thisIsPointless );

            free( saveFilename );

            // Parse the commands file
            parseFile( argv[i], titles );
        }
    }
}

void loadTitlesFromFile( char *filename, List *titles ) {
    char *titlesFilename = calloc( strlen(filename) + strlen(".txt"), sizeof(char) );
    strcpy( titlesFilename, filename );
    strcat( titlesFilename, ".txt" );

    FILE *file = fopen( titlesFilename, "r" );

    if( file == NULL ) {
        debug( E_ERROR, "Error openning \'%s\' for reading!\n", titlesFilename);
    } else {
        int prefixLength = strlen( "Title:  " );

        while( !feof(file) ) {
            char *nextTitle = processLine(file);
            char *actualTitle = strdup( nextTitle + prefixLength );

            /*debug( E_DEBUG, "Inserting '%s' into titles\n", actualTitle );*/
            insertElement( actualTitle, titles );

            free( nextTitle );
        }
    }

    free( titlesFilename );
}
