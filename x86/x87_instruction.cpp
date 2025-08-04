#include <math.h>
#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"
#include "x87_register.h"
#include "x87_register.inl"
#include "x87_instruction.h"

#define HAVE_PC 0

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x87_instruction::FLDZ(Format& format)
{
    format.length = 2;
    format.instruction = "FLDZ";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP - 1;
#if HAVE_PC
        switch (PC) {
        case 0b00:  ST(0).f = 0.0;  break;
        case 0b01:                  break;
        case 0b10:  ST(0).d = 0.0;  break;
        case 0b11:  ST(0).l = 0.0;  break;
        }
#else
        ST(0).l = 0.0;
#endif
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLD1(Format& format)
{
    format.length = 2;
    format.instruction = "FLD1";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP - 1;
#if HAVE_PC
        switch (PC) {
        case 0b00:  ST(0).f = 1.0;  break;
        case 0b01:                  break;
        case 0b10:  ST(0).d = 1.0;  break;
        case 0b11:  ST(0).l = 1.0;  break;
        }
#else
        ST(0).l = 1.0;
#endif
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLDL2E(Format& format)
{
    format.length = 2;
    format.instruction = "FLDL2E";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP - 1;
#if HAVE_PC
        switch (PC) {
        case 0b00:  ST(0).f = M_LOG2E;  break;
        case 0b01:                      break;
        case 0b10:  ST(0).d = M_LOG2E;  break;
        case 0b11:  ST(0).l = M_LOG2E;  break;
        }
#else
        ST(0).l = M_LOG2E;
#endif
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLDL2T(Format& format)
{
    format.length = 2;
    format.instruction = "FLDL2T";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP - 1;
#if HAVE_PC
        switch (PC) {
        case 0b00:  ST(0).f = M_LN10 * M_LOG2E; break;
        case 0b01:                              break;
        case 0b10:  ST(0).d = M_LN10 * M_LOG2E; break;
        case 0b11:  ST(0).l = M_LN10 * M_LOG2E; break;
        }
#else
        ST(0).l = M_LN10 * M_LOG2E;
#endif
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLDLG2(Format& format)
{
    format.length = 2;
    format.instruction = "FLDLG2";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP - 1;
#if HAVE_PC
        switch (PC) {
        case 0b00:  ST(0).f = M_LN2 * M_LOG10E; break;
        case 0b01:                              break;
        case 0b10:  ST(0).d = M_LN2 * M_LOG10E; break;
        case 0b11:  ST(0).l = M_LN2 * M_LOG10E; break;
        }
#else
        ST(0).l = M_LN2 * M_LOG10E;
#endif
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLDLN2(Format& format)
{
    format.length = 2;
    format.instruction = "FLDLN2";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP - 1;
#if HAVE_PC
        switch (PC) {
        case 0b00:  ST(0).f = M_LN2;    break;
        case 0b01:                      break;
        case 0b10:  ST(0).d = M_LN2;    break;
        case 0b11:  ST(0).l = M_LN2;    break;
        }
#else
        ST(0).l = M_LN2;
#endif
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLDPI(Format& format)
{
    format.length = 2;
    format.instruction = "FLDPI";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP - 1;
#if HAVE_PC
        switch (PC) {
        case 0b00:  ST(0).f = M_PI; break;
        case 0b01:                  break;
        case 0b10:  ST(0).d = M_PI; break;
        case 0b11:  ST(0).l = M_PI; break;
        }
#else
        ST(0).l = M_PI;
#endif
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
        long double TEMP = 0.0;
#if HAVE_PC
        switch (PC) {
        case 0b00:  TEMP = ST(0).f; break;
        case 0b01:                  break;
        case 0b10:  TEMP = ST(0).d; break;
        case 0b11:  TEMP = ST(0).l; break;
        }
#else
        TEMP = ST(0).l;
#endif
        switch (sizeof(DEST)) {
        case 4:     (float&)DEST = TEMP;        break;
        case 8:     (double&)DEST = TEMP;       break;
        case 10:    (long double&)DEST = TEMP;  break;
        default:                                break;
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
        long double TEMP = 0.0;
#if HAVE_PC
        switch (PC) {
        case 0b00:  TEMP = ST(0).f; break;
        case 0b01:                  break;
        case 0b10:  TEMP = ST(0).d; break;
        case 0b11:  TEMP = ST(0).l; break;
        }
#else
        TEMP = ST(0).l;
#endif
        switch (sizeof(DEST)) {
        case 4:     (float&)DEST = TEMP;        break;
        case 8:     (double&)DEST = TEMP;       break;
        case 10:    (long double&)DEST = TEMP;  break;
        default:                                break;
        }
        TOP = TOP + 1;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
