#include <bit>
#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"

//------------------------------------------------------------------------------
void x86_instruction::BSF(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "BSF", 2, 0, OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        ZF = 1;
        if (SRC) {
            ZF = 0;
            DEST = std::countr_zero(SRC);
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::BSR(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "BSR", 2, 0, OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        ZF = 1;
        if (SRC) {
            ZF = 0;
            DEST = std::countl_zero(SRC);
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::BT(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0xA3:  Decode(format, opcode, "BT", 2, 0, OPERAND_SIZE);  break;
    case 0xBA:  Decode(format, opcode, "BT", 2, 8, OPERAND_SIZE);  break;
    }

    BEGIN_OPERATION() {
        int COUNT = (SRC % (sizeof(DEST) * 8));
        auto ONE = (DEST & 0) | 1;
        CF = (DEST & (ONE << COUNT)) ? 1 : 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::BTC(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0xBB:  Decode(format, opcode, "BTC", 2, 0, OPERAND_SIZE);  break;
    case 0xBA:  Decode(format, opcode, "BTC", 2, 8, OPERAND_SIZE);  break;
    }

    BEGIN_OPERATION() {
        int COUNT = (SRC % (sizeof(DEST) * 8));
        auto ONE = (DEST & 0) | 1;
        CF = (DEST & (ONE << COUNT)) ? 1 : 0;
        DEST = (DEST ^ (ONE << COUNT));
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::BTR(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0xB3:  Decode(format, opcode, "BTR", 2, 0, OPERAND_SIZE);  break;
    case 0xBA:  Decode(format, opcode, "BTR", 2, 8, OPERAND_SIZE);  break;
    }

    BEGIN_OPERATION() {
        int COUNT = (SRC % (sizeof(DEST) * 8));
        auto ONE = (DEST & 0) | 1;
        CF = (DEST & (ONE << COUNT)) ? 1 : 0;
        DEST = (DEST & ~(ONE << COUNT));
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::BTS(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0xAB:  Decode(format, opcode, "BTS", 2, 0, OPERAND_SIZE);  break;
    case 0xBA:  Decode(format, opcode, "BTS", 2, 8, OPERAND_SIZE);  break;
    }

    BEGIN_OPERATION() {
        int COUNT = (SRC % (sizeof(DEST) * 8));
        auto ONE = (DEST & 0) | 1;
        CF = (DEST & (ONE << COUNT)) ? 1 : 0;
        DEST = (DEST | (ONE << COUNT));
    } END_OPERATION;
}
//------------------------------------------------------------------------------
