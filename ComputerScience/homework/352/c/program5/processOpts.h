/* FLAG INDICES */
#define SEED_INDEX          0
#define GAME_INDEX          1
#define HAND_SIZE_INDEX     2

/* CONSTANT DEFINITIONS */
#define POKER               1
#define PINOCHLE            2
#define HEARTS              3
#define DEFAULT_HAND_SIZE   4

#define DEBUG               1

/*
 * Processes the command line arguments and returns an array of flags that represent the various
 * settings for the program. The organization of the flags follows the constants defined in
 * processOpts.h
 *
 * Arguments:
 * argv  -- The list of command line arguments provided to main
 * argc  -- The number of command line arguments provided to main
 * flags -- The destination array for the flags to be set in
 */
void processOpts( char *argv[], int argc, int *flags );


/*
 * Prints debug information if the debug flag is enabled and if the option has been
 * passed into the command line using either --debug or -d.
 *
 * Arguments:
 * format   -- The format for printing. This follows the same rules as printf
 * VA_ARGS  -- The items that will be printed by printf
 */
void debug( const char *format, ... );
