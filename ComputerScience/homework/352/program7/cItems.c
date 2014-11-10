#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLANKLINE 			0
#define STRING_LITERAL		1
#define CHARACTER_CONSTANT	2
#define ONE_LINE_COMMENT	3
#define BLOCK_COMMENT		4
#define LINES               5

#define TRUE                1
#define FALSE               0

/*
 * Function Prototypes
 */
int parseComment( char currentChar, int *totals, int lineElements[][5] );
int parseSingleLineComment( char currentChar, int *totals, int lineElements[][5] );
void parseBlockComment( char currentChar, int *totals, int lineElements[][5] );
void parseStringLiteral( char currentChar, int *totals, int lineElements[][5] );
void parseCharacterConstant( char currentChar, int *totals, int lineElements[][5] );
void genericParse( char currentChar, char endParseCharacter, int *totals, int totalsIndex,
        int lineElements[][5] );

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
    lineElements[ totals[LINES] ][ STRING_LITERAL ] = TRUE;
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
    lineElements[ totals[LINES] ][ CHARACTER_CONSTANT ] = TRUE;
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
int parseComment(char currentChar, int *totals, int lineElements[][5] ) {

    // Determine what kind of comment has been encountered
    char nextChar = getchar();
    if( nextChar == '/' ) {
        printf("%c", currentChar);
        return parseSingleLineComment( nextChar, totals, lineElements );
    } else if( nextChar == '*' ) {
        printf("%c", currentChar);
        parseBlockComment( nextChar, totals, lineElements );
        return FALSE;
    } else {
        printf("  ");
        return FALSE;
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
 * lineElements -- Represents whether or not a certain character type is present on a particular
 *                  line
 */
int parseSingleLineComment( char currentChar, int *totals, int lineElements[][5] ) {
    lineElements[ totals[LINES] ][ ONE_LINE_COMMENT ] = TRUE;
    genericParse( currentChar, '\n', totals, ONE_LINE_COMMENT, lineElements);
    totals[LINES]++;
    return TRUE;
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
 * lineElements -- Represents whether or not a certain character type is present on a particular
 *                  line
 */
void parseBlockComment( char currentChar, int *totals, int lineElements[][5] ) {
    // This flag represents whether or not an asterick has been encountered
    short lookingForEnd = FALSE;
    totals[BLOCK_COMMENT]++;
    lineElements[ totals[LINES] ][ BLOCK_COMMENT ] = TRUE;
    printf("%c", currentChar);

    // Consume the characters for the block comment
    while( TRUE ) {
        currentChar = getchar();
        printf("%c", currentChar );

        // Increment line counts
        if( currentChar == '\n' ) {
            lineElements[ totals[LINES] ][ BLOCK_COMMENT ] = TRUE;
            totals[LINES]++;
            lineElements[ totals[LINES] ][ BLOCK_COMMENT ] = TRUE;
        }

        // Check for the end of the block comment
        if( currentChar == '*' ) {
            lookingForEnd = TRUE;
        } else if( currentChar == '/' ) {
            if( lookingForEnd ) {
                return;
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
    int lineElements[9999][5] = { {FALSE} };
    int isBlankLine = TRUE;

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
                // If nothing has occured on the line, mark the line as blank
                if( isBlankLine ) {
                    totals[ BLANKLINE ]++;
                    lineElements[ totals[LINES] ][BLANKLINE] = TRUE;
                }

                printf("%c", ch);
                totals[LINES]++;
                isBlankLine = TRUE;
                break;
            case ' ':
            case '\t':
                printf("%c", ch);
                break;
            case '"':
                isBlankLine = FALSE;
                parseStringLiteral(ch, totals, lineElements);
                break;
            case '\'':
                isBlankLine = FALSE;
                parseCharacterConstant(ch, totals, lineElements);
                break;
            case '/':
                isBlankLine = FALSE;
                int consumesLine = parseComment(ch, totals, lineElements);
                isBlankLine = consumesLine;
                break;
            default:
                // If we encounter a character on the line, it is not blank and we print a space
                isBlankLine = FALSE;
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

            // If it is a blank line, just print that and continue the loop. Don't check other
            // conditions
            if( lineElements[i][BLANKLINE] == TRUE ) {
                printf("%s|           |           |           |           |\n", present);
                continue;
            } else {
                printf("%s|", notPresent);
            }

            // Otherwise, check the other conditions because several can exist on the same line
            for( int j = 1; j < 5; j++ ) {
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
