import java.util.Iterator;

/*
 * Christopher Chapline
 * Tyson Hoekstra
 * Section 3
 *
 * Assignment 5 - Problem 1, CSc 227, Spring 2013
 * SingleLinkedList: Implementation of a singly linked list
 */

public class SingleLinkedList<E extends Comparable<E> > implements Iterable<E>
{
	private LinkNode<E> head;
	
	public LinkNode<E> getHead()
	{
		return this.head;
	}
	
	public Iterator<E> iterator()
	{
		return new SingleLinkedListIterator<E>( this.head );
	}
	
	public SingleLinkedList()
	{
		this.head = new LinkNode<E>();
	}
	
	public E find( E elementToFind )
	{
		for( E element : this )
			if( element.equals( elementToFind ) )
				return element;
		
		return null;
	}
	
	public void insertInOrder( E elementToAdd )
	{
		LinkNode<E> current = this.head;
		
		while( current.getData() != null && current.getNext() != null && current.getData().compareTo( elementToAdd ) < 0 )
			current = current.getNext();
		
		LinkNode<E> temp = new LinkNode<E>( current.getData(), current.getNext() );
		current.setData( elementToAdd );
		current.setNext( temp );
	}
	
	public void add( E elementToAdd )
	{
		LinkNode<E> current = this.head;
		
		while( current.getData() != null && current.getNext() != null )
			current = current.getNext();
		
		current.setData( elementToAdd );
		current.setNext( new LinkNode<E>() );
	}
	
	public boolean remove( E elementToRemove )
	{
		if( find( elementToRemove) == null )
			return false;
		else
		{
			LinkNode<E> current = this.head;  
			
			while( current.getNext() != null && current.getData().equals( elementToRemove ) == false )
				current = current.getNext();
			
			current.setData( current.getNext().getData() );
			current.setNext( current.getNext().getNext() );
			
			return true;
		}
	} // end remove()

}
