/*
 * Christopher Chapline
 * Tyson Hoekstra
 * 3
 *
 * Assignment 3 - Problem 2, CSc 227, Spring 2013
 * Creates and maintains a 5 card poker hand
 */

import java.util.Random;

public class Hand
{
	//instance variable
	private Card[] playerHand;
	
	//Constructor
	public Hand( Random aRandom )
	{
		playerHand = new Card[5];
		
		for( int i = 0; i < playerHand.length; i++ )
		{
			playerHand[i] = new Card( aRandom );
			
			for( int k = 0; k < i; k++ )
				if( k != i && playerHand[i].getCard() == playerHand[k].getCard() )
					playerHand[i] = new Card( aRandom );
		}
			
	} //end constructor
	
	public String toString()
	{
		String handContents = "";
		for( Card c : playerHand )
			handContents += c.toString() + "\n";
			
		return handContents;
	}
	
	public boolean pair()
	{
		for( int i = 0; i < playerHand.length; i++ )
		{
			int sameNameCards = 0;
			for( int k = 0; k < playerHand.length; k++ )
				if( playerHand[i].getName().equals( playerHand[k].getName() ) ) sameNameCards++; //if card names are identical
				
			if( sameNameCards == 2 ) return true; // pair found
		}
		
		return false; //all cards tested - return false
	}
	
	public boolean threeOfAKind()
	{
		for( int i = 0; i < playerHand.length; i++ )
		{
			int sameNameCards = 0;
			for( int k = 0; k < playerHand.length; k++ )
				if( playerHand[i].getName().equals( playerHand[k].getName() ) ) sameNameCards++; //if card names are identical
				
			if( sameNameCards == 3 ) return true; // three of a kind found
		}
		
		return false; //all cards tested - return false
	}
	
	public boolean fourOfAKind()
	{
		for( int i = 0; i < playerHand.length; i++ )
		{
			int sameNameCards = 0;
			for( int k = 0; k < playerHand.length; k++ )
				if( playerHand[i].getName().equals( playerHand[k].getName() ) ) sameNameCards++; //if card names are identical
				
			if( sameNameCards == 4 ) return true; // four of a kind found
		}
		
		return false; //all cards tested - return false
	}
	
	public boolean fullHouse()
	{
		if( pair() == true && threeOfAKind() == true )
			return true;
		else
			return false;
	}
	
	public boolean flush() //tests first card against all others, then returns result
	{
		return ( playerHand[0].getSuit().equals( playerHand[1].getSuit() ) && 
					playerHand[0].getSuit().equals( playerHand[2].getSuit() ) && 
					playerHand[0].getSuit().equals( playerHand[3].getSuit() ) &&
					playerHand[0].getSuit().equals( playerHand[4].getSuit() ) );  
	}
} //end Hand class