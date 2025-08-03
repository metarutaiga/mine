#include "x86_instruction.h"
#include "x86_register.h"
#include "x86_register.inl"

//------------------------------------------------------------------------------
void x86_instruction::AAA(Format& format)
{
    format.instruction = "AAA";
    format.operation = [](x86_instruction& x86, const Format&, void*, const void*, const void*) {
        if ((AL & 0x0F) > 9 || AF == 1) {
            AL = AL + 6;
            AL = AL & 0x0F;
            AH = AH + 1;
            AF = 1;
            CF = 1;
        }
        else {
            CF = 0;
            AF = 0;
        }
    };
}
//------------------------------------------------------------------------------
void x86_instruction::AAD(Format& format)
{
    format.instruction = "AAD";
    format.operation = [](x86_instruction& x86, const Format&, void*, const void*, const void*) {
        AL = AH * 10 + AL;
        AH = 0;
    };
}
//------------------------------------------------------------------------------
void x86_instruction::AAM(Format& format)
{
    format.instruction = "AAM";
    format.operation = [](x86_instruction& x86, const Format&, void*, const void*, const void*) {
        AH = AL / 10;
        AL = AL % 10;
    };
}
//------------------------------------------------------------------------------
void x86_instruction::AAS(Format& format)
{
    format.instruction = "AAS";
    format.operation = [](x86_instruction& x86, const Format&, void*, const void*, const void*) {
        if ((AL & 0x0F) > 9 || AF == 1) {
            AL = AL - 6;
            AL = AL & 0x0F;
            AH = AH - 1;
            AF = 1;
            CF = 1;
        }
        else {
            CF = 0;
            AF = 0;
        }
    };
}
//------------------------------------------------------------------------------
void x86_instruction::DAA(Format& format)
{
    format.instruction = "DAA";
    format.operation = [](x86_instruction& x86, const Format&, void*, const void*, const void*) {
        if ((AL & 0x0F) > 9 || AF == 1) {
            AL = AL + 6;
            AF = 1;
        }
        else {
            AF = 0;
        }
        if (AL > 0x9F || CF == 1) {
            AL = AL + 0x60;
            CF = 1;
        }
        else {
            CF = 0;
        }
    };
}
//------------------------------------------------------------------------------
void x86_instruction::DAS(Format& format)
{
    format.instruction = "DAS";
    format.operation = [](x86_instruction& x86, const Format&, void*, const void*, const void*) {
        if ((AL & 0x0F) > 9 || AF == 1) {
            AL = AL - 6;
            AF = 1;
        }
        else {
            AF = 0;
        }
        if (AL > 0x9F || CF == 1) {
            AL = AL - 0x60;
            CF = 1;
        }
        else {
            CF = 0;
        }
    };
}
//------------------------------------------------------------------------------
