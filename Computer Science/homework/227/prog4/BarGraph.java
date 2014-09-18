/*
 * Christopher Chapline
 * Tyson Hoekstra
 * Section 3
 *
 * Assignment 4 - Problem 1, CSc 227, Spring 2013
 * Draws bar graphs based on an array of passed Integers
 */

import java.text.DecimalFormat;

public class BarGraph
{
	// Instance variables
	private int graphSize;
	private char fillCharacter;
	private int[] numbers;
	DecimalFormat formatter = new DecimalFormat( "0.000000");
	
	// Constructor
	public BarGraph( int[] numbers, int graphSize, char fillCharacter )
	{
		// check for a valid graph size ( a graph of size < 0 does not make rational sense
		if( graphSize > 0 )
			this.graphSize = graphSize;
		else
			this.graphSize = 1;
		
		this.fillCharacter = fillCharacter;
		
		// create deep copy of passed integer array
		this.numbers = new int[ numbers.length ];
		for( int i = 0; i < this.numbers.length; i++ )
			this.numbers[i] = numbers[i];
	}
	
	// Set and get methods
	
	public void setGraphSize( int graphSize )
	{
		// Check for valid graph size ( a graph of size < 0 makes no physical sense )
		if( graphSize > 0 )
			this.graphSize = graphSize;
		else
			this.graphSize = 1;
	}
	
	public int getGraphSize()
	{
		return this.graphSize;
	}
	
	public void setFillCharacter( char fillCharacter )
	{
		this.fillCharacter = fillCharacter;
	}
	
	public char getFillCharacter()
	{
		return this.fillCharacter;
	}
	
	public int[] getNumbers()
	{
		// Declare and instantiate integer array
		int[] copyOfNumbers = new int[ this.numbers.length ];
		
		// Create deep copy of instance array
		for( int i = 0; i < this.numbers.length; i++ )
			copyOfNumbers[i] = this.numbers[i];
		
		return copyOfNumbers;
	}
	
	public void setNumbers( int[] numbers )
	{
		// re-instantiate instance array
		this.numbers = new int[ numbers.length ];
		
		// create deep copy of array passed to method
		for( int i = 0; i < this.numbers.length; i++ )
			this.numbers[i] = numbers[i];
	}
	
	public void printBarGraph()
	{	
		// Indentify the largest value within the instance array
		int largestInArray = this.numbers[0];
		int indexOfLargest = 0;
		
		for( int i = 0; i < this.numbers.length; i++ )
		{
			if( this.numbers[i] > largestInArray )
			{
				largestInArray = this.numbers[i];
				indexOfLargest = i;
			}
		}
		
		double scaleFactor = (double)this.graphSize / (double)largestInArray;
		
		// Print the largest value within the instance array & graph information
		System.out.println( "Largest: numbers[" + indexOfLargest + "] = " + largestInArray );
		System.out.println( "graphSize = " + this.graphSize + ", scaleFactor = " + formatter.format( scaleFactor ) );
		
		char[][] graph = new char[ this.numbers.length ][ (int) (largestInArray * scaleFactor) ];
		for( int row = 0; row < graph.length; row++ ) //for each row
		{
			for( int col = 0; col < graph[row].length; col++ ) //for each column in each row
			{
				// add an asterick if the k value (column) is less than the integer value in instance array * scaleFactor
				if( col < Math.round( this.numbers[row] * scaleFactor ) )
					graph[row][col] = '*';
				else // fill character for non-graph characters
					graph[row][col] = this.fillCharacter;
			}
		}
		
		//print the graph
		for( int h = 0; h < graph.length; h++ )
		{
			String temp = "";
			temp += numbers[h];
			int numSpaces = 5 - temp.length();
			
			for( int spaces = 0; spaces < numSpaces; spaces++ )
				System.out.print( " " );
			
			System.out.print( numbers[h] + ": " );
			
			for( int g = 0; g < graph[h].length; g++ )
				System.out.print( graph[h][g] );
			
			System.out.println( " " + Math.round( this.numbers[h] * scaleFactor ) );
		}
	} //printBarGraph
	
	public void printVerticalGraph()
	{	
		// Indentify the largest value within the instance array
		int indexOfLargest = 0;
		
		for( int i = 0; i < this.numbers.length; i++ )
		{
			if( this.numbers[i] > numbers[ indexOfLargest ] )
			{
				indexOfLargest = i;
			}
		}
		
		double scaleFactor = (double)this.graphSize / (double)numbers[ indexOfLargest];
		
		// Print the largest value within the instance array & graph information
		System.out.println( "Largest: numbers[" + indexOfLargest + "] = " + numbers[ indexOfLargest ] );
		System.out.println( "graphSize = " + this.graphSize + ", scaleFactor = " + formatter.format( scaleFactor ) );
				
		char[][] graph = new char[ this.graphSize ][ this.numbers.length ];
		
		// Fill the graph
		for( int h = 0; h < graph.length; h++ )
		{
			for( int k = 0; k < graph[h].length; k++ )
			{
				if( Math.round(numbers[k] * scaleFactor) >= this.graphSize - h )
					graph[h][k] = '*';
				else
					graph[h][k] = this.fillCharacter;
					
			}
		}
		
		// Print the graph
		for( int row = 0; row < graph.length; row++ ) //for each row
		{			
		
			if( this.graphSize - row > 99 )
				System.out.println();
			else if( this.graphSize - row > 9 )
				System.out.print( " " );
			else
				System.out.print( "  " );
			
			System.out.print( graphSize - row ); //print row #
			
			for( int col = 0; col < graph[row].length; col++ ) //for each col in each row
			{
				System.out.print( "  " + graph[row][col] );
			}
			
			System.out.println();
		}
		
		
		// Find number with largest string length
		int largestStringLength = 0;
		for( int i = 0; i < numbers.length; i++ )
			if( new Integer( numbers[i] ).toString().length() > largestStringLength )
				largestStringLength = new Integer( numbers[i] ).toString().length();
		
		char[][] numbersAtBottom = new char[ largestStringLength ][ numbers.length ];
		
		// Fill array
		for( int row = 0; row < numbersAtBottom.length; row++ )
		{
			for( int col = 0; col < numbersAtBottom[row].length; col++ )
			{
				if( new Integer( numbers[col] ).toString().length() > row )
					numbersAtBottom[row][col] = new Integer( numbers[col] ).toString().charAt( row );
				else
					numbersAtBottom[row][col] = ' ';
			}
		}
		
		// Print array
		for( int h = 0; h < numbersAtBottom.length; h++ )
		{
			System.out.print( "   " );
			
			for( int k = 0; k < numbersAtBottom[h].length; k++ )
				System.out.print( "  " + numbersAtBottom[h][k] );
			
			System.out.println();
		}
			
		
	} //printVerticleGraph
} //BarGraph