import static java.lang.Math.max;
import static java.lang.Math.abs;

import java.util.Arrays;
import java.util.concurrent.Semaphore;
import java.util.concurrent.CountDownLatch;

public class Jacobi {

    private static double[][] grid, new_grid;
    private static double maxDifference;

    public static void main( String[] args ) {
        // The largest difference between successive iterations that we will terminate on
        final double EPSILON = 0.1;

        // Ensure that the user has entered the correct number of arguments
        if( args.length < 2 ) {
            System.err.println("Usage: java Jacobi <gridSize> <numThreads>");
            System.exit(1);
        }

        // Starting edge values for the grid (optional arguments)
        double leftStart    = args.length >= 3 ? Double.parseDouble(args[2]) : 1.0,
               topStart     = args.length >= 4 ? Double.parseDouble(args[3]) : 1.0,
               rightStart   = args.length >= 5 ? Double.parseDouble(args[4]) : 80.0,
               bottomStart  = args.length >= 6 ? Double.parseDouble(args[5]) : 80.0;

        // Read in the command line arguments
        int gridSize = Integer.parseInt( args[0] );
        int numThreads = Integer.parseInt( args[1] );

        int numberOfIterations = 0;
        int rowsPerThread = gridSize / numThreads;
        int leftover = gridSize % numThreads;

        // Print out information to stdout
        System.out.printf("N = %d\n", gridSize);
        System.out.printf("numProcs = %d\n", numThreads);
        System.out.printf("left = %.1f, top = %.1f, right = %.1f, bottom = %.1f\n", leftStart, topStart,
                rightStart, bottomStart);
        System.out.printf("EPSILON = %.1f\n", EPSILON);

        // Initialize our grids
        grid = new double[gridSize + 2][gridSize + 2];
        new_grid = new double[gridSize + 2][gridSize + 2];

        // Set the top and bottom boundary points
        for( int i = 0; i < grid.length; i++ ) {
            grid[0][i] = topStart;
            new_grid[0][i] = topStart;

            grid[grid.length - 1][i] = bottomStart;
            new_grid[grid.length - 1][i] = bottomStart;
        }

        // Set the left and right boundary points
        for( int i = 1; i < grid.length - 1; i++ ) {
            grid[i][0] = leftStart;
            new_grid[i][0] = leftStart;

            grid[i][grid.length - 1] = rightStart;
            new_grid[i][grid.length - 1] = rightStart;
        }

        grid[grid.length - 1][0] = bottomStart;

        // Start the timer
        long start = System.nanoTime();

        // Semaphore for interacting with maxDifference
        Semaphore maxSem = new Semaphore(1);

        // Perform the iterations
        for( ; ; numberOfIterations++ ) {
            maxDifference = 0.0;
            CountDownLatch counter = new CountDownLatch(numThreads);

            // Create threads to perform the iterations
            for( int thread = 0; thread < numThreads; thread++ ) {
                int startingRow = (thread * rowsPerThread) + 1;
                int endingRow = startingRow + rowsPerThread;

                WorkerThread worker;

                if( (thread + 1) == numThreads ) {
                    worker = new WorkerThread(thread, gridSize, startingRow, endingRow + leftover,
                            maxSem, counter);
                } else {
                    worker = new WorkerThread(thread, gridSize, startingRow, endingRow, maxSem,
                            counter);
                }

                worker.start();
            }

            // Block until the counter reaches zero
            try {
                counter.await();
            } catch( InterruptedException ie ) {
                System.err.println("Main thread interrupted on await(counter)!");
                System.exit(1);
            }

            // Determine if we've completed the iteration
            if( maxDifference < EPSILON ) {
                break;
            }

            // Copy the updated values in
            for( int i = 0; i < grid.length; i++ ) {
                for( int j = 0; j < grid.length; j++ ) {
                    grid[i][j] = new_grid[i][j];
                }
            }
        }

        // Stop the timer
        long finish = System.nanoTime();

        System.out.printf("Number of iterations to complete: %d\n", numberOfIterations );
        System.out.printf("Time to complete: %d microseconds\n", (finish - start) / 1000 );

        for( int i = 0; i < grid.length; i++ ) {
            for( int j = 0; j < grid.length; j++ ) {
                System.out.printf("%7.4f ", grid[i][j]);
            }

            System.out.println();
        }
    }

    private static class WorkerThread extends Thread {

        private int gridSize, firstRow, lastRow, threadId;
        private Semaphore maxSem;
        private CountDownLatch counter;

        public WorkerThread(int threadId, int gridSize, int firstRow, int lastRow, Semaphore maxSem,
                CountDownLatch counter) {
            this.gridSize = gridSize;
            this.firstRow = firstRow;
            this.lastRow = lastRow;
            this.maxSem = maxSem;
            this.counter = counter;

            // Print out that this thread has been created
            //safePrintf("[WT %d] Created!\n", threadId);
        }

        @Override public void run() {
            //safePrintf("[WT %d] Started!\n", threadId);
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
                    localMax = max(localMax, abs(new_grid[row][column] - grid[row][column]));
                }
            }

            // Determine if the local max is larger than the global max
            try {
                maxSem.acquire();
                maxDifference = max(maxDifference, localMax);
                maxSem.release();
            } catch( InterruptedException ie ) {
                System.err.println("Max difference calculation interrupted! Exiting!");
                System.exit(1);
            }

            //safePrintf("[WT %d] Finished!\n", threadId);
            counter.countDown();
        }
    }
}
