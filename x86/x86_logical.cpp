#include "x86_instruction.h"
#include "x86_instruction.inl"
#include "x86_register.h"
#include "x86_register.inl"

//------------------------------------------------------------------------------
x86_format::Format x86_instruction::AND()
{
    Format format;
    switch (opcode[0]) {
    case 0x20:
    case 0x21:
    case 0x22:
    case 0x23:  Decode(format, 1, "AND", opcode[0] & 0b10, opcode[0] & 0b01, 0);   break;
    case 0x24:
    case 0x25:  Decode(format, 0, "AND",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x80:
    case 0x81:  Decode(format, 1, "AND",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x83:  Decode(format, 1, "AND",             0b00, opcode[0] & 0b01, 8);   break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        UpdateFlags(x86, DEST, DEST & SRC);
        CF = 0;
        OF = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::NOT()
{
    Format format;
    switch (opcode[1]) {
    case 0xF6:
    case 0xF7:  Decode(format, 1, "NOT", 0, opcode[1] & 0b01, 0);  break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        DEST = !DEST;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::OR()
{
    Format format;
    switch (opcode[0]) {
    case 0x08:
    case 0x09:
    case 0x0A:
    case 0x0B:  Decode(format, 1, "OR", opcode[0] & 0b10, opcode[0] & 0b01, 0);    break;
    case 0x0C:
    case 0x0D:  Decode(format, 0, "OR",             0b00, opcode[0] & 0b01, -1);   break;
    case 0x80:
    case 0x81:  Decode(format, 1, "OR",             0b00, opcode[0] & 0b01, -1);   break;
    case 0x83:  Decode(format, 1, "OR",             0b00, opcode[0] & 0b01, 8);    break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        UpdateFlags(x86, DEST, DEST | SRC);
        CF = 0;
        OF = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::XOR()
{
    Format format;
    switch (opcode[0]) {
    case 0x30:
    case 0x31:
    case 0x32:
    case 0x33:  Decode(format, 1, "XOR", opcode[0] & 0b10, opcode[0] & 0b01, 0);   break;
    case 0x34:
    case 0x35:  Decode(format, 0, "XOR",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x80:
    case 0x81:  Decode(format, 1, "XOR",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x83:  Decode(format, 1, "XOR",             0b00, opcode[0] & 0b01, 8);   break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        UpdateFlags(x86, DEST, DEST ^ SRC);
        CF = 0;
        OF = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
