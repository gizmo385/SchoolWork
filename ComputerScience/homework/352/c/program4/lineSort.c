#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>
#include <getopt.h>

/* Command line option modes */
#define CASE_SENSITIVE      0
#define CASE_INSENSITIVE    1
#define REVERSE_ORDER       2
#define NORMAL_ORDER        3
#define BOTH                4

/* CONSTANTS */
#define TRUE                1
#define FALSE               0
#define MAX_LINES           12000
#define READ_LENGTH         201

/* Debug options */
#define DEBUG               TRUE

/* Global variables */
int debugEnabled = 0;

/* Function Prototypes */
void debug( const char *format, ...);
void usage( char *progName, FILE *file);
void processFile( char *filename, int sortOrder, int (*compare)(const char *, const char *),
        char *progName );
char *processLine( FILE *file, int currentLine );
void sortLines( char *filename, char *lines[], int numberOfLines,
        int (*compare)(const char *, const char *), int sortOrder );

/*
 * Prints the usage information for this program to the specified file
 * descriptor.
 *
 * Arguments:
 * progName -- The name of the program being executed
 * file     -- The file descriptor to write usage information to
 *
 * Exit Status:
 * 1 -- If the file descriptor is stderr, exit status will be 1
 * 0 -- If the file descriptor is stdout, exit status will be 0
 */
void usage( char *progName, FILE *file) {
    fprintf(file, "Usage: %s [-i] [-r] [-b] file [file ...]\n", progName);
    fprintf(file, "  --usage           print this information and exit\n");
    fprintf(file, "  -i --insensitive  case insensitive sort\n");
    fprintf(file, "  -r --reverse      sort in reverse alphabetic order\n");
    fprintf(file, "  -b --both         sort both normal and reverse\n");
    fprintf(file, "Default is case-sensitive sort in alphabetic order\n");

    // Exit from the usage information
    file == stdout ?
        exit(0) :
        exit(1);
}

/*
 * Prints debug information if the debug flag is enabled and if the option has been
 * passed into the command line using either --debug or -d.
 *
 * Arguments:
 * format   -- The format for printing. This follows the same rules as printf
 * VA_ARGS  -- The items that will be printed by printf
 */
void debug( const char *format, ... ) {
    if( DEBUG && debugEnabled ) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}

/*
 * Sorts the lines in the provided array, using the specified comparison function in a particular
 * sort order.
 *
 * Arguments:
 * filename      -- The name of the file that the lines were read from
 * lines         -- The lines that are being sorted
 * numberOfLines -- The number of lines in the line array
 * compare       -- A function to compare two strings
 * sortOrder     -- One of either NORMAL_ORDER, REVERSE_ORDER, or BOTH. This determines how the
 *                  lines will be ordered.
 */
void sortLines( char *filename, char *lines[], int numberOfLines,
        int (*compare)(const char *, const char *), int sortOrder ) {

    debug( "Sorting %d %s from file %s.\n", numberOfLines,
            numberOfLines == 1 ? "line" : "lines", filename );

    // Sort the lines
    int i, j, m;
    for( i = 0; i < numberOfLines; i++ ) {
        char *min = lines[i];

        for( j = i; j < numberOfLines; j++ ) {
            char *curr = lines[j];

            /*if( (*compare)(min, curr) > 0 ) {*/
            if( (*compare)(curr, min) <= 0 ) {
                min = curr;
                m = j;
            }
        }

        char *temp = lines[i];
        lines[i] = lines[m];
        lines[m] = temp;
    }

    // Write the sorted lines to a file
    char *outputFilename = strcat( filename, ".out" );
    FILE *outputFile = fopen( outputFilename, "w" );

    if( outputFile == NULL ) {
        // TODO: Handle failed file open here
    }

    debug( "Writing sorted output to %s.\n", outputFilename );

    switch( sortOrder ) {
        case NORMAL_ORDER:
            debug( "Sort order: Normal. Outputing result of alphabetic sort.\n");
            fprintf( outputFile, "Alphabetic Sort:\n\n" );
            for( int lineNumber = 0; lineNumber < numberOfLines; lineNumber++ ) {
                debug( "Line number %d: %s", lineNumber, lines[ lineNumber ] );
                fprintf( outputFile, "%s", lines[ lineNumber ] );
            }

            debug( "Finished outputting alphabetic sort to file.\n");

            break;
        case REVERSE_ORDER:
            debug( "Sort order: Reverse. Outputing result of reverse alphabetic sort.\n");
            fprintf( outputFile, "Reverse Alphabetic Sort:\n\n" );
            for( int lineNumber = numberOfLines - 1; lineNumber >= 0; lineNumber-- ) {
                debug( "Line number %d: %s", lineNumber, lines[ lineNumber ] );
                fprintf( outputFile, "%s", lines[ lineNumber ] );
            }

            debug( "Finished outputting reverse alphabetic sort to file.\n");

            break;
        case BOTH:
            debug( "Sort order: Both. Outputing result of alphabetic & reverse alphabetic sort.\n");

            fprintf( outputFile, "Alphabetic Sort:\n\n" );
            for( int lineNumber = 0; lineNumber < numberOfLines; lineNumber++ ) {
                debug( "Line number %d: %s", lineNumber, lines[ lineNumber ] );
                fprintf( outputFile, "%s", lines[ lineNumber ] );
            }

            fprintf( outputFile, "\n" );

            fprintf( outputFile, "Reverse Alphabetic Sort:\n\n" );
            for( int lineNumber = numberOfLines; lineNumber >= 0; lineNumber-- ) {
                debug( "Line number %d: %s", lineNumber, lines[ lineNumber ] );
                fprintf( outputFile, "%s", lines[ lineNumber ] );
            }

            debug( "Finished outputting alphabetic and reverse alphabetic sort to file.\n");

            break;
        default:
            debug( "Invalid sortOrder value encountered! sortOrder = %d", sortOrder );
            break;
    }

    int result = fclose( outputFile );

    if( result != 0 ) {
        fprintf( stderr, "There was an error while attempting to close the file!");
    }
}

/*
 * Processses the file that is located at the path specified by the provided filename
 *
 * Arguments:
 * filename  -- The path to the file that is going to be processed
 * sortOrder -- One of either NORMAL_ORDER, REVERSE_ORDER, or BOTH. This determines how the
 *              lines will be ordered.
 * compare   -- A function to compare two strings
 * progName  -- The name of the program being run
 */
void processFile( char *filename, int sortOrder, int (*compare)(const char *, const char *),
        char *progName ) {
    debug( "Processing file: %s\n", filename);
    // Open our file
    FILE *file = fopen(filename, "r");

    if( file != NULL ) {
        char *lines[MAX_LINES];
        int currentLine = 0;

        // Read until the end of the file
        while( !feof(file) && currentLine < MAX_LINES ) {

            char *line = processLine( file, currentLine );
            lines[ currentLine ] = line;

            currentLine++;
            // Exit the loop if we've read the first 12,000 lines
            if( currentLine >= MAX_LINES ) {
                break;
            }
        }

        currentLine--;

        // Sort the lines
        sortLines( filename, lines, currentLine, compare, sortOrder );

        // Free the memory for each line
        for( int i = 0; i < currentLine; i++ ) {
            free( lines[i] );
        }
    } else {
        // Report errors
        fprintf( stderr, "Unable to open %s for reading\n", filename );
        usage( progName, stderr );
    }

    // Close our file handle
    fclose( file );
}

/*
 * Reads a single line from a file
 *
 * Arguments:
 * file        -- The file that the line is being read from
 * currentLine -- The current line in the file being processed (for debug output)
 *
 * Returns:
 * A pointer to the line that was read from the file
 */
char *processLine( FILE *file, int currentLine ) {
    int lineLength = 201;
    int fgetsOffset = 0;
    char *line = malloc( lineLength * sizeof(char) );

    // Parse lines until we don't wish to read them anymore
    while( TRUE ) {
        fgets(line + fgetsOffset, READ_LENGTH, file );

        /*if( strlen( line ) < lineLength ) {*/
        if( line[lineLength - 2] == '\0' || line[lineLength - 2] == '\n' ) {
            // If we've reached the end of the line, add the line to the array of lines
            debug( "----------------------------\n" );
            debug( "Reached the end of the line:\n" );
            debug( "lineLength   = %d\n", lineLength );
            debug( "fgetsOffsett = %d\n", fgetsOffset );
            debug( "line = %s\n", line );
            debug( "currentLine = %d\n", currentLine + 1 );
            debug( "----------------------------\n\n" );
            break;
        } else {
            fgetsOffset += 200;
            lineLength += 200;
            line = realloc( line, lineLength * sizeof(char) );
            debug( "----------------------------\n" );
            debug( "Expanding the length of the line:\n" );
            debug( "lineLength   = %d\n", lineLength );
            debug( "fgetsOffsett = %d\n", fgetsOffset );
            debug( "----------------------------\n\n" );
        }
    }

    return line;
}

/*
 * Option Summary
 *
 * Usage: lineSort [-i] [-r] [-b] file [file ...]
 * --usage              Prints this information to stdout
 * -i --insensitive     Case insensitive sort
 * -r --reverse         Sort in reverse alphabetic order
 * -b --both            Sort both normal and reverse
 *
 * Default is case-sensitive sort in alphabetic order
 */
struct option longopts[] = {
    {"usage",       no_argument,    NULL, 'u'},
    {"insensitive", no_argument,    NULL, 'i'},
    {"reverse",     no_argument,    NULL, 'r'},
    {"both",        no_argument,    NULL, 'b'},

    // This option is transparent to the user and enables debug information
    {"debug",       no_argument,    NULL, 'd'},
    {NULL,          0,              NULL,  0 }
};

int main(int argc, char *argv[]) {
    char *progName = argv[0];

    // The function that will be used to compare strings
    int (*compare)(const char *, const char *) = strcmp;

    // Determines whether or not we are sorting in normal order, reverse order, or both
    int sortOrder = NORMAL_ORDER;

    // Parse command line arguments
    char ch = 0;
    while(( ch = getopt_long(argc, argv, "irbd", longopts, NULL)) != -1 ) {
        switch( ch ) {
            case 'r':
                debug( "Setting line sort mode to be reverse.\n");
                sortOrder = REVERSE_ORDER;
                break;
            case 'i':
                compare = strcasecmp;
                debug( "Sorting will now be case-insensitive\n");
                break;
            case 'b':
                debug( "Line sort will now sort reversed and non-reversed.\n");
                sortOrder = BOTH;
                break;
            case 'd':
                debugEnabled = DEBUG;
                debug("Debug output enabled!\n");
                break;
            case 'u':
                usage(progName, stdout);
                break;
            case '?':
                usage(progName, stderr);
                break;
            default:
                fprintf(stderr, "main: reached default\n");
                break;
        }
    }

    // Shift argv so that the parsed arguments are no longer reachable
    argc -= optind;
    argv += optind;

    if( argc == 0 ) {
        fprintf(stderr, "Missing file name\n");
        usage( progName, stderr );
    } else {
        for( int i = 0; i < argc; i++ ) {
            char *filename = argv[i];
            processFile( filename, sortOrder, compare, progName );
        }
    }

    return 0;
}
