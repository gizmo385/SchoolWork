import static java.lang.Math.max;
import static java.lang.Math.abs;

import java.io.File;
import java.io.PrintWriter;
import java.io.IOException;

import java.util.Arrays;
import java.util.concurrent.Semaphore;
import java.util.concurrent.CountDownLatch;

public class Jacobi {

    private static double[][] grid, new_grid;
    private static double maxDifference;

    /**
     * Performs Jacobi iteration using a grid of a specified size and parsing it with
     * a pre-determined number of threads.
     *
     * @param args The first argument for this program is the size of the interior grid being
     * iterated over. The second argument determines the number of threads that the iterations are
     * taking place on. Optional arguments include the initial values for the left, top, right, and
     * bottom exteriors respectively.
     */
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

        // Iterations and batch size
        int numberOfIterations = 0;
        int rowsPerThread = gridSize / numThreads;
        int leftover = gridSize % numThreads;


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
        Semaphore threadsSemaphore = new Semaphore(numThreads);

        // Perform the iterations
        for( ; ; numberOfIterations++ ) {
            maxDifference = 0.0;

            // Create threads to perform the iterations
            for( int thread = 0; thread < numThreads; thread++ ) {
                // Acquire a permit from the semaphore for each thread that we're creating
                try {
                    threadsSemaphore.acquire();
                } catch( InterruptedException ie ) {
                    System.err.println("interrupted while attempting to acquire semaphore!");
                    System.exit(1);
                }

                // Create the worker
                WorkerThread worker;
                int startingRow = (thread * rowsPerThread) + 1;
                int endingRow = startingRow + rowsPerThread;

                if( (thread + 1) == numThreads ) {
                    worker = new WorkerThread(gridSize, startingRow, endingRow + leftover, maxSem,
                            threadsSemaphore);
                } else {
                    worker = new WorkerThread(gridSize, startingRow, endingRow, maxSem,
                            threadsSemaphore);
                }

                // Start the worker
                worker.start();
            }

            // Block until the counter reaches zero
            while( threadsSemaphore.availablePermits() != numThreads ) {
                // Spin
            }

            // Determine if we've completed the iteration
            if( maxDifference < EPSILON ) {
                break;
            }

            for( int i = 0; i < grid.length; i++ ) {
                System.arraycopy(new_grid[i], 0, grid[i], 0, grid.length);
            }
        }

        // Stop the timer
        long finish = System.nanoTime();

        // Standard out
        System.out.printf("Finished %d iterations in %d microseconds using %d threads\n",
                numberOfIterations, (finish - start) / 1000, numThreads);

        // Write output to a file
        File outputFile = createFile("JavaJacobiResults.txt");
        try (PrintWriter fileOut = new PrintWriter(outputFile)) {
            fileOut.printf("Grid size: %d\n", gridSize);
            fileOut.printf("Number of threads: %d\n", numThreads);
            fileOut.printf("Edge values: left = %f, right = %f, top = %f, bottom = %f\n",
                    leftStart, rightStart, topStart, bottomStart);
            fileOut.printf("Epsilon: %f\n\n", EPSILON);

            // Write lines to the file
            for( int i = 0; i < grid.length; i++ ) {
                for( int j = 0; j < grid.length; j++ ) {
                    fileOut.printf("%7.4f ", grid[i][j]);
                }

                fileOut.println();
            }
        } catch( IOException ioe ) {
            System.err.printf( "Error attempting to write to file: %s\n", outputFile.getName() );
        }
    }

    /**
     * Creates a file with the specified filename. This method will return the File that has been
     * created. In addition to creating the file object, this method will create the file on the
     * file system if it does not already exist.
     *
     * @param filename The name of the file being created.
     *
     * @return The file object which has been created.
     */
    private static File createFile(String filename) {
        File file = new File( filename );

        try {
            file.createNewFile();
        } catch( IOException ioe ) {
            System.err.printf("Error while trying to create file \"%s\"!\n", filename);
        }

        return file;
    }

    /**
     * A thread which will perform Jacobi iterations on a particular partition of the grid.
     */
    private static class WorkerThread extends Thread {

        private int gridSize, firstRow, lastRow;
        private Semaphore maxSem, threadsSemaphore;

        /**
         * Creates a thread which will perform Jacobi iterations.
         *
         * @param gridSize          the size of the interior grid that is being iterated over.
         * @param firstRow          the index of the first row being operated on.
         * @param lastRow           the index of the last row being operated on.
         * @param maxSem            the semaphore that controls access to maxDifference.
         * @param threadsSemaphore  the semaphore that counts the number of threads active.
         */
        public WorkerThread(int gridSize, int firstRow, int lastRow, Semaphore maxSem,
                Semaphore threadsSemaphore) {
            this.gridSize = gridSize;
            this.firstRow = firstRow;
            this.lastRow = lastRow;
            this.maxSem = maxSem;
            this.threadsSemaphore = threadsSemaphore;
        }

        /**
         * Will perform Jacobi iterations on the partition of the grid. This will terminated after
         * acquiring a lock on a semaphore and comparing its local maximum difference to the maximum
         * global distance.
         */
        @Override public void run() {
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

            // Release a permit from the threads semaphore
            threadsSemaphore.release();
        }
    }
}
