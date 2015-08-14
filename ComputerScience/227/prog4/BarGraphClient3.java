/* Tests getGraphSize() and getFillCharacter() methods
 */

public class BarGraphClient3
{
   public static void main( String [] args )
   {
      int [] others = {  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
         14, 13, 12, 11, 10,  9,  8,  7,  6,  5};
      BarGraph graph;
      char fillCharacter = '-';
      
      graph = new BarGraph( others, 40, fillCharacter );
      
      System.out.printf("graph size = %d\n", graph.getGraphSize());
      System.out.printf("fill character = '%c'\n", graph.getFillCharacter());
   } // main
} // BarGraphClient3