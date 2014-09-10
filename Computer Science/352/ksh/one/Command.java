import java.io.InputStream;
import java.io.OutputStream;
import java.util.Scanner;

public class Command {
    public static void main( String[] args ) {
        Runtime runtime = Runtime.getRuntime();

        for( String arg : args ) {
            try {
                // Execute the command
                System.out.println( "Executing:\n  " + arg );
                Process child = runtime.exec(arg);
                child.waitFor();

                // Process the error stream from the command
                InputStream error = child.getErrorStream();
                StringBuilder errorOutput = new StringBuilder();
                Scanner errorScan = new Scanner( error );
                while( errorScan.hasNextLine() ) {
                    errorOutput.append( errorScan.nextLine() ).append("\n");
                }

                if( errorOutput.length() == 0 ) {
                    System.out.println( "Nothing on standard error" );
                } else {
                    System.out.println("Standard error results:");
                    errorOutput.setLength( errorOutput.length() - 1 );
                    System.out.println( errorOutput );
                }


                // Process the output stream
                InputStream out = child.getInputStream();
                StringBuilder outOutput = new StringBuilder();
                Scanner outScan = new Scanner( out );
                while( outScan.hasNextLine() ) {
                    outOutput.append( outScan.nextLine() ).append("\n");
                }

                if( outOutput.length() == 0 ) {
                    System.out.println( "Nothing on standard output" );
                } else {
                    System.out.println("Standard output results:");
                    outOutput.setLength( outOutput.length() - 1 );
                    System.out.println( outOutput );
                }

                // Get and print the exit code from the command
                int exitCode = child.exitValue();
                System.out.println( "Exit code: " + exitCode );
            } catch( Exception e ) {
                System.out.println(e);
            }

            System.out.println();
        }
    }
}
