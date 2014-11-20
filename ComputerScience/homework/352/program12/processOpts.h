#ifndef PROCESSOPTS_H
#define PROCESSOPTS_H

/* INCLUDES */
#include <getopt.h>
#include <stdio.h>

/* OPTION FLAGS */
#define REVERSE_ORDER       1
#define NORMAL_ORDER        2

#define CASE_SENSITIVE      1
#define CASE_INSENSITIVE    2

/* FLAG INDICES */
#define CASE_INDEX          0
#define ORDER_INDEX         1
#define NUMBER_OF_FLAGS     2

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
extern void usage( char *progName, FILE *file);

#endif
