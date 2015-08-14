/* Tests the getNumbers method of BarGraph.java
 */

public class BarGraphClient1
{
   public static void main( String [] args )
   {
      int [] values = { 18, 43, 99, 62, 5, 1, 7, 3, 85, 75, 92, 100, 51, 52};
      BarGraph graph;
      int [] newValues;
      int [] moreValues;
      int i;
      
      graph = new BarGraph( values, 40, ' ' );
      newValues = graph.getNumbers();
      
      for ( i = 0; i < newValues.length; i++ )
         System.out.printf("newValues[%2d] = %3d\n", i, newValues[i]);
      
      System.out.println();
      
      // Want to make sure we were given a copy of the array of values.
      // Will change the numbers in newValues, then see if we get the
      // original numbers from a second call to getNumbers()
      
      // Change the entries in newValues
      for ( i = 0; i < newValues.length; i++ )
         newValues[i] *= 2;
      
      // Get the numbers from graph
      moreValues = graph.getNumbers();
      for ( i = 0; i < moreValues.length; i++ )
         System.out.printf("moreValues[%2d] = %3d\n", i, moreValues[i]);

      
   } // main
} // BarGraphClient1
