import static java.lang.Math.max;
import static java.lang.Math.abs;

public class Jacobi {

    public static void main( String[] args ) {
        final int MAX_ITERS = 1000;

        // Ensure that the user has entered the correct number of arguments
        if( args.length < 2 ) {
            System.err.println("Usage: java Jacobi <gridSize> <numThreads>");
            System.exit(1);
        }

        // Starting edge values for the grid (optional arguments)
        double leftStart    = args.length >= 3 ? Double.parseDouble(args[2]) : 1.0,
               topStart     = args.length >= 4 ? Double.parseDouble(args[3]) : 1.0,
               rightStart   = args.length >= 5 ? Double.parseDouble(args[4]) : 80.0,
               bottomStart  = args.length >= 6 ? Double.parseDouble(args[5]) : 80.0,

        // The largest difference between successive iterations that we will terminate on
        double epsilon = 0.1;

        // Read in the command line arguments
        int gridSize = Integer.parseInt( args[0] );
        int numThreads = Integer.parseInt( args[1] );

        int numberOfIterations = 0;

        // Print out information to stdout
        System.out.printf("N = %d\n", gridSize);
        System.out.printf("numProcs = %d\n", numThreads);
        System.out.printf("left = %.1f, top = %.1f, right = %.1f, bottom = %.1f\n", leftStart, topStart,
                rightStart, bottomStart);
        System.out.printf("epsilon = %.1f\n", epsilon);

        // Create our grids
        double grid[][] = new double[gridSize + 2][gridSize + 2];
        double new_grid[][] = new double[gridSize + 2][gridSize + 2];

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

        System.out.println("Starting values:");
        for( int i = 0; i < grid.length; i++ ) {
            for( int j = 0; j < grid.length; j++ ) {
                System.out.printf("%4.1f ", grid[i][j]);
            }

            System.out.println();
        }

        long start = System.nanoTime();

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
                    maxDifference = max(maxDifference, abs(new_grid[i][j] - grid[i][j]));
                }
            }

            // Determine if we've completed the iteration
            if( maxDifference < epsilon ) {
                break;
            }

            // Copy the updated values in
            for( int i = 0; i < grid.length; i++ ) {
                for( int j = 0; j < grid.length; j++ ) {
                    grid[i][j] = new_grid[i][j];
                }
            }
        }

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

}
