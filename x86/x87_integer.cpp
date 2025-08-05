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
void x87_instruction::FIADD(Format& format)
{
    Decode(format, "FIADD", 2);
    switch (opcode[0]) {
    case 0xDA:  format.width = 32;  break;
    case 0xDE:  format.width = 16;  break;
    }

    BEGIN_OPERATION() {
        auto& DEST = ST(0);
        switch (sizeof(SRC)) {
        case 2: DEST = DEST + (int16_t&)SRC;    break;
        case 4: DEST = DEST + (int32_t&)SRC;    break;
        default:                                return;
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FICOM(Format& format)
{
    Decode(format, "FICOM", 2);

    BEGIN_OPERATION() {
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FICOMP(Format& format)
{
    Decode(format, "FICOMP", 2);

    BEGIN_OPERATION() {
        TOP = TOP + 1;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FIDIV(Format& format)
{
    Decode(format, "FIDIV", 2);
    switch (opcode[0]) {
    case 0xDA:  format.width = 32;  break;
    case 0xDE:  format.width = 16;  break;
    }

    BEGIN_OPERATION() {
        auto& DEST = ST(0);
        switch (sizeof(SRC)) {
        case 2: DEST = DEST / (int16_t&)SRC;    break;
        case 4: DEST = DEST / (int32_t&)SRC;    break;
        default:                                return;
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FIDIVR(Format& format)
{
    Decode(format, "FIDIVR", 2);
    switch (opcode[0]) {
    case 0xDA:  format.width = 32;  break;
    case 0xDE:  format.width = 16;  break;
    }

    BEGIN_OPERATION() {
        auto& DEST = ST(0);
        switch (sizeof(SRC)) {
        case 2: DEST = (int16_t&)SRC / DEST;    break;
        case 4: DEST = (int32_t&)SRC / DEST;    break;
        default:                                return;
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FILD(Format& format)
{
    Decode(format, "FILD", 2);
    switch (opcode[0]) {
    case 0xDB:  format.width = 32;  break;
    case 0xDF:
        switch (opcode[1] & 0b00111000) {
        case 0b00000000:    format.width = 16;  break;
        case 0b00101000:    format.width = 64;  break;
        }
        break;
    }

    BEGIN_OPERATION() {
        auto& DEST = ST(0);
        TOP = TOP - 1;
        switch (sizeof(SRC)) {
        case 2: DEST = (int16_t&)SRC;   break;
        case 4: DEST = (int32_t&)SRC;   break;
        case 8: DEST = (int64_t&)SRC;   break;
        default:                        return;
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FIMUL(Format& format)
{
    Decode(format, "FIMUL", 2);
    switch (opcode[0]) {
    case 0xDA:  format.width = 32;  break;
    case 0xDE:  format.width = 16;  break;
    }

    BEGIN_OPERATION() {
        auto& DEST = ST(0);
        switch (sizeof(SRC)) {
        case 2: DEST = DEST * (int16_t&)SRC;    break;
        case 4: DEST = DEST * (int32_t&)SRC;    break;
        default:                                return;
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FIST(Format& format)
{
    Decode(format, "FIST", 2);
    switch (opcode[0]) {
    case 0xDB:  format.width = 32;  break;
    case 0xDF:  format.width = 16;  break;
    }

    BEGIN_OPERATION() {
        DEST = std::remove_reference_t<decltype(DEST)>((int64_t)SRC);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FISTP(Format& format)
{
    Decode(format, "FISTP", 2);
    switch (opcode[0]) {
    case 0xDB:  format.width = 32;  break;
    case 0xDF:
        switch (opcode[1] & 0b00111000) {
        case 0b00011000:    format.width = 16;  break;
        case 0b00111000:    format.width = 64;  break;
        }
        break;
    }

    BEGIN_OPERATION() {
        DEST = std::remove_reference_t<decltype(DEST)>((int64_t)SRC);
        TOP = TOP + 1;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FISUB(Format& format)
{
    Decode(format, "FISUB", 2);
    switch (opcode[0]) {
    case 0xDA:  format.width = 32;  break;
    case 0xDE:  format.width = 16;  break;
    }

    BEGIN_OPERATION() {
        auto& DEST = ST(0);
        switch (sizeof(SRC)) {
        case 2: DEST = DEST - (int16_t&)SRC;    break;
        case 4: DEST = DEST - (int32_t&)SRC;    break;
        default:                                return;
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x87_instruction::FISUBR(Format& format)
{
    Decode(format, "FISUBR", 2);
    switch (opcode[0]) {
    case 0xDA:  format.width = 32;  break;
    case 0xDE:  format.width = 16;  break;
    }

    BEGIN_OPERATION() {
        auto& DEST = ST(0);
        switch (sizeof(SRC)) {
        case 2: DEST = (int16_t&)SRC - DEST;    break;
        case 4: DEST = (int32_t&)SRC - DEST;    break;
        default:                                return;
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
