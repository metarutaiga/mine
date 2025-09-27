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
void x87_instruction::F2XM1(Format& format, const uint8_t* opcode)
{
    format.instruction = "F2XM1";
    format.operand_count = 1;
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = 0;

    BEGIN_OPERATION() {
        DEST = pow(2.0, DEST) - 1.0;
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FABS(Format& format, const uint8_t* opcode)
{
    format.instruction = "FABS";
    format.operand_count = 1;
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = 0;

    BEGIN_OPERATION() {
        DEST = fabs(DEST);
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FCOS(Format& format, const uint8_t* opcode)
{
    format.instruction = "FCOS";
    format.operand_count = 1;
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = 0;

    BEGIN_OPERATION() {
        DEST = cos(DEST);
        C1 = 0;
        C2 = 0;
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FPATAN(Format& format, const uint8_t* opcode)
{
    format.instruction = "FPATAN";
    format.operand[0].type = Format::Operand::X87;
    format.operand[1].type = Format::Operand::X87;
    format.operand[0].base = 1;
    format.operand[1].base = 0;

    BEGIN_OPERATION() {
        DEST = atan(DEST / SRC);
        C1 = 0;
        TOP = TOP + 1;
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FPREM(Format& format, const uint8_t* opcode)
{
    format.instruction = "FPREM";
    format.operand[0].type = Format::Operand::X87;
    format.operand[1].type = Format::Operand::X87;
    format.operand[0].base = 0;
    format.operand[1].base = 1;

    BEGIN_OPERATION() {
        auto Q = (uint64_t)trunc(DEST / SRC);
        DEST = DEST - (SRC * Q);
        C0 = Q & 0b100 ? 1 : 0;
        C1 = Q & 0b001 ? 1 : 0;
        C2 = 0;
        C3 = Q & 0b010 ? 1 : 0;
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FPREM1(Format& format, const uint8_t* opcode)
{
    format.instruction = "FPREM1";
    format.operand[0].type = Format::Operand::X87;
    format.operand[1].type = Format::Operand::X87;
    format.operand[0].base = 0;
    format.operand[1].base = 1;

    BEGIN_OPERATION() {
        auto Q = (uint64_t)round(DEST / SRC);
        DEST = DEST - (SRC * Q);
        C0 = Q & 0b100 ? 1 : 0;
        C1 = Q & 0b001 ? 1 : 0;
        C2 = 0;
        C3 = Q & 0b010 ? 1 : 0;
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FPTAN(Format& format, const uint8_t* opcode)
{
    format.instruction = "FPTAN";
    format.operand[0].type = Format::Operand::X87;
    format.operand[1].type = Format::Operand::X87;
    format.operand[0].base = -1;
    format.operand[1].base = 0;

    BEGIN_OPERATION() {
        TOP = TOP - 1;
        DEST1 = 1.0;
        DEST2 = tan(SRC);
        C1 = 0;
        C2 = 0;
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FSCALE(Format& format, const uint8_t* opcode)
{
    format.instruction = "FSCALE";
    format.operand[0].type = Format::Operand::X87;
    format.operand[1].type = Format::Operand::X87;
    format.operand[0].base = 0;
    format.operand[1].base = 1;

    BEGIN_OPERATION() {
        DEST = DEST * pow(2.0, trunc(SRC));
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FSIN(Format& format, const uint8_t* opcode)
{
    format.instruction = "FSIN";
    format.operand_count = 1;
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = 0;

    BEGIN_OPERATION() {
        DEST = sin(DEST);
        C1 = 0;
        C2 = 0;
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FSINCOS(Format& format, const uint8_t* opcode)
{
    format.instruction = "FSINCOS";
    format.operand[0].type = Format::Operand::X87;
    format.operand[1].type = Format::Operand::X87;
    format.operand[0].base = -1;
    format.operand[1].base = 0;

    BEGIN_OPERATION() {
        TOP = TOP - 1;
        DEST1 = cos(SRC);
        DEST2 = sin(SRC);
        C1 = 0;
        C2 = 0;
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FSQRT(Format& format, const uint8_t* opcode)
{
    format.instruction = "FSQRT";
    format.operand_count = 1;
    format.operand[0].type = Format::Operand::X87;
    format.operand[0].base = 0;

    BEGIN_OPERATION() {
        DEST = sqrt(DEST);
        C1 = 0;
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FYL2X(Format& format, const uint8_t* opcode)
{
    format.instruction = "FYL2X";
    format.operand[0].type = Format::Operand::X87;
    format.operand[1].type = Format::Operand::X87;
    format.operand[0].base = 1;
    format.operand[1].base = 0;

    BEGIN_OPERATION() {
        DEST = DEST * log2(SRC);
        C1 = 0;
        TOP = TOP + 1;
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FYL2XP1(Format& format, const uint8_t* opcode)
{
    format.instruction = "FYL2XP1";
    format.operand[0].type = Format::Operand::X87;
    format.operand[1].type = Format::Operand::X87;
    format.operand[0].base = 1;
    format.operand[1].base = 0;

    BEGIN_OPERATION() {
        DEST = DEST * log2(SRC + 1.0);
        C1 = 0;
        TOP = TOP + 1;
    } END_OPERATION_RANGE_FLOAT(64, 64);
}
//------------------------------------------------------------------------------
