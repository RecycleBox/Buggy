#include    <windows.h>
#include    "x86.h"

int WINAPI  BuggyDecode(
    LPBYTE lpInstruction,
    LPINSTRUCTION_X86 lpInstruction
    BYTE CPUMode
    )
{
    // The Intel 386 processor sets a limit of 15 bytes on instruction length.
    INSTRUCTION_X86 Instruction;
    int i = 0;

    // 1. Instruction Prefixes : Up to four prefixes of 1 byte each (optional)
    {
        int count = 0;
        for (; count < 3; i++)
        {
            BYTE mask = 0x00;

            if (PrefixTable[lpInstruction[i]])  //this byte is a defined prefix
            {
                if (mask & lpInstruction[i])    //contains same bit => have two prefixes from the same group
                {
                    return  0;  //FIXME
                }

                Instruction.Prefix[count] = lpInstruction[i];
                count++;
                mask = mask | lpInstruction[i];
            }
            else
            {
                break;
            }
        }
        Instruction.nPrefix = count;
    }

    // 2. REX prefix : 1 byte (if required) (64-Bit only)
    if (CPUMode == CPU_X86_64BITS)
    {
        int count = 0;
        for (; count < 1; i++)
        {
            if (REXTable[lpInstruction[i]])  //this byte is a defined REX prefix
            {
                Instruction.REX[count] = lpInstruction[i];
                count++;
            }
            else
            {
                break;
            }
        }
        Instruction.nREX = count;
    }
    else
    {
        Instruction.nREX = 0;
    }

    // 3. Opcode : 1 - , 2 - , or 3 - byte opcode
    {
        BYTE(*lpOpcodeTable)[256];
        int count = 0;
        if (lpInstruction[i] != 0x0F)   //One-byte Opcode
        {
            lpOpcodeTable = &OpcodeTable[0];
        }
        else
        {
            switch (lpInstruction[i + 1])
            {
            case 0x38:
                lpOpcodeTable = &OpcodeTable[2];    //Three-byte Opcode Map (First two bytes are 0F 38H)
                Instruction.Opcode[0] = 0x0F;
                Instruction.Opcode[1] = 0x38;
                i = i + 2;
                count = 2;
                break;
            case 0x3A:
                lpOpcodeTable = &OpcodeTable[3];    //Three-byte Opcode Map (First two bytes are 0F 3AH)
                Instruction.Opcode[0] = 0x0F;
                Instruction.Opcode[1] = 0x3A;
                i = i + 2;
                count = 2;
                break;
            default:
                lpOpcodeTable = &OpcodeTable[1];    //Two-byte Opcode Map (First Byte is 0FH)
                Instruction.Opcode[0] = 0x0F;
                i = i + 1;
                count = 1;
            }
        }

        if (*lpOpcodeTable[lpInstruction[i]] & CPUMode)  //this byte is a defined opcode
        {
            Instruction.Opcode[count] = lpInstruction[i];
            count++;
            i++;
        }
        else
        {
            return 0;
        }
        Instruction.nOpcode = count;
    }


    // 4. ModR/M (Addressing-form specifier byte) : 1 byte (if required)

    // SIB (Scale-Index-Base byte) : 1 byte (if required)
    // Displacement : Address displacement of 1, 2, or 4 bytes or none
    // Immediate : Immediate data of 1, 2, or 4 bytes or none

    return 0;
}