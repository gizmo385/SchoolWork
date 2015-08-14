#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "parser.h"
#include "processOpts.h"

#define READ_LENGTH 201

void writeTitlesHelper( BSTNode *currentNode, int flags[], FILE *outputFile );

/*
 * The title command searches the list for the desired title.
 *
 * Arguments:
 * title   -- The title to search for
 * titles  -- The list of titles to operate on
 */
void title( char *title, BST *titles ) {
    void *foundElement = bstFind( titles, title );

    printf( "TITLE\n" );
    if( foundElement ) {
        printf( "FOUND: %s\n", title );
    } else {
        printf( "NOT FOUND: %s\n", title );
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
void saveTitles( char *filename, int flags[], BST *titles ) {
    // Open save file
    char *outputFilename = calloc( strlen(filename) + strlen(".save") + 1, sizeof(char) );
    strcpy(outputFilename, filename);
    strcat(outputFilename, ".save" );
    FILE *outputFile = fopen( outputFilename, "a" );

    // Handle the case where the output file could not be opened
    if( outputFile == NULL ) {
        debug( E_ERROR, "There was an error while opening output file %s\n", outputFilename );
        return;
    }

    debug( E_DEBUG, "Saving titles\n");
    printf( "SAVE-TITLES\n" );

    writeTitlesHelper( titles->root, flags, outputFile );

    // Close the output file and free the filename
    fclose( outputFile );
    free( outputFilename );
}

void writeTitlesHelper( BSTNode *currentNode, int flags[], FILE *outputFile ) {
    if( currentNode ) {
        if( flags[ORDER_INDEX] == REVERSE_ORDER ) {
            writeTitlesHelper( currentNode->right, flags, outputFile );
        } else {
            writeTitlesHelper( currentNode->left, flags, outputFile );
        }

        char *title = currentNode->data;
        if( title && strlen(title) != 0 ) {
            fprintf( outputFile, "Title:  %s\n", title );
        }

        if( flags[ORDER_INDEX] == REVERSE_ORDER ) {
            writeTitlesHelper( currentNode->left, flags, outputFile );
        } else {
            writeTitlesHelper( currentNode->right, flags, outputFile );
        }
    }
}

/*
 * Adds the desired title to the list
 *
 * Arguments:
 * title   -- The title that you wish to add to the list of titles
 * titles  -- The list of titles to operate on
 */
void add( char *title, BST *titles ) {
    if( strlen(title) == 0 ) {
        free( title );
        return;
    }

    printf("ADD\n");
    void *foundElement = bstFind( titles, title );

    if( foundElement ) {
        printf( "FOUND: %s\n", title );
        free( title );
        return;
    }

    bstInsert( titles, title );
    printf( "ADDED: %s\n", title );
}

/*
 * Saves the current titles in file.save. Each title will be proceeded by "Title:" and two blank
 * spaces in the save file.
 *
 * Arguments:
 * title   -- The title that you wish to delete
 * titles  -- The list of titles to operate on
 */
void deleteTitle( char *title, BST *titles ) {
    printf( "DELETE-TITLE\n" );
    void *removedElement = bstRemove( titles, title );

    if( removedElement ) {
        printf( "DELETED: %s\n", title );
    } else {
        printf( "NOT FOUND: %s\n", title );
    }

    free( removedElement );
}

/*
 * Parses a file and pulls catelog commands out of it
 *
 * Arguments:
 * filename -- The name of the file to parse
 * flags    -- The settings for the execution of the program
 * titles   -- The current list of titles
 */
void parseFile( char *filename, int flags[], BST *titles ) {
    printf( "%s:\n\n", filename );
    char *commandsFilename = calloc(strlen(filename) + strlen(".commands") + 1, sizeof(char));
    strcpy( commandsFilename, filename );
    strcat( commandsFilename, ".commands" );

    FILE *commandFile = fopen( commandsFilename, "r" );

    // Handle the case where the command file could not be openned
    if( commandFile == NULL ) {
        debug( E_ERROR, "Error opening \'%s\' for reading!\n", filename );
        return;
    }

    // Parse the commands in the file
    while( !feof(commandFile) ) {
        char *line = processLine( commandFile );
        debug( E_DEBUG, "Command: '%s'\n", line );

        // Parse the command
        if( startsWith(line, "TITLE ") && (strcmp(line, "TITLE ") != 0 ) ){
            // Find the title
            title( line + strlen("TITLE "), titles );
        } else if( strcmp(line, "SAVE-TITLES") == 0 ) {
            // Save the titles to a file
            saveTitles( filename, flags, titles );
        } else if( startsWith(line, "ADD ") ) {
            // Add a new filename to the file
            char *title = strdup( line + strlen("ADD ") );
            add( title, titles );
        } else if( startsWith(line, "DELETE-TITLE ") ) {
            // Remove the title from the list
            deleteTitle(line + strlen("DELETE-TITLE "), titles);
        } else {
            // Invalid command
            debug( E_WARNING, "Encountered an invalid command: '%s'\n", line );
        }

        free( line );
    }

    printf( "\n" );

    free( commandsFilename );
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

    // Remove ending newlines
    for( int i = strlen(line) - 1; i >= 0; i-- ) {
        if( line[i] == '\n' ) {
            line[i] = 0;
            break;
        }
    }

    return line;
}

int startsWith( char *string, const char *substring ) {
    int stringLength = strlen( string );
    int substringLength = strlen( substring );

    if( substringLength > stringLength ) {
        return 0;
    }

    for( int i = 0; i < substringLength; i++ ) {
        if( string[i] != substring[i] ) {
            return 0;
        }
    }

    return 1;
}
