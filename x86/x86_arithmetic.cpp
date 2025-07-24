#include "x86_instruction.h"
#include "x86_instruction.inl"
#include "x86_register.h"
#include "x86_register.inl"

//------------------------------------------------------------------------------
x86_format::Format x86_instruction::ADC()
{
    Format format;
    switch (opcode[0]) {
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:  Decode(format, 1, "ADC", opcode[0] & 0b10, opcode[0] & 0b01, 0);   break;
    case 0x14:
    case 0x15:  Decode(format, 0, "ADC",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x80:
    case 0x81:  Decode(format, 1, "ADC",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x83:  Decode(format, 1, "ADC",             0b00, opcode[0] & 0b01, 8);   break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        UpdateFlags(x86, DEST, DEST + (SRC + CF));
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::ADD()
{
    Format format;
    switch (opcode[0]) {
    case 0x00:
    case 0x01:
    case 0x02:
    case 0x03:  Decode(format, 1, "ADD", opcode[0] & 0b10, opcode[0] & 0b01, 0);   break;
    case 0x04:
    case 0x05:  Decode(format, 0, "ADD",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x80:
    case 0x81:  Decode(format, 1, "ADD",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x83:  Decode(format, 1, "ADD",             0b00, opcode[0] & 0b01, 8);   break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        UpdateFlags(x86, DEST, DEST + SRC);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::DEC()
{
    Format format;
    switch (opcode[0]) {
    case 0x48:  Decode(format, 0, "DEC", 0,             0b01, 0);  break;
    case 0xFE:
    case 0xFF:  Decode(format, 1, "DEC", 0, opcode[0] & 0b01, 0);  break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        UpdateFlags(x86, DEST, DEST - 1);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::DIV()
{
    Format format;
    switch (opcode[0]) {
    case 0xF6:
    case 0xF7:  Decode(format, 1, "DIV", 0, opcode[0] & 0b01, 0);  break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        DEST = DEST / SRC;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::IDIV()
{
    Format format;
    switch (opcode[0]) {
    case 0xF6:
    case 0xF7:  Decode(format, 1, "IDIV", 0, opcode[0] & 0b01, 0);  break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        typename std::make_signed_t<std::remove_reference_t<decltype(DEST)>> dest = DEST;
        typename std::make_signed_t<std::remove_reference_t<decltype(SRC)>> src = SRC;
        DEST = dest / src;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::IMUL()
{
    Format format;
    switch (opcode[0]) {
    case 0x0F:  Decode(format, 2, "IMUL", 0,             0b01, 0);     break;
    case 0x69:  Decode(format, 1, "IMUL", 0,             0b01, -1);    break;
    case 0x6B:  Decode(format, 1, "IMUL", 0,             0b01, 8);     break;
    case 0xF6:
    case 0xF7:  Decode(format, 1, "IMUL", 0, opcode[0] & 0b01, 0);     break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        typename std::make_signed_t<std::remove_reference_t<decltype(DEST)>> dest = DEST;
        typename std::make_signed_t<std::remove_reference_t<decltype(SRC)>> src = SRC;
        DEST = dest * src;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::INC()
{
    Format format;
    switch (opcode[0]) {
    case 0x40:  Decode(format, 0, "INC", 0,             0b01, 0);  break;
    case 0xFE:
    case 0xFF:  Decode(format, 1, "INC", 0, opcode[0] & 0b01, 0);  break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        UpdateFlags(x86, DEST, DEST + 1);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::MUL()
{
    Format format;
    switch (opcode[0]) {
    case 0xF6:
    case 0xF7:  Decode(format, 1, "MUL", 0, opcode[0] & 0b01, 0);  break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        DEST = DEST / SRC;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::NEG()
{
    Format format;
    switch (opcode[1]) {
    case 0xF6:
    case 0xF7:  Decode(format, 1, "NEG", 0, opcode[1] & 0b01, 0);  break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        CF = (DEST == 0) ? 0 : 1;
        DEST = -DEST;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::SBB()
{
    Format format;
    switch (opcode[0]) {
    case 0x18:
    case 0x19:
    case 0x1A:
    case 0x1B:  Decode(format, 1, "SBB", opcode[0] & 0b10, opcode[0] & 0b01, 0);   break;
    case 0x1C:
    case 0x1D:  Decode(format, 0, "SBB",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x80:
    case 0x81:  Decode(format, 1, "SBB",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x83:  Decode(format, 1, "SBB",             0b00, opcode[0] & 0b01, 8);   break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        UpdateFlags(x86, DEST, DEST - (SRC + CF));
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::SUB()
{
    Format format;
    switch (opcode[0]) {
    case 0x28:
    case 0x29:
    case 0x2A:
    case 0x2B:  Decode(format, 1, "SUB", opcode[0] & 0b10, opcode[0] & 0b01, 0);   break;
    case 0x2C:
    case 0x2D:  Decode(format, 0, "SUB",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x80:
    case 0x81:  Decode(format, 1, "SUB",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x83:  Decode(format, 1, "SUB",             0b00, opcode[0] & 0b01, 8);   break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        UpdateFlags(x86, DEST, DEST - SRC);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
