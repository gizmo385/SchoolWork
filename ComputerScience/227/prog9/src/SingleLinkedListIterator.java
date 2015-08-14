import java.util.Iterator;
import java.util.NoSuchElementException;

/*
 * Christopher Chapline
 * Tyson Hoekstra
 * Section 3
 *
 * Assignment 5 - Problem 1, CSc 227, Spring 2013
 * SingleLinkedListIterator: Iterates over a SingleLinkedList
 */

public class SingleLinkedListIterator<E extends Comparable<E> > implements Iterator<E>
{

	private LinkNode<E> nextElement;
	
	public SingleLinkedListIterator( LinkNode<E> listHead )
	{
		this.nextElement = listHead;
	}
	
	@Override
	public boolean hasNext()
	{
		return nextElement != null && nextElement.getNext() != null && nextElement.getData() != null;
	}

	@Override
	public E next()
	{
		if( !hasNext() )
			throw new NoSuchElementException();
		
		E dataToReturn = nextElement.getData();
		nextElement = nextElement.getNext();
		
		return dataToReturn;

	}

	@Override
	public void remove()
	{
		throw new UnsupportedOperationException();
		
	}

}
