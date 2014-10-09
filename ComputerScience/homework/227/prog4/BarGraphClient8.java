/* Tests printVerticalGraph, setGraphSize and setFillCharacter.
 * Uses larger numbers, up to 5 digits in length.
 * Uses 15 and 20 as the graph sizes.
 * Prints a | as the fill character for the first graph.
 * Prints a blank space as the fill character for the second graph.
 */

public class BarGraphClient8
{
   public static void main( String [] args )
   {
      int [] values = { 1805, 4373,   999,   662, 2512, 1024, 2370, 3000,
                        6857,  750, 9272, 10000, 10521, 1252,  637, 9823 };
      BarGraph graph;
      
      graph = new BarGraph( values, 15, '|' );
      graph.printVerticalGraph();
      
      graph.setGraphSize(20);
      graph.setFillCharacter(' ');
      graph.printVerticalGraph();
      
   } // main
} // BarGraphClient8