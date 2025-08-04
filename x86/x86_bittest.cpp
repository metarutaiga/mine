#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"

//------------------------------------------------------------------------------
void x86_instruction::BT(Format& format)
{
    switch (opcode[1]) {
    case 0xA3:  Decode(format, "BT", 2, 0, OPERAND_SIZE);  break;
    case 0xBA:  Decode(format, "BT", 2, 8, OPERAND_SIZE);  break;
    }

    BEGIN_OPERATION() {
        CF = (DEST & (1 << SRC)) ? 1 : 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::BTC(Format& format)
{
    switch (opcode[1]) {
    case 0xBB:  Decode(format, "BTC", 2, 0, OPERAND_SIZE);  break;
    case 0xBA:  Decode(format, "BTC", 2, 8, OPERAND_SIZE);  break;
    }

    BEGIN_OPERATION() {
        CF = (DEST & (1 << SRC)) ? 1 : 0;
        DEST = (DEST ^ (1 << SRC));
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::BTR(Format& format)
{
    switch (opcode[1]) {
    case 0xB3:  Decode(format, "BTR", 2, 0, OPERAND_SIZE);  break;
    case 0xBA:  Decode(format, "BTR", 2, 8, OPERAND_SIZE);  break;
    }

    BEGIN_OPERATION() {
        CF = (DEST & (1 << SRC)) ? 1 : 0;
        DEST = (DEST & ~(1 << SRC));
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::BTS(Format& format)
{
    switch (opcode[1]) {
    case 0xAB:  Decode(format, "BTS", 2, 0, OPERAND_SIZE);  break;
    case 0xBA:  Decode(format, "BTS", 2, 8, OPERAND_SIZE);  break;
    }

    BEGIN_OPERATION() {
        CF = (DEST & (1 << SRC)) ? 1 : 0;
        DEST = (DEST | (1 << SRC));
    } END_OPERATION;
}
//------------------------------------------------------------------------------
