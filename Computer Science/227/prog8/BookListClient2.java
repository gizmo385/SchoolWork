import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

public class BookListClient2
{
   public static void main( String [] args ) throws FileNotFoundException
   {
      String   fileName, commandsName;
      Scanner  keyInput = new Scanner( System.in );
      File     commandsFile;
      Scanner  commandsInput;
      BookList myBooks;
      String   testNum;
      String   line, title;
      Title    currentTitle;
      
      System.out.print("Enter the test number: ");
      testNum = keyInput.next();
      
      // testNum =args[0];
      
      fileName = "test" + testNum + ".txt";
      commandsName = "test" + testNum + ".commands";
      
      myBooks = new BookList( fileName );
      
      System.out.printf("\nBooklist after reading %s:\n\n", fileName);
      System.out.println(myBooks);

      commandsFile = new File( commandsName );
      commandsInput = new Scanner( commandsFile );
      
      System.out.printf("\nProcessing commands from %s\n\n", commandsFile);
      
      while ( commandsInput.hasNext() ) {
         line = commandsInput.nextLine();
         
         // Process TITLE
         if ( line.length() >= 6 &&
              line.substring(0,6).equals("TITLE ") ) {
            System.out.println("TITLE");
            title = line.substring(6);
            currentTitle = myBooks.findTitle( title );
            
            if ( currentTitle == null )
               System.out.printf("NOT FOUND: %s\n", title);
            else
               System.out.printf("FOUND: %s\n", currentTitle.getTitle());
         }
         
         // Process ADD
         else if ( line.length() >= 4 &&
                  line.substring(0,4).equals("ADD ") ) {
            System.out.println("ADD");
            title = line.substring(4);
            if ( myBooks.findTitle(title) != null )
               System.out.printf("FOUND: %s\n", title);
            else {
               myBooks.add(title);
               System.out.printf("ADDED: %s\n", title);
            }
         }

         // Process TITLE-MIN
         else if ( line.length() >= 9 &&
                  line.substring(0,9).equals("TITLE-MIN") ) {
            System.out.println("TITLE-MIN");
            currentTitle = myBooks.findMin();
            if ( currentTitle != null )
               System.out.printf("TITLE-MIN: %s", currentTitle);
            else {
               System.out.printf("TITLE-MIN: tree is empty\n");
            }
         }
         
         // Process TITLE-MAX
         else if ( line.length() >= 9 &&
                  line.substring(0,9).equals("TITLE-MAX") ) {
            System.out.println("TITLE-MAX");
            currentTitle = myBooks.findMax();
            if ( currentTitle != null )
               System.out.printf("TITLE-MAX: %s", currentTitle);
            else {
               System.out.printf("TITLE-MAX: tree is empty\n");
            }
         }

         // Process DELETE-TITLE
         else if ( line.length() >= 13 &&
                   line.substring(0,13).equals("DELETE-TITLE ") ) {
            System.out.println("DELETE-TITLE");
            title = line.substring(13);
            // Look for the title first
            if ( myBooks.findTitle(title) != null ) {
               // Now, we know the title is present, remove it
               myBooks.remove(title);
               System.out.printf("DELETED %s\n", title);
            }
            else
               System.out.printf("NOT FOUND %s\n", title);
         }

         // Process PTINT-TITLES-DEPTH
         else if ( line.length() >= 18 &&
                  line.substring(0,18).equals("PRINT-TITLES-DEPTH") ) {
            System.out.println("PRINT-TITLES-DEPTH");
            System.out.println( myBooks.toStringDepth() );
         }
         
         // Process PRINT-TITLES
         else if ( line.length() >= 12 &&
                   line.substring(0,12).equals("PRINT-TITLES") ) {
            System.out.println("PRINT-TITLES");
            // System.out.println( myBooks.toStringTitles() );
            System.out.println( myBooks.toString() );
         }
         
         // Process MAX-DEPTH
         else if ( line.length() >= 9 &&
                  line.substring(0,9).equals("MAX-DEPTH") ) {
            System.out.printf("MAX-DEPTH of the tree is %d\n ",
                              myBooks.maxDepth());
         }

      }
      
      System.out.printf("\nBooklist after processing commands from %s:\n\n",
                        commandsName );
      System.out.println(myBooks);
   } // main
   
} // BookListClient2
