import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;

public class BookList
{

   private SingleLinkedList<Title> titles;
   private Title currentTitle;
   
   // Constructor:
   // Opens the file and reads in Title and Author lines
   // Creates a singly-linked list of titles.  Titles are maintained in
   // alphabetical order.
   // Each title has a singly-linked list of authors.  Authors are in
   // the order listed in the file (not necessarily alphabetical).
   public BookList( String fileName ) throws FileNotFoundException
   {
      File    inputFile = new File( fileName );
      Scanner inputScan = new Scanner( inputFile );
      String  line;
      Title   tempTitle = null;
      Title   aTitle;
      String  tempAuthor;
      
      currentTitle = null;
      
      // Create an empty list
      titles = new SingleLinkedList<Title>();
      
      while ( inputScan.hasNext() ) {
         line = inputScan.nextLine();
  
         // If this is a title, put it in the Titles list in sorted order
         if ( line.substring(0,8).equals("Title:  ") ) {
            // System.out.println("Title!");
            tempTitle = new Title( line.substring(8) );

            // check if the title is already in the list
            currentTitle = titles.find( tempTitle );
            if ( currentTitle == null ) {
               titles.insertInOrder( tempTitle );  // not in list, can add it
               currentTitle = tempTitle;
            }
         }

         // If this is an author, add it to the list of authors for the
         // current title
         if ( line.substring(0,8).equals("Author: ") ) {

            if ( currentTitle != null ) {
               // System.out.println("Author!");
               tempAuthor = line.substring(8);
               currentTitle.addAuthor( tempAuthor );
            }
         }

      }
      
   } // constructor
   
   
   public boolean insertTitle( String newTitle )
   {
      Title   tempTitle = new Title( newTitle );
      Title   aTitle;
      boolean found;
      int     i;
      
      // Is this title already in the list?
      if ( titles.find(tempTitle) != null )
         return false;  // already in the list
      
      // insert the title in order, since we now know it is NOT already there
      titles.insertInOrder( tempTitle );
      currentTitle = tempTitle;
      
      return true;  // we succeeded in adding the title
      
   } // insertTitle
   
      
   public Title findTitle( String aTitle )
   {
      int     i;
      boolean found;
      Title   titleWalker = null; // assume title is NOT in the list
      Title   tempTitle = new Title( aTitle );

      currentTitle = titles.find( tempTitle );

      // currentTitle will be null if find did not find the title
      // else, currentTitle will be the located title.
      // Either way, we return currentTitle
      return currentTitle;

   } // findTitle
   
   
   public boolean remove( String title )
   {
      Title tempTitle = new Title( title );
      
      return titles.remove( tempTitle );
   } // remove
   
   
   public String toStringTitles()
   {
      String tempString = "";
      
      for ( Title currentTitle: titles )
         tempString = tempString + currentTitle.getTitle() + "\n";
      
      return tempString;
   } // toStringTitles


   public String toString()
   {
      String tempString = "";
      
      for ( Title currentTitle: titles )
         tempString = tempString + currentTitle.toString();
      
      return tempString;
   } // toString

} // BookList
