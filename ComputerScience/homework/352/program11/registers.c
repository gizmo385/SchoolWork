#include "registers.h"

/*
 * There are 32 registers in the MIPS architecture:
 * 1 zero register
 * 1 register reserved for the assembler
 * 2 function result registers
 * 4 argument registers
 * 10 temporary registers
 * 8 "saved registers"
 * 2 registers reserved for the kernel
 * 1 global area pointer register
 * 1 stack pointer register
 * 1 frame pointer register
 * 1 return address register
 *
 * The registers in this array are organized numerically as they are organized in the MIPS
 * archiecture. That is to say, the 0th-31st registers can have their associated names accessed by
 * accessing the 0th-31st indices in this array respectively.
 */
MIPSRegister mipsRegisters[] = {
    { "$zero" },
    { "$at" },
    { "$v0" },
    { "$v1" },
    { "$a0" },
    { "$a1" },
    { "$a2" },
    { "$a3" },
    { "$t0" },
    { "$t1" },
    { "$t2" },
    { "$t3" },
    { "$t4" },
    { "$t5" },
    { "$t6" },
    { "$t7" },
    { "$s0" },
    { "$s1" },
    { "$s2" },
    { "$s3" },
    { "$s4" },
    { "$s5" },
    { "$s6" },
    { "$s7" },
    { "$t8" },
    { "$t9" },
    { "$k0" },
    { "$k1" },
    { "$gp" },
    { "$sp" },
    { "$fp" },
    { "$ra" }
};
