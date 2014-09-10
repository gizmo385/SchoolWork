public class PatrickTreeNode<E>
{
   private E data;
   private PatrickTreeNode<E> leftChild;
   private PatrickTreeNode<E> rightChild;
   
   
   public PatrickTreeNode( E myData )
   {
      setData(myData);
      setLeftChild(null);
      setRightChild(null);
   } // Constructor

   public void setData( E someData )
   {
      data = someData;
   }

   public E getData()
   {
      return data;
   } // getData

   public void setLeftChild( PatrickTreeNode<E> nextOne )
   {
      leftChild = nextOne;
   } // setLeftChild
   
   public void setRightChild( PatrickTreeNode<E> nextOne )
   {
      rightChild = nextOne;
   } // setRightChild
   
   public PatrickTreeNode<E> getLeftChild()
   {
      return leftChild;
   } // getLeftChild
   
   public PatrickTreeNode<E> getRightChild()
   {
      return rightChild;
   } // getRightChild
} // TreeNode
