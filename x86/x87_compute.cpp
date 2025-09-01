#include <fenv.h>
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
void x87_instruction::F2XM1(Format& format, const uint8_t* opcode)
{
    format.instruction = "F2XM1";

    OPERATION() {
        ST(0) = pow(2.0, ST(0)) - 1.0;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FABS(Format& format, const uint8_t* opcode)
{
    format.instruction = "FABS";

    OPERATION() {
        ST(0) = fabs(ST(0));
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FCOS(Format& format, const uint8_t* opcode)
{
    format.instruction = "FCOS";

    OPERATION() {
        ST(0) = cos(ST(0));
        C1 = 0;
        C2 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FPATAN(Format& format, const uint8_t* opcode)
{
    format.instruction = "FPATAN";

    OPERATION() {
        ST(1) = atan(ST(1) / ST(0));
        C1 = 0;
        TOP = TOP + 1;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FPREM(Format& format, const uint8_t* opcode)
{
    format.instruction = "FPREM";

    OPERATION() {
        auto Q = (uint64_t)trunc(ST(0) / ST(1));
        ST(0) = ST(0) - (ST(1) * Q);
        C0 = Q & 0b100 ? 1 : 0;
        C1 = Q & 0b001 ? 1 : 0;
        C2 = 0;
        C3 = Q & 0b010 ? 1 : 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FPREM1(Format& format, const uint8_t* opcode)
{
    format.instruction = "FPREM1";

    OPERATION() {
        int origin = fegetround();
        fesetround(FE_TONEAREST);
        auto Q = (uint64_t)nearbyint(ST(0) / ST(1));
        fesetround(origin);
        ST(0) = ST(0) - (ST(1) * Q);
        C0 = Q & 0b100 ? 1 : 0;
        C1 = Q & 0b001 ? 1 : 0;
        C2 = 0;
        C3 = Q & 0b010 ? 1 : 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FPTAN(Format& format, const uint8_t* opcode)
{
    format.instruction = "FPTAN";

    OPERATION() {
        ST(0) = tan(ST(0));
        TOP = TOP - 1;
        ST(0) = 1.0;
        C1 = 0;
        C2 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FSCALE(Format& format, const uint8_t* opcode)
{
    format.instruction = "FSCALE";

    OPERATION() {
        ST(0) = ST(0) * pow(2.0, trunc(ST(1)));
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FSIN(Format& format, const uint8_t* opcode)
{
    format.instruction = "FSIN";

    OPERATION() {
        ST(0) = sin(ST(0));
        C1 = 0;
        C2 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FSINCOS(Format& format, const uint8_t* opcode)
{
    format.instruction = "FSINCOS";

    OPERATION() {
        auto TEMP = cos(ST(0));
        ST(0) = sin(ST(0));
        TOP = TOP - 1;
        ST(0) = TEMP;
        C1 = 0;
        C2 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FSQRT(Format& format, const uint8_t* opcode)
{
    format.instruction = "FSQRT";

    OPERATION() {
        ST(0) = sqrt(ST(0));
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FYL2X(Format& format, const uint8_t* opcode)
{
    format.instruction = "FYL2X";

    OPERATION() {
        ST(1) = ST(1) * log2(ST(0));
        C1 = 0;
        TOP = TOP + 1;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FYL2XP1(Format& format, const uint8_t* opcode)
{
    format.instruction = "FYL2XP1";

    OPERATION() {
        ST(1) = ST(1) * log2(ST(0) + 1.0);
        C1 = 0;
        TOP = TOP + 1;
    };
}
//------------------------------------------------------------------------------
