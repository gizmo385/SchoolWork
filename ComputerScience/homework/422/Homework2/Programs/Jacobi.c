#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <semaphore.h>
#include <pthread.h>

#define MAX(a,b) (((a)>(b))?(a):(b))

// Structures
typedef struct WorkerArgs {
    int gridSize;               // The size of the interior grid being iterated over
    int firstRow;               // The row that the iterations will being on
    int lastRow;                // The row that the iterations will terminate on
    double *maxDifference;      // A pointer to the variable monitoring the max difference
    sem_t* maxSem;              // A semaphore for controlling access to maxDifference
    sem_t* threadsSemaphore;    // A semaphore that counts the number of active threads

    double*** grid;             // A pointer to the 2D grid
    double*** new_grid;         // A pointer to the 2D grid that iterations will enter into
} WorkerArgs;

// Function prototypes
void* iterationWorker( void* arg);

int main( int argc, char *argv[] ) {
    // The largest difference between successive iterations that we will terminate on
    const double EPSILON = 0.1;

    // Ensure that the user has entered the correct number of arguments
    if( argc < 2 ) {
        fprintf(stderr, "Usage: java Jacobi <gridSize> <numThreads>\n");
        exit(1);
    }

    // Starting edge values for the grid (optional arguments)
    double leftStart    = argc >= 4 ? atof(argv[3]) : 1.0,
           topStart     = argc >= 5 ? atof(argv[4]) : 1.0,
           rightStart   = argc >= 6 ? atof(argv[5]) : 80.0,
           bottomStart  = argc >= 7 ? atof(argv[6]) : 80.0;

    // Read in the command line arguments
    int gridSize = atoi( argv[1] );
    int numThreads = atoi( argv[2] );

    // Calculate grid information
    int numberOfIterations = 0;
    int rowsPerThread = gridSize / numThreads;
    int leftover = gridSize % numThreads;

    // Print out information to stdout
    printf("N = %d\n", gridSize);
    printf("numProcs = %d\n", numThreads);
    printf("left = %.1f, top = %.1f, right = %.1f, bottom = %.1f\n", leftStart, topStart,
            rightStart, bottomStart);
    printf("EPSILON = %.1f\n", EPSILON);

    // Create our grids
    int arraySize = gridSize + 2;
    double** grid = calloc( arraySize, sizeof(double*) );
    double** new_grid = calloc( arraySize, sizeof(double*) );

    // Intialize our grid
    for( int i = 0; i < arraySize; i++ ) {
        grid[i] = calloc( arraySize, sizeof(double) );
        new_grid[i] = calloc( arraySize, sizeof(double) );
    }

    for( int i = 0; i < arraySize; i++ ) {
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

    // Create our semaphores
    sem_t maxSem, threadsSemaphore;
    sem_init(&maxSem, 0, 1);
    sem_init(&threadsSemaphore, 0, numThreads);

    // Set up pthreads
    pthread_attr_t attr;
    pthread_attr_init( &attr );
    pthread_attr_setscope( &attr, PTHREAD_SCOPE_SYSTEM );

    // Start the timer
    struct timeval start;
    gettimeofday(&start, NULL);

    // Perform the iterations
    for( ; ; numberOfIterations++ ) {
        double maxDifference = 0.0;

        // Create workers
        for( int thread = 0; thread < numThreads; thread++ ) {
            printf( "[Iteration %d] Created thread %d\n", numberOfIterations, thread);
            sem_wait(&threadsSemaphore);
            int startingRow = (thread * rowsPerThread) + 1;
            int endingRow = startingRow + 1;

            // Set up our arguments for the thread
            WorkerArgs wa;
            wa.gridSize = gridSize;
            wa.firstRow = startingRow;
            wa.maxDifference = &maxDifference;
            wa.maxSem = &maxSem;
            wa.threadsSemaphore = &threadsSemaphore;
            wa.grid = &grid;
            wa.new_grid = &new_grid;

            // Handle edge case
            if( (thread + 1) == numThreads ) {
                wa.lastRow = endingRow + leftover;
            } else {
                wa.lastRow = endingRow;
            }

            // Create the thread
            pthread_t thread;
            pthread_create(&thread, &attr, iterationWorker, (void*)&wa);
        }

        // Wait until all threads have finished
        int val;
        do {
            sem_getvalue(&threadsSemaphore, &val);
        } while( val != numThreads );

        // Determine if we've completed the iteration
        printf("[Iteration %d] maxDifference = %f\n", numberOfIterations, maxDifference);
        if( maxDifference < EPSILON ) {
            break;
        }

        // Copy the updated values in
        for( int i = 0; i < arraySize; i++ ) {
            for( int j = 0; j < arraySize; j++ ) {
                grid[i][j] = new_grid[i][j];
            }
        }
    }

    // Stop timer
    struct timeval end;
    gettimeofday(&end, NULL);

    // Determine the number of seconds
    int seconds = end.tv_sec - start.tv_sec;
    int micros = end.tv_usec - start.tv_usec;
    if ( end.tv_usec < start.tv_usec ) {
        micros += 1000000;
        seconds--;
    }

    // Print information about the iterations
    printf("Number of iterations to complete: %d\n", numberOfIterations );
    printf("execution time = %d seconds, %d microseconds\n", seconds, micros);

    for( int i = 0; i < arraySize; i++ ) {
        for( int j = 0; j < arraySize; j++ ) {
            printf("%7.4f ", grid[i][j]);
        }

        printf("\n");
    }

    // Cleanup
    for( int i = 0; i < arraySize; i++ ) {
        free( grid[i] );
        free( new_grid[i] );
    }

    free(grid);
    free(new_grid);
}

void* iterationWorker( void* arg) {
    // Extract arguments
    WorkerArgs* wa = arg;
    double*** grid = wa->grid;
    double*** new_grid = wa->new_grid;
    int firstRow = wa->firstRow;
    int lastRow = wa->lastRow;
    int gridSize = wa->gridSize;

    // Calculate a subset of the rows
    for( int row = firstRow; row < lastRow; row++ ) {
        for( int column = 1; column <= gridSize; column++ ) {
            (*new_grid)[row][column] = ((*grid)[row - 1][column] + (*grid)[row + 1][column]
                    + (*grid)[row][column - 1] + (*grid)[row][column + 1]) * 0.25;
        }
    }

    // Compute the maximal difference amongst these rows
    double localMax = 0.0;
    for( int row = firstRow; row < lastRow; row++ ) {
        for( int column = 1; column <= gridSize; column++ ) {
            localMax = MAX(localMax, fabs( ((*new_grid)[row][column] - (*grid)[row][column])) );
        }
    }

    // Determine if the local max is larger than the global max
    sem_wait(wa->maxSem);
    *(wa->maxDifference) = MAX(localMax, *(wa->maxDifference));
    sem_post(wa->maxSem);

    // Release permit from threads semaphore
    sem_post(wa->threadsSemaphore);

    return NULL;
}
