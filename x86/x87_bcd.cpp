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
    Decode(format, opcode, "FBLD", 2);
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP - 1;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FBSTP(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FBSTP", 2);
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP + 1;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
