#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>

#define MAX(a,b) (((a)>(b))?(a):(b))

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

    int numberOfIterations = 0;

    // Print out information to stdout
    printf("N = %d\n", gridSize);
    printf("numProcs = %d\n", numThreads);
    printf("left = %.1f, top = %.1f, right = %.1f, bottom = %.1f\n", leftStart, topStart,
            rightStart, bottomStart);
    printf("EPSILON = %.1f\n", EPSILON);

    // Create our grids
    int array_size = gridSize + 2;
    double grid[gridSize + 2][gridSize + 2];//= new double[gridSize + 2][gridSize + 2];
    double new_grid[gridSize + 2][gridSize + 2];//= new double[gridSize + 2][gridSize + 2];

    for( int i = 0; i < array_size; i++ ) {
        for( int j = 0; j < array_size; j++ ) {
            grid[i][j] = 0.0;
            new_grid[i][j] = 0.0;
        }
    }

    // Set the top and bottom boundary points
    for( int i = 0; i < array_size; i++ ) {
        grid[0][i] = topStart;
        new_grid[0][i] = topStart;

        grid[array_size - 1][i] = bottomStart;
        new_grid[array_size - 1][i] = bottomStart;
    }

    // Set the left and right boundary points
    for( int i = 1; i < array_size - 1; i++ ) {
        grid[i][0] = leftStart;
        new_grid[i][0] = leftStart;

        grid[i][array_size - 1] = rightStart;
        new_grid[i][array_size - 1] = rightStart;
    }

    grid[array_size - 1][0] = bottomStart;

    printf("Starting values:\n");
    for( int i = 0; i < array_size; i++ ) {
        for( int j = 0; j < array_size; j++ ) {
            printf("%4.1f ", grid[i][j]);
        }

        printf("\n");
    }

    struct timeval start;
    gettimeofday(&start, NULL);

    // Perform the iterations
    for( ; ; numberOfIterations++ ) {

        // Compute all iterior points
        for( int i = 1; i <= gridSize; i++ ) {
            for( int j = 1; j <= gridSize; j++ ) {
                new_grid[i][j] = (grid[i - 1][j] + grid[i + 1][j] + grid[i][j - 1]
                        + grid[i][j + 1]) * 0.25;
            }
        }

        // Compute the maximal difference across the values
        double maxDifference = 0.0;
        for( int i = 1; i <= gridSize; i++ ) {
            for( int j = 1; j <= gridSize; j++ ) {
                maxDifference = MAX(maxDifference, fabs(new_grid[i][j] - grid[i][j]));
            }
        }

        // Determine if we've completed the iteration
        if( maxDifference < EPSILON ) {
            break;
        }

        // Copy the updated values in
        for( int i = 0; i < array_size; i++ ) {
            for( int j = 0; j < array_size; j++ ) {
                grid[i][j] = new_grid[i][j];
            }
        }
    }

    struct timeval end;
    gettimeofday(&end, NULL);


    int seconds = end.tv_sec - start.tv_sec;
    int micros = end.tv_usec - start.tv_usec;
    if ( end.tv_usec < start.tv_usec ) {
        micros += 1000000;
        seconds--;
    }

    printf("Number of iterations to complete: %d\n", numberOfIterations );
    printf("execution time = %d seconds, %d microseconds\n", seconds, micros);

    for( int i = 0; i < array_size; i++ ) {
        for( int j = 0; j < array_size; j++ ) {
            printf("%7.4f ", grid[i][j]);
        }

        printf("\n");
    }
}

/*inline int max( int a, int b ) {*/
    /*return a > b ? a : b;*/
/*}*/
