#include "x86_instruction.h"
#include "x86_instruction.inl"
#include "x86_register.h"
#include "x86_register.inl"

//------------------------------------------------------------------------------
x86_format::Format x86_instruction::Rxx()
{
    Format format;
    switch (opcode[0]) {
    case 0xC0:
    case 0xC1:
        switch (opcode[1] >> 3 & 7) {
        case 0: Decode(format, 1, "ROL", 0, opcode[0] & 0b01, 8);  break;
        case 1: Decode(format, 1, "ROR", 0, opcode[0] & 0b01, 8);  break;
        case 2: Decode(format, 1, "RCL", 0, opcode[0] & 0b01, 8);  break;
        case 3: Decode(format, 1, "RCR", 0, opcode[0] & 0b01, 8);  break;
        }
        break;
    case 0xD0:
    case 0xD1:
        switch (opcode[1] >> 3 & 7) {
        case 0: Decode(format, 1, "ROL", 0, opcode[0] & 0b01, 0);  break;
        case 1: Decode(format, 1, "ROR", 0, opcode[0] & 0b01, 0);  break;
        case 2: Decode(format, 1, "RCL", 0, opcode[0] & 0b01, 0);  break;
        case 3: Decode(format, 1, "RCR", 0, opcode[0] & 0b01, 0);  break;
        }
        format.operand[1].type = Format::Operand::IMM;
        format.operand[1].displacement = 1;
        break;
    case 0xD2:
    case 0xD3:
        switch (opcode[1] >> 3 & 7) {
        case 0: Decode(format, 1, "ROL", 0, opcode[0] & 0b01, 0);  break;
        case 1: Decode(format, 1, "ROR", 0, opcode[0] & 0b01, 0);  break;
        case 2: Decode(format, 1, "RCL", 0, opcode[0] & 0b01, 0);  break;
        case 3: Decode(format, 1, "RCR", 0, opcode[0] & 0b01, 0);  break;
        }
        format.operand[1].type = Format::Operand::REG;
        format.operand[1].base = REG(ECX);
        break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        switch (x86.opcode[1] >> 3 & 7) {
        case 0:
            if (sizeof(DEST) == 1)  DEST = __builtin_rotateleft8(DEST, SRC);
            if (sizeof(DEST) == 2)  DEST = __builtin_rotateleft16(DEST, SRC);
            if (sizeof(DEST) == 4)  DEST = __builtin_rotateleft32(DEST, SRC);
            break;
        case 1:
            if (sizeof(DEST) == 1)  DEST = __builtin_rotateright8(DEST, SRC);
            if (sizeof(DEST) == 2)  DEST = __builtin_rotateright16(DEST, SRC);
            if (sizeof(DEST) == 4)  DEST = __builtin_rotateright32(DEST, SRC);
            break;
        case 2: // TODO
            if (sizeof(DEST) == 1)  DEST = __builtin_rotateleft8(DEST, SRC);
            if (sizeof(DEST) == 2)  DEST = __builtin_rotateleft16(DEST, SRC);
            if (sizeof(DEST) == 4)  DEST = __builtin_rotateleft32(DEST, SRC);
            break;
        case 3: // TODO
            if (sizeof(DEST) == 1)  DEST = __builtin_rotateright8(DEST, SRC);
            if (sizeof(DEST) == 2)  DEST = __builtin_rotateright16(DEST, SRC);
            if (sizeof(DEST) == 4)  DEST = __builtin_rotateright32(DEST, SRC);
            break;
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
