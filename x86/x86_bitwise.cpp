#include <bit>
#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"

//------------------------------------------------------------------------------
void x86_instruction::BSF(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "BSF", 2, OPERAND_SIZE | DIRECTION);

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
    Decode(format, opcode, "BSR", 2, OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        ZF = 1;
        if (SRC) {
            ZF = 0;
            DEST = sizeof(SRC) * 8 - 1 - std::countl_zero(SRC);
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::BT(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0xA3:  Decode(format, opcode, "BT", 2, OPERAND_SIZE);              break;
    case 0xBA:  Decode(format, opcode, "BT", 2, OPERAND_SIZE | IMM_8BIT);   break;
    }

    BEGIN_OPERATION() {
        auto ONE = (DEST & 0) | 1;
        CF = (DEST & (ONE << SRC)) ? 1 : 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::BTC(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0xBB:  Decode(format, opcode, "BTC", 2, OPERAND_SIZE);             break;
    case 0xBA:  Decode(format, opcode, "BTC", 2, OPERAND_SIZE | IMM_8BIT);  break;
    }

    BEGIN_OPERATION() {
        auto ONE = (DEST & 0) | 1;
        CF = (DEST & (ONE << SRC)) ? 1 : 0;
        DEST = (DEST ^ (ONE << SRC));
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::BTR(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0xB3:  Decode(format, opcode, "BTR", 2, OPERAND_SIZE);             break;
    case 0xBA:  Decode(format, opcode, "BTR", 2, OPERAND_SIZE | IMM_8BIT);  break;
    }

    BEGIN_OPERATION() {
        auto ONE = (DEST & 0) | 1;
        CF = (DEST & (ONE << SRC)) ? 1 : 0;
        DEST = (DEST & ~(ONE << SRC));
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::BTS(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0xAB:  Decode(format, opcode, "BTS", 2, OPERAND_SIZE);             break;
    case 0xBA:  Decode(format, opcode, "BTS", 2, OPERAND_SIZE | IMM_8BIT);  break;
    }

    BEGIN_OPERATION() {
        auto ONE = (DEST & 0) | 1;
        CF = (DEST & (ONE << SRC)) ? 1 : 0;
        DEST = (DEST | (ONE << SRC));
    } END_OPERATION;
}
//------------------------------------------------------------------------------
