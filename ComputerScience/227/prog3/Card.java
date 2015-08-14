/*
 * Chris Chapline
 * Tyson Hoekstra
 * Your section #
 *
 * Assignment 3 - Problem 1, CSc 227, Spring 2013
 * Holds data on a single card in a card deck
 */
 
import java.util.Random;
 
public class Card
{
	// Instance variables
	private int card;
	String [] names = { "Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King" };
	String [] suits = { "Hearts", "Clubs", "Diamonds", "Spades" };
	
	// Constructor
	public Card( Random aRandom )
	{
		setCard( aRandom.nextInt( 52 ) );
	}
	
	// Instance methods 
	public int getCard()
	{
		return this.card;
	}
	
	public void setCard( int cardID  )
	{
		if( cardID >= 0 && cardID <= 51	)
			this.card = cardID;
			
		else this.card = 0;
	}
	
	public String getName()
	{
		return names[ getCard() % 13 ];
	}
	
	public String getSuit()
	{
		if( this.card >= 0 && this.card <= 12 )
			return "Hearts";
		else if( this.card >= 13 && this.card <= 25 )
			return "Clubs";
		else if( this.card >= 26 && this.card <= 38 )
			return "Diamonds";
		else return "Spades";
	}
	
	public String toString()
	{
		return getName() + " of " + getSuit();
	}
}