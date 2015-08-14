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

    int id;
} SortingArguments;

typedef struct MergeArguments {
    // The location that firstLength is at in the original array
    int start;

    // The array to memcpy into
    char **lines;

    // The first array to merge
    char **first;
    int firstLength;

    // The second array to merge
    char **second;
    int secondLength;

    // The id for the thread running the worker
    int id;
} MergeArguments;

static inline void *SortingWorker( void *args ) {
    SortingArguments *sa = args;
    char **lines = sa->lines;
    int length = sa->size;
    /*int id = sa->id;*/

    /*printf( "Sorting worker:\n");*/
    /*printf( "(id = %d) - Lines = %p\n", id, lines );*/
    /*printf( "(id = %d) - Length = %d\n", id, length);*/

    quicksort( lines, length );

    return NULL;
}

static inline void *MergeWorker( void *args ) {
    // Get the arguments
    MergeArguments *ma = args;
    char **lines = ma->lines;
    int start = ma->start;
    /*int id = ma->id;*/

    /*printf("\n\n(id = %d) - Values in first:\n", id);*/
    /*for( int i = 0; i < ma->firstLength; i++ ) {*/
        /*printf("(id = %d) - %s", id, ma->first[i]);*/
    /*}*/

    /*printf("\n---\n");*/

    /*printf("(id = %d) - Values in second:\n", id);*/
    /*for( int i = 0; i < ma->secondLength; i++ ) {*/
        /*printf("(id = %d) - %s", id, ma->second[i]);*/
    /*}*/

    /*printf("\n---\n");*/

    // Merge the lines
    char **res = merge(ma->first, ma->firstLength, ma->second, ma->secondLength);

    /*printf( "(id = %d) - Merge results:\n", id);*/
    /*for( int i = 0; i < ma->firstLength + ma->secondLength; i++ ) {*/
        /*printf( "(id = %d) %s\n", id, res[i] );*/
    /*}*/

    memcpy( lines + start, res, ma->firstLength + ma->secondLength );

    return NULL;
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
        SortingArguments *args = malloc( sizeof(SortingArguments) );
        args->lines = lines + (id * numElements);
        args->size = numElements;
        args->id = id;

        if( (id + 1) == numThreads ) {
            args->size += (length % numThreads);
        }

        /*printf( "Creating sorting thread:\n");*/
        /*printf( "(id = %d) - Lines = %p\n", id, lines + (id * numElements) );*/
        /*printf( "(id = %d) - Length = %d\n", id, numElements);*/

        int result = pthread_create( &workers[id], &attr, &SortingWorker, (void *)args );

        if( result != 0 ) {
            fprintf( stderr, "There was an error creating the sort thread id=%d!\n", id );
            exit(1);
        }
    }

    // Wait for all the threads to finish
    for( int id = 0; id < numThreads; id++ ) {
        pthread_join(workers[id], NULL);
    }

    /*printf("After sort, before merge:\n");*/
    /*for( int i = 0; i < length; i++ ) {*/
        /*printf( "%s", lines[i]);*/
    /*}*/

    /*printf("\n\n");*/

    // Create merge threads
    numThreads /= 2;
    for( ; numThreads > 0; numThreads /= 2 ) {
        pthread_t mergeWorkers[numThreads];
        int numElements = length / (numThreads * 2);
        int remainder = length % (numThreads * 2);

        // Create merge workers
        for( int id = 0; id < numThreads; id++ ) {
            // Create the arguments for the merge step
            MergeArguments *args = malloc(sizeof(MergeArguments));
            args->start = id * numElements;
            args->lines = lines;
            args->first = lines + (id * numElements);
            args->firstLength = numElements;
            args->second = lines + ((id + 1) * numElements);
            args->secondLength = numElements;

            if( (id + 1) == numThreads ) {
                args->secondLength += remainder;
            }

            /*printf( "Creating merging thread:\n");*/
            /*printf( "(id = %d) - Start = %d\n", id, args->start );*/
            /*printf( "(id = %d) - Lines = %p\n", id, args->lines );*/
            /*printf( "(id = %d) - First = %p\n", id, args->first );*/
            /*printf( "(id = %d) - FirstLength = %d\n", id, args->firstLength );*/
            /*printf( "(id = %d) - Second = %p\n", id, args->second );*/
            /*printf( "(id = %d) - secondLength = %d\n", id, args->secondLength );*/

            // Create a thread to merge 2 array segments
            int result = pthread_create( &mergeWorkers[id], &attr, &MergeWorker, (void *)args );

            if( result != 0 ) {
                fprintf( stderr, "There was an error creating the merge thread id=%d!\n", id );
                exit(1);
            }
        }

        // Join merge workers
        for( int id = 0; id < numThreads; id++ ) {
            pthread_join(mergeWorkers[id], NULL);
        }
    }

    double microseconds = ((double) (clock() - start) / CLOCKS_PER_SEC * 1000000 );

    // Print the lines and the information about the sorted data
    int seconds = (int)(microseconds / CLOCKS_PER_SEC);

    /*printf("\nAfter merge:\n");*/
    for( int i = 0; i < length; i++ ) {
        printf("%s", lines[i]);
    }

    fprintf( stderr, "runtime: %d seconds, %f microseconds\n", seconds, microseconds  );
}
