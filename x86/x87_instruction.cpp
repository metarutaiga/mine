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
    format.length = 2;
    format.instruction = "FCLEX";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        FPUStatusWord = FPUStatusWord & 0b0111111100000000;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FDECSTP(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    format.instruction = "FDECSTP";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP - 1;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FFREE(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    format.instruction = "FFREE";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FINCSTP(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    format.instruction = "FINCSTP";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP + 1;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FLD", 2);
    switch (opcode[0]) {
    case 0xD9:  format.width = 32;  break;
    case 0xDB:  format.width = 64;  break;
    case 0xDD:  format.width = 80;  break;
    }

    BEGIN_OPERATION() {
        auto& DEST = ST(0);
        TOP = TOP - 1;
        if (format.operand[0].type == Format::Operand::ADR) {
            switch (sizeof(SRC)) {
            case sizeof(float):     DEST = (float&)SRC;     break;
            case sizeof(double):    DEST = (double&)SRC;    break;
            default:                                        return;
            }
        }
        else {
            int i = format.operand[0].base;
            DEST = ST(i + 1);
        }
        C1 = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FLDZ(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    format.instruction = "FLDZ";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP - 1;
        ST(0) = 0.0;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLD1(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    format.instruction = "FLD1";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP - 1;
        ST(0) = 1.0;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLDL2E(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    format.instruction = "FLDL2E";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP - 1;
        ST(0) = M_LOG2E;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLDL2T(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    format.instruction = "FLDL2T";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP - 1;
        ST(0) = M_LN10 * M_LOG2E;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLDLG2(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    format.instruction = "FLDLG2";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP - 1;
        ST(0) = M_LN2 * M_LOG10E;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLDLN2(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    format.instruction = "FLDLN2";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP - 1;
        ST(0) = M_LN2;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLDPI(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    format.instruction = "FLDPI";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP - 1;
        ST(0) = M_PI;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLDCW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FLDCW", 2);
    format.width = 16;

    BEGIN_OPERATION() {
        FPUControlWord = SRC;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FNOP(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    format.instruction = "FNOP";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {};
}
//------------------------------------------------------------------------------
void x87_instruction::FST(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FST", 1);
    switch (opcode[0]) {
    case 0xD9:  format.width = 32;  break;
    case 0xDD:  format.width = 64;  break;
    case 0xDB:  format.width = 80;  break;
    }
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        switch (sizeof(DEST)) {
        case sizeof(float):         (float&)DEST = ST(0);       break;
        case sizeof(double):        (double&)DEST = ST(0);      break;
//      case sizeof(long double):   (long double&)DEST = ST(0); break;
        default:                                                return;
        }
        C1 = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FSTP(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FSTP", 1);
    switch (opcode[0]) {
    case 0xD9:  format.width = 32;  break;
    case 0xDD:  format.width = 64;  break;
    case 0xDB:  format.width = 80;  break;
    }
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        switch (sizeof(DEST)) {
        case sizeof(float):         (float&)DEST = ST(0);       break;
        case sizeof(double):        (double&)DEST = ST(0);      break;
//      case sizeof(long double):   (long double&)DEST = ST(0); break;
        default:                                                return;
        }
        TOP = TOP + 1;
        C1 = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FSTCW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FSTCW", 2);
    format.width = 16;

    BEGIN_OPERATION() {
        DEST = FPUControlWord;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FSTSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FSTSW", 2);
    format.width = 16;

    BEGIN_OPERATION() {
        DEST = FPUStatusWord;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FTST(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    format.instruction = "FTST";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        C0 = ST(0) < 0.0;
        C1 = 0;
        C2 = 0;
        C3 = ST(0) == 0.0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FUCOM(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    format.instruction = "FUCOM";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format& format, void*, const void*, const void*) {
        auto SRC = ST(format.operand[0].base);
        C0 = ST(0) < SRC;
        C1 = 0;
        C2 = 0;
        C3 = ST(0) == SRC;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FUCOMP(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    format.instruction = "FUCOMP";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format& format, void*, const void*, const void*) {
        auto SRC = ST(format.operand[0].base);
        C0 = ST(0) < SRC;
        C1 = 0;
        C2 = 0;
        C3 = ST(0) == SRC;
        TOP = TOP + 1;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FUCOMPP(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    format.instruction = "FUCOMPP";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format& format, void*, const void*, const void*) {
        auto SRC = ST(format.operand[0].base);
        C0 = ST(0) < SRC;
        C1 = 0;
        C2 = 0;
        C3 = ST(0) == SRC;
        TOP = TOP + 2;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FXAM(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    format.instruction = "FXAM";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        C0 = 0;
        C1 = ST(0) < 0.0;
        C2 = 0;
        C3 = 0;
        switch (fpclassify(ST(0))) {
        case FP_NAN:        C3 = 0; C2 = 0; C0 = 1; break;
        case FP_NORMAL:     C3 = 0; C2 = 1; C0 = 0; break;
        case FP_INFINITE:   C3 = 0; C2 = 1; C0 = 1; break;
        case FP_ZERO:       C3 = 1; C2 = 0; C0 = 0; break;
        case FP_SUBNORMAL:  C3 = 1; C2 = 1; C0 = 0; break;
        }
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FXCH(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    format.instruction = "FXCH";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format& format, void*, const void*, const void*) {
        int i = format.operand[0].base;
        auto TEMP = ST(0);
        ST(0) = ST(i);
        ST(1) = TEMP;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FXTRACT(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    format.instruction = "FXTRACT";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format& format, void*, const void*, const void*) {
        auto TEMP = copysign(0.0, ST(0));
        ST(0) = exp(ST(0));
        TOP = TOP - 1;
        ST(0) = TEMP;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
