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
void x87_instruction::FADD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FADD", 2);
    switch (opcode[0]) {
    case 0xD8:  format.width = 32;  break;
    case 0xDC:  format.width = 64;  break;
    }

    BEGIN_OPERATION() {
        if (format.operand[0].type == Format::Operand::ADR) {
            switch (sizeof(SRC)) {
            case sizeof(float):     ST(0) = ST(0) + (float&)SRC;    break;
            case sizeof(double):    ST(0) = ST(0) + (double&)SRC;   break;
            default:                                                return;
            }
        }
        else {
            int i = format.operand[0].base;
            switch (sizeof(SRC)) {
            case sizeof(float):     ST(0) = ST(0) + ST(i);  break;
            case sizeof(double):    ST(i) = ST(i) + ST(0);  break;
            default:                                        return;
            }
        }
        C1 = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FADDP(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FADDP", 2);
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format& format, void*, const void*, const void*) {
        int i = format.operand[0].base;
        ST(i) = ST(i) + ST(0);
        TOP = TOP + 1;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FCHS(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    format.instruction = "FCHS";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        ST(0) = -(ST(0));
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FCOM(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    format.instruction = "FCOM";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format& format, void*, const void*, const void*) {
        auto SRC = ST(format.operand[0].base);
        C0 = ST(0) < SRC;
        C1 = 0;
        C2 = 0;
        C3 = ST(0) == SRC;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FCOMP(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    format.instruction = "FCOMP";
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
void x87_instruction::FCOMPP(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    format.instruction = "FCOMPP";
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
void x87_instruction::FDIV(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FDIV", 2);
    switch (opcode[0]) {
    case 0xD8:  format.width = 32;  break;
    case 0xDC:  format.width = 64;  break;
    }

    BEGIN_OPERATION() {
        if (format.operand[0].type == Format::Operand::ADR) {
            switch (sizeof(SRC)) {
            case sizeof(float):     ST(0) = ST(0) / (float&)SRC;    break;
            case sizeof(double):    ST(0) = ST(0) / (double&)SRC;   break;
            default:                                                return;
            }
        }
        else {
            int i = format.operand[0].base;
            switch (sizeof(SRC)) {
            case sizeof(float):     ST(0) = ST(0) / ST(i);  break;
            case sizeof(double):    ST(i) = ST(i) / ST(0);  break;
            default:                                        return;
            }
        }
        C1 = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FDIVP(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FDIVP", 2);
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format& format, void*, const void*, const void*) {
        int i = format.operand[0].base;
        ST(i) = ST(i) / ST(0);
        TOP = TOP + 1;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FDIVR(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FDIVR", 2);
    switch (opcode[0]) {
    case 0xD8:  format.width = 32;  break;
    case 0xDC:  format.width = 64;  break;
    }

    BEGIN_OPERATION() {
        if (format.operand[0].type == Format::Operand::ADR) {
            switch (sizeof(SRC)) {
            case sizeof(float):     ST(0) = (float&)SRC / ST(0);    break;
            case sizeof(double):    ST(0) = (double&)SRC / ST(0);   break;
            default:                                                return;
            }
        }
        else {
            int i = format.operand[0].base;
            switch (sizeof(SRC)) {
            case sizeof(float):     ST(0) = ST(i) / ST(0);  break;
            case sizeof(double):    ST(i) = ST(0) / ST(i);  break;
            default:                                        return;
            }
        }
        C1 = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FDIVRP(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FDIVRP", 2);
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format& format, void*, const void*, const void*) {
        int i = format.operand[0].base;
        ST(i) = ST(0) / ST(i);
        TOP = TOP + 1;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FMUL(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FMUL", 2);
    switch (opcode[0]) {
    case 0xD8:  format.width = 32;  break;
    case 0xDC:  format.width = 64;  break;
    }

    BEGIN_OPERATION() {
        if (format.operand[0].type == Format::Operand::ADR) {
            switch (sizeof(SRC)) {
            case sizeof(float):     ST(0) = ST(0) * (float&)SRC;    break;
            case sizeof(double):    ST(0) = ST(0) * (double&)SRC;   break;
            default:                                                return;
            }
        }
        else {
            int i = format.operand[0].base;
            switch (sizeof(SRC)) {
            case sizeof(float):     ST(0) = ST(0) * ST(i);  break;
            case sizeof(double):    ST(i) = ST(i) * ST(0);  break;
            default:                                        return;
            }
        }
        C1 = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FMULP(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FMULP", 2);
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format& format, void*, const void*, const void*) {
        int i = format.operand[0].base;
        ST(i) = ST(i) * ST(0);
        TOP = TOP + 1;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FRNDINT(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    format.instruction = "FRNDINT";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        int origin = fegetround();
        int round = FE_TOWARDZERO;
        switch (RC) {
        case RoundNearest:  round = FE_TONEAREST;   break;
        case RoundDown:     round = FE_DOWNWARD;    break;
        case RoundUp:       round = FE_UPWARD;      break;
        case RoundChop:     round = FE_TOWARDZERO;  break;
        }
        fesetround(round);
        ST(0) = nearbyint(ST(0));
        fesetround(origin);
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FSUB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FSUB", 2);
    switch (opcode[0]) {
    case 0xD8:  format.width = 32;  break;
    case 0xDC:  format.width = 64;  break;
    }

    BEGIN_OPERATION() {
        if (format.operand[0].type == Format::Operand::ADR) {
            switch (sizeof(SRC)) {
            case sizeof(float):     ST(0) = ST(0) - (float&)SRC;    break;
            case sizeof(double):    ST(0) = ST(0) - (double&)SRC;   break;
            default:                                                return;
            }
        }
        else {
            int i = format.operand[0].base;
            switch (sizeof(SRC)) {
            case sizeof(float):     ST(0) = ST(0) - ST(i);  break;
            case sizeof(double):    ST(i) = ST(i) - ST(0);  break;
            default:                                        return;
            }
        }
        C1 = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FSUBP(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FSUBP", 2);
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format& format, void*, const void*, const void*) {
        int i = format.operand[0].base;
        ST(i) = ST(i) - ST(0);
        TOP = TOP + 1;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FSUBR(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FSUBR", 2);
    switch (opcode[0]) {
    case 0xD8:  format.width = 32;  break;
    case 0xDC:  format.width = 64;  break;
    }

    BEGIN_OPERATION() {
        if (format.operand[0].type == Format::Operand::ADR) {
            switch (sizeof(SRC)) {;
            case sizeof(float):     ST(0) = (float&)SRC - ST(0);    break;
            case sizeof(double):    ST(0) = (double&)SRC - ST(0);   break;
            default:                                                return;
            }
        }
        else {
            int i = format.operand[0].base;
            switch (sizeof(SRC)) {
            case sizeof(float):     ST(0) = ST(i) - ST(0);  break;
            case sizeof(double):    ST(i) = ST(0) - ST(i);  break;
            default:                                        return;
            }
        }
        C1 = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FSUBRP(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FSUBRP", 2);
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format& format, void*, const void*, const void*) {
        int i = format.operand[0].base;
        ST(i) = ST(0) - ST(i);
        TOP = TOP + 1;
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
