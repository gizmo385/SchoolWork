#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLANKLINE 			0
#define ONE_LINE_COMMENT	1
#define STRING_LITERAL		2
#define CHARACTER_CONSTANT	3
#define BLOCK_COMMENT		4
#define LINES               5

#define TRUE                1
#define FALSE               0

/*
 * Function Prototypes
 */
void genericParse( char currentChar, char endParseCharacter, int *totals, int totalsIndex,
        int lineElements[][5] );
void parseStringLiteral( char currentChar, int *totals, int lineElements[][5] );
void parseComment( char currentChar, int *totals, int lineElements[][5] );
void parseSingleLineComment( char currentChar, int *totals, int lineElements[][5] );
void parseBlockComment( char currentChar, int *totals, int lineElements[][5] );
void parseCharacterConstant( char currentChar, int *totals, int lineElements[][5] );

/**
 * A more generic parse method to easily define further parsing functions.
 *
 * Arguments:
 * --------------
 * currentChar -- The character that the reader is currently on.
 * endParseCharacter -- The character that should end the call. After that character is
 *                      encountered, parsing should stop.
 * totals -- An array that contains the totals for the different characters being parsed
 * totalsindex -- The index into totals that is being incremented
 * lineElements -- Represents whether or not a certain character type is present on a particular
 *                  line
 */
void genericParse( char currentChar, char endParseCharacter, int *totals, int totalsIndex,
        int lineElements[][5] ) {
    short escapingNextChar = FALSE;
    totals[ totalsIndex ]++;

    do {
        // Handle escape characters
        if( !escapingNextChar ) {
            if( currentChar == '\\' ) {
                escapingNextChar = TRUE;
            }
        } else {
            escapingNextChar = FALSE;
        }

        printf("%c", currentChar);
    } while( (currentChar = getchar()) != endParseCharacter || escapingNextChar );

    printf("%c", currentChar);
}

/**
 * Parses out string literals which begin and end with double-quotation marks
 *
 * Arguments:
 * --------------
 * currentChar -- The character that the reader is currently on.
 * totals -- An array that contains the totals for the different characters being parsed
 * lineElements -- Represents whether or not a certain character type is present on a particular
 *                  line
 */
void parseStringLiteral( char currentChar, int *totals, int lineElements[][5] ) {
    genericParse(currentChar, '"', totals, STRING_LITERAL, lineElements );
}

/**
 * Parses out character constants which begin and end with single-quotation marks
 *
 * Arguments:
 * --------------
 * currentChar -- The character that the reader is currently on.
 * totals -- An array that contains the totals for the different characters being parsed
 * lineElements -- Represents whether or not a certain character type is present on a particular
 *                  line
 */
void parseCharacterConstant( char currentChar, int *totals, int lineElements[][5] ) {
    genericParse( currentChar, '\'', totals, CHARACTER_CONSTANT, lineElements );
}

/**
 * Parses out comments. This will delegate to either parseSingleLineComment or
 * parseBlockComment depending on the character immediately following the
 * initial forward slash.
 *
 * Arguments:
 * --------------
 * currentChar -- The character that the reader is currently on.
 * totals -- An array that contains the totals for the different characters being parsed
 * lineElements -- Represents whether or not a certain character type is present on a particular
 *                  line
 */
void parseComment(char currentChar, int *totals, int lineElements[][5] ) {
    printf("%c", currentChar);

    // Determine what kind of comment has been encountered
    currentChar = getchar();
    if( currentChar == '/' ) {
        parseSingleLineComment( currentChar, totals, lineElements );
    } else if( currentChar == '*' ) {
        parseBlockComment( currentChar, totals, lineElements );
    } else {
        printf(" ");
    }
}

/**
 * Parses out a single-line comment. A single line comment begins with "//" and
 * continues until the end of the line.
 *
 * Arguments:
 * --------------
 * currentChar -- The character that the reader is currently on.
 * totals -- An array that contains the totals for the different characters being parsed
 * lineElements -- Represents whether or not a certain character type is present on a particular line
 */
void parseSingleLineComment( char currentChar, int *totals, int lineElements[][5] ) {
    genericParse( currentChar, '\n', totals, ONE_LINE_COMMENT, lineElements);
    totals[LINES]++;
}

/**
 * Parses a block comment. A block comment can span multiple lines, begins with
 * a forward slash followed by an asterick, and ends with an asterick followed
 * by a forward slash
 *
 * Arguments:
 * --------------
 * currentChar -- The character that the reader is currently on.
 * totals -- An array that contains the totals for the different characters being parsed
 * lineElements -- Represents whether or not a certain character type is present on a particular line
 */
void parseBlockComment( char currentChar, int *totals, int lineElements[][5] ) {
    // This flag represents whether or not an asterick has been encountered
    short lookingForEnd = FALSE;
    totals[BLOCK_COMMENT]++;
    printf("%c", currentChar);

    // Consume the characters for the block comment
    while( TRUE ) {
        currentChar = getchar();
        printf("%c", currentChar );

        // Increment line counts
        if( currentChar == '\n' ) {
            totals[LINES]++;
        }

        // Check for the end of the block comment
        if( currentChar == '*' ) {
            lookingForEnd = TRUE;
        } else if( currentChar == '/' ) {
            if( lookingForEnd ) {
                break;
            }
        } else {
            if( lookingForEnd ) {
                lookingForEnd = FALSE;
            }
        }
    }
}

int main( int argc, char *argv[] ) {
    int totalsEnabled = FALSE;
    int lineElements[9999][5];
    /*int currentLine = 0;*/

	// Check if file totals are enabled
	if( argc >= 2 ) {
        if( strcmp(argv[1], "-t") == 0 ) {
            totalsEnabled = TRUE;
        }
	}

    int totals[6] = {0};
	char ch = 0;

    // Parse the characters
    while( (ch = getchar()) != EOF ) {
        switch(ch) {
            case '\n':
                printf("%c", ch);
                totals[LINES]++;
                break;
            case '\t':
                printf("%c", ch);
                break;
            case '"':
                parseStringLiteral(ch, totals, lineElements);
                break;
            case '\'':
                parseCharacterConstant(ch, totals, lineElements);
                break;
            case '/':
                parseComment(ch, totals, lineElements);
                break;
            default:
                printf(" ");
                break;
        }
    }

    // If the totals option has been requested, print the calculated totals and line summary
    if( totalsEnabled == TRUE ) {
        printf("The program contains:\n");
        printf("%10d blank lines\n", totals[BLANKLINE]);
        printf("%10d string literals\n", totals[STRING_LITERAL]);
        printf("%10d character constants\n", totals[CHARACTER_CONSTANT]);
        printf("%10d one line comments\n", totals[ONE_LINE_COMMENT]);
        printf("%10d block comments\n", totals[BLOCK_COMMENT]);
        printf("%10d lines\n", totals[LINES]);

        printf("     |   blank   |  string   | character |  1 line   |   block   |\n");
        printf("line |   line    | litteral  | constant  |  comment  |  comment  |\n");
        printf("-----|-----------|-----------|-----------|-----------|-----------|\n");

        char *present    = "    XXX    ";
        char *notPresent = "           ";

        for( int i = 0; i < totals[LINES]; i++ ) {
            printf("%4d |", i + 1);

            if( lineElements[i][BLANKLINE] == TRUE ) {
                printf("%s|           |           |           |           |\n", present);
                continue;
            } else {
                printf("%s|", notPresent);
            }

            for( int j = 0; j < 4; j++ ) {
                if( lineElements[i][j] == TRUE ) {
                    printf("%s|", present);
                } else {
                    printf("%s|", notPresent);
                }
            }

            printf("\n");
        }
    }

    return 0;
}
