#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"

//------------------------------------------------------------------------------
void x86_instruction::SHL(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0xC0:
    case 0xC1:  Decode(format, opcode, "SHL", 1, 8, opcode[0] & 0b01);  break;
    case 0xD0:
    case 0xD1:
        Decode(format, opcode, "SHL", 1, 0, opcode[0] & 0b01);
        format.operand[1].type = Format::Operand::IMM;
        format.operand[1].displacement = 1;
        break;
    case 0xD2:
    case 0xD3:
        Decode(format, opcode, "SHL", 1, 0, opcode[0] & 0b01);
        format.operand[1].flags = Format::Operand::BIT8;
        format.operand[1].type = Format::Operand::REG;
        format.operand[1].base = IndexREG(ECX);
        break;
    }

    BEGIN_OPERATION() {
        uint32_t SIZE = (sizeof(DEST) * 8);
        uint32_t COUNT = (SRC % 32);
        if (COUNT == 0)
            return;
        auto TEMP = DEST << COUNT;
        CF = MSB(DEST << (COUNT - 1));
        if (COUNT == 1)
            OF = MSB(TEMP) ^ CF;
        UpdateFlags<_SZ_P_>(x86, DEST, TEMP);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::SHR(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0xC0:
    case 0xC1:  Decode(format, opcode, "SHR", 1, 8, opcode[0] & 0b01);  break;
    case 0xD0:
    case 0xD1:
        Decode(format, opcode, "SHR", 1, 0, opcode[0] & 0b01);
        format.operand[1].type = Format::Operand::IMM;
        format.operand[1].displacement = 1;
        break;
    case 0xD2:
    case 0xD3:
        Decode(format, opcode, "SHR", 1, 0, opcode[0] & 0b01);
        format.operand[1].flags = Format::Operand::BIT8;
        format.operand[1].type = Format::Operand::REG;
        format.operand[1].base = IndexREG(ECX);
        break;
    }

    BEGIN_OPERATION() {
        uint32_t SIZE = (sizeof(DEST) * 8);
        uint32_t COUNT = (SRC % 32);
        if (COUNT == 0)
            return;
        auto TEMP = DEST >> COUNT;
        CF = LSB(DEST >> (COUNT - 1));
        if (COUNT == 1)
            OF = MSB(DEST);
        UpdateFlags<_SZ_P_>(x86, DEST, TEMP);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::SAR(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0xC0:
    case 0xC1:  Decode(format, opcode, "SAR", 1, 8, opcode[0] & 0b01);  break;
    case 0xD0:
    case 0xD1:
        Decode(format, opcode, "SAR", 1, 0, opcode[0] & 0b01);
        format.operand[1].type = Format::Operand::IMM;
        format.operand[1].displacement = 1;
        break;
    case 0xD2:
    case 0xD3:
        Decode(format, opcode, "SAR", 1, 0, opcode[0] & 0b01);
        format.operand[1].flags = Format::Operand::BIT8;
        format.operand[1].type = Format::Operand::REG;
        format.operand[1].base = IndexREG(ECX);
        break;
    }

    BEGIN_OPERATION() {
        uint32_t SIZE = (sizeof(DEST) * 8);
        uint32_t COUNT = (SRC % 32);
        if (COUNT == 0)
            return;
        auto TEMP = DEST >> COUNT;
        CF = LSB(DEST >> (COUNT - 1));
        if (COUNT == 1)
            OF = 0;
        UpdateFlags<_SZ_P_>(x86, DEST, TEMP);
    } END_OPERATION_SIGNED;
}
//------------------------------------------------------------------------------
void x86_instruction::SHLD(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0xA4:  Decode(format, opcode, "SHLD", 2, 8, OPERAND_SIZE | THREE_OPERAND); break;
    case 0xA5:
        Decode(format, opcode, "SHLD", 2, 0, OPERAND_SIZE);
        format.operand_count = 3;
        format.operand[2].flags = Format::Operand::BIT8;
        format.operand[2].type = Format::Operand::REG;
        format.operand[2].base = IndexREG(ECX);
        break;
    }

    BEGIN_OPERATION() {
        uint32_t SIZE = (sizeof(DEST) * 8);
        uint32_t COUNT = (SRC2 % 32);
        if (COUNT == 0)
            return;
        auto TEMP = (DEST << COUNT) | (SRC1 >> (SIZE - COUNT));
        CF = MSB(DEST << (COUNT - 1));
        if (COUNT == 1)
            OF = MSB(TEMP) ^ CF;
        UpdateFlags<_SZ_P_>(x86, DEST, TEMP);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::SHRD(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0xAC:  Decode(format, opcode, "SHRD", 2, 8, OPERAND_SIZE | THREE_OPERAND); break;
    case 0xAD:
        Decode(format, opcode, "SHRD", 2, 0, OPERAND_SIZE);
        format.operand_count = 3;
        format.operand[2].flags = Format::Operand::BIT8;
        format.operand[2].type = Format::Operand::REG;
        format.operand[2].base = IndexREG(ECX);
        break;
    }

    BEGIN_OPERATION() {
        uint32_t SIZE = (sizeof(DEST) * 8);
        uint32_t COUNT = (SRC2 % 32);
        if (COUNT == 0)
            return;
        auto TEMP = (DEST >> COUNT) | (SRC1 << (SIZE - COUNT));
        CF = LSB(DEST >> (COUNT - 1));
        if (COUNT == 1)
            OF = MSB(DEST);
        UpdateFlags<_SZ_P_>(x86, DEST, TEMP);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
