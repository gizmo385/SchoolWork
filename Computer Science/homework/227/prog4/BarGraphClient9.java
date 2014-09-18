/* This test produces the two example graphs shown in the program
 * description. The first graph is a horizontal graph that uses
 * a size of 40 and a blank space for the fill character.
 * The second graph is a vertical graph that uses a size of 30,
 * and a hyphen as the fill character.
 */

public class BarGraphClient9
{
   public static void main( String [] args )
   {
      int [] values = { 18, 43, 99, 62, 5, 1, 7, 3, 85, 75, 92, 100, 51, 52};
      BarGraph graph;
      
      // Create a graph that uses 40 as the graphSize and a blank space
      // for the fill character.  Print the horizontal bar graph.
      graph = new BarGraph( values, 40, ' ' );
      graph.printBarGraph();
      System.out.println();
      
      // Change the graphSize to 30.
      // Print the vertical bar graph.
      graph.setGraphSize(30);
      graph.printVerticalGraph();
      
   } // main
} // BarGraphClient9
