
public class Test {
	public static void main( String[] args ) {
		BinarySearchTree<String> strings = new BinarySearchTree<String>();
		
		//Add items
		strings.add( "Christopher" );
		strings.add( "Barney" );
		strings.add( "James" );
		strings.add( "Frederick" );
		strings.add( "Arturo" );
		strings.add( "Antonio" );
		strings.add( "Christina" );
		strings.add( "Greta" );
		strings.add( "Taylor" );
		
		System.out.println( strings.find( "Christopher" ) != null ? "FOUND" : "NOT FOUND" );
		strings.remove( "Christopher" );
		System.out.println( strings.find( "Christopher" ) != null ? "FOUND" : "NOT FOUND" );
	}
}
