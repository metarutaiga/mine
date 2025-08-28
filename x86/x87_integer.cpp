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
void x87_instruction::FIADD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FIADD", 2);
    switch (opcode[0]) {
    case 0xDA:  format.width = 32;  break;
    case 0xDE:  format.width = 16;  break;
    }
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        auto& DEST = ST(0);
        switch (sizeof(SRC)) {
        case sizeof(int16_t):   DEST = DEST + (int16_t)SRC; break;
        case sizeof(int32_t):   DEST = DEST + (int32_t)SRC; break;
        default:                                            return;
        }
        C1 = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FICOM(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FICOM", 2);
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        C0 = ST(0) < SRC;
        C1 = 0;
        C2 = 0;
        C3 = ST(0) == SRC;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FICOMP(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FICOMP", 2);
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        C0 = ST(0) < SRC;
        C1 = 0;
        C2 = 0;
        C3 = ST(0) == SRC;
        TOP = TOP + 1;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FIDIV(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FIDIV", 2);
    switch (opcode[0]) {
    case 0xDA:  format.width = 32;  break;
    case 0xDE:  format.width = 16;  break;
    }
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        auto& DEST = ST(0);
        switch (sizeof(SRC)) {
        case sizeof(int16_t):   DEST = DEST / (int16_t)SRC; break;
        case sizeof(int32_t):   DEST = DEST / (int32_t)SRC; break;
        default:                                            return;
        }
        C1 = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FIDIVR(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FIDIVR", 2);
    switch (opcode[0]) {
    case 0xDA:  format.width = 32;  break;
    case 0xDE:  format.width = 16;  break;
    }
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        auto& DEST = ST(0);
        switch (sizeof(SRC)) {
        case sizeof(int16_t):   DEST = (int16_t)SRC / DEST; break;
        case sizeof(int32_t):   DEST = (int32_t)SRC / DEST; break;
        default:                                            return;
        }
        C1 = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FILD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FILD", 2);
    switch (opcode[0]) {
    case 0xDB:  format.width = 32;  break;
    case 0xDF:
        switch (opcode[1] & 0b00111000) {
        case 0b00000000:    format.width = 16;  break;
        case 0b00101000:    format.width = 64;  break;
        }
        break;
    }
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        auto& DEST = ST(0);
        TOP = TOP - 1;
        switch (sizeof(SRC)) {
        case sizeof(int16_t):   DEST = (int16_t)SRC;    break;
        case sizeof(int32_t):   DEST = (int32_t)SRC;    break;
        case sizeof(int64_t):   DEST = (int64_t)SRC;    break;
        default:                                        return;
        }
        C1 = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FIMUL(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FIMUL", 2);
    switch (opcode[0]) {
    case 0xDA:  format.width = 32;  break;
    case 0xDE:  format.width = 16;  break;
    }
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        auto& DEST = ST(0);
        switch (sizeof(SRC)) {
        case sizeof(int16_t):   DEST = DEST * (int16_t)SRC; break;
        case sizeof(int32_t):   DEST = DEST * (int32_t)SRC; break;
        default:                                            return;
        }
        C1 = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FIST(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FIST", 2);
    switch (opcode[0]) {
    case 0xDB:  format.width = 32;  break;
    case 0xDF:  format.width = 16;  break;
    }
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        int origin = fegetround();
        int round = FE_TOWARDZERO;
        switch (RC) {
        case RoundNearest:  round = FE_TONEAREST;   break;
        case RoundDown:     round = FE_DOWNWARD;    break;
        case RoundUp:       round = FE_UPWARD;      break;
        case RoundChop:     round = FE_TOWARDZERO;  break;
        }
        fesetround(round);
        DEST = std::remove_reference_t<decltype(DEST)>(llrint(ST(0)));
        fesetround(origin);
        C1 = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FISTP(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FISTP", 2);
    switch (opcode[0]) {
    case 0xDB:  format.width = 32;  break;
    case 0xDF:
        switch (opcode[1] & 0b00111000) {
        case 0b00011000:    format.width = 16;  break;
        case 0b00111000:    format.width = 64;  break;
        }
        break;
    }
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        int origin = fegetround();
        int round = FE_TOWARDZERO;
        switch (RC) {
        case RoundNearest:  round = FE_TONEAREST;   break;
        case RoundDown:     round = FE_DOWNWARD;    break;
        case RoundUp:       round = FE_UPWARD;      break;
        case RoundChop:     round = FE_TOWARDZERO;  break;
        }
        fesetround(round);
        DEST = std::remove_reference_t<decltype(DEST)>(llrint(ST(0)));
        fesetround(origin);
        TOP = TOP + 1;
        C1 = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FISUB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FISUB", 2);
    switch (opcode[0]) {
    case 0xDA:  format.width = 32;  break;
    case 0xDE:  format.width = 16;  break;
    }
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        auto& DEST = ST(0);
        switch (sizeof(SRC)) {
        case sizeof(int16_t):   DEST = DEST - (int16_t)SRC; break;
        case sizeof(int32_t):   DEST = DEST - (int32_t)SRC; break;
        default:                                            return;
        }
        C1 = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FISUBR(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FISUBR", 2);
    switch (opcode[0]) {
    case 0xDA:  format.width = 32;  break;
    case 0xDE:  format.width = 16;  break;
    }
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        auto& DEST = ST(0);
        switch (sizeof(SRC)) {
        case sizeof(int16_t):   DEST = (int16_t)SRC - DEST;     break;
        case sizeof(int32_t):   DEST = (int32_t)SRC - DEST;     break;
        default:                                                return;
        }
        C1 = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
