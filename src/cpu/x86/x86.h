#ifndef _BUGGY_DASM_X86_H_
#define _BUGGY_DASM_X86_H_

typedef struct
{
    BYTE    nPrefix;
    BYTE    Prefix[4];
    BYTE    nREX;
    BYTE    REX[1];
    BYTE    nOpcode;
    BYTE    Opcode[3];
    BYTE    nModRM;
    BYTE    ModRM[1];
    BYTE    nSIB;
    BYTE    SIB[1];
    BYTE    nDisplacement;
    BYTE    Displacement[4];
    BYTE    nImmediate;
    BYTE    Immediate[4];
} INSTRUCTION_X86, *LPINSTRUCTION_X86;

#define OPCODE_INVALID      0x00
#define CPU_X86_32BITS  0x01
#define CPU_X86_64BITS  0x02
#define OPCODE_32BITS_VALID 0x01
#define OPCODE_64BITS_VALID 0x02
#define OPCODE_VALID        0x03
#define OPCODE_ESCAPE       0xFF

extern  BYTE PrefixTable[256];
extern  BYTE REXTable[256];
extern  BYTE OpcodeTable[4][256];

#endif  /* _BUGGY_DASM_X86_H_ */