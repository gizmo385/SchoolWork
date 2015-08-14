import java.text.DecimalFormat;

/*
 * Christopher Chapline
 * Tyson H
 * Section 3
 *
 * Assignment 8, CSc 227, Spring 2013
 * TreeNode - a node in a binary search tree
 */

public class BinarySearchTree< E extends Comparable<E> > {
	
	private TreeNode<E> root;
	
	/** Constructs a tree where the root is a leaf */
	public BinarySearchTree( E data ) {
		this.root = new TreeNode<E>( data );
	}
	
	/** Constructs an empty tree */
	public BinarySearchTree() {
		this.root = null;
	}
	
	/**
	 * Will find the smallest item in the tree
	 * @return The smallest item in the tree or null if tree is empty
	 */
	public E findMin() {
		return findMinHelper( this.root );
	}
	
	/**
	 * Recursively locates the smallest item
	 */
	public E findMinHelper( TreeNode<E> root ) {
		if( root == null ) {
			return null;
		}
		
		else return root.hasLeftChild() ? findMinHelper( root.getLeftChild() ) : root.getData();
	}
	
	/**
	 * Will find the largest item in the tree
	 * @return The smallest item in the tree or null if the tree is empty
	 * @see #findMaxHelper(TreeNode)
	 */
	public E findMax() {
		return findMaxHelper( this.root ).getData();
	}
	
	/**
	 * Recursively locates the largest item in the tree
	 */
	public TreeNode<E> findMaxHelper( TreeNode<E> root ) {
		if( root == null ) {
			return null;
		}
		
		else return root.hasRightChild() ? findMaxHelper( root.getRightChild() ) : root;
	}
	
	/**
	 * Adds the specified element to the list
	 * @param element Element to add to the list
	 * @see #addHelper(Comparable, TreeNode)
	 */
	public void add( E element ) {
		this.root = addHelper( element, this.root );
	}
	
	/**
	 * Recursively adds the element
	 */
	public TreeNode<E> addHelper( E element, TreeNode<E> current ) {
		if( current == null ) {
			current = new TreeNode<E>( element );
			return current;
		}
		
		else if( current.getData().equals( element ) ) {
			return current;
		}
		
		else if( element.compareTo( current.getData() ) < 0 ) {
			current.setLeftChild( addHelper( element, current.getLeftChild() ) );
		}
		
		else {
			current.setRightChild( addHelper( element, current.getRightChild() ) );
		}
		
		return current;
		
	}
	
	public E find( E elementToFind ) {
		return findHelper( elementToFind, this.root );
	}
	
	public E findHelper( E elementToFind, TreeNode<E> current ) {
		if( current == null ) {
			return null;
		}
		
		else if( current.getData().equals( elementToFind ) ) {
			return current.getData();
		}
		
		else if( elementToFind.compareTo( current.getData() ) < 0 ) {
			return findHelper( elementToFind, current.getLeftChild() );
		}
		
		else return findHelper( elementToFind, current.getRightChild() );
	}
	
	
	/**
	 * Returns a string representation of this binary tree
	 * @see #toStringHelper(TreeNode)
	 */
	public String toString() {
		
		return toStringHelper( this.root );
	}
	
	/**
	 * Recursively returns the string representation of each tree element using an in-order traversal (Left subtree -> root -> right subtree )
	 */
	public String toStringHelper( TreeNode<E> current ) {
		if( current == null ) {
			return "";
		}
		
		String traverse = "";
		
		//visit left child
		if( current.hasLeftChild() ) {
			traverse += toStringHelper(current.getLeftChild() );
		}
		
		//visit root
		traverse += current.getData().toString();
		
		//visit right child
		if( current.hasRightChild() ) {
			traverse += toStringHelper( current.getRightChild() );
		}
		
		return traverse;
	}
	
	/** 
	 * Prints the depth of the tree along with the items toString() 
	 * @see #toStringDepthHelper(TreeNode, int)
	 */
	public String toStringDepth() {
		return toStringDepthHelper( this.root , 1 );
	}
	
	/**
	 * Recursively writes the depth and toString representation of the item
	 */
	public String toStringDepthHelper( TreeNode<E> current, int depth ) {
		if( current == null ) {
			return "";
		}
		
		String traverse = "";
		
		if( current.hasLeftChild() ) {
			traverse += toStringDepthHelper(current.getLeftChild(), depth + 1 );
		}
		
		traverse += new DecimalFormat("000").format( depth ) + ": " +current.getData().toString(); 
		
		if( current.hasRightChild() ) {
			traverse += toStringDepthHelper( current.getRightChild(), depth + 1 );
		}
		
		return traverse;
	}
	
	/**
	 * Returns the max depth of the tree
	 * @see #maxDepthHelper(TreeNode)
	 */
	public int maxDepth() {
		return maxDepthHelper( this.root );
	}
	
	/**
	 * Recursively finds the max depth of this binary tree
	 */
	public int maxDepthHelper( TreeNode<E> current ) {
		
		if( current == null ) {
			return 0;
		}
		
		int leftHeight = maxDepthHelper( current.getLeftChild() );
		int rightHeight = maxDepthHelper( current.getRightChild() );
		return leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
	}
	
	public void remove( E elementToRemove ) {
		this.root = removeHelper( elementToRemove, this.root );
	}
	
	private TreeNode<E> removeHelper( E elementToDelete, TreeNode<E> current ) {
		if( current == null ) return null;
		
		int comparisonResult = elementToDelete.compareTo( current.getData() );
		
		if( comparisonResult < 0 ) {
			current.setLeftChild( removeHelper( elementToDelete, current.getLeftChild() ) );
			return current;
		}
		
		else if( comparisonResult > 0 ) {
			current.setRightChild( removeHelper( elementToDelete, current.getRightChild() ) );
			return current;
		}
		
		else {
			if( !current.hasLeftChild() ) {
				return current.getRightChild();
			}
			
			else if( !current.hasRightChild() ) {
				return current.getLeftChild();
			}
			
			else {
				if( !current.getLeftChild().hasRightChild() ) {
					current.setData( current.getLeftChild().getData() );
					current.setLeftChild( current.getLeftChild().getLeftChild() );
					return current;
				}
				
				else {
					current.setData( findLargestChild( current.getLeftChild() ) );
					return current;
				}
			}
		}
	}
	
	private E findLargestChild( TreeNode<E> parentNode ) {
		if( !parentNode.getRightChild().hasRightChild() ) {
			E returnVal = parentNode.getRightChild().getData();
			parentNode.setRightChild( parentNode.getRightChild().getLeftChild() );
			return returnVal;
		}
		else return findLargestChild( parentNode.getRightChild() );
	}
}
