#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <pthread.h>
#include <unistd.h>     // to use fork, getpid, sleep
#include <sys/mman.h>   // to use shared memory
#include <sys/wait.h>   // wait functions
#include <string.h>     // memcpy function

#include "quicksort.h"

typedef struct SortingArguments {
    // The lines to sort
    char **lines;

    // The number of lines being sorted
    int size;
} SortingArguments;

typedef struct MergeArguments {
    // The location that firstLength is at in the original array
    int start;

    // The first array to merge
    char **first;
    int firstLength;

    // The second array to merge
    char **second;
    int secondLength;
} MergeArguments;

typedef struct MergeReturn {
    // The start of the original the merged blocks in the original array
    int start;

    // The size of the merged block
    int size;

    // The result of the merge
    char **mergeResult;
} MergeReturn;

static inline void *SortingWorker( void *args ) {
    SortingArguments *sa = args;
    char **lines = sa->lines;
    int length = sa->size;

    quicksort( lines, length );

    return NULL;
}

static inline void *MergeWorker( void *args ) {
    MergeArguments *ma = args;
    char **res = merge(ma->first, ma->firstLength, ma->second, ma->secondLength);

    MergeReturn *mr = malloc( sizeof(MergeReturn) );
    mr->start = ma->start;
    mr->mergeResult = res;
    mr->size = ma->firstLength + ma->secondLength;
    return mr;
}

int main( int argc, char *argv[] ) {
    // Read command line arguments
    int numThreads = atoi(argv[1]);
    char *filename = argv[2];

    // Map the shared memory
    FILE *file = fopen(filename, "r");
    char **lines = calloc( MAX_LINES, sizeof(char *) );
    int length = 0;

    // Read lines from the file
    while( !feof(file) ) {
        char *line = calloc(LINE_LENGTH, sizeof(char));

        fgets(line, LINE_LENGTH, file);
        lines[length] = line;
        length++;
    }

    pthread_attr_t attr;
    pthread_t workers[numThreads];

    /* set global thread attributes */
    pthread_attr_init( &attr );
    pthread_attr_setscope( &attr, PTHREAD_SCOPE_SYSTEM );

    // Sort the lines
    clock_t start = clock();

    // Create threads for sorting
    int numElements = length / numThreads;
    for( int id = 0; id < numThreads; id++ ) {
        SortingArguments args;
        args.lines = lines + (id * numElements);
        args.size = numElements;

        int result = pthread_create( &workers[id], &attr, &SortingWorker, (void *)&args );

        if( result != 0 ) {
            fprintf( stderr, "There was an error creating the sort thread id=%d!\n", id );
            exit(1);
        }
    }

    // Wait for all the threads to finish
    for( int id = 0; id < numThreads; id++ ) {
        pthread_join(workers[id], NULL);
    }

    // Create merge threads
    numThreads /= 2;
    for( ; numThreads > 0; numThreads /= 2 ) {
        pthread_t mergeWorkers[numThreads];
        int numElements = length / numThreads;

        // Create merge workers
        for( int id = 0; id < numThreads; id++ ) {
            // Create the arguments for the merge step
            MergeArguments args;
            args.start = 2 * id * numElements;
            args.first = lines + (2 * id * numElements);
            args.firstLength = numElements;
            args.second = lines + (2 * (id + 1) * numElements);
            args.secondLength = numElements;

            // Create a thread to merge 2 array segments
            int result = pthread_create( &mergeWorkers[id], &attr, &MergeWorker, (void *)&args );

            if( result != 0 ) {
                fprintf( stderr, "There was an error creating the merge thread id=%d!\n", id );
                exit(1);
            }
        }

        // Join merge workers
        for( int id = 0; id < numThreads; id++ ) {
            // Get the result from the thread
            void *mergeResult;
            pthread_join(workers[id], &mergeResult);

            // Get the content of the return
            MergeReturn *mr = mergeResult;
            int mrStart = mr->size;
            char **res = mr->mergeResult;
            int size = mr->size;

            memcpy(lines + mrStart, res, size);
        }
    }

    double microseconds = ((double) (clock() - start) / CLOCKS_PER_SEC * 1000000 );

    // Print the lines and the information about the sorted data
    int seconds = (int)(microseconds / CLOCKS_PER_SEC);

    for( int i = 0; i < length; i++ ) {
        printf("%s", lines[i]);
    }

    fprintf( stderr, "runtime: %d seconds, %f microseconds\n", seconds, microseconds  );
}
