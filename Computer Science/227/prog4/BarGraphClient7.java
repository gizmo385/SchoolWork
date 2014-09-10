/* Tests printVerticalGraph. Uses a graph size that results in a
 * scale factor of 1.0.
 * Prints a | for the fill character, since this makes debugging a little
 * easier.
 */

public class BarGraphClient7
{
   public static void main( String [] args )
   {
      int [] others = {  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
                        14, 13, 12, 11, 10,  9,  8,  7,  6,  5 };
      BarGraph graph;
      
      graph = new BarGraph( others, 15, '|' );
      graph.printVerticalGraph();
            
   } // main
} // BarGraphClient7
