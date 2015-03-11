#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <stdbool.h>
#include <math.h>

#include <sys/time.h>

#include <semaphore.h>
#include <pthread.h>

#define MAX(a,b) (((a)>(b))?(a):(b))

/* Our structure definitions */
typedef struct WorkerArgs {
    int thread, gridSize, firstRow, lastRow;
    sem_t *maxSem;
    sem_t *countSemaphore;
    sem_t *changeCountSemaphore;
    pthread_barrier_t *barrier;
} WorkerArgs;

/* Global variables */

// Our Jacobi grids
double** grid;
double** new_grid;

// The largest difference found across the grids
double maxDifference;

// The number of threads being used
int numThreads;

int count;

// the number of iterations it took to discover the answer
int numberOfIterations = 1;

// The largest possible difference between values
#define EPSILON 0.1

// Determines whether or not iterations have finished computing
bool finished = false;

/* Function prototypes */
void* iterationWorker(void* arg);


/**
 * Performs Jacobi iteration using a grid of a specified size and parsing it with
 * a pre-determined number of threads.
 *
 */
int main( int argc, char *argv[] ) {
    // Ensure that the user has entered the correct number of arguments
    if( argc < 3 ) {
        printf("Usage: java Jacobi <gridSize> <numThreads>\n");
        exit(1);
    }

    // Starting edge values for the grid (optional arguments)
    double leftStart    = argc >= 4 ? atof(argv[3]) : 1.0,
           topStart     = argc >= 5 ? atof(argv[4]) : 1.0,
           rightStart   = argc >= 6 ? atof(argv[5]) : 80.0,
           bottomStart  = argc >= 7 ? atof(argv[6]) : 80.0;

    // Read in the command line arguments
    int gridSize = atoi( argv[1] );
    numThreads = atoi( argv[2] );

    // Iterations and batch size
    int rowsPerThread = gridSize / numThreads;
    int leftover = gridSize % numThreads;

    // Initialize our grids
    int arraySize = gridSize + 2;
    grid = calloc(arraySize, sizeof(double*));
    new_grid = calloc(arraySize, sizeof(double*));

    for( int i = 0; i < arraySize; i++ ) {
        grid[i] = calloc(arraySize, sizeof(double));
        new_grid[i] = calloc(arraySize, sizeof(double));

        for( int j = 0; j < arraySize; j++ ) {
            grid[i][j] = 0.0;
            new_grid[i][j] = 0.0;
        }
    }

    // Set the top and bottom boundary points
    for( int i = 0; i < arraySize; i++ ) {
        grid[0][i] = topStart;
        new_grid[0][i] = topStart;

        grid[arraySize - 1][i] = bottomStart;
        new_grid[arraySize - 1][i] = bottomStart;
    }

    // Set the left and right boundary points
    for( int i = 1; i < arraySize - 1; i++ ) {
        grid[i][0] = leftStart;
        new_grid[i][0] = leftStart;

        grid[i][arraySize - 1] = rightStart;
        new_grid[i][arraySize - 1] = rightStart;
    }

    grid[arraySize - 1][0] = bottomStart;

    // Semaphore for interacting with maxDifference
    sem_t maxSem;
    sem_t countSemaphore;
    sem_t changeCountSemaphore;

    sem_init(&maxSem, 0, 1);
    /*sem_init(&countSemaphore, 0, 0);*/
    sem_init(&countSemaphore, 0, numThreads - 1);
    sem_init(&changeCountSemaphore, 0, 1);

    // Set global thread attributes
    pthread_attr_t attr;
    pthread_attr_init( &attr );
    pthread_attr_setscope( &attr, PTHREAD_SCOPE_SYSTEM );

    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, NULL, numThreads);

    maxDifference = 0.0;

    // Start the timer
    struct timeval start;
    gettimeofday(&start, NULL);
    pthread_t threads[numThreads];

    // Create threads to perform the iterations
    for( int thread = 0; thread < numThreads; thread++ ) {

        // Set up the worker
        int startingRow = (thread * rowsPerThread) + 1;
        int endingRow = startingRow + rowsPerThread;

        // Create worker arguments
        WorkerArgs *wa = malloc(sizeof(WorkerArgs));
        wa->thread = thread;
        wa->gridSize = gridSize;
        wa->firstRow = startingRow;
        wa->lastRow = endingRow;
        wa->maxSem = &maxSem;
        wa->barrier = &barrier;
        wa->changeCountSemaphore = &changeCountSemaphore;
        wa->countSemaphore = &countSemaphore;

        if( (thread + 1) == numThreads ) {
            wa->lastRow += leftover;
        }

        // Create our thread
        pthread_create(&(threads[thread]), &attr, iterationWorker, wa);
    }

    for( int thread = 0; thread < numThreads; thread++ ) {
        pthread_join(threads[thread], NULL);
    }

    // Stop the timer
    struct timeval end;
    gettimeofday(&end, NULL);

    // Determine the number of seconds
    int seconds = end.tv_sec - start.tv_sec;
    int micros = end.tv_usec - start.tv_usec;
    if ( end.tv_usec < start.tv_usec ) {
        micros += 1000000;
        seconds--;
    }

    // Standard out
    printf("Finished %d iterations in %d seconds, %d microseconds using %d threads\n",
            numberOfIterations, seconds, micros, numThreads);

    // Write output to a file
    FILE* outputFile = fopen("CJacobiResults.txt", "w");
    fprintf(outputFile, "Grid size: %d\n", gridSize);
    fprintf(outputFile, "Number of threads: %d\n", numThreads);
    fprintf(outputFile, "Edge values: left = %f, right = %f, top = %f, bottom = %f\n",
            leftStart, rightStart, topStart, bottomStart);
    fprintf(outputFile, "Epsilon: %f\n\n", EPSILON);

    // Write lines to the file
    for( int i = 0; i < arraySize; i++ ) {
        for( int j = 0; j < arraySize; j++ ) {
            fprintf(outputFile, "%7.4f ", grid[i][j]);
        }

        fprintf(outputFile, "\n");
    }
}

/**
 * Will perform Jacobi iterations on the partition of the grid. This will terminated after
 * acquiring a lock on a semaphore and comparing its local maximum difference to the maximum
 * global distance.
 */
void* iterationWorker(void* arg) {
    // Extract arguments
    WorkerArgs *wa = (WorkerArgs*)arg;
    /*int threadId = wa->thread;*/
    int gridSize = wa->gridSize;
    int firstRow = wa->firstRow;
    int lastRow = wa->lastRow;
    sem_t *maxSem = wa->maxSem;
    sem_t *countSemaphore = wa->countSemaphore;
    sem_t *changeCountSemaphore = wa->changeCountSemaphore;

    /*printf("Thread [%d] checking in!\n", threadId);*/

    while(!finished) {
        /*sem_wait(iterationSemaphore);*/

        // Calculate a subset of the rows
        for( int row = firstRow; row < lastRow; row++ ) {
            for( int column = 1; column <= gridSize; column++ ) {
                new_grid[row][column] = (grid[row - 1][column] + grid[row + 1][column]
                        + grid[row][column - 1] + grid[row][column + 1]) * 0.25;
            }
        }

        // Compute the maximal difference amongst these rows
        double localMax = 0.0;
        for( int row = firstRow; row < lastRow; row++ ) {
            for( int column = 1; column <= gridSize; column++ ) {
                localMax = MAX(localMax, fabs(new_grid[row][column] - grid[row][column]));
            }
        }

        // Determine if the local max is larger than the global max
        sem_wait(maxSem);
        maxDifference = MAX(maxDifference, localMax);
        sem_post(maxSem);

#if 0
        pthread_barrier_wait(wa->barrier);

        if( maxDifference < EPSILON ) {
            return NULL;
        } else {
            if( threadId == 0 ) {
                numberOfIterations++;
            }


        }
        pthread_barrier_wait(wa->barrier);
#endif

/*#if 0*/
        // Increment the counter
        sem_wait(changeCountSemaphore);
        count++;

        // Wait for the other threads to finish
        // The first thread will update global state
        if( count == (numThreads - 1) ) {
            /*sem_wait(countSemaphore);*/

            // Check if we've finished iterating
            /*if( count == 17 ) {*/
            if( maxDifference < EPSILON ) {
                finished = true;
            } else {
                // If haven't, update the state
                maxDifference = 0.0;
                numberOfIterations++;
                /*printf("[Thread %d] Updating global state\n", threadId);*/
            }

            int old_count = count;
            count = 0;

            for( int i = 0; i < old_count; i++ ) {
                sem_post(countSemaphore);
            }
        } else {
            /*printf("[Thread %d] Waiting for count semaphore...\n", threadId);*/
            sem_wait(countSemaphore);
        }

        sem_post(changeCountSemaphore);

        if( !finished ) {
            for( int i = firstRow; i < lastRow; i++ ) {
                for( int j = 0; j < (gridSize + 2); j++ ) {
                    grid[i][j] = new_grid[i][j];
                }
            }
        }
        /*#endif*/

    }

    return NULL;
}
