/*=============================================================================
 |   Assignment:  Program #5: Prime Factorization
 |       Author:  Christopher Chapline (cachapline8@email.arizona.edu)
 |       Grader:  Nick Topping
 |
 |       Course:  Introduction to Discrete Structures (245)
 |   Instructor:  L. McCann
 |     Due Date:  November 8th, 2013 2PM
 |
 |  Description:  This program demonstrates the Fundamental Theorem of Arithmetic
 |			      which states that every integer is either prime, or the product
 |				  of 2 or more primes.
 |                
 | Deficiencies: 
 *===========================================================================*/
public class Hmwk5 {

	 /*---------------------------------------------------------------------
    |  Method main
    |
    |  Purpose: Handles I/O with the client, validates the client's input and then
    |			calls private helper methods to establish the prime factorization
    |			of the provided integer.
    |
    |  Pre-condition: None.
    |
    |  Post-condition: None.
    |
    |  Parameters:
    |      args -- An array of String's passed to the program through the java Hmwk5
    |				command. This input is parses as an integer for the purpose of
    |				this program.
    |
    |  Returns:  None.
    *-------------------------------------------------------------------*/
	public static void main( String[] args ) {
		int numberToFactor = 0;

		//Validate input
		if( args.length < 1 ) {
			System.out.println( "Usage: java Hmwk5 <integer>" );
			System.exit(0);
		}
		else {
			try {
				numberToFactor = Integer.parseInt( args[0].trim() );
			}
			catch( NumberFormatException nfe ) {
				System.out.println( "Invalid input!" );
				System.out.println( "Usage: java Hmwk5 <integer>" );
				System.exit(0);
			}
		}

		//Factor the number
		factor( numberToFactor, 0 );
		System.out.println ("\nAs this output shows, the Fundamental Theorem of Arithemtic holds for " + numberToFactor );
	}
	
	 /*---------------------------------------------------------------------
    |  Method factor
    |
    |  Purpose:  This method will recursively determine the prime factorization
    |			 of a given integer. It will print throughout its running the
    |			 process of factorization until it reaches complete prime
    |			 factorization of the given integer.
    |
    |  Pre-condition: The integer passed to this method must be greater than
    |				  or equal to 2. This satisfies the condition that prime
    |				  and composites only partition the set of integers that
    |				  are greater than or equal to 2.
    |
    |  Post-condition: None.
    |
    |  Parameters:
    |      numberToFactor -- This is the integer that the method is
    |						 generating the prime factorization for. It is
    |						 assumed that this integer is >= 2.
    |      currentIndentationLevel -- This is the number of indentations to
    |								  place at the beginning of each print
    |								  statement. Used strictly for formatting.
    |
    |  Returns: None.
    *-------------------------------------------------------------------*/
	private static void factor( int numberToFactor, int currentIndentationLevel ) {
		String indentation = "";
		
		//Add indentation
		for( int i = 0; i < currentIndentationLevel; i++ ) {
			indentation += "\t";
		}
		
		System.out.print( indentation );
		
		//Check if the number to factor is prime
		if( isPrime( numberToFactor ) ) {
			System.out.println( numberToFactor + " is prime." );
		}
		
		//Otherwise, determine it's factors and factorize them
		else {
			int firstFactor = 0;
			int secondFactor = 0;
			for( int i = 2; i < numberToFactor; i++ ) {
				if( numberToFactor % i == 0 ) {
					firstFactor = i;
					secondFactor = numberToFactor / firstFactor;
					break;
				}
			}
			
			//If the two factors are equal (numberToFactor is the square of some number)
			if( firstFactor == secondFactor ) {
				System.out.println(numberToFactor + " = " + firstFactor + " squared; is this factor either prime or the product of primes? ");
				
				//Factor the singular factor
				factor( firstFactor, currentIndentationLevel + 1 );
			}
			
			
			else {
				System.out.println( numberToFactor + " = " + firstFactor + " * " + secondFactor + "; are these factors either prime or products of primes." );
				
				//Factor the two factors
				factor( firstFactor, currentIndentationLevel + 1 );
				factor( secondFactor , currentIndentationLevel + 1 );
				System.out.println( indentation + numberToFactor + " is the product of primes (" + firstFactor + " and " + secondFactor + " are prime or prime products)." );
			}
		}
	}


	/*---------------------------------------------------------------------
    |  Method isPrime
    |
    |  Purpose: This method will test the primality of some given integer
    |			by testing for a prime factor that is less than or equal to
    |			the square root of itself.
    |
    |  Pre-condition: The integer passed to this method must be a positive
    |					integer greater than or equal to 2.
    |
    |  Post-condition: None
    |
    |  Parameters:
    |      numberToTest -- This is the integer for which we are testing
    |						primality. It is expected by the method that
    |						this integer is greater than or equal to 2,
    |						as testing primality on an integer that does not
    |						satisfy this condition is not possible.
    |
    |  Returns: True if the provided integer meets the conditions of a prime
    |			integer in that its only factors are 1 and itself. The method
    			will return false otherwise.
	 *-------------------------------------------------------------------*/
	private static boolean isPrime( int numberToTest ) {
		//First test 2, since it is the only even prime number
		if( numberToTest % 2 == 0  && numberToTest != 2 ) return false;
		
		//Now we can test strictly odd numbers
		else {
			for( int i = 3; i <= Math.sqrt( numberToTest ); i += 2 ) {
				//If some number evenly divides the number, return false
				if( numberToTest % i == 0 ) {
					return false;
				}
			}
		}
		
		//At this point, we know numberToTest is prime, therefore return true
		return true;
	}
}