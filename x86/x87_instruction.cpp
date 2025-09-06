#define _USE_MATH_DEFINES
#include <math.h>
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
void x87_instruction::FCLEX(Format& format, const uint8_t* opcode)
{
    format.instruction = "FCLEX";

    OPERATION() {
        FPUStatusWord = FPUStatusWord & 0b0111111100000000;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FDECSTP(Format& format, const uint8_t* opcode)
{
    format.instruction = "FDECSTP";

    OPERATION() {
        TOP = TOP - 1;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FINCSTP(Format& format, const uint8_t* opcode)
{
    format.instruction = "FINCSTP";

    OPERATION() {
        C1 = 0;
        TOP = TOP + 1;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLDZ(Format& format, const uint8_t* opcode)
{
    format.instruction = "FLDZ";

    OPERATION() {
        TOP = TOP - 1;
        ST(0) = 0.0;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLD1(Format& format, const uint8_t* opcode)
{
    format.instruction = "FLD1";

    OPERATION() {
        TOP = TOP - 1;
        ST(0) = 1.0;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLDL2E(Format& format, const uint8_t* opcode)
{
    format.instruction = "FLDL2E";

    OPERATION() {
        TOP = TOP - 1;
        ST(0) = M_LOG2E;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLDL2T(Format& format, const uint8_t* opcode)
{
    format.instruction = "FLDL2T";

    OPERATION() {
        TOP = TOP - 1;
        ST(0) = M_LN10 * M_LOG2E;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLDLG2(Format& format, const uint8_t* opcode)
{
    format.instruction = "FLDLG2";

    OPERATION() {
        TOP = TOP - 1;
        ST(0) = M_LN2 * M_LOG10E;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLDLN2(Format& format, const uint8_t* opcode)
{
    format.instruction = "FLDLN2";

    OPERATION() {
        TOP = TOP - 1;
        ST(0) = M_LN2;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLDPI(Format& format, const uint8_t* opcode)
{
    format.instruction = "FLDPI";

    OPERATION() {
        TOP = TOP - 1;
        ST(0) = M_PI;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLDCW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FLDCW", 1, 0, DIRECTION);
    format.width = 16;
    format.operand[0].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        FPUControlWord = SRC;
    } END_OPERATION_RANGE(16, 16);
}
//------------------------------------------------------------------------------
void x87_instruction::FSTCW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FSTCW", 1);
    format.width = 16;
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        DEST = FPUControlWord;
    } END_OPERATION_RANGE(16, 16);
}
//------------------------------------------------------------------------------
void x87_instruction::FSTSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FSTSW", 1);
    format.width = 16;
    format.length = (opcode[0] == 0xDF) ? 2 : format.length;
    format.operand[0].base = (opcode[0] == 0xDF) ? 0 : format.operand[0].base;
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        DEST = FPUStatusWord;
    } END_OPERATION_RANGE(16, 16);
}
//------------------------------------------------------------------------------
void x87_instruction::FTST(Format& format, const uint8_t* opcode)
{
    format.instruction = "FTST";
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = 0;

    BEGIN_OPERATION() {
        C0 = DEST < 0.0;
        C2 = 0;
        C3 = DEST == 0.0;
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FXAM(Format& format, const uint8_t* opcode)
{
    format.instruction = "FXAM";
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = 0;

    BEGIN_OPERATION() {
        C0 = 0;
        C1 = DEST < 0.0;
        C2 = 0;
        C3 = 0;
        switch (fpclassify(DEST)) {
        case FP_NAN:        C3 = 0; C2 = 0; C0 = 1; break;
        case FP_NORMAL:     C3 = 0; C2 = 1; C0 = 0; break;
        case FP_INFINITE:   C3 = 0; C2 = 1; C0 = 1; break;
        case FP_ZERO:       C3 = 1; C2 = 0; C0 = 0; break;
        case FP_SUBNORMAL:  C3 = 1; C2 = 1; C0 = 0; break;
        }
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FXCH(Format& format, const uint8_t* opcode)
{
    format.instruction = "FXCH";
    format.operand[0].type = Format::Operand::X87;
    format.operand[1].type = Format::Operand::X87;
    format.operand[0].base = 0;
    format.operand[1].base = opcode[1] & 0b111;

    BEGIN_OPERATION() {
        auto& SRC = *(std::remove_reference_t<decltype(DEST)>*)format.operand[1].memory;
        auto TEMP = DEST;
        DEST = SRC;
        SRC = TEMP;
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FXTRACT(Format& format, const uint8_t* opcode)
{
    format.instruction = "FXTRACT";

    OPERATION() {
        auto TEMP = copysign(0.0, ST(0));
        ST(0) = exp(ST(0));
        TOP = TOP - 1;
        ST(0) = TEMP;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
