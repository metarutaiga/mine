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
void x87_instruction::F2XM1(Format& format)
{
    format.length = 2;
    format.instruction = "F2XM1";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        ST(0) = pow(2.0, ST(0)) - 1;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FABS(Format& format)
{
    format.length = 2;
    format.instruction = "FABS";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        ST(0) = fabs(ST(0));
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FCOS(Format& format)
{
    format.length = 2;
    format.instruction = "FCOS";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        ST(0) = cos(ST(0));
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FPATAN(Format& format)
{
    format.length = 2;
    format.instruction = "FPATAN";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        ST(1) = atan(ST(1) / ST(0));
        TOP = TOP + 1;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FPREM(Format& format)
{
    format.length = 2;
    format.instruction = "FPREM";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        ST(0) = remainder(ST(0), ST(1));
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FPREM1(Format& format)
{
    format.length = 2;
    format.instruction = "FPREM1";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        ST(0) = remainder(ST(0), ST(1));
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FPTAN(Format& format)
{
    format.length = 2;
    format.instruction = "FPTAN";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        ST(0) = tan(ST(0));
        TOP = TOP - 1;
        ST(0) = 1.0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FSCALE(Format& format)
{
    format.length = 2;
    format.instruction = "FSCALE";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        ST(0) = ST(0) * pow(2.0, trunc(ST(1)));
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FSIN(Format& format)
{
    format.length = 2;
    format.instruction = "FSIN";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        ST(0) = sin(ST(0));
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FSINCOS(Format& format)
{
    format.length = 2;
    format.instruction = "FSINCOS";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        auto TEMP = cos(ST(0));
        ST(0) = sin(ST(0));
        TOP = TOP - 1;
        ST(0) = TEMP;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FSQRT(Format& format)
{
    format.length = 2;
    format.instruction = "FSQRT";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        ST(0) = sqrt(ST(0));
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FYL2X(Format& format)
{
    format.length = 2;
    format.instruction = "FYL2X";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        ST(1) = ST(1) * log2(ST(0));
        TOP = TOP + 1;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FYL2XP1(Format& format)
{
    format.length = 2;
    format.instruction = "FYL2XP1";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        ST(1) = ST(1) * log2(ST(0) + 1.0);
        TOP = TOP + 1;
    };
}
//------------------------------------------------------------------------------
