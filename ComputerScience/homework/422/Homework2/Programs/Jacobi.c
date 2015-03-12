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
} WorkerArgs;

/* Global variables */

// Our Jacobi grids
double** grid;
double** new_grid;

// The largest difference found across the grids
double maxDifference;

// The number of threads being used
int numThreads;

// the number of iterations it took to discover the answer
int numberOfIterations = 1;

// The largest possible difference between values
#define EPSILON 0.1

// Determines whether or not iterations have finished computing
bool finished = false;

// A barrier to ensure thread synchronization
pthread_barrier_t barrier;

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
        printf("Usage: ./Jacobi <gridSize> <numThreads>\n");
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
    sem_init(&maxSem, 0, 1);

    // Set global thread and barrier attributes
    pthread_attr_t attr;
    pthread_attr_init( &attr );
    pthread_attr_setscope( &attr, PTHREAD_SCOPE_SYSTEM );

    pthread_barrier_init(&barrier, NULL, numThreads);

    // Initialize max difference to be zero. This will be changed by the threads
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

        // The last thread will need to pick up a little bit of slack
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
    int threadId = wa->thread;
    int gridSize = wa->gridSize;
    int firstRow = wa->firstRow;
    int lastRow = wa->lastRow;
    sem_t *maxSem = wa->maxSem;

    while(!finished) {
        /*
         * We will have every worker thread calculate their particular partition of the grid. Each
         * thread will handle (gridSize / numThreads) rows in the grid with the last thread
         * potentially handling an extra (gridSize % num) rows.
         */
        for( int row = firstRow; row < lastRow; row++ ) {
            for( int column = 1; column <= gridSize; column++ ) {
                new_grid[row][column] = (grid[row - 1][column] + grid[row + 1][column]
                        + grid[row][column - 1] + grid[row][column + 1]) * 0.25;
            }
        }

        /*
         * Each thread will compute the maximum difference between iterations that it encountered
         * for its partition of the grid.
         */
        double localMax = 0.0;
        for( int row = firstRow; row < lastRow; row++ ) {
            for( int column = 1; column <= gridSize; column++ ) {
                localMax = MAX(localMax, fabs(new_grid[row][column] - grid[row][column]));
            }
        }

        /*
         * Every thread will update the global max to be the larger of either (1) the global max or
         * (2) their local max. This is operation is gated by a semaphore to ensure mutual
         * exclusion.
         */
        sem_wait(maxSem);
        maxDifference = MAX(maxDifference, localMax);
        sem_post(maxSem);

        /*
         * We must wait until all threads have arrived at this point. At this point, we know that
         * the value in maxDifference is its final value for this iteration.
         */
        pthread_barrier_wait(&barrier);

        /*
         * If the maxDifference value is less than the set EPSILON value, all threads will set
         * finished to true. This is redundent, but does not provide a measurable performance impact
         * overall. Setting finished to true will cause all threads to exit when they loop around to
         * the top of the loop.
         */
        if( maxDifference < EPSILON ) {
            //printf("[Thread %d] maxDifference < Epsilon!\n", threadId);
            finished = true;
        } else {
            /*
             * We want to have a single thread increment the number of iterations. Since there will
             * always be a thread will ID 0 in this solution (it is the first thread created), we
             * will use that thread.
             */
            if( threadId == 0 ) {
                numberOfIterations++;
            }

            /*
             * Now we will have each thread copy its rows into the final array. This operation could
             * be optimized out and will be in the future.
             */
            for( int i = firstRow; i < lastRow; i++ ) {
                for( int j = 0; j < (gridSize + 2); j++ ) {
                    grid[i][j] = new_grid[i][j];
                }
            }
        }

        /*
         * Here we must wait for the threads to finish their calculations involving maxDifference
         * before we reset it back to 0, so we barrier here.
         */
        pthread_barrier_wait(&barrier);

         // We will have the first thread (ID of 0) reset max difference to 0.0.
        if( threadId == 0 ) {
            maxDifference = 0.0;
        }

        /*
         * At this point, all calculations for this iteration are finished and maxDifference has
         * been reset to 0.0, allowing for the next iteration to begin. We barrier here to ensure
         * that all threads start with maxDifference having a value of 0.0. Removing the barrier
         * introduces the possibility that one thread will begin its iteration and change max
         * difference, only to have thread 0 reset it back to 0.0.
         */
        pthread_barrier_wait(&barrier);
    }

    return NULL;
}
