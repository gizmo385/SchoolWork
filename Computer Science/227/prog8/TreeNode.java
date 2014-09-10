/*
 * Christopher Chapline
 * Tyson H
 * Section 3
 *
 * Assignment 8, CSc 227, Spring 2013
 * TreeNode - a node in a binary search tree
 */


public class TreeNode< E extends Comparable<E> > {
	
	private E 			data;
	private TreeNode<E> leftChild;
	private TreeNode<E> rightChild;
	
	/** Creates a leaf node with only data */
	public TreeNode( E data ) {
		this.data = data;
		this.leftChild = null;
		this.rightChild = null;
	}

	public E getData() {
		return data;
	}

	public void setData(E data) {
		this.data = data;
	}

	public TreeNode<E> getLeftChild() {
		return leftChild;
	}

	public void setLeftChild(TreeNode<E> leftChild) {
		this.leftChild = leftChild;
	}

	public TreeNode<E> getRightChild() {
		return rightChild;
	}

	public void setRightChild(TreeNode<E> rightChild) {
		this.rightChild = rightChild;
	}
	
	public boolean hasLeftChild() {
		return this.leftChild != null;
	}
	
	public boolean hasRightChild() {
		return this.rightChild != null;
	}
	
	public boolean isLeaf() {
		return this.hasLeftChild() == false && this.hasRightChild() == false;
	}
	
}
