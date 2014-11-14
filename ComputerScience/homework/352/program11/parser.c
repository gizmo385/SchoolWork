#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "processOpts.h"
#include "functCodes.h"
#include "opCodes.h"

void parseFile( char *srcFilename, int *flags) {
    debug( "Parsing file: %s\n", srcFilename );

    FILE *file = fopen( srcFilename, "r" );
    if( file == NULL ) {
        fprintf( stderr, "The file \"%s\" does not exist!\n", srcFilename );
    }

    int programCounter = 0x00400024;
    while( !feof( file ) ) {
        Instruction *instruction = readInstruction( file );
        parseInstruction( instruction, programCounter, flags );
        free( instruction );
        programCounter += 4;
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

void parseInstruction( Instruction *instruction, int programCounter, int *flags ) {

    Iformat iformat = instruction->iformat;
    Rformat rformat = instruction->rformat;
    Jformat jformat = instruction->jformat;

    int opcode = jformat.opcode;
    int funcCode = rformat.funcCode;

    char *opCodeName = OpCodes[opcode].name;

    switch( opcode ) {
    // R Format Instructions
    case 0:
        if( flags[ R_FORMAT_INDEX ] ) {
            char *funcCodeName = FunctCodes[ funcCode ].name;

            switch( funcCode ) {
                // Shift instructions
                case 0: // sll
                case 2: // srl
                case 3: // sra
                    printf( "%x %-7s %s, %s, %d\n", programCounter, funcCodeName,
                            mipsRegisters[ rformat.rt ].registerName,
                            mipsRegisters[ rformat.rd ].registerName,
                            rformat.shamt );
                    break;

                case 8: // jr
                    printf( "%x %-7s %s\n", programCounter, funcCodeName,
                            mipsRegisters[ rformat.rs ].registerName );
                    break;

                case 12: // syscall
                    printf("%x syscall\n", programCounter );
                    break;
                case 32: // add
                case 33: // addu
                case 34: // sub
                case 35: // subu
                case 36: // and
                case 37: // or
                case 38: // xor
                case 39: // nor
                case 42: // slt
                case 43: // sltu
                    printf( "%x %-7s %s, %s, %s\n", programCounter, funcCodeName,
                            mipsRegisters[ rformat.rd ].registerName,
                            mipsRegisters[ rformat.rs ].registerName,
                            mipsRegisters[ rformat.rd ].registerName );
                    break;
            }
        }
        break;
    // Jump instructions
    case 2:
    case 3:
        if( flags[ J_FORMAT_INDEX ] ) {
            printf( "%x %-7s 0x00%x\n", programCounter, opCodeName, jformat.target * 4 );
        }
        break;
    // Branch Instructions
    case 4:
    case 5:
        if( flags[ I_FORMAT_INDEX ] ) {
            printf( "%x %-7s %s, %s, %x\n", programCounter, opCodeName,
                    mipsRegisters[ iformat.rs ].registerName,
                    mipsRegisters[ iformat.rt ].registerName, iformat.immediate );
        }
        break;
    // Arithmetic instructions using an immediate
    case 8:  // addi
    case 9:  // addiu
    case 10: // slti
    case 12: // andi
    case 13: // ori
    case 14: // xori
        if( flags[ I_FORMAT_INDEX ] ) {
            printf( "%x %-7s %s, %s, %d\n", programCounter, opCodeName,
                    mipsRegisters[ iformat.rs ].registerName,
                    mipsRegisters[ iformat.rs ].registerName, iformat.immediate );
        }
        break;
    // Load upper immediate
    case 15:    // lui
        if( flags[ I_FORMAT_INDEX ] ) {
            printf( "%x %-7s %s, %d\n", programCounter, opCodeName,
                    mipsRegisters[ iformat.rs ].registerName, iformat.immediate );
        }
        break;
    // Memory reference instructions
    case 32: // lb
    case 33: // lh
    case 35: // lw
    case 40: // sb
    case 41: // sh
    case 43: // sw
        if( flags[ I_FORMAT_INDEX ] ) {
            printf( "%x %-7s %s, %d(%s)\n", programCounter, opCodeName,
                    mipsRegisters[ iformat.rt ].registerName, iformat.immediate,
                    mipsRegisters[ iformat.rs ].registerName );
        }
        break;
    }
}
