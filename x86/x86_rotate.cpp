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
        format.operand[1].flags = Format::Operand::BIT8;
        format.operand[1].type = Format::Operand::REG;
        format.operand[1].base = IndexREG(ECX);
        break;
    }

    switch (opcode[1] >> 3 & 7) {
    case 0:
        BEGIN_OPERATION() {
            uint32_t SIZE = sizeof(DEST) * 8;
            uint32_t COUNT = (SRC % SIZE);
            if (COUNT == 0)
                return;
            DEST = std::rotl(DEST, COUNT);
            CF = LSB(DEST);
            if (SRC == 1)
                OF = MSB(DEST) ^ CF;
        } END_OPERATION;
        break;
    case 1:
        BEGIN_OPERATION() {
            uint32_t SIZE = sizeof(DEST) * 8;
            uint32_t COUNT = (SRC % SIZE);
            if (COUNT == 0)
                return;
            DEST = std::rotr(DEST, COUNT);
            CF = MSB(DEST);
            if (SRC == 1)
                OF = MSB(DEST) ^ SMSB(DEST);
        } END_OPERATION;
        break;
    case 2:
        BEGIN_OPERATION() {
            uint32_t SIZE = sizeof(DEST) * 8;
            uint32_t COUNT = (SRC % (SIZE + 1));
            if (COUNT == 0)
                return;
            while (COUNT) {
                int TEMP = MSB(DEST);
                DEST = (DEST << 1) + CF;
                CF = TEMP;
                COUNT = COUNT - 1;
            }
            if (SRC == 1)
                OF = MSB(DEST) ^ CF;
        } END_OPERATION;
        break;
    case 3:
        BEGIN_OPERATION() {
            uint32_t SIZE = sizeof(DEST) * 8;
            uint32_t COUNT = (SRC % (SIZE + 1));
            if (COUNT == 0)
                return;
            if (SRC == 1)
                OF = MSB(DEST) ^ CF;
            while (COUNT) {
                auto cf = (DEST & 0) | CF;
                int TEMP = LSB(DEST);
                DEST = (DEST >> 1) + (cf << (SIZE - 1));
                CF = TEMP;
                COUNT = COUNT - 1;
            }
        } END_OPERATION;
        break;
    }
}
//------------------------------------------------------------------------------
