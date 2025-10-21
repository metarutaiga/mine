#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"

//------------------------------------------------------------------------------
void x86_instruction::RCL(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0xC0:
    case 0xC1:  Decode(format, opcode, "RCL", 1, (opcode[0] & 0b01) | IMM_8BIT);    break;
    case 0xD0:
    case 0xD1:
        Decode(format, opcode, "RCL", 1, opcode[0] & 0b01);
        format.operand[1].type = Format::Operand::IMM;
        format.operand[1].displacement = 1;
        break;
    case 0xD2:
    case 0xD3:
        Decode(format, opcode, "RCL", 1, opcode[0] & 0b01);
        format.operand[1].flags = Format::Operand::BIT8;
        format.operand[1].type = Format::Operand::REG;
        format.operand[1].base = IndexREG(ECX);
        break;
    }

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
}
//------------------------------------------------------------------------------
void x86_instruction::RCR(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0xC0:
    case 0xC1:  Decode(format, opcode, "RCR", 1, (opcode[0] & 0b01) | IMM_8BIT);    break;
    case 0xD0:
    case 0xD1:
        Decode(format, opcode, "RCR", 1, opcode[0] & 0b01);
        format.operand[1].type = Format::Operand::IMM;
        format.operand[1].displacement = 1;
        break;
    case 0xD2:
    case 0xD3:
        Decode(format, opcode, "RCR", 1, opcode[0] & 0b01);
        format.operand[1].flags = Format::Operand::BIT8;
        format.operand[1].type = Format::Operand::REG;
        format.operand[1].base = IndexREG(ECX);
        break;
    }

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
}
//------------------------------------------------------------------------------
void x86_instruction::ROL(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0xC0:
    case 0xC1:  Decode(format, opcode, "ROL", 1, (opcode[0] & 0b01) | IMM_8BIT);    break;
    case 0xD0:
    case 0xD1:
        Decode(format, opcode, "ROL", 1, opcode[0] & 0b01);
        format.operand[1].type = Format::Operand::IMM;
        format.operand[1].displacement = 1;
        break;
    case 0xD2:
    case 0xD3:
        Decode(format, opcode, "ROL", 1, opcode[0] & 0b01);
        format.operand[1].flags = Format::Operand::BIT8;
        format.operand[1].type = Format::Operand::REG;
        format.operand[1].base = IndexREG(ECX);
        break;
    }

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
}
//------------------------------------------------------------------------------
void x86_instruction::ROR(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0xC0:
    case 0xC1:  Decode(format, opcode, "ROR", 1, (opcode[0] & 0b01) | IMM_8BIT);    break;
    case 0xD0:
    case 0xD1:
        Decode(format, opcode, "ROR", 1, opcode[0] & 0b01);
        format.operand[1].type = Format::Operand::IMM;
        format.operand[1].displacement = 1;
        break;
    case 0xD2:
    case 0xD3:
        Decode(format, opcode, "ROR", 1, opcode[0] & 0b01);
        format.operand[1].flags = Format::Operand::BIT8;
        format.operand[1].type = Format::Operand::REG;
        format.operand[1].base = IndexREG(ECX);
        break;
    }

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
}
//------------------------------------------------------------------------------
