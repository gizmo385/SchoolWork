import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;

public class BookList
{
   
   private BinarySearchTree<Title> titles;
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
      Title   aTitle;
      String  tempAuthor;
      
      currentTitle = null;
      
      // Create an empty Tree
      titles = new BinarySearchTree<Title>();
      
      while ( inputScan.hasNext() ) {
         line = inputScan.nextLine();
         
         // If this is a title, put it in the Titles list in sorted order
         if ( line.substring(0,8).equals("Title:  ") ) {
            currentTitle = new Title( line.substring(8) );
            // System.out.printf("Title! '%s'\n", currentTitle);
            
            // add title to the list.
            // Note: the add method will not add a duplicate title
            titles.add( currentTitle );
            
         }
 /*
         // If this is an author, add it to the list of authors for the
         // current title
         if ( line.substring(0,8).equals("Author: ") ) {
            
            if ( currentTitle != null ) {
               // System.out.println("Author!");
               tempAuthor = line.substring(8);
               currentTitle.addAuthor( tempAuthor );
            }
         }
 */
      }
      
   } // constructor

   
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
   
   public void add( String aTitle )
   {
      Title tempTitle = new Title( aTitle );
      titles.add( tempTitle );
   } // add
   

   public void remove( String title )
   {
      Title tempTitle = new Title( title );
      
      titles.remove( tempTitle );
   } // remove


/*
   public String toStringTitles()
   {
      String tempString = titles.toStringTitle();
      
      return tempString;
   } // toStringTitles
*/ 

   public String toString()
   {
      String tempString = titles.toString();
      
      return tempString;
   } // toString
   
   public String toStringDepth()
   {
      return titles.toStringDepth();
   } // toStringDepth
   
   public int maxDepth()
   {
      return titles.maxDepth();
   } // maxDepth
   
   public Title findMin()
   {
      return titles.findMin();
   } // findMin
   
   public Title findMax()
   {
      return titles.findMax();
   } // findMax

} // BookList
