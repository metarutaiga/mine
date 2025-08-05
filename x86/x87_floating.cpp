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
void x87_instruction::FADD(Format& format)
{
    Decode(format, "FADD", 2);
    switch (opcode[0]) {
    case 0xD8:  format.width = 32;  break;
    case 0xDC:  format.width = 64;  break;
    }

    BEGIN_OPERATION() {
        if (format.operand[0].type == Format::Operand::ADR) {
            switch (sizeof(SRC)) {
            case 4: ST(0) = ST(0) + (float&)SRC;    break;
            case 8: ST(0) = ST(0) + (double&)SRC;   break;
            default:                                return;
            }
        }
        else {
            int i = format.operand[0].base;
            switch (sizeof(SRC)) {
            case 4: ST(0) = ST(0) + ST(i);  break;
            case 8: ST(i) = ST(i) + ST(0);  break;
            default:                        return;
            }
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FADDP(Format& format)
{
    Decode(format, "FADDP", 2);

    BEGIN_OPERATION() {
        int i = format.operand[0].base;
        ST(i) = ST(i) + ST(0);
        TOP = TOP + 1;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FCHS(Format& format)
{
    format.length = 2;
    format.instruction = "FCHS";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        ST(0) = -(ST(0));
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FCOM(Format& format)
{
    format.length = 2;
    format.instruction = "FCOM";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FCOMP(Format& format)
{
    format.length = 2;
    format.instruction = "FCOMP";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP + 1;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FCOMPP(Format& format)
{
    format.length = 2;
    format.instruction = "FCOMPP";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        TOP = TOP + 2;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FDIV(Format& format)
{
    Decode(format, "FDIV", 2);
    switch (opcode[0]) {
    case 0xD8:  format.width = 32;  break;
    case 0xDC:  format.width = 64;  break;
    }

    BEGIN_OPERATION() {
        if (format.operand[0].type == Format::Operand::ADR) {
            switch (sizeof(SRC)) {
            case 4: ST(0) = ST(0) / (float&)SRC;    break;
            case 8: ST(0) = ST(0) / (double&)SRC;   break;
            default:                                return;
            }
        }
        else {
            int i = format.operand[0].base;
            switch (sizeof(SRC)) {
            case 4: ST(0) = ST(0) / ST(i);  break;
            case 8: ST(i) = ST(i) / ST(0);  break;
            default:                        return;
            }
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FDIVP(Format& format)
{
    Decode(format, "FDIVP", 2);

    BEGIN_OPERATION() {
        int i = format.operand[0].base;
        ST(i) = ST(i) / ST(0);
        TOP = TOP + 1;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FDIVR(Format& format)
{
    Decode(format, "FDIVR", 2);
    switch (opcode[0]) {
    case 0xD8:  format.width = 32;  break;
    case 0xDC:  format.width = 64;  break;
    }

    BEGIN_OPERATION() {
        if (format.operand[0].type == Format::Operand::ADR) {
            switch (sizeof(SRC)) {
            case 4: ST(0) = (float&)SRC / ST(0);    break;
            case 8: ST(0) = (double&)SRC / ST(0);   break;
            default:                                return;
            }
        }
        else {
            int i = format.operand[0].base;
            switch (sizeof(SRC)) {
            case 4: ST(0) = ST(i) / ST(0);  break;
            case 8: ST(i) = ST(0) / ST(i);  break;
            default:                        return;
            }
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FDIVRP(Format& format)
{
    Decode(format, "FDIVRP", 2);

    BEGIN_OPERATION() {
        int i = format.operand[0].base;
        ST(i) = ST(0) / ST(i);
        TOP = TOP + 1;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FMUL(Format& format)
{
    Decode(format, "FMUL", 2);
    switch (opcode[0]) {
    case 0xD8:  format.width = 32;  break;
    case 0xDC:  format.width = 64;  break;
    }

    BEGIN_OPERATION() {
        if (format.operand[0].type == Format::Operand::ADR) {
            switch (sizeof(SRC)) {
            case 4: ST(0) = ST(0) * (float&)SRC;    break;
            case 8: ST(0) = ST(0) * (double&)SRC;   break;
            default:                                return;
            }
        }
        else {
            int i = format.operand[0].base;
            switch (sizeof(SRC)) {
            case 4: ST(0) = ST(0) * ST(i);  break;
            case 8: ST(i) = ST(i) * ST(0);  break;
            default:                        return;
            }
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FMULP(Format& format)
{
    Decode(format, "FMULP", 2);

    BEGIN_OPERATION() {
        int i = format.operand[0].base;
        ST(i) = ST(i) * ST(0);
        TOP = TOP + 1;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FRNDINT(Format& format)
{
    format.length = 2;
    format.instruction = "FRNDINT";
    format.operation = [](x86_instruction&, x87_instruction& x87, const Format&, void*, const void*, const void*) {
        ST(0) = rint(ST(0));
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FSUB(Format& format)
{
    Decode(format, "FSUB", 2);
    switch (opcode[0]) {
    case 0xD8:  format.width = 32;  break;
    case 0xDC:  format.width = 64;  break;
    }

    BEGIN_OPERATION() {
        if (format.operand[0].type == Format::Operand::ADR) {
            switch (sizeof(SRC)) {
            case 4: ST(0) = ST(0) - (float&)SRC;    break;
            case 8: ST(0) = ST(0) - (double&)SRC;   break;
            default:                                return;
            }
        }
        else {
            int i = format.operand[0].base;
            switch (sizeof(SRC)) {
            case 4: ST(0) = ST(0) - ST(i);  break;
            case 8: ST(i) = ST(i) - ST(0);  break;
            default:                        return;
            }
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FSUBP(Format& format)
{
    Decode(format, "FSUBP", 2);

    BEGIN_OPERATION() {
        int i = format.operand[0].base;
        ST(i) = ST(i) - ST(0);
        TOP = TOP + 1;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FSUBR(Format& format)
{
    Decode(format, "FSUBR", 2);
    switch (opcode[0]) {
    case 0xD8:  format.width = 32;  break;
    case 0xDC:  format.width = 64;  break;
    }

    BEGIN_OPERATION() {
        if (format.operand[0].type == Format::Operand::ADR) {
            switch (sizeof(SRC)) {;
            case 4: ST(0) = (float&)SRC - ST(0);    break;
            case 8: ST(0) = (double&)SRC - ST(0);   break;
            default:                                return;
            }
        }
        else {
            int i = format.operand[0].base;
            switch (sizeof(SRC)) {
            case 4: ST(0) = ST(i) - ST(0);  break;
            case 8: ST(i) = ST(0) - ST(i);  break;
            default:                        return;
            }
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FSUBRP(Format& format)
{
    Decode(format, "FSUBRP", 2);

    BEGIN_OPERATION() {
        int i = format.operand[0].base;
        ST(i) = ST(0) - ST(i);
        TOP = TOP + 1;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
