import java.util.Iterator;


public class SingleLinkedList< E extends Comparable<E> > implements Iterable<E>
{
	private LinkNode<E> head;
	
	public SingleLinkedList()
	{
		this.head = new LinkNode<E>( new LinkNode<E>() );
	}
	
	public LinkNode<E> getHead()
	{
		return this.head;
	}
	
	public E find( E elementToFind )
	{
		for( E element : this )
			if( element.equals( elementToFind ) ) return element;
		
		return null;
		
	}
	
	public void add( E elementToAdd )
	{
		LinkNode<E> current = new LinkNode<E>( this.head );
		LinkNode<E> temp = new LinkNode<E>( elementToAdd );
		
		for( E nextElement : this )
			current = current.getNext();
		
		temp.setNext( current.getNext() );
		current.setNext( temp );
	}
	
	public void insertInOrder( E elementToAdd )
	{
		LinkNode<E> current = this.head.getNext();
		
		while( current.getNext() != null && current.getData() != null && current.getData().compareTo( elementToAdd ) < 0 )
			current = current.getNext();
		
		LinkNode<E> temp = new LinkNode<E>( elementToAdd );
		temp.setNext( current.getNext() );
		current.setNext( temp );
	}
	
	public boolean remove( E elementToRemove )
	{
		if( this.find( elementToRemove ) == null )
			return false;
		else
		{
			LinkNode<E> current = this.head.getNext();
			
			while( current.getNext() != null  && current.getNext().getData() != elementToRemove )
				current = current.getNext();
			
			current.setData( current.getNext().getData() );
			current.setNext( current.getNext().getNext() );
			
			return true;
		}
	}

	@Override
	public Iterator<E> iterator()
	{
		return new SingleLinkedListIterator<E>( this.head );
	}
}
