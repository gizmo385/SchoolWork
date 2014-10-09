import java.util.Iterator;
import java.util.NoSuchElementException;


public class SingleLinkedListIterator<E> implements Iterator< E >
{
	LinkNode<E> elementToReturn;
	
	public SingleLinkedListIterator( LinkNode<E> first )
	{
		this.elementToReturn = first;
	}
	
	@Override
	public boolean hasNext()
	{
		return elementToReturn.getNext().getData() != null;
	}

	@Override
	public E next()
	{
		if( this.hasNext() )
		{
			elementToReturn = elementToReturn.getNext();
			return elementToReturn.getData();
		}
		else throw new NoSuchElementException();
	}

	@Override
	public void remove()
	{
		throw new UnsupportedOperationException();		
	}

}
