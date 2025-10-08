#include <fenv.h>
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

    BEGIN_OPERATION() {
        DEST = DEST + SRC;
        C1 = 0;
        TOP = TOP + 1;
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FCHS(Format& format, const uint8_t* opcode)
{
    format.instruction = "FCHS";
    format.operand_count = 1;
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = 0;

    BEGIN_OPERATION() {
        DEST = -DEST;
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT(64, 64);
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

    BEGIN_OPERATION() {
        DEST = DEST / SRC;
        C1 = 0;
        TOP = TOP + 1;
    } END_OPERATION_RANGE_FLOAT(64, 64);
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

    BEGIN_OPERATION() {
        DEST = SRC / DEST;
        C1 = 0;
        TOP = TOP + 1;
    } END_OPERATION_RANGE_FLOAT(64, 64);
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
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = -1;
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
        DEST = SRC;
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

    BEGIN_OPERATION() {
        DEST = DEST * SRC;
        C1 = 0;
        TOP = TOP + 1;
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FRNDINT(Format& format, const uint8_t* opcode)
{
    format.instruction = "FRNDINT";
    format.operand_count = 1;
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = 0;

    BEGIN_OPERATION() {
        switch (RC) {
        case RoundNearest:  DEST = round(DEST); break;
        case RoundDown:     DEST = floor(DEST); break;
        case RoundUp:       DEST = ceil(DEST);  break;
        case RoundChop:     DEST = trunc(DEST); break;
        }
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FST(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FST", 1, 0, X87_REGISTER);
    switch (opcode[0]) {
    case 0xD9:  format.width = 32;  break;
    case 0xDD:  format.width = 64;  break;
    case 0xDB:  format.width = 80;  break;
    }
    format.operand[1].type = Format::Operand::X87;
    format.operand[1].base = 0;

    BEGIN_OPERATION() {
        DEST = SRC;
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT_REVERSE(32, 80);
}
//------------------------------------------------------------------------------
void x87_instruction::FSTP(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FSTP", 1, 0, X87_REGISTER);
    switch (opcode[0]) {
    case 0xD9:  format.width = 32;  break;
    case 0xDD:  format.width = 64;  break;
    case 0xDB:  format.width = 80;  break;
    }
    format.operand[1].type = Format::Operand::X87;
    format.operand[1].base = 0;

    BEGIN_OPERATION() {
        DEST = SRC;
        C1 = 0;
        TOP = TOP + 1;
    } END_OPERATION_RANGE_FLOAT_REVERSE(32, 80);
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

    BEGIN_OPERATION() {
        DEST = DEST - SRC;
        C1 = 0;
        TOP = TOP + 1;
    } END_OPERATION_RANGE_FLOAT(64, 64);
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

    BEGIN_OPERATION() {
        DEST = SRC - DEST;
        C1 = 0;
        TOP = TOP + 1;
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
