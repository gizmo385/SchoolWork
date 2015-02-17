#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <unistd.h>      // to use fork, getpid, sleep
#include <sys/mman.h>    // to use shared memory
#include <sys/wait.h>    // wait functions
#include <errno.h>       // to print error messages

#include "quicksort.h"

int main( int argc, char *argv[] ) {
    // Read command line arguments
    int numProcesses = atoi(argv[1]);
    char *filename = argv[2];

    // Map the shared memory
    FILE *file = fopen(filename, "r");
    char **lines = mmap(NULL, (MAX_LINES * LINE_LENGTH * (sizeof (char *))),
            PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    int length = 0;

    // Read lines from the file
    while( !feof(file) ) {
        char *line = calloc(LINE_LENGTH, sizeof(char));

        fgets(line, LINE_LENGTH, file);
        lines[length] = line;
        length++;
    }

    // Determine chunk size
    int elementsPerProc = length / numProcesses;
    int remainingElements = length % numProcesses;

    // Sort the lines
    clock_t start = clock();

    // Create children processes for sorting
    for( int procNum = 0; procNum < numProcesses; procNum++ ) {
        pid_t kidpid = fork();

        if( kidpid < 0 ) {
            fprintf(stderr, "Fork failed!\n");
            exit(1);
        } else if( kidpid == 0 ) {
            // The last process gets a slightly larger chunk
            // Call quicksort on each process with 1/(numProcesses) of the elements
            // Then exit

            if( procNum == numProcesses ) {
                quicksort(lines + (procNum * elementsPerProc),
                        elementsPerProc + remainingElements);
            } else {
                quicksort( lines + (procNum * elementsPerProc), elementsPerProc);
            }

            exit(getpid());
        }
    }

    // Wait for all of the children to finish executing
    int kid_status;
    for( int procNum = 0; procNum < numProcesses; procNum++ ) {
        wait(&kid_status);

        if( WIFEXITED(kid_status) ) {
            // The process has successfully exited
            // This means that a portion of the array has been sorted
        } else {
            // One of the children died without calling exit
        }
    }

    int numMergeProcesses = numProcesses / 2;
    while( numMergeProcesses > 0 ) {
        // Fork into processes to start merge
        for( int procNum = 0; procNum < numMergeProcesses; procNum++ ) {
            pid_t kidpid = fork();

            if( kidpid < 0 ) {
                fprintf(stderr, "Fork failed!\n");
                exit(1);
            } else if( kidpid == 0 ) {

            }
        }

        // Wait for merge processes for finish
        for( int numCompleted = 0; numCompleted < numMergeProcesses; numCompleted++ ) {

        }

        // Cut the number of merge processes in half
        mergeProcesses /= 2;
    }

    double microseconds = ((double) (clock() - start) / CLOCKS_PER_SEC * 1000000 );

    // Print the lines and the information about the sorted data
    int seconds = (int)(microseconds / CLOCKS_PER_SEC);

    for( int i = 0; i < length; i++ ) {
        printf("%s", lines[i]);
    }

    printf( "runtime: %d seconds, %f microseconds\n", seconds, microseconds  );

    // Free the memory used by the program
    for( int i = 0; i < length; i++ ) {
        free(lines[i]);
    }

    munmap( lines, (MAX_LINES * LINE_LENGTH * (sizeof (char *))) );
    fclose( file );
}
