import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.Date;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Random;

public class Comparison {
	public static void main( String[] args ) throws FileNotFoundException {
		//Create data structures
		PatrickBinarySearchTree<String> pbst = new PatrickBinarySearchTree<String>();
		PatrickBinarySearchTree<String> orderedLinkedList = new PatrickBinarySearchTree<String>();
		SingleLinkedList<String> linkedList = new SingleLinkedList<String>();
		ArrayList<String> holder = new ArrayList<String>(); //holds file data so we don't have to reread file each time

		//Read in file data
		Scanner fileScan = new Scanner( new File( args.length > 0 ? args[0] : "./books/sawyer.txt" ) );
		while( fileScan.hasNext() )
			 holder.add( fileScan.next() );
		fileScan.close();

		Collections.shuffle( holder ); //shuffle the contents

		//Creation of binary tree
		Date startInsertPBST = new Date();
		for( String s : holder ) {
			pbst.add( s );
		}
		Date endInsertPBST = new Date();

		System.out.println("--------------------------------------");
		System.out.println("Binary search tree creation: " + ( endInsertPBST.getTime() - startInsertPBST.getTime() ) + " milliseconds" );
		System.out.println("--------------------------------------");

		//Measure traversal time - using String class (NOT STRINGBUILDER)
		Date startTraversalPBST = new Date();
		String traversePBST = pbst.toString();
		Date endTraversalPBST = new Date();

		System.out.println("Binary search tree traversal: " + (endTraversalPBST.getTime() - startTraversalPBST.getTime() ) + " milliseconds" );

		//Measure traversal time - using StringBuilder class
		Date startTraversalPBST2 = new Date();
		StringBuilder traversePBST2 = pbst.toStringBuilder();
		Date endTraversalPBST2 = new Date();

		System.out.println("Binary search tree traversal with StringBuilder: " + (endTraversalPBST2.getTime() - startTraversalPBST2.getTime() ) + " milliseconds" );
		System.out.println("--------------------------------------");
		
		//Creation of an ordered linked list
		Date startOrderedLinkedList = new Date();
		//for( String s : holder ) linkedList.insertInOrder( s );
		Scanner stringParse = new Scanner( pbst.toString() );
		while( stringParse.hasNext() ) orderedLinkedList.add( stringParse.next() );
		Date endOrderedLinkedList = new Date();

		System.out.println("Creation of ordered linked list: " + ( endOrderedLinkedList.getTime() - startOrderedLinkedList.getTime() ) + " milliseconds" );

		//LinkedList traversal
		Date startLinkedListTraversal = new Date();
		StringBuilder traverseLinkedList = new StringBuilder();
		for( String s : linkedList ) traverseLinkedList.append( s );
		Date endLinkedListTraversal = new Date();

		System.out.println("Linked list traversal: " + (endLinkedListTraversal.getTime() - startLinkedListTraversal.getTime() ) + " milliseconds\n\n\n\n" );
	}
}
