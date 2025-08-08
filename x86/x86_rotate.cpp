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
        auto TEMP = DEST;
        switch (x86.opcode[1] >> 3 & 7) {
        case 0: TEMP = std::rotl(DEST, (int)SRC);   break;
        case 1: TEMP = std::rotr(DEST, (int)SRC);   break;
        case 2: TEMP = std::rotl(DEST, (int)SRC);   break;  // TODO
        case 3: TEMP = std::rotr(DEST, (int)SRC);   break;  // TODO
        }
        CF = 0; // TODO
        OF = 0; // TODO
    } END_OPERATION;
}
//------------------------------------------------------------------------------
