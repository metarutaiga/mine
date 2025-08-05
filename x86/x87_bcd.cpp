#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"
#include "x87_register.h"
#include "x87_register.inl"
#include "x87_instruction.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x87_instruction::FBLD(Format& format)
{
    Decode(format, "FBLD", 2);

    BEGIN_OPERATION() {
        TOP = TOP - 1;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FBSTP(Format& format)
{
    Decode(format, "FBSTP", 2);

    BEGIN_OPERATION() {
        TOP = TOP + 1;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
