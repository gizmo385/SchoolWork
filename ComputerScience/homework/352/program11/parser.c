#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "processOpts.h"
#include "functCodes.h"
#include "opCodes.h"

void parseFile( char *srcFilename ) {
    debug( "Parsing file: %s\n", srcFilename );

    FILE *file = fopen( srcFilename, "r" );
    if( file == NULL ) {
        fprintf( stderr, "The file \"%s\" does not exist!\n", srcFilename );
    }

    while( !feof( file ) ) {
        Instruction *instruction = readInstruction( file );
        parseInstruction( instruction );
        free( instruction );
    }
}

Instruction *readInstruction( FILE *file ) {
    // Read the next 32-bit value from the file
    int *instructionValue = (int *) malloc( sizeof(int) );
    fread( instructionValue, sizeof(int), 1, file );

    // Create an instruction from the instruction value
    Instruction *parsedInstruction = (Instruction *) malloc( sizeof(Instruction) );
    parsedInstruction->instruction = *instructionValue;

    free( instructionValue );

    return parsedInstruction;
}

void parseInstruction( Instruction *instruction ) {

    int opcode = instruction->jformat.opcode;

    /*
     * Based on the opCode, determine the format of the instruction. When the opcode is 0, it is
     * an R format instruction and has a function code.
     */
    if( opcode == 0 ) {
        printf( "functCode: %s\n", FunctCodes[instruction->rformat.funcCode].name );
    } else {
        printf( "opcode: %s\n", OpCodes[opcode].name );
    }
}
