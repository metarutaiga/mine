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
    case 0x23:  Decode(format, opcode, "AND", 1, opcode[0] & 0b11);                 break;
    case 0x24:
    case 0x25:  Decode(format, opcode, "AND", 0, (opcode[0] & 0b01) | IMM_SIZE);    break;
    case 0x80:
    case 0x81:
    case 0x82:  Decode(format, opcode, "AND", 1, (opcode[0] & 0b01) | IMM_SIZE);    break;
    case 0x83:  Decode(format, opcode, "AND", 1, (opcode[0] & 0b01) | IMM_8BIT);    break;
    }

    BEGIN_OPERATION() {
        UpdateFlags<_SZ_P_>(x86, DEST, DEST & SRC, DEST, SRC);
        CF = 0;
        OF = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::NOT(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0xF6:
    case 0xF7:  Decode(format, opcode, "NOT", 1, opcode[0] & 0b01);  break;
    }
    format.operand_count = 1;

    BEGIN_OPERATION() {
        DEST = ~DEST;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::OR(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0x08:
    case 0x09:
    case 0x0A:
    case 0x0B:  Decode(format, opcode, "OR", 1, opcode[0] & 0b11);              break;
    case 0x0C:
    case 0x0D:  Decode(format, opcode, "OR", 0, (opcode[0] & 0b01) | IMM_SIZE); break;
    case 0x80:
    case 0x81:
    case 0x82:  Decode(format, opcode, "OR", 1, (opcode[0] & 0b01) | IMM_SIZE); break;
    case 0x83:  Decode(format, opcode, "OR", 1, (opcode[0] & 0b01) | IMM_8BIT); break;
    }

    BEGIN_OPERATION() {
        UpdateFlags<_SZ_P_>(x86, DEST, DEST | SRC);
        CF = 0;
        OF = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::TEST(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0x84:
    case 0x85:  Decode(format, opcode, "TEST", 1, opcode[0] & 0b01);                break;
    case 0xA8:
    case 0xA9:  Decode(format, opcode, "TEST", 0, (opcode[0] & 0b01) | IMM_SIZE);   break;
    case 0xF6:
    case 0xF7:  Decode(format, opcode, "TEST", 1, (opcode[0] & 0b01) | IMM_SIZE);   break;
    }

    BEGIN_OPERATION() {
        auto TEMP = DEST;
        UpdateFlags<_SZ_P_>(x86, TEMP, TEMP & SRC);
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
    case 0x33:  Decode(format, opcode, "XOR", 1, opcode[0] & 0b11);                 break;
    case 0x34:
    case 0x35:  Decode(format, opcode, "XOR", 0, (opcode[0] & 0b01) | IMM_SIZE);    break;
    case 0x80:
    case 0x81:
    case 0x82:  Decode(format, opcode, "XOR", 1, (opcode[0] & 0b01) | IMM_SIZE);    break;
    case 0x83:  Decode(format, opcode, "XOR", 1, (opcode[0] & 0b01) | IMM_8BIT);    break;
    }

    BEGIN_OPERATION() {
        UpdateFlags<_SZ_P_>(x86, DEST, DEST ^ SRC);
        CF = 0;
        OF = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
