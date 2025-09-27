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
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = -1;

    BEGIN_OPERATION() {
        TOP = TOP - 1;
        DEST = 0.0;
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FLD1(Format& format, const uint8_t* opcode)
{
    format.instruction = "FLD1";
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = -1;

    BEGIN_OPERATION() {
        TOP = TOP - 1;
        DEST = 1.0;
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FLDL2E(Format& format, const uint8_t* opcode)
{
    format.instruction = "FLDL2E";
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = -1;

    BEGIN_OPERATION() {
        TOP = TOP - 1;
        DEST = M_LOG2E;
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FLDL2T(Format& format, const uint8_t* opcode)
{
    format.instruction = "FLDL2T";
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = -1;

    BEGIN_OPERATION() {
        TOP = TOP - 1;
        DEST = M_LN10 * M_LOG2E;
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FLDLG2(Format& format, const uint8_t* opcode)
{
    format.instruction = "FLDLG2";
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = -1;

    BEGIN_OPERATION() {
        TOP = TOP - 1;
        DEST = M_LN2 * M_LOG10E;
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FLDLN2(Format& format, const uint8_t* opcode)
{
    format.instruction = "FLDLN2";
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = -1;

    BEGIN_OPERATION() {
        TOP = TOP - 1;
        DEST = M_LN2;
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FLDPI(Format& format, const uint8_t* opcode)
{
    format.instruction = "FLDPI";
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = -1;

    BEGIN_OPERATION() {
        TOP = TOP - 1;
        DEST = M_PI;
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FLDCW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FLDCW", 1, 0, DIRECTION);
    format.width = 16;
    format.operand[0].type = Format::Operand::IMM;
    format.operand[0].flags = Format::Operand::HIDE;

    BEGIN_OPERATION() {
        FPUControlWord = SRC;
    } END_OPERATION_RANGE(16, 16);
}
//------------------------------------------------------------------------------
void x87_instruction::FSTCW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FSTCW", 1);
    format.width = 16;
    format.operand_count = 1;

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
    format.operand_count = 1;
    format.operand[0].base = (opcode[0] == 0xDF) ? 0 : format.operand[0].base;

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
        std::swap(DEST1, DEST2);
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FXTRACT(Format& format, const uint8_t* opcode)
{
    format.instruction = "FXTRACT";
    format.operand[0].type = Format::Operand::X87;
    format.operand[1].type = Format::Operand::X87;
    format.operand[0].base = -1;
    format.operand[1].base = 0;

    BEGIN_OPERATION() {
        TOP = TOP - 1;
        DEST1 = copysign(0.0, SRC);
        DEST2 = exp(SRC);
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
