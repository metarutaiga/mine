#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"

//------------------------------------------------------------------------------
void x86_instruction::Rxx(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0xC0:
    case 0xC1:
        switch (opcode[1] >> 3 & 7) {
        case 0: Decode(format, opcode, "ROL", 1, 8, opcode[0] & 0b01);  break;
        case 1: Decode(format, opcode, "ROR", 1, 8, opcode[0] & 0b01);  break;
        case 2: Decode(format, opcode, "RCL", 1, 8, opcode[0] & 0b01);  break;
        case 3: Decode(format, opcode, "RCR", 1, 8, opcode[0] & 0b01);  break;
        }
        break;
    case 0xD0:
    case 0xD1:
        switch (opcode[1] >> 3 & 7) {
        case 0: Decode(format, opcode, "ROL", 1, 0, opcode[0] & 0b01);  break;
        case 1: Decode(format, opcode, "ROR", 1, 0, opcode[0] & 0b01);  break;
        case 2: Decode(format, opcode, "RCL", 1, 0, opcode[0] & 0b01);  break;
        case 3: Decode(format, opcode, "RCR", 1, 0, opcode[0] & 0b01);  break;
        }
        format.operand[1].type = Format::Operand::IMM;
        format.operand[1].displacement = 1;
        break;
    case 0xD2:
    case 0xD3:
        switch (opcode[1] >> 3 & 7) {
        case 0: Decode(format, opcode, "ROL", 1, 0, opcode[0] & 0b01);  break;
        case 1: Decode(format, opcode, "ROR", 1, 0, opcode[0] & 0b01);  break;
        case 2: Decode(format, opcode, "RCL", 1, 0, opcode[0] & 0b01);  break;
        case 3: Decode(format, opcode, "RCR", 1, 0, opcode[0] & 0b01);  break;
        }
        format.operand[1].type = Format::Operand::REG;
        format.operand[1].base = REG(ECX);
        break;
    }

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
