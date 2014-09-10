import java.util.ArrayList;


/*
 * Christopher Chapline
 * Tyson Hoekstra
 * Section 3
 *
 * Assignment 8 - Problem 1, CSc 227, Spring 2013
 * Title: Contains fields and methods to represent a book
 */
public class Title implements Comparable<Title>
{
	private String title;
	
	/**
	 * Creates a book that has a title
	 * 
	 * Still instantiates the ArrayList object
	 * @param title
	 */
	public Title( String title ) {
		this.title = title.trim();
	}
	
	/**
	 * This Title's title
	 * @return This instance's title String
	 */
	public String getTitle() {
		return this.title;
	}
	
	/**
	 * Change this Title's title
	 * @param title New title for this Title instance
	 */
	public void setTitle( String title ) {
		this.title = title;
	}
	
	/**
	 * Returns true if o is of instance Title if if o's title field is equal to this instance's title field
	 */
	public boolean equals(Object o ) {
		return ( o instanceof Title && ((Title) o).getTitle().equals( this.title ) );
	}
	
	/**
	 * Returns the compareTo method for the title fields
	 */
	public int compareTo( Title t ) {
		return this.title.compareTo( t.getTitle() );
	}
	
	/**
	 * Returns a string representation of this Title object
	 */
	public String toString() {
		return "Title: " + this.title + "\n";
	}
}