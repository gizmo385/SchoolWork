#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Command line option modes */
#define CASE_SENSITIVE      0
#define NON_CASE_SENSITIVE  1
#define REVERSED            2
#define NOT_REVERSED        3
#define BOTH                4

/* Function Prototypes */
void usage( char *progName, FILE *file);

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
    {"usage",       no_argument,    NULL,  0 },
    {"insensitive", no_argument,    NULL, 'i'},
    {"reverse",     no_argument,    NULL, 'r'},
    {"both",        no_argument,    NULL, 'b'},
    {NULL,          0,              NULL,  0 }
};

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

int main(int argc, char *argv[]) {


    return 0;
}
