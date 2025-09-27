#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"
#include "x87_register.h"
#include "x87_register.inl"
#include "x87_instruction.h"
#include "x87_instruction.inl"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x87_instruction::FBLD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FBLD", 1);
    format.width = 80;
    format.operand_count = 1;

    OPERATION() {
        TOP = TOP - 1;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FBSTP(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FBSTP", 1);
    format.width = 80;
    format.operand_count = 1;

    OPERATION() {
        C1 = 0;
        TOP = TOP + 1;
    };
}
//------------------------------------------------------------------------------
