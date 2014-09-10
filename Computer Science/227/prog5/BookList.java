import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Scanner;

/*
 * Christopher Chapline
 * Tyson Hoekstra
 * Section 3
 *
 * Assignment 5 - Problem 1, CSc 227, Spring 2013
 * BookList: Manages a small collection of Title objects
 */

public class BookList
{
	private ArrayList<Title> library;
	
	/**
	 * Loads books from a file
	 */
	public BookList( String fileName ) throws IOException
	{
		Scanner fileScan = new Scanner( new File( fileName ) );
		this.library = new ArrayList<Title>();
		Title temp = new Title( "" );
		
		while( fileScan.hasNextLine() )
		{
			String nextLine = fileScan.nextLine();
			
			if( nextLine.startsWith( "Title:  " ) )
			{
				if( !temp.getTitle().equals( "" ) )
					this.library.add( temp );
				
				temp = new Title( nextLine.substring( nextLine.indexOf( "  " ) ) );
			}
			
			else if( nextLine.startsWith( "Author: " ) )
				temp.addAuthor( nextLine.substring( nextLine.indexOf( " " ) + 1 ) );
		}
		this.library.add( temp );
		fileScan.close();
		
		Collections.sort( this.library );
	} // end constructor
	
	public boolean addTitle( String title )
	{
		Title temp = new Title( title );
		
		for( Title t : this.library )
		{
			if( temp.compareTo( t ) == 0 )
				return false;
			else if( temp.compareTo( t ) < 0 )
			{
				this.library.add( this.library.indexOf( t ), temp );
				return true;
			}
		}
		
		this.library.add( temp );
		return true;
	}
	
	public Title findTitle( String title )
	{
		for( Title t : this.library )
			if( t.getTitle().equals( title ) )
				return this.library.get( this.library.indexOf( t ) );
		
		return null;
	}
	
	public boolean remove( String title )
	{
		Title temp = new Title( title );
		
		for( Title t : this.library )
			if( t.equals( temp ) ) 
				return this.library.remove( t );
		
		return false;
	}
	
	public String toString()
	{
		StringBuilder sb = new StringBuilder();
		
		for( Title t : this.library )
		{
			sb.append( t.toString() );
		}
		
		return sb.toString();
		
	}
	
	public String toStringTitles()
	{
		StringBuilder sb = new StringBuilder();
		
		for( Title t : this.library )
			sb.append( t.getTitle() + "\n" );
		
		return sb.toString();
	}
}