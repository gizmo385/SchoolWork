/*
 * CSc 352 - Homework 1
 * Patrick Homer
 * Christopher Chapline
 */

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class Counting {
    public static void main( String[] args ) {
        if( args.length == 0 ) {
            Scanner in = new Scanner( System.in );

            int inChars = 0, inLines = 0;

            while( in.hasNextLine() ) {
                String nextLine = in.nextLine();
                inLines++;
                inChars += nextLine.length() + 1;
            }
            in.close();

            System.out.println( inLines + "\t" + inChars );
        } else {
            int totalChars = 0, totalLines = 0;

            for( String arg : args ) {
                int localChars = 0, localLines = 0;
                File file = new File( arg );

                try {
                    Scanner fileScan = new Scanner( file );
                    while( fileScan.hasNextLine() ) {
                        String nextLine = fileScan.nextLine();
                        localLines++;
                        localChars += nextLine.length() + 1;
                     }

                    fileScan.close();

                    System.out.println( localLines + "\t" + localChars + "\t" + arg );
                    totalChars += localChars;
                    totalLines += localLines;
                } catch( FileNotFoundException fnfe ) { /* Do nothing */ }
            }

            if( args.length > 1 ) {
                System.out.println( totalLines + "\t" + totalChars + "\ttotal" );
            }
        }
    }
}
