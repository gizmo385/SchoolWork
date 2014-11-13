#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <stdio.h>

#include "registers.h"

/* Function definitions */
extern void parseFile( char *srcFilename );
extern void parseInstruction( Instruction *instruction );
extern Instruction *readInstruction( FILE *instructionValue );

#endif
