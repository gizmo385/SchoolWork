/* Tests the setNumbers and getNumbers methods of BarGraph.java
 */

public class BarGraphClient2
{
   public static void main( String [] args )
   {
      int [] values = { 18, 43, 99, 62, 5, 1, 7, 3, 85, 75, 92, 100, 51, 52};
      int [] others = {  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
                        14, 13, 12, 11, 10,  9,  8,  7,  6,  5};
      BarGraph graph;
      int [] newValues;
      int [] moreValues;
      int i;
      
      graph = new BarGraph( values, 40, ' ' );
      
      // Try putting a different array of numbers using setNumbers
      // then get them back using getNumbers
      graph.setNumbers( others );
      newValues = graph.getNumbers();
      
      for ( i = 0; i < newValues.length; i++ )
         System.out.printf("newValues[%2d] = %3d\n", i, newValues[i]);
      
      System.out.println();
      
      // Want to make sure the setNumbers method creates a copy of the
      // array it is given.  Change some numbers in others, then do another
      // getNumbers to see if they have changed (they should not).
      for ( i = 0; i < others.length; i += 2 )
         others[i] *= 3;
      
      moreValues = graph.getNumbers();
      
      for ( i = 0; i < moreValues.length; i++ )
         System.out.printf("moreValues[%2d] = %3d\n", i, moreValues[i]);
      

   } // main
} // BarGraphClient2
