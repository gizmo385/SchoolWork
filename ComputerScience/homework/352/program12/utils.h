/*
 * General utility functions. This includes functions to write debug statements and make assertions.
 * This assertions and debug statements can be turned off by changing the debug reporting level
 * using the setDebugReporting function.
 *
 * Created by Christopher Chapline.
 *
 */
#ifndef UTILS_H
#define UTILS_H

/* Debug levels */
#define E_FATAL     1
#define E_ERROR     2
#define E_WARNING   4
#define E_DEBUG     8
#define E_INFO      16
#define E_ALL       E_FATAL | E_ERROR | E_WARNING | E_DEBUG | E_INFO

extern int globalDebugLevel;

/*
 * Prints debug information if the debug flag is enabled and if the option has been
 * passed into the command line using either --debug or -d.
 *
 * Arguments:
 * debugType -- The type of error message that is being reported
 * format    -- The format for printing. This follows the same rules as printf
 * VA_ARGS   -- The items that will be printed by printf
 */
extern void debug( int debugType, const char *format, ... );

/*
 * Sets the types of debug messages that will be printed.
 *
 * Arguments:
 * debugLevel -- This is an integer that is formed by taking the defined debug levels that you wish
 *               to enable and performing a bitwise OR on all of them. An example is provided below.
 *
 * Enabling E_FATAL, E_ERROR, and E_DEBUG:
 * setDebugReporting( E_FATAL | E_ERROR | E_DEBUG );
 */
extern void setDebugReporting( int debugLevel );

/*
 * Asserts that the value is true, otherwise, it will print the failureFormat and variable
 * substitutions at an E_ERROR debug level.
 *
 * Arguments:
 * value         -- The value to test for truthiness. If this is true, the function will exit.
 * failureFormat -- The format for printing in the case that the value is not true. This follows the
 *                  same format rules as printf.
 * VA_ARGS       -- The items that will be printed at substitution locations in the failureFormat.
 */
extern void assertTrue( int value, char *failureFormat, ... );

/*
 * Asserts that the value is false, otherwise, it will print the failureFormat and variable
 * substitutions at an E_ERROR debug level.
 *
 * Arguments:
 * value         -- The value to test for falsiness. If this is true, the function will exit.
 * failureFormat -- The format for printing in the case that the value is true. This follows the
 *                  same format rules as printf.
 * VA_ARGS       -- The items that will be printed at substitution locations in the failureFormat.
 */
extern void assertFalse( int value, char *failureFormat, ... );

/*
 * Asserts that the value is NULL, otherwise, it will print the failureFormat and variable
 * substitutions at an E_ERROR debug level.
 *
 * Arguments:
 * value         -- The value to test for NULL. If this is NULL, the function will exit.
 * failureFormat -- The format for printing in the case that the value is not NULL. This follows the
 *                  same format rules as printf.
 * VA_ARGS       -- The items that will be printed at substitution locations in the failureFormat.
 */
extern void assertNull( void *value, char *failureFormat, ... );

/*
 * Asserts that the value is not NULL, otherwise, it will print the failureFormat and variable
 * substitutions at an E_ERROR debug level.
 *
 * Arguments:
 * value         -- The value to test for not NULL. If this is not NULL, the function will exit.
 * failureFormat -- The format for printing in the case that the value is NULL. This follows the
 *                  same format rules as printf.
 * VA_ARGS       -- The items that will be printed at substitution locations in the failureFormat.
 */
extern void assertNotNull( void *value, char *failureFormat, ... );

#endif
