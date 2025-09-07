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
    Decode(format, opcode, "FIADD", 1, 0, DIRECTION | X87_REGISTER);
    switch (opcode[0]) {
    case 0xDA:  format.width = 32;  break;
    case 0xDE:  format.width = 16;  break;
    }
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = 0;

    BEGIN_OPERATION() {
        DEST = DEST + SRC;
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT_SIGNED(16, 32);
}
//------------------------------------------------------------------------------
void x87_instruction::FICOM(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FICOM", 1, 0, DIRECTION | X87_REGISTER);
    switch (opcode[0]) {
    case 0xDA:  format.width = 32;  break;
    case 0xDE:  format.width = 16;  break;
    }
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = 0;

    BEGIN_OPERATION() {
        C0 = DEST < SRC;
        C2 = 0;
        C3 = DEST == SRC;
    } END_OPERATION_RANGE_FLOAT_SIGNED(16, 32);
}
//------------------------------------------------------------------------------
void x87_instruction::FICOMP(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FICOMP", 1, 0, DIRECTION | X87_REGISTER);
    switch (opcode[0]) {
    case 0xDA:  format.width = 32;  break;
    case 0xDE:  format.width = 16;  break;
    }
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = 0;

    BEGIN_OPERATION() {
        C0 = DEST < SRC;
        C2 = 0;
        C3 = DEST == SRC;
        TOP = TOP + 1;
    } END_OPERATION_RANGE_FLOAT_SIGNED(16, 32);
}
//------------------------------------------------------------------------------
void x87_instruction::FIDIV(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FIDIV", 1, 0, DIRECTION | X87_REGISTER);
    switch (opcode[0]) {
    case 0xDA:  format.width = 32;  break;
    case 0xDE:  format.width = 16;  break;
    }
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = 0;

    BEGIN_OPERATION() {
        DEST = DEST / SRC;
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT_SIGNED(16, 32);
}
//------------------------------------------------------------------------------
void x87_instruction::FIDIVR(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FIDIVR", 1, 0, DIRECTION | X87_REGISTER);
    switch (opcode[0]) {
    case 0xDA:  format.width = 32;  break;
    case 0xDE:  format.width = 16;  break;
    }
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = 0;

    BEGIN_OPERATION() {
        DEST = SRC / DEST;
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT_SIGNED(16, 32);
}
//------------------------------------------------------------------------------
void x87_instruction::FILD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FILD", 1, 0, DIRECTION | X87_REGISTER);
    switch (opcode[0]) {
    case 0xDB:  format.width = 32;  break;
    case 0xDF:
        switch (opcode[1] & 0b00111000) {
        case 0b00000000:    format.width = 16;  break;
        case 0b00101000:    format.width = 64;  break;
        }
        break;
    }
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = -1;

    BEGIN_OPERATION() {
        TOP = TOP - 1;
        DEST = SRC;
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT_SIGNED(16, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FIMUL(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FIMUL", 1, 0, DIRECTION | X87_REGISTER);
    switch (opcode[0]) {
    case 0xDA:  format.width = 32;  break;
    case 0xDE:  format.width = 16;  break;
    }
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = 0;

    BEGIN_OPERATION() {
        DEST = DEST * SRC;
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT_SIGNED(16, 32);
}
//------------------------------------------------------------------------------
void x87_instruction::FIST(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FIST", 1, 0, X87_REGISTER);
    switch (opcode[0]) {
    case 0xDB:  format.width = 32;  break;
    case 0xDF:  format.width = 16;  break;
    }
    format.operand[1].type = Format::Operand::X87;
    format.operand[1].base = 0;

    BEGIN_OPERATION() {
        switch (RC) {
        case RoundNearest:  DEST = std::remove_reference_t<decltype(DEST)>(round(SRC));     break;
        case RoundDown:     DEST = std::remove_reference_t<decltype(DEST)>(floor(SRC));     break;
        case RoundUp:       DEST = std::remove_reference_t<decltype(DEST)>(ceil(SRC));      break;
        case RoundChop:     DEST = std::remove_reference_t<decltype(DEST)>(trunc(SRC));     break;
        }
        C1 = 0;
    } END_OPERATION_RANGE_SIGNED_FLOAT(16, 32);
}
//------------------------------------------------------------------------------
void x87_instruction::FISTP(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FISTP", 1, 0, X87_REGISTER);
    switch (opcode[0]) {
    case 0xDB:  format.width = 32;  break;
    case 0xDF:
        switch (opcode[1] & 0b00111000) {
        case 0b00011000:    format.width = 16;  break;
        case 0b00111000:    format.width = 64;  break;
        }
        break;
    }
    format.operand[1].type = Format::Operand::X87;
    format.operand[1].base = 0;

    BEGIN_OPERATION() {
        switch (RC) {
        case RoundNearest:  DEST = std::remove_reference_t<decltype(DEST)>(round(SRC));     break;
        case RoundDown:     DEST = std::remove_reference_t<decltype(DEST)>(floor(SRC));     break;
        case RoundUp:       DEST = std::remove_reference_t<decltype(DEST)>(ceil(SRC));      break;
        case RoundChop:     DEST = std::remove_reference_t<decltype(DEST)>(trunc(SRC));     break;
        }
        C1 = 0;
        TOP = TOP + 1;
    } END_OPERATION_RANGE_SIGNED_FLOAT(16, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FISUB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FISUB", 1, 0, DIRECTION | X87_REGISTER);
    switch (opcode[0]) {
    case 0xDA:  format.width = 32;  break;
    case 0xDE:  format.width = 16;  break;
    }
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = 0;

    BEGIN_OPERATION() {
        DEST = DEST - SRC;
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT_SIGNED(16, 32);
}
//------------------------------------------------------------------------------
void x87_instruction::FISUBR(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FISUBR", 1, 0, DIRECTION | X87_REGISTER);
    switch (opcode[0]) {
    case 0xDA:  format.width = 32;  break;
    case 0xDE:  format.width = 16;  break;
    }
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = 0;

    BEGIN_OPERATION() {
        DEST = SRC - DEST;
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT_SIGNED(16, 32);
}
//------------------------------------------------------------------------------
