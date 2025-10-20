#include <math.h>
#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"
#include "x87_register.h"
#include "x87_register.inl"
#include "x87_instruction.h"
#include "x87_instruction.inl"
#include "sse_register.h"
#include "sse_register.inl"
#include "sse_instruction.h"
#include "sse_instruction.inl"
#include "sse3_instruction.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void sse3_instruction::ADDSUBPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "ADDSUBPD", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = DEST.f64[0] - SRC.f64[0];
        DEST.f64[1] = DEST.f64[1] + SRC.f64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse3_instruction::ADDSUBPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "ADDSUBPS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32[0] = DEST.f32[0] - SRC.f32[0];
        DEST.f32[1] = DEST.f32[1] + SRC.f32[1];
        DEST.f32[2] = DEST.f32[2] - SRC.f32[2];
        DEST.f32[3] = DEST.f32[3] + SRC.f32[3];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse3_instruction::FISTTP(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FISTTP", 1, X87_REGISTER);
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
        DEST = SRC;
    } END_OPERATION_RANGE_SIGNED_FLOAT(16, 64);
}
//------------------------------------------------------------------------------
void sse3_instruction::HADDPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "HADDPD", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64 = {
            DEST.f64[0] + DEST.f64[1],
            SRC.f64[0] + SRC.f64[1],
        };
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse3_instruction::HADDPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "HADDPS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32 = {
            DEST.f32[0] + DEST.f32[1],
            DEST.f32[2] + DEST.f32[3],
            SRC.f32[0] + SRC.f32[1],
            SRC.f32[2] + SRC.f32[3],
        };
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse3_instruction::HSUBPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "HSUBPD", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64 = {
            DEST.f64[0] - DEST.f64[1],
            SRC.f64[0] - SRC.f64[1],
        };
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse3_instruction::HSUBPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "HSUBPS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32 = {
            DEST.f32[0] - DEST.f32[1],
            DEST.f32[2] - DEST.f32[3],
            SRC.f32[0] - SRC.f32[1],
            SRC.f32[2] - SRC.f32[3],
        };
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse3_instruction::LDDQU(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "LDDQU", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST = SRC;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse3_instruction::MONITOR(Format& format, const uint8_t* opcode)
{
    format.instruction = "MONITOR";
    format.length = 3;
    format.operand_count = 0;

    OPERATION() {};
}
//------------------------------------------------------------------------------
void sse3_instruction::MOVDDUP(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVDDUP", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64 = {
            SRC.f64[0],
            SRC.f64[0],
        };
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse3_instruction::MOVSHDUP(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVSHDUP", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32 = {
            SRC.f32[1],
            SRC.f32[1],
            SRC.f32[3],
            SRC.f32[3],
        };
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse3_instruction::MOVSLDUP(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVSLDUP", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32 = {
            SRC.f32[0],
            SRC.f32[0],
            SRC.f32[2],
            SRC.f32[2],
        };
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse3_instruction::MWAIT(Format& format, const uint8_t* opcode)
{
    format.instruction = "MWAIT";
    format.length = 3;
    format.operand_count = 0;

    OPERATION() {};
}
//------------------------------------------------------------------------------
