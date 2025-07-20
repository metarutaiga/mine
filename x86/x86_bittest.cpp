#include "x86_instruction.h"
#include "x86_instruction.inl"
#include "x86_register.h"
#include "x86_register.inl"

//------------------------------------------------------------------------------
x86_format::Format x86_instruction::BT()
{
    Format format;
    switch (opcode[1]) {
    case 0xA3:  Decode(format, 2, "BT", 0, 1, 0);  break;
    case 0xBA:  Decode(format, 2, "BT", 0, 1, -1); break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        CF = (DEST & (1 << SRC)) ? 1 : 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::BTC()
{
    Format format;
    switch (opcode[1]) {
    case 0xBA:  Decode(format, 2, "BTC", 0, 1, -1);    break;
    case 0xBB:  Decode(format, 2, "BTC", 0, 1, 0);     break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        CF = (DEST & (1 << SRC)) ? 1 : 0;
        DEST = (DEST ^ (1 << SRC));
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::BTR()
{
    Format format;
    switch (opcode[1]) {
    case 0xB3:  Decode(format, 2, "BTR", 0, 1, 0);     break;
    case 0xBA:  Decode(format, 2, "BTR", 0, 1, -1);    break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        CF = (DEST & (1 << SRC)) ? 1 : 0;
        DEST = (DEST & ~(1 << SRC));
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::BTS()
{
    Format format;
    switch (opcode[1]) {
    case 0xAB:  Decode(format, 2, "BTS", 0, 1, 0);     break;
    case 0xBA:  Decode(format, 2, "BTS", 0, 1, -1);    break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        CF = (DEST & (1 << SRC)) ? 1 : 0;
        DEST = (DEST | (1 << SRC));
    } END_OPERATION;
}
//------------------------------------------------------------------------------
