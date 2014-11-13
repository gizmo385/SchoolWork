#ifndef REGISTERS_H
#define REGISTERS_H

/* Type definitions */
typedef struct Jformat {
    unsigned int target:26;
    unsigned int opcode:6;
} Jformat;

typedef struct Iformat {
    unsigned int immediate:16;
    unsigned int rt:5;
    unsigned int rs:5;
    unsigned int opcode:6;
} Iformat;

typedef struct Rformat {
    unsigned int funcCode:6;
    unsigned int shamt:5;
    unsigned int rd:5;
    unsigned int rt:5;
    unsigned int rs:5;
    unsigned int opcode:6;
} Rformat;

typedef union Instruction {
    unsigned int instruction;
    Jformat jformat;
    Iformat iformat;
    Rformat rformat;
} Instruction;

typedef struct MIPSRegister {
    char *registerName;
} MIPSRegister;

extern MIPSRegister mipsRegisters[];

#endif
