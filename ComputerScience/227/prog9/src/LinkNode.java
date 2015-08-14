/*
 * Christopher Chapline
 * Tyson Hoekstra
 * Section 3
 *
 * Assignment 5 - Problem 1, CSc 227, Spring 2013
 * LinkNode: Represents a Node in a linked list
 */
public class LinkNode<E extends Comparable<E> >
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
	
	public LinkNode( LinkNode<E> next )
	{
		this.next = next;
		this.data = null;
	}
	
	public LinkNode( E data, LinkNode<E> next )
	{
		this.next = next;
		this.data = data;
	}
	
	public LinkNode<E> getNext()
	{
		return this.next;
	}
	
	public E getData()
	{
		return this.data;
	}
	
	public void setNext( LinkNode<E> next )
	{
		this.next = next;
	}
	
	public void setData( E data )
	{
		this.data = data;
	}
}
