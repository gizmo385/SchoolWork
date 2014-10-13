#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLANKLINE 			0
#define ONE_LINE_COMMENT	1
#define STRING_LITERAL		2
#define CHARACTER_CONSTANT	3
#define BLOCK_COMMENT		4

#define TRUE                1
#define FALSE               0

/*
 * Function Prototypes
 */
void genericParse( char currentChar, char endParseCharacter );
void parseStringLiteral( char currentChar );
void parseSingleLineComment( char currentChar );
void parseBlockComment( char currentChar );
void parseCharacterConstant( char currentChar );

/**
 * A more generic parse method to easily define further parsing functions.
 *
 * Arguments:
 * --------------
 * currentChar -- The character that the reader is currently on.
 * endParseCharacter -- The character that should end the call. After that
 *                      character is encountered, parsing should stop.
 */
void genericParse( char currentChar, char endParseCharacter) {
    short escapingNextChar = FALSE;
    do {

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
 */
void parseStringLiteral(char currentChar) {
    genericParse(currentChar, '"');
}

/**
 * Parses out character constants which begin and end with single-quotation marks
 *
 * Arguments:
 * --------------
 * currentChar -- The character that the reader is currently on.
 */
void parseCharacterConstant(char currentChar) {
    genericParse(currentChar, '\'');
}

/**
 * Parses out comments. This will delegate to either parseSingleLineComment or
 * parseBlockComment depending on the character immediately following the
 * initial forward slash.
 *
 * Arguments:
 * --------------
 * currentChar -- The character that the reader is currently on.
 */
void parseComment(char currentChar) {
    printf("%c", currentChar);

    // Determine what kind of comment has been encountered
    currentChar = getchar();
    if( currentChar == '/' ) {
        parseSingleLineComment(currentChar);
    } else if( currentChar == '*' ) {
        parseBlockComment(currentChar);
    }
}

/**
 * Parses out a single-line comment. A single line comment begins with "//" and
 * continues until the end of the line.
 *
 * Arguments:
 * --------------
 * currentChar -- The character that the reader is currently on.
 */
void parseSingleLineComment(char currentChar) {
    genericParse(currentChar, '\n');
}

/**
 * Parses a block comment. A block comment can span multiple lines, begins with
 * a forward slash followed by an asterick, and ends with an asterick followed
 * by a forward slash
 *
 * Arguments:
 * --------------
 * currentChar -- The character that the reader is currently on.
 */
void parseBlockComment( char currentChar ) {
    short lookingForEnd = FALSE;

    printf("%c", currentChar);
    while( TRUE ) {
        currentChar = getchar();
        printf("%c", currentChar );

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
	/*int totalsEnabled = FALSE;*/

	// Check if file totals are enabled
	if( argc > 1 ) {
		if( strcmp(argv[0], "-t") == 0 ) {
			/*totalsEnabled = TRUE;*/
		}
	}

	/*int totals[5] = {0};*/

	char ch = 0;

    while( (ch = getchar()) != EOF ) {
        switch(ch) {
            case '"':
                parseStringLiteral(ch);
                break;
            case '\n':
            case '\t':
                printf("%c", ch);
                break;
            case '\'':
                parseCharacterConstant(ch);
                break;
            case '/':
                parseComment(ch);
                break;
            default:
                printf(" ");
                break;
        }
    }

    /*printf("\n");*/
}
