#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <stdio.h>

#include "registers.h"

/* Function definitions */
extern void parseFile( char *srcFilename, int *flags  );
extern void parseInstruction( Instruction *instruction, int programCounter, int *flags );
extern Instruction *readInstruction( FILE *instructionValue );

#endif
