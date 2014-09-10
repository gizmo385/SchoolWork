import java.util.ArrayList;
import java.util.List;

/*
 * Christopher Chapline
 * Tyson Hoekstra
 * Section 3
 *
 * Assignment 5 - Problem 1, CSc 227, Spring 2013
 * Title: Contains fields and methods to represent a book
 */
public class Title implements Comparable<Title>
{
	private String title;
	private List<String> authors;
	
	/**
	 * Creates a book that has a title and a set of authors
	 * @param title Book title
	 * @param authors List of authors
	 */
	public Title( String title, ArrayList<String> authors )
	{
		this.title = title.trim();
		this.authors = new ArrayList<String>( authors.size() );
		for( String s : authors )
			this.authors.add( new String( s ).trim() );
	}
	
	/**
	 * Creates a book that has a title
	 * 
	 * Still instantiates the ArrayList object
	 * @param title
	 */
	public Title( String title )
	{
		this.title = title.trim();
		this.authors = new ArrayList<String>();
	}
	
	/**
	 * This Title's title
	 * @return This instance's title String
	 */
	public String getTitle()
	{
		return this.title;
	}
	
	/**
	 * This Title's authors
	 * @return A copy of this instances ArrayList of authors
	 */
	public ArrayList<String> getAuthors()
	{
		ArrayList<String> authorsCopy = new ArrayList<String>( this.authors.size() );
		for( String s : this.authors )
			authorsCopy.add( new String( s ) );
		
		return authorsCopy;
	}
	
	/**
	 * Change this Title's title
	 * @param title New title for this Title instance
	 */
	public void setTitle( String title )
	{
		this.title = title;
	}
	
	/**
	 * Adds an author to the author list if it isn't already present
	 * @param author The author to add to the list
	 */
	public void addAuthor( String author )
	{
		if( !this.authors.contains( author ) )
			this.authors.add( author );
	}
	
	/**
	 * Returns true if o is of instance Title if if o's title field is equal to this instance's title field
	 */
	public boolean equals(Object o )
	{
		return ( o instanceof Title && ((Title) o).getTitle().equals( this.title ) );
	}
	
	/**
	 * Returns the compareTo method for the title fields
	 */
	public int compareTo( Title t )
	{
		return this.title.compareTo( t.getTitle() );
	}
	
	/**
	 * Returns a string representation of this Title object
	 */
	public String toString()
	{
		StringBuilder sb = new StringBuilder( "Title: " + this.title );
		for( String s : this.authors )
			sb.append( "\n   " + s );
		
		sb.append( "\n" );
		return sb.toString();
	}
}