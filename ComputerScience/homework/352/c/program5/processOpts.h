/* FLAG INDICES */
#define SEED_INDEX          0
#define GAME_INDEX          1
#define HAND_SIZE_INDEX     2

#define NUMBER_OF_FLAGS     3

/* CONSTANT DEFINITIONS */
#define POKER               1
#define PINOCHLE            2
#define HEARTS              3
#define DEFAULT_HAND_SIZE   4

#define DEBUG               1

extern int debugEnabled;

/*
 * Option Summary:
 *
 * Usage: cardDealer -s=# [-p] [-P] [-h] [-H[=#]]
 * --usage       print this information and exit
 * -s --seed     initialize random number generator (required argument)
 * -p --poker    deal 5-card poker hands
 * -P --pinochle deal pinochle hands
 * -h --hearts   deal cards for hearts
 * -H --hands    number of hands to deal  (optional argument)
 *               Number of hands defaults to 4
 */
struct option longopts[] = {
    // User facing options
    {"usage",       no_argument,        NULL, 'u'},
    {"seed",        required_argument,  NULL, 's'},
    {"poker",       no_argument,        NULL, 'p'},
    {"pinochle",    no_argument,        NULL, 'P'},
    {"hearts",      no_argument,        NULL, 'h'},
    {"hands",       optional_argument,  NULL, 'H'},

    // Transparent options
    {"debug",       no_argument,    NULL, 'd'},
    {NULL,          0,              NULL,  0 }
};

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
