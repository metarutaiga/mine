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
    Decode(format, opcode, "FADD", 1, 0, DIRECTION | X87_REGISTER);
    switch (opcode[0]) {
    case 0xD8:  format.width = 32;  break;
    case 0xDC:  format.width = 64;  break;
    }
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = 0;
    switch (opcode[1] & 0xF8) {
    case 0xC0:
        format.width = 64;
        format.length = 2;
        format.operand[0].type = Format::Operand::X87;
        format.operand[1].type = Format::Operand::X87;
        format.operand[0].base = (opcode[0] == 0xD8) ? 0 : opcode[1] & 0b111;
        format.operand[1].base = (opcode[0] == 0xDC) ? 0 : opcode[1] & 0b111;
        break;
    }

    BEGIN_OPERATION() {
        DEST = DEST + SRC;
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT(32, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FADDP(Format& format, const uint8_t* opcode)
{
    format.instruction = "FADDP";
    format.operand[0].type = Format::Operand::X87;
    format.operand[1].type = Format::Operand::X87;
    format.operand[0].base = opcode[1] & 0b111;
    format.operand[1].base = 0;

    OPERATION() {
        auto& DEST = ST(format.operand[0].base);
        DEST = DEST + ST(0);
        C1 = 0;
        TOP = TOP + 1;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FCHS(Format& format, const uint8_t* opcode)
{
    format.instruction = "FCHS";

    OPERATION() {
        ST(0) = -(ST(0));
        C1 = 0;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FDIV(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FDIV", 1, 0, DIRECTION | X87_REGISTER);
    switch (opcode[0]) {
    case 0xD8:  format.width = 32;  break;
    case 0xDC:  format.width = 64;  break;
    }
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = 0;
    switch (opcode[1] & 0xF0) {
    case 0xF0:
        format.width = 64;
        format.length = 2;
        format.operand[0].type = Format::Operand::X87;
        format.operand[1].type = Format::Operand::X87;
        format.operand[0].base = (opcode[0] == 0xD8) ? 0 : opcode[1] & 0b111;
        format.operand[1].base = (opcode[0] == 0xDC) ? 0 : opcode[1] & 0b111;
        break;
    }

    BEGIN_OPERATION() {
        DEST = DEST / SRC;
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT(32, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FDIVP(Format& format, const uint8_t* opcode)
{
    format.instruction = "FDIVP";
    format.operand[0].type = Format::Operand::X87;
    format.operand[1].type = Format::Operand::X87;
    format.operand[0].base = opcode[1] & 0b111;
    format.operand[1].base = 0;

    OPERATION() {
        auto& DEST = ST(format.operand[0].base);
        DEST = DEST / ST(0);
        C1 = 0;
        TOP = TOP + 1;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FDIVR(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FDIVR", 1, 0, DIRECTION | X87_REGISTER);
    switch (opcode[0]) {
    case 0xD8:  format.width = 32;  break;
    case 0xDC:  format.width = 64;  break;
    }
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = 0;
    switch (opcode[1] & 0xF0) {
    case 0xF0:
        format.width = 64;
        format.length = 2;
        format.operand[0].type = Format::Operand::X87;
        format.operand[1].type = Format::Operand::X87;
        format.operand[0].base = (opcode[0] == 0xD8) ? 0 : opcode[1] & 0b111;
        format.operand[1].base = (opcode[0] == 0xDC) ? 0 : opcode[1] & 0b111;
        break;
    }

    BEGIN_OPERATION() {
        DEST = SRC / DEST;
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT(32, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FDIVRP(Format& format, const uint8_t* opcode)
{
    format.instruction = "FDIVRP";
    format.operand[0].type = Format::Operand::X87;
    format.operand[1].type = Format::Operand::X87;
    format.operand[0].base = opcode[1] & 0b111;
    format.operand[1].base = 0;

    OPERATION() {
        auto& DEST = ST(format.operand[0].base);
        DEST = ST(0) / DEST;
        C1 = 0;
        TOP = TOP + 1;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FLD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FLD", 1, 0, DIRECTION | X87_REGISTER);
    switch (opcode[0]) {
    case 0xD9:  format.width = 32;  break;
    case 0xDD:  format.width = 64;  break;
    case 0xDB:  format.width = 80;  break;
    }
    format.operand[0].type = Format::Operand::NOP;
    switch (opcode[1] & 0xF8) {
    case 0xC0:
        format.width = 64;
        format.length = 2;
        format.operand[1].type = Format::Operand::X87;
        format.operand[1].base = opcode[1] & 0b111;
        break;
    }

    BEGIN_OPERATION() {
        TOP = TOP - 1;
        ST(0) = SRC;
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT(32, 80);
}
//------------------------------------------------------------------------------
void x87_instruction::FMUL(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FMUL", 1, 0, DIRECTION | X87_REGISTER);
    switch (opcode[0]) {
    case 0xD8:  format.width = 32;  break;
    case 0xDC:  format.width = 64;  break;
    }
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = 0;
    switch (opcode[1] & 0xF8) {
    case 0xC8:
        format.width = 64;
        format.length = 2;
        format.operand[0].type = Format::Operand::X87;
        format.operand[1].type = Format::Operand::X87;
        format.operand[0].base = (opcode[0] == 0xD8) ? 0 : opcode[1] & 0b111;
        format.operand[1].base = (opcode[0] == 0xDC) ? 0 : opcode[1] & 0b111;
        break;
    }

    BEGIN_OPERATION() {
        DEST = DEST * SRC;
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT(32, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FMULP(Format& format, const uint8_t* opcode)
{
    format.instruction = "FMULP";
    format.operand[0].type = Format::Operand::X87;
    format.operand[1].type = Format::Operand::X87;
    format.operand[0].base = opcode[1] & 0b111;
    format.operand[1].base = 0;

    OPERATION() {
        auto& DEST = ST(format.operand[0].base);
        DEST = DEST * ST(0);
        C1 = 0;
        TOP = TOP + 1;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FRNDINT(Format& format, const uint8_t* opcode)
{
    format.instruction = "FRNDINT";

    OPERATION() {
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
        DEST = ST(0);
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT_STORE(32, 80);
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
        DEST = ST(0);
        C1 = 0;
        TOP = TOP + 1;
    } END_OPERATION_RANGE_FLOAT_STORE(32, 80);
}
//------------------------------------------------------------------------------
void x87_instruction::FSUB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FSUB", 1, 0, DIRECTION | X87_REGISTER);
    switch (opcode[0]) {
    case 0xD8:  format.width = 32;  break;
    case 0xDC:  format.width = 64;  break;
    }
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = 0;
    switch (opcode[1] & 0xF0) {
    case 0xE0:
        format.width = 64;
        format.length = 2;
        format.operand[0].type = Format::Operand::X87;
        format.operand[1].type = Format::Operand::X87;
        format.operand[0].base = (opcode[0] == 0xD8) ? 0 : opcode[1] & 0b111;
        format.operand[1].base = (opcode[0] == 0xDC) ? 0 : opcode[1] & 0b111;
        break;
    }

    BEGIN_OPERATION() {
        DEST = DEST - SRC;
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT(32, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FSUBP(Format& format, const uint8_t* opcode)
{
    format.instruction = "FSUBP";
    format.operand[0].type = Format::Operand::X87;
    format.operand[1].type = Format::Operand::X87;
    format.operand[0].base = opcode[1] & 0b111;
    format.operand[1].base = 0;

    OPERATION() {
        auto& DEST = ST(format.operand[0].base);
        DEST = DEST - ST(0);
        C1 = 0;
        TOP = TOP + 1;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FSUBR(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FSUBR", 1, 0, DIRECTION | X87_REGISTER);
    switch (opcode[0]) {
    case 0xD8:  format.width = 32;  break;
    case 0xDC:  format.width = 64;  break;
    }
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = 0;
    switch (opcode[1] & 0xF0) {
    case 0xE0:
        format.width = 64;
        format.length = 2;
        format.operand[0].type = Format::Operand::X87;
        format.operand[1].type = Format::Operand::X87;
        format.operand[0].base = (opcode[0] == 0xD8) ? 0 : opcode[1] & 0b111;
        format.operand[1].base = (opcode[0] == 0xDC) ? 0 : opcode[1] & 0b111;
        break;
    }

    BEGIN_OPERATION() {
        DEST = SRC - DEST;
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT(32, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FSUBRP(Format& format, const uint8_t* opcode)
{
    format.instruction = "FSUBRP";
    format.operand[0].type = Format::Operand::X87;
    format.operand[1].type = Format::Operand::X87;
    format.operand[0].base = opcode[1] & 0b111;
    format.operand[1].base = 0;

    OPERATION() {
        auto& DEST = ST(format.operand[0].base);
        DEST = ST(0) - DEST;
        C1 = 0;
        TOP = TOP + 1;
    };
}
//------------------------------------------------------------------------------
