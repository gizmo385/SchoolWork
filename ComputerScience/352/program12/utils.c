/*
 * General utility functions. This includes functions to write debug statements and make assertions.
 * This assertions and debug statements can be turned off by changing the debug reporting level
 * using the setDebugReporting function.
 *
 * Created by Christopher Chapline.
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "utils.h"

#ifndef GLOBAL_DEBUG_LEVEL
#define GLOBAL_DEBUG_LEVEL

int globalDebugLevel;

#endif

/*
 * Prints debug information if the debug flag is enabled and if the option has been
 * passed into the command line using either --debug or -d.
 *
 * Arguments:
 * debugType -- The type of error message that is being reported
 * format    -- The format for printing. This follows the same rules as printf
 * VA_ARGS   -- The items that will be printed by printf
 */
void debug( int debugType, const char *format, ... ) {
    if( (globalDebugLevel & debugType) == debugType ) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}

/*
 * Prints debug information if the debug flag is enabled and if the option has been
 * passed into the command line using either --debug or -d.
 *
 * Arguments:
 * debugType -- The type of error message that is being reported
 * format    -- The format for printing. This follows the same rules as printf
 * args      -- The args to print
 */
void vdebug( int debugType, const char *format, va_list args ) {
    if( (globalDebugLevel & debugType) == debugType ) {
        vprintf(format, args);
    }
}

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
void setDebugReporting( int debugLevel ) {
    globalDebugLevel = debugLevel;
    debug( E_INFO, "Debug level set to 0x%X\n", globalDebugLevel );
}

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
void assertTrue( int value, char *failureFormat, ... ) {
    if( ! value ) {
        va_list args;
        va_start(args, failureFormat);
        vdebug(E_ERROR, failureFormat, args);
        va_end(args);
    }
}

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
void assertFalse( int value, char *failureFormat, ... ) {
    if( value ) {
        va_list args;
        va_start(args, failureFormat);
        vdebug(E_ERROR, failureFormat, args);
        va_end(args);
    }
}

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
void assertNull( void *value, char *failureFormat, ... ) {
    if( value != NULL ) {
        va_list args;
        va_start(args, failureFormat);
        vdebug(E_ERROR, failureFormat, args);
        va_end(args);
    }
}

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
void assertNotNull( void *value, char *failureFormat, ... ) {
    if( value == NULL ) {
        va_list args;
        va_start(args, failureFormat);
        vdebug(E_ERROR, failureFormat, args);
        va_end(args);
    }
}
