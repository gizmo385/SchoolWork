/* INCLUDES */
#include <getopt.h>
#include <stdio.h>

/* FLAG INDICES */
#define SEED_INDEX          0
#define NUMBER_OF_HANDS     1
#define POKER_FLAG          2
#define PINOCHLE_FLAG       3
#define HEARTS_FLAG         4
#define NUMBER_OF_FLAGS     5

/* CONSTANT DEFINITIONS */
#define DEFAULT_HAND_SIZE   4

/* DEBUG OPTIONS */
#define DEBUG               1
extern int debugEnabled;


/*
 * Processes the command line arguments and returns an array of flags that represent the various
 * settings for the program. The organization of the flags follows the constants defined in
 * processOpts.h
 *
 * Arguments:
 * argv     -- The list of command line arguments provided to main
 * argc     -- The number of command line arguments provided to main
 * progName -- The name of the program
 * flags    -- The destination array for the flags to be set in
 */
extern void processOpts( char *argv[], int argc, char *progName, int *flags );


/*
 * Prints debug information if the debug flag is enabled and if the option has been
 * passed into the command line using either --debug or -d.
 *
 * Arguments:
 * format   -- The format for printing. This follows the same rules as printf
 * VA_ARGS  -- The items that will be printed by printf
 */
extern void debug( const char *format, ... );

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
void usage( char *progName, FILE *file);
