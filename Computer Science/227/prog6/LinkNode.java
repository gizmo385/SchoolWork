
public class LinkNode<E>
{
	private E data;
	private LinkNode<E> next;
	
	public LinkNode()
	{
		this.data = null;
		this.next = null;
	}
	
	public LinkNode( E data )
	{
		this.data = data;
		this.next = null;
	}
	
	public LinkNode( E data, LinkNode<E> next )
	{
		this.data = data;
		this.next = next;
	}
	
	public LinkNode( LinkNode<E> next )
	{
		this.data = null;
		this.next = next;
	}
	
	public LinkNode<E> getNext()
	{
		return this.next;
	}
	
	public E getData()
	{
		return this.data;
	}
	
	public void setData( E data )
	{
		this.data = data;
	}
	
	public void setNext( LinkNode<E> next )
	{
		this.next = next;
	}
}
