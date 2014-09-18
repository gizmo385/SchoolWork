import java.text.DecimalFormat;

public class PatrickBinarySearchTree<E extends Comparable<E>>
{
   private PatrickTreeNode<E> root;
   private DecimalFormat depthFormat = new DecimalFormat("000");
   
   public PatrickBinarySearchTree()
   {
      root = null;
   } // Constructor
/*
   public BinarySearchTree( E data )
   {
      root = new PatrickTreeNode<E>(data);
   } // Constructor
*/   
   // Recursive helper method for find
   private E findHelper( E item, PatrickTreeNode<E> current )
   {
      // item not present in tree if current is null
      if ( current == null )
         return null;
         
      // Did we find item?
      if ( current.getData().equals(item) )
         return current.getData();
      
      if ( item.compareTo(current.getData()) < 0 )
         // search the left sub-tree
         return findHelper( item, current.getLeftChild() );
      else
         // search the right sub-tree
         return findHelper( item, current.getRightChild() );
   } // find helper method
   
   public E find( E item )
   {
      return findHelper( item, root );
   } // find

   private PatrickTreeNode<E> addHelper( E item, PatrickTreeNode<E> current )
   {
      // Base case, reached null.
      if ( current == null ) {
         // Create and return a tree node containing item
         // System.out.printf("addHelper creating a new PatrickTreeNode\n");
         current = new PatrickTreeNode<E>(item);
         // System.out.printf("addHelper created a new node\n");
         // System.out.printf("containing: %s\n", item);
         return current;
      }
      
      if ( current.getData().equals(item) )
         return current;
      
      if ( item.compareTo(current.getData()) < 0 )
          current.setLeftChild( addHelper( item, current.getLeftChild() ) );
      else
          current.setRightChild( addHelper( item, current.getRightChild() ) );
      
      return current;
   } // addHelper

   public void add( E item )
   {
      // System.out.printf("add called to add %s", item);
      root = addHelper( item, root );
   } // add
   
   
   private String walkTree( PatrickTreeNode<E> current, int depth )
   {
      String temp = "";
      
      if ( current == null )
         return "";
      
      temp = temp + walkTree( current.getLeftChild(), depth + 1 );
      temp = temp + depthFormat.format(depth) + ": " + current.getData();
      temp = temp + walkTree( current.getRightChild(), depth + 1 );
      
      return temp;
   }
   
   public String toStringDepth()
   {
      return walkTree( root, 1 );
   } // toStringWithDepth

   private String walkTree( PatrickTreeNode<E> current )
   {
      String temp = "";
      
      if ( current == null )
         return "";
      
      temp = temp + walkTree( current.getLeftChild() );
      temp = temp + current.getData();
      temp = temp + walkTree( current.getRightChild() );
      
      return temp;
   }

   public StringBuilder toStringBuilder() {
	return toStringBuilderHelper( this.root, new StringBuilder() );
   }

   public StringBuilder toStringBuilderHelper( PatrickTreeNode<E> current, StringBuilder sb ) {
	if( current.getLeftChild() != null ) toStringBuilderHelper( current.getLeftChild(), sb );
	sb.append( current.getData() );
	if( current.getRightChild() != null ) toStringBuilderHelper( current.getRightChild(), sb );

	return sb;
   }
   
   public String toString()
   {      
      return walkTree( root );
   }

   private E findMinHelper( PatrickTreeNode<E> current )
   {
      if ( current == null )
         return null;

      if ( current.getLeftChild() == null )
         return current.getData();

      return findMinHelper( current.getLeftChild() );

   } // findMinHelper

   public E findMin()
   {
      return findMinHelper( root );
   } // findMin

   private E findMaxHelper( PatrickTreeNode<E> current )
   {
      if ( current == null )
         return null;
      
      if ( current.getRightChild() == null )
         return current.getData();
      
      return findMaxHelper( current.getRightChild() );
      
   } // findMaxHelper
   
   public E findMax()
   {
      return findMaxHelper( root );
   } // findMax
   
   private int maxDepthHelper( PatrickTreeNode<E> current )
   {
      int leftDepth, rightDepth;
      
      if ( current == null )
         return 0;
      
      leftDepth  = maxDepthHelper( current.getLeftChild()  );
      rightDepth = maxDepthHelper( current.getRightChild() );
      return 1 + Math.max( leftDepth, rightDepth );
   } // maxDepthHelper
   
   public int maxDepth()
   {
      return 0 + maxDepthHelper( root );
   } // maxDepth


   private PatrickTreeNode<E> removeHelper( E item, PatrickTreeNode<E> current )
   {
      if ( current == null )
         return current;  // requested node not in tree

      if ( item.compareTo(current.getData()) < 0 ) {
         // search the left sub-tree
         current.setLeftChild(  removeHelper( item, current.getLeftChild()  ) );
         return current;
      }
      else if ( item.compareTo(current.getData()) > 0 ) {
         // search the right sub-tree
         current.setRightChild( removeHelper( item, current.getRightChild() ) );
         return current;
      }

      // current node is the one we want to delete
      // Need to know if current has two children, or less
      if ( current.getLeftChild() != null && current.getRightChild() != null ) {
         // Has two children.  Use the in-order successor
         // System.out.printf("Deleted node '%s' has two children\n", current.getData());
         current.setData( findMinHelper( current.getRightChild() ) );
         // System.out.printf("removeHelper: findMin found '%s'\n", current.getData());
         current.setRightChild( 
                removeHelper( current.getData(), current.getRightChild() ) );
         return current;
      }

      // has at most one child
      if ( current.getLeftChild() != null ) {
         // has a left child, no right child
         // replace the current node with the left child
         // the left child may be carrying along a sub-tree, but that is
         // okay since the node we are deleting has no right child.
         // System.out.printf("Deleted node '%s' has left child\n", current.getData());
         return current.getLeftChild();
      }

      else {
         // might have a right child, no left child
         // replace the current node with the right child
         /*
         if ( current.getRightChild() != null )
            System.out.printf("Deleted node '%s' has right child %s\n", current.getData(), current.getRightChild().getData());
         else
            System.out.printf("Deleted node '%s' has no right child\n", current.getData());
         */
         return current.getRightChild();
      }
   } // removeHelper

   public void remove( E item )
   {
      root = removeHelper( item, root );
   }
} // BinarySearchTree
