#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "list.h"
#include "parser.h"
#include "processOpts.h"

#define READ_LENGTH 201


/*
 * The title command searches the list for the desired title.
 *
 * Arguments:
 * title   -- The title to search for
 * titles  -- The list of titles to operate on
 */
void title( char *title, List *titles ) {
    Node *current = titles->head;
    while( current->next != NULL && strcmp( title, current->title ) != 0 ) {
        current = current->next;
    }

    printf( "TITLE\n" );
    if( (current == NULL) || (current->title == NULL) || (strcmp(title, current->title) != 0) ) {
        printf( "NOT FOUND: %s\n", title );
    } else {
        printf( "FOUND: %s\n", title );
    }
}

/*
 * Saves the current titles in file.save. Each title will be proceeded by "Title:" and two blank
 * spaces in the save file.
 *
 * Arguments:
 * filename -- The name of the file to store the saved titles in
 * titles   -- The list of titles to operate on
 */
void saveTitles( char *filename, List *titles ) {
    char *outputFilename = strcat( filename, ".save" );
    FILE *outputFile = fopen( outputFilename, "w" );

    // Handle the case where the output file could not be opened
    if( outputFile == NULL ) {
        return;
    }

    // Iterate over all title nodes and write the titles to the output file
    Node *current = titles->head;
    while( current->next != NULL ) {
        fprintf( outputFile, "Title:  %s\n", current->title );
        current = current->next;
    }

    // Close the output file and free the filename
    fclose( outputFile );
    free( outputFilename );
}

/*
 * Adds the desired title to the list
 *
 * Arguments:
 * title   -- The title that you wish to add to the list of titles
 * titles  -- The list of titles to operate on
 */
void add( char *command, List *titles ) {

}

/*
 * Saves the current titles in file.save. Each title will be proceeded by "Title:" and two blank
 * spaces in the save file.
 *
 * Arguments:
 * title   -- The title that you wish to delete
 * titles  -- The list of titles to operate on
 */
void deleteTitle( char *command, List *titles ) {

}

/*
 * Parses a file and pulls catelog commands out of it
 *
 * Arguments:
 * filename -- The name of the file to parse
 * titles   -- The current list of titles
 */
void parseFile( char *filename, List *titles ) {
    debug( E_DEBUG, "Parsing file: %s\n", filename );
    FILE *commandFile = fopen( filename, "r" );

    // Handle the case where the command file could not be openned
    if( commandFile == NULL ) {
        debug( E_ERROR, "Error opening \'%s\' for reading!\n", filename );
        return;
    }

    while( !feof(commandFile) ) {
        char *line = processLine( commandFile );
        // Parse the command

        if( startsWith(line, "TITLE ") ) {
            // Find the title
        } else if( startsWith(line, "SAVE-TITLES") ) {
            // Save the titles to a file
        } else if( startsWith(line, "ADD ") ) {
            // Add the title to the list of titles
        } else if( startsWith(line, "DELETE-TITLE ") ) {
            // Remove the title from the list
        } else {
            // Invalid command
        }

        free( line );
    }

    fclose( commandFile );
}

char *processLine( FILE * file ) {
    int lineLength = READ_LENGTH;
    int fgetsOffset = 0;
    char *line = calloc( lineLength, sizeof(char) );

    // Parse lines until we don't wish to read them anymore
    while( 1 ) {
        fgets(line + fgetsOffset, READ_LENGTH, file );

        // Test whether or not we've encountered the end of the line
        if( line[lineLength - 2] == '\0' || line[lineLength - 2] == '\n' ) {
            break;
        } else {
            fgetsOffset += 200;
            lineLength += 200;
            line = realloc( line, lineLength * sizeof(char) );
        }
    }

    return line;
}

int startsWith( char *string, const char *substring ) {
    return strncmp( string, substring, strlen(substring) );
}
