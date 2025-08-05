#include <math.h>
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
void x87_instruction::FCLEX(Format& format)
{
    format.length = 2;
    format.instruction = "FCLEX";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FDECSTP(Format& format)
{
    format.length = 2;
    format.instruction = "FDECSTP";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP - 1;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FFREE(Format& format)
{
    format.length = 2;
    format.instruction = "FFREE";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FINCSTP(Format& format)
{
    format.length = 2;
    format.instruction = "FINCSTP";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP + 1;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLD(Format& format)
{
    Decode(format, "FLD", 2);
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
            case 4: DEST = (float&)SRC;     break;
            case 8: DEST = (double&)SRC;    break;
            default:                        return;
            }
        }
        else {
            int i = format.operand[0].base;
            DEST = ST(i + 1);
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FLDZ(Format& format)
{
    format.length = 2;
    format.instruction = "FLDZ";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP - 1;
        ST(0) = 0.0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLD1(Format& format)
{
    format.length = 2;
    format.instruction = "FLD1";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP - 1;
        ST(0) = 1.0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLDL2E(Format& format)
{
    format.length = 2;
    format.instruction = "FLDL2E";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP - 1;
        ST(0) = M_LOG2E;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLDL2T(Format& format)
{
    format.length = 2;
    format.instruction = "FLDL2T";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP - 1;
        ST(0) = M_LN10 * M_LOG2E;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLDLG2(Format& format)
{
    format.length = 2;
    format.instruction = "FLDLG2";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP - 1;
        ST(0) = M_LN2 * M_LOG10E;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLDLN2(Format& format)
{
    format.length = 2;
    format.instruction = "FLDLN2";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP - 1;
        ST(0) = M_LN2;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLDPI(Format& format)
{
    format.length = 2;
    format.instruction = "FLDPI";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP - 1;
        ST(0) = M_PI;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLDCW(Format& format)
{
    Decode(format, "FLDCW", 2);
    format.width = 16;

    BEGIN_OPERATION() {
        FPUControlWord = SRC;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FNOP(Format& format)
{
    format.length = 2;
    format.instruction = "FNOP";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FST(Format& format)
{
    Decode(format, "FST", 1);
    switch (opcode[0]) {
    case 0xD9:  format.width = 32;  break;
    case 0xDD:  format.width = 64;  break;
    case 0xDB:  format.width = 80;  break;
    }
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        switch (sizeof(DEST)) {
        case 4:     (float&)DEST = ST(0);       break;
        case 8:     (double&)DEST = ST(0);      break;
        case 10:    (long double&)DEST = ST(0); break;
        default:                                return;
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FSTP(Format& format)
{
    Decode(format, "FSTP", 1);
    switch (opcode[0]) {
    case 0xD9:  format.width = 32;  break;
    case 0xDD:  format.width = 64;  break;
    case 0xDB:  format.width = 80;  break;
    }
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        switch (sizeof(DEST)) {
        case 4:     (float&)DEST = ST(0);       break;
        case 8:     (double&)DEST = ST(0);      break;
        case 10:    (long double&)DEST = ST(0); break;
        default:                                return;
        }
        TOP = TOP + 1;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FSTCW(Format& format)
{
    Decode(format, "FSTCW", 2);
    format.width = 16;

    BEGIN_OPERATION() {
        DEST = FPUControlWord;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FSTSW(Format& format)
{
    Decode(format, "FSTSW", 2);
    format.width = 16;

    BEGIN_OPERATION() {
        DEST = FPUStatusWord;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FTST(Format& format)
{
    format.length = 2;
    format.instruction = "FTST";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FUCOM(Format& format)
{
    format.length = 2;
    format.instruction = "FUCOM";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FUCOMP(Format& format)
{
    format.length = 2;
    format.instruction = "FUCOMP";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP + 1;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FUCOMPP(Format& format)
{
    format.length = 2;
    format.instruction = "FUCOMPP";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP + 2;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FXAM(Format& format)
{
    format.length = 2;
    format.instruction = "FXAM";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FXCH(Format& format)
{
    format.length = 2;
    format.instruction = "FXCH";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format& format, void*, const void*, const void*) {
        int i = format.operand[0].base;
        auto TEMP = ST(0);
        ST(0) = ST(i);
        ST(1) = TEMP;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FXTRACT(Format& format)
{
    format.length = 2;
    format.instruction = "FXTRACT";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format& format, void*, const void*, const void*) {
        auto TEMP = copysign(0.0, ST(0));
        ST(0) = exp(ST(0));
        TOP = TOP - 1;
        ST(0) = TEMP;
    };
}
//------------------------------------------------------------------------------
