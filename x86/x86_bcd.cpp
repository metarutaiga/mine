#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"

//------------------------------------------------------------------------------
void x86_instruction::AAA(Format& format, const uint8_t* opcode)
{
    format.instruction = "AAA";
    format.operand_count = 0;

    OPERATION() {
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
void x86_instruction::AAD(Format& format, const uint8_t* opcode)
{
    format.instruction = "AAD";
    format.operand_count = 0;

    OPERATION() {
        AL = AH * 10 + AL;
        AH = 0;
        UpdateFlags<_SZ_P_>(x86, AX, AX);
    };
}
//------------------------------------------------------------------------------
void x86_instruction::AAM(Format& format, const uint8_t* opcode)
{
    format.instruction = "AAM";
    format.operand_count = 0;

    OPERATION() {
        AH = AL / 10;
        AL = AL % 10;
        UpdateFlags<_SZ_P_>(x86, AX, AX);
    };
}
//------------------------------------------------------------------------------
void x86_instruction::AAS(Format& format, const uint8_t* opcode)
{
    format.instruction = "AAS";
    format.operand_count = 0;

    OPERATION() {
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
void x86_instruction::DAA(Format& format, const uint8_t* opcode)
{
    format.instruction = "DAA";
    format.operand_count = 0;

    OPERATION() {
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
        UpdateFlags<_SZ_P_>(x86, AL, AL);
    };
}
//------------------------------------------------------------------------------
void x86_instruction::DAS(Format& format, const uint8_t* opcode)
{
    format.instruction = "DAS";
    format.operand_count = 0;

    OPERATION() {
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
        UpdateFlags<_SZ_P_>(x86, AL, AL);
    };
}
//------------------------------------------------------------------------------
