#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "processOpts.h"
#include "functCodes.h"
#include "opCodes.h"

void printRFormatInstruction( Rformat rformat, int programCounter );

void parseFile( char *progName, char *srcFilename, int *flags) {
    debug( "Parsing file: %s\n", srcFilename );

    FILE *file = fopen( srcFilename, "r" );
    if( file == NULL ) {
        fprintf( stderr, "Unable to open %s for reading\n", srcFilename );
        usage( progName, stderr );
    }

    int programCounter = 0x00400024;
    while( !feof( file ) ) {
        Instruction *instruction = readInstruction( file );
        if( ! feof( file ) ) {
            parseInstruction( instruction, programCounter, flags );
        }
        free( instruction );
        programCounter += 4;
    }

    printf("\n");

    fclose( file );
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
    char *opCodeName = OpCodes[opcode].name;

    switch( opcode ) {
    // R Format Instructions
    case 0:
        if( flags[ R_FORMAT_INDEX ] ) {
            printRFormatInstruction( rformat, programCounter );
        }
        break;
    // Jump instructions
    case 2:
    case 3:
        if( flags[ J_FORMAT_INDEX ] ) {
            printf( "0x00%X %-7s  0x00%X\n", programCounter, opCodeName, jformat.target * 4 );
        }
        break;
    // Branch Instructions
    case 4:
    case 5:
        if( flags[ I_FORMAT_INDEX ] ) {
            printf( "0x00%X %-7s  %s, %s, 0x00%X\n", programCounter, opCodeName,
                    mipsRegisters[ iformat.rs ].registerName,
                    mipsRegisters[ iformat.rt ].registerName,
                    programCounter + (iformat.immediate << 2) );
        }
        break;
    // Arithmetic instructions using an immediate
    case 8:  // addi
    case 10: // slti
        if( flags[ I_FORMAT_INDEX ] ) {
            int i = iformat.immediate;
            printf( "0x00%X %-7s  %s, %s, %hd\n", programCounter, opCodeName,
                    mipsRegisters[ iformat.rt ].registerName,
                    mipsRegisters[ iformat.rs ].registerName, i);
        }
        break;
    case 9:  // addiu
        if( flags[ I_FORMAT_INDEX ] ) {
            unsigned int actualImmediate = iformat.immediate;
            printf( "0x00%X %-7s  %s, %s, %hu\n", programCounter, opCodeName,
                    mipsRegisters[ iformat.rt ].registerName,
                    mipsRegisters[ iformat.rs ].registerName,
                    actualImmediate );
        }
        break;
    case 12: // andi
    case 13: // ori
    case 14: // xori
        if( flags[ I_FORMAT_INDEX ] ) {
            unsigned int actualImmediate = iformat.immediate;
            printf( "0x00%X %-7s  %s, %s, %hu\n", programCounter, opCodeName,
                    mipsRegisters[ iformat.rt ].registerName,
                    mipsRegisters[ iformat.rs ].registerName,
                    actualImmediate);
        }
        break;
    // Load upper immediate
    case 15:    // lui
        if( flags[ I_FORMAT_INDEX ] ) {
            unsigned int actualImmediate = iformat.immediate;
            printf( "0x00%X %-7s  %s, %hu\n", programCounter, opCodeName,
                    mipsRegisters[ iformat.rt ].registerName, actualImmediate );
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
            printf( "0x00%X %-7s  %s, %hd(%s)\n", programCounter, opCodeName,
                    mipsRegisters[ iformat.rt ].registerName, iformat.immediate,
                    mipsRegisters[ iformat.rs ].registerName );
        }
        break;
    default:
        debug("Opcode == %d\n", opcode );
    }
}

void printRFormatInstruction( Rformat rformat, int programCounter ) {
    int funcCode = rformat.funcCode;
    char *funcCodeName = FunctCodes[ funcCode ].name;

    switch( funcCode ) {
        debug( "Function code: %d(%s)\n", funcCode, funcCodeName );
        // Shift instructions
        case 0: // sll
            printf( "0x00%X %-7s  %s, %s, %d\n", programCounter, funcCodeName,
                    mipsRegisters[ rformat.rd ].registerName,
                    mipsRegisters[ rformat.rt ].registerName,
                    rformat.shamt);
            return;
        case 2: // srl
        case 3: // sra
            printf( "0x00%X %-7s  %s, %s, %d\n", programCounter, funcCodeName,
                    mipsRegisters[ rformat.rd ].registerName,
                    mipsRegisters[ rformat.rt ].registerName,
                    rformat.shamt );
            return;
        case 8: // jr
            printf( "0x00%X %-7s  %s\n", programCounter, funcCodeName,
                    mipsRegisters[ rformat.rs ].registerName );
            return;
        case 12: // syscall
            printf("0x00%X syscall  \n", programCounter );
            return;
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
            printf( "0x00%X %-7s  %s, %s, %s\n", programCounter, funcCodeName,
                    mipsRegisters[ rformat.rd ].registerName,
                    mipsRegisters[ rformat.rs ].registerName,
                    mipsRegisters[ rformat.rt ].registerName );
            return;
        default:
            debug( "Opcode == 0 && funcCode == %d\n", funcCode );
            return;
    }
}
