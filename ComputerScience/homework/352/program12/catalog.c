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

            List *titles = createList( comparisonFunction, flags[ORDER_INDEX] );
            loadTitlesFromFile( argv[i], titles );
            char *commandsFilename = strcat( argv[i], ".commands" );

            parseFile( commandsFilename, titles );
        }
    }
}

void loadTitlesFromFile( char *filename, List *titles ) {
    char *titlesFilename = strcat( filename, ".txt" );
    FILE *file = fopen( titlesFilename, "r" );

    if( file == NULL ) {
        debug( E_ERROR, "Error openning \'%s\' for reading!\n", filename );
    } else {
        int prefixLength = strlen( "Title:  " );

        while( !feof(file) ) {
            char *nextTitle = processLine(file);
            char *actualTitle = calloc( strlen(nextTitle) - prefixLength + 1, sizeof(char) );
            strncpy( actualTitle, nextTitle + prefixLength, strlen(nextTitle) - prefixLength + 1 );
            insertElement( actualTitle, titles );

            free( nextTitle );
        }
    }
}
