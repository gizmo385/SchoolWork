/* Tests printBarGraph and setGraphSize. Uses larger numbers, up to 5 digits
 * in length. Uses 15, 30, and 5 as the graph sizes.
 * Prints a period for the fill character, since this makes debugging a little
 * easier.
 */

public class BarGraphClient6
{
   public static void main( String [] args )
   {
      int [] values = { 1805, 4373,   999,   662, 2512, 1024, 2370, 3000,
                        6857,  750, 9272, 10000, 10521, 1252,  637, 9823 };
      BarGraph graph;
      
      graph = new BarGraph( values, 15, '.' );
      graph.printBarGraph();
      
      graph.setGraphSize(30);
      graph.printBarGraph();
      
      graph.setGraphSize(5);
      graph.printBarGraph();
      
   } // main
} // BarGraphClient6