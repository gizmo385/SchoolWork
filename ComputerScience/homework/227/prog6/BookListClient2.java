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
      String   line, title, author;
      Title    currentTitle;
      ArrayList<String> authors;
      
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
         
         // Process TITLE-AUTHORS
         else if ( line.length() >= 14 &&
                   line.substring(0,14).equals("TITLE-AUTHORS ") ) {
            System.out.println("TITLE-AUTHORS");
            title = line.substring(14);
            currentTitle = myBooks.findTitle( title );
            
            if ( currentTitle == null )
               System.out.printf("NOT FOUND: %s\n", title);
            else {
               // Print Title and Authors, each author on its own line
               System.out.printf("FOUND: %s\n", currentTitle.getTitle());
               authors = currentTitle.getAuthors();
               for ( String name : authors )
                  System.out.printf("   %s\n", name);
            }
         }

         // Process ADD
         else if ( line.length() >= 4 &&
                   line.substring(0,4).equals("ADD ") ) {
            System.out.println("ADD");
            title = line.substring(4);
            if ( myBooks.insertTitle(title) )
               System.out.printf("ADDED: %s\n", title);
            else
               System.out.printf("FOUND: %s\n", title);
         }
         
         // Process DELETE-TITLE
         else if ( line.length() >= 13 &&
                   line.substring(0,13).equals("DELETE-TITLE ") ) {
            System.out.println("DELETE-TITLE");
            title = line.substring(13);
            if ( myBooks.remove(title) )
               System.out.printf("DELETED %s\n", title);
            else
               System.out.printf("NOT FOUND %s\n", title);
         }
         
         // Process PRINT-TITLES-AUTHORS
         else if ( line.length() >= 20 &&
                  line.substring(0,20).equals("PRINT-TITLES-AUTHORS") ) {
            System.out.println("PRINT-TITLES-AUTHORS");
            System.out.println( myBooks.toString() );
         }
         
         // Process PRINT-TITLES
         else if ( line.length() >= 12 &&
                   line.substring(0,12).equals("PRINT-TITLES") ) {
            System.out.println("PRINT-TITLES");
            System.out.println( myBooks.toStringTitles() );
         }
         
         // Process TITLE-ADD-AUTHOR
         else if ( line.length() >= 17 &&
                   line.substring(0,17).equals("TITLE-ADD-AUTHOR ") ) {
            System.out.println("TITLE-ADD-AUTHOR");
            title = line.substring(17);
            // now, get the author on the next input line
            line = commandsInput.nextLine();
            // and strip off "AUTHOR "
            author = line.substring(7);
            currentTitle = myBooks.findTitle( title );
            if ( currentTitle == null )
               System.out.printf("NOT Found %s\n", title);
            else {
               System.out.printf("FOUND %s\n", title);
               currentTitle.addAuthor(author);
               System.out.printf("ADDED AUTHOR %s\n", author);
            }
         }
      }
      
      System.out.printf("\nBooklist after processing commands from %s:\n\n",
                        commandsName );
      System.out.println(myBooks);
   } // main
   
} // BookListClient2
