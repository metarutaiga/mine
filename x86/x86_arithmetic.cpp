#include "x86_instruction.h"
#include "x86_instruction.inl"
#include "x86_register.h"
#include "x86_register.inl"

//------------------------------------------------------------------------------
void x86_instruction::ADC(Format& format)
{
    switch (opcode[0]) {
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:  Decode(format, "ADC", 1,  0, opcode[0] & 0b11); break;
    case 0x14:
    case 0x15:  Decode(format, "ADC", 0, -1, opcode[0] & 0b01); break;
    case 0x80:
    case 0x81:  Decode(format, "ADC", 1, -1, opcode[0] & 0b01); break;
    case 0x83:  Decode(format, "ADC", 1,  8, opcode[0] & 0b01); break;
    }

    BEGIN_OPERATION() {
        UpdateFlags(x86, DEST, DEST + (SRC + CF));
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::ADD(Format& format)
{
    switch (opcode[0]) {
    case 0x00:
    case 0x01:
    case 0x02:
    case 0x03:  Decode(format, "ADD", 1,  0, opcode[0] & 0b11); break;
    case 0x04:
    case 0x05:  Decode(format, "ADD", 0, -1, opcode[0] & 0b01); break;
    case 0x80:
    case 0x81:  Decode(format, "ADD", 1, -1, opcode[0] & 0b01); break;
    case 0x83:  Decode(format, "ADD", 1,  8, opcode[0] & 0b01); break;
    }

    BEGIN_OPERATION() {
        UpdateFlags(x86, DEST, DEST + SRC);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::DEC(Format& format)
{
    switch (opcode[0]) {
    case 0x48:
    case 0x49:
    case 0x4A:
    case 0x4B:
    case 0x4C:
    case 0x4D:
    case 0x4E:
    case 0x4F:
        format.instruction = "DEC";
        format.operand[0].type = Format::Operand::REG;
        format.operand[0].base = opcode[0] - 0x48;
        break;
    case 0xFE:
    case 0xFF:  Decode(format, "DEC", 1, 0, opcode[0] & 0b01);  break;
    }

    BEGIN_OPERATION() {
        UpdateFlags(x86, DEST, DEST - 1);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::DIV(Format& format)
{
    switch (opcode[0]) {
    case 0xF6:
    case 0xF7:
        Decode(format, "DIV", 1, 0, opcode[0] & 0b11);
        format.operand[0].type = Format::Operand::REG;
        format.operand[0].base = REG(EAX);
        break;
    }

    BEGIN_OPERATION() {
        DEST = DEST / SRC;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::IDIV(Format& format)
{
    switch (opcode[0]) {
    case 0xF6:
    case 0xF7:
        Decode(format, "IDIV", 1, 0, opcode[0] & 0b11);
        format.operand[0].type = Format::Operand::REG;
        format.operand[0].base = REG(EAX);
        break;
    }

    BEGIN_OPERATION() {
        typename std::make_signed_t<std::remove_reference_t<decltype(DEST)>> dest = DEST;
        typename std::make_signed_t<std::remove_reference_t<decltype(SRC)>> src = SRC;
        DEST = dest / src;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::IMUL(Format& format)
{
    switch (opcode[0]) {
    case 0x0F:  Decode(format, "IMUL", 2,  0, OPERAND_SIZE | DIRECTION);                    break;
    case 0x69:  Decode(format, "IMUL", 1, -1, OPERAND_SIZE | DIRECTION | THREE_OPERAND);    break;
    case 0x6B:  Decode(format, "IMUL", 1,  8, OPERAND_SIZE | DIRECTION | THREE_OPERAND);    break;
    case 0xF6:
    case 0xF7:
        Decode(format, "IMUL", 1, 0, opcode[0] & 0b11);
        format.operand[0].type = Format::Operand::REG;
        format.operand[0].base = REG(EAX);
        break;
    }

    BEGIN_OPERATION() {
        typename std::make_signed_t<std::remove_reference_t<decltype(DEST)>> dest = DEST;
        typename std::make_signed_t<std::remove_reference_t<decltype(SRC)>> src = SRC;
        DEST = dest * src;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::INC(Format& format)
{
    switch (opcode[0]) {
    case 0x40:
    case 0x41:
    case 0x42:
    case 0x43:
    case 0x44:
    case 0x45:
    case 0x46:
    case 0x47:
        format.instruction = "INC";
        format.operand[0].type = Format::Operand::REG;
        format.operand[0].base = opcode[0] - 0x40;
        break;
    case 0xFE:
    case 0xFF:  Decode(format, "INC", 1, 0, opcode[0] & 0b01);  break;
    }

    BEGIN_OPERATION() {
        UpdateFlags(x86, DEST, DEST + 1);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::MUL(Format& format)
{
    switch (opcode[0]) {
    case 0xF6:
    case 0xF7:
        Decode(format, "MUL", 1, 0, opcode[0] & 0b11);
        format.operand[0].type = Format::Operand::REG;
        format.operand[0].base = REG(EAX);
        break;
    }

    BEGIN_OPERATION() {
        switch (sizeof(SRC)) {
        case sizeof(uint8_t):   { auto TEMP = AL * uint16_t(SRC);   AX = uint16_t(TEMP);                                break; }
        case sizeof(uint16_t):  { auto TEMP = AX * uint32_t(SRC);   AX = uint16_t(TEMP);  DX = uint16_t(TEMP >> 16);    break; }
        case sizeof(uint32_t):  { auto TEMP = EAX * uint64_t(SRC);  EAX = uint32_t(TEMP); EDX = uint32_t(TEMP >> 32);   break; }
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::NEG(Format& format)
{
    switch (opcode[0]) {
    case 0xF6:
    case 0xF7:  Decode(format, "NEG", 1, 0, opcode[0] & 0b01);  break;
    }
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        CF = (DEST == 0) ? 0 : 1;
        DEST = -DEST;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::SBB(Format& format)
{
    switch (opcode[0]) {
    case 0x18:
    case 0x19:
    case 0x1A:
    case 0x1B:  Decode(format, "SBB", 1,  0, opcode[0] & 0b11); break;
    case 0x1C:
    case 0x1D:  Decode(format, "SBB", 0, -1, opcode[0] & 0b01); break;
    case 0x80:
    case 0x81:  Decode(format, "SBB", 1, -1, opcode[0] & 0b01); break;
    case 0x83:  Decode(format, "SBB", 1,  8, opcode[0] & 0b01); break;
    }

    BEGIN_OPERATION() {
        UpdateFlags(x86, DEST, DEST - (SRC + CF));
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::SUB(Format& format)
{
    switch (opcode[0]) {
    case 0x28:
    case 0x29:
    case 0x2A:
    case 0x2B:  Decode(format, "SUB", 1,  0, opcode[0] & 0b11); break;
    case 0x2C:
    case 0x2D:  Decode(format, "SUB", 0, -1, opcode[0] & 0b01); break;
    case 0x80:
    case 0x81:  Decode(format, "SUB", 1, -1, opcode[0] & 0b01); break;
    case 0x83:  Decode(format, "SUB", 1,  8, opcode[0] & 0b01); break;
    }

    BEGIN_OPERATION() {
        UpdateFlags(x86, DEST, DEST - SRC);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
