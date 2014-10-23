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

/* Convinience definitions */
#define TRUE                1
#define FALSE               0

/* Debug options */
#define DEBUG               TRUE

/* Global variables */
int debugEnabled = 0;

/* Function Prototypes */
void debug( const char *format, ...);
void usage( char *progName, FILE *file);
void processFile( char *filename );
void processLine( char *line );
void sortLines( char *lines[], int (*compare)(const char *, const char *),
        int sortOrder );

/*
 * Prints the usage information for this program to the specified file
 * descriptor.
 *
 * Arguments:
 * progName -- The name of the program being executed
 * file     -- The file descriptor to write usage information to
 */
void usage( char *progName, FILE *file) {
    fprintf(file, "Usage: %s [-i] [-r] [-b] file [file ...]}\n", progName);
    fprintf(file, "\t--usage           print this information and exit\n");
    fprintf(file, "\t-i --insensitive  case insensitive sort\n");
    fprintf(file, "\t-r --reverse      sort in reverse alphabetic order\n");
    fprintf(file, "\t-b --both         sort both normal and reverse\n");
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
 * Processses the file that is located at the path specified by the provided filename
 *
 * Arguments:
 * filename -- The path to the file that is going to be processed
 */
void processFile( char *filename ) {
    debug( "Processing file: %s\n", filename);
    // Open our file
    FILE *file = fopen(filename, "r");
    int MAX_LINES = 12000;

    if( file != NULL ) {
        char *lines[MAX_LINES];
        int currentLine = 0;

        // Read until the end of the file
        while( !feof(file) ) {
            int currentLineLength = 201;
            char *line = malloc( currentLineLength * sizeof(char) );

            // Parse lines until we don't wish to read them anymore
            while( currentLine < MAX_LINES ) {
                fgets(line, currentLineLength, file);

                if( line[ currentLineLength ] == '\n' ) {
                    // If we've reached the end of the line, add the line to the array of lines
                    debug( "Reached the end of the line. Length = %d", currentLineLength );
                    lines[currentLine] = line;
                    currentLine++;
                } else {
                    currentLineLength += 200;
                    line = realloc( line, currentLineLength );

                    debug( "Resizing line length. Length is now %d", currentLineLength );
                }
            }

            // Exit the loop if we've read the first 12,000 lines
            if( currentLine >= MAX_LINES ) {
                break;
            }
        }

        free( lines );
    } else {
        // Report errors
        debug( "Could not locate file with the name \"%s\"", filename);
        exit(1);
    }

    // Close our file handle
    //
    fclose( file );
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



    return 0;
}
