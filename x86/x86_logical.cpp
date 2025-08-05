#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"

//------------------------------------------------------------------------------
void x86_instruction::AND(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0x20:
    case 0x21:
    case 0x22:
    case 0x23:  Decode(format, opcode, "AND", 1, 0, opcode[0] & 0b11);  break;
    case 0x24:
    case 0x25:  Decode(format, opcode, "AND", 0, -1, opcode[0] & 0b01); break;
    case 0x80:
    case 0x81:  Decode(format, opcode, "AND", 1, -1, opcode[0] & 0b01); break;
    case 0x83:  Decode(format, opcode, "AND", 1, 8, opcode[0] & 0b01);  break;
    }

    BEGIN_OPERATION() {
        UpdateFlags<1, 1, 1, 1, 1, 1>(x86, DEST, DEST & SRC);
        CF = 0;
        OF = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::NOT(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0xF6:
    case 0xF7:  Decode(format, opcode, "NOT", 1, 0, opcode[0] & 0b01);  break;
    }
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        DEST = !DEST;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::OR(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0x08:
    case 0x09:
    case 0x0A:
    case 0x0B:  Decode(format, opcode, "OR", 1, 0, opcode[0] & 0b11);   break;
    case 0x0C:
    case 0x0D:  Decode(format, opcode, "OR", 0, -1, opcode[0] & 0b01);  break;
    case 0x80:
    case 0x81:  Decode(format, opcode, "OR", 1, -1, opcode[0] & 0b01);  break;
    case 0x83:  Decode(format, opcode, "OR", 1, 8, opcode[0] & 0b01);   break;
    }

    BEGIN_OPERATION() {
        UpdateFlags<1, 1, 1, 1, 1, 1>(x86, DEST, DEST | SRC);
        CF = 0;
        OF = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::XOR(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0x30:
    case 0x31:
    case 0x32:
    case 0x33:  Decode(format, opcode, "XOR", 1, 0, opcode[0] & 0b11);  break;
    case 0x34:
    case 0x35:  Decode(format, opcode, "XOR", 0, -1, opcode[0] & 0b01); break;
    case 0x80:
    case 0x81:  Decode(format, opcode, "XOR", 1, -1, opcode[0] & 0b01); break;
    case 0x83:  Decode(format, opcode, "XOR", 1, 8, opcode[0] & 0b01);  break;
    }

    BEGIN_OPERATION() {
        UpdateFlags<1, 1, 1, 1, 1, 1>(x86, DEST, DEST ^ SRC);
        CF = 0;
        OF = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
