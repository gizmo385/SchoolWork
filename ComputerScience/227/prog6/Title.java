
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
	private SingleLinkedList<String> authors;
	
	/**
	 * Creates a book that has a title and a set of authors
	 * @param title Book title
	 * @param authors List of authors
	 */
	public Title( String title, SingleLinkedList<String> authors )
	{
		this.title = title.trim();
		this.authors = new SingleLinkedList<String>();
		SingleLinkedListIterator<String> iterator = new SingleLinkedListIterator<String>( this.authors.getHead() );
		
		while( iterator.hasNext() )
			this.authors.add( iterator.next() );
			
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
		this.authors = new SingleLinkedList<String>();
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
	public SingleLinkedList<String> getAuthors()
	{
		SingleLinkedList<String> authorsCopy = new SingleLinkedList<String>();
		SingleLinkedListIterator<String> iterator = new SingleLinkedListIterator<String>( this.authors.getHead() );
		
		while( iterator.hasNext() )
			authorsCopy.add( iterator.next() );
		
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
		SingleLinkedListIterator<String> iterator = new SingleLinkedListIterator<String>( this.authors.getHead() );
		while( iterator.hasNext() )
			sb.append( "\n   " + iterator.next() );
		
		sb.append( "\n" );
		return sb.toString();
	}
}