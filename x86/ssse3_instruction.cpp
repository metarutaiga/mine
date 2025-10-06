#include <math.h>
#include <algorithm>
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
#include "ssse3_instruction.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ssse3_instruction::PABSB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PABSB", 3, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i8[0] = abs(SRC.i8[0]);
        DEST.i8[1] = abs(SRC.i8[1]);
        DEST.i8[2] = abs(SRC.i8[2]);
        DEST.i8[3] = abs(SRC.i8[3]);
        DEST.i8[4] = abs(SRC.i8[4]);
        DEST.i8[5] = abs(SRC.i8[5]);
        DEST.i8[6] = abs(SRC.i8[6]);
        DEST.i8[7] = abs(SRC.i8[7]);
        DEST.i8[8] = abs(SRC.i8[8]);
        DEST.i8[9] = abs(SRC.i8[9]);
        DEST.i8[10] = abs(SRC.i8[10]);
        DEST.i8[11] = abs(SRC.i8[11]);
        DEST.i8[12] = abs(SRC.i8[12]);
        DEST.i8[13] = abs(SRC.i8[13]);
        DEST.i8[14] = abs(SRC.i8[14]);
        DEST.i8[15] = abs(SRC.i8[15]);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void ssse3_instruction::PABSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PABSW", 3, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = abs(SRC.i16[0]);
        DEST.i16[1] = abs(SRC.i16[1]);
        DEST.i16[2] = abs(SRC.i16[2]);
        DEST.i16[3] = abs(SRC.i16[3]);
        DEST.i16[4] = abs(SRC.i16[4]);
        DEST.i16[5] = abs(SRC.i16[5]);
        DEST.i16[6] = abs(SRC.i16[6]);
        DEST.i16[7] = abs(SRC.i16[7]);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void ssse3_instruction::PABSD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PABSD", 3, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32[0] = abs(SRC.i32[0]);
        DEST.i32[1] = abs(SRC.i32[1]);
        DEST.i32[2] = abs(SRC.i32[2]);
        DEST.i32[3] = abs(SRC.i32[3]);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void ssse3_instruction::PALIGNR(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PALIGNR", 3, 8, SSE_REGISTER | OPERAND_SIZE | DIRECTION | THREE_OPERAND);

    BEGIN_OPERATION() {
        auto COUNT = SRC2.u8[0];
        if (COUNT > 31) {
            for (int i = 0; i < 16; ++i) {
                DEST.u8[i] = 0;
            }
        }
        else {
            uint8_t TEMP[32];
            for (int i = 0; i < 16; ++i) {
                TEMP[i] = DEST.u8[i];
            }
            for (int i = 0; i < 16; ++i) {
                TEMP[16 + i] = SRC.u8[i];
            }
            for (int i = 0; i < 16; ++i) {
                DEST.u8[i] = TEMP[COUNT + i];
            }
        }
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void ssse3_instruction::PHADDW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PHADDW", 3, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = DEST.i16[1] + DEST.i16[0];
        DEST.i16[1] = DEST.i16[3] + DEST.i16[2];
        DEST.i16[2] = DEST.i16[5] + DEST.i16[4];
        DEST.i16[3] = DEST.i16[7] + DEST.i16[6];
        DEST.i16[4] = SRC.i16[1] + SRC.i16[0];
        DEST.i16[5] = SRC.i16[3] + SRC.i16[2];
        DEST.i16[6] = SRC.i16[5] + SRC.i16[4];
        DEST.i16[7] = SRC.i16[7] + SRC.i16[6];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void ssse3_instruction::PHADDD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PHADDD", 3, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32[0] = DEST.i32[1] + DEST.i32[0];
        DEST.i32[1] = DEST.i32[3] + DEST.i32[2];
        DEST.i32[2] = SRC.i32[1] + SRC.i32[0];
        DEST.i32[3] = SRC.i32[3] + SRC.i32[2];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void ssse3_instruction::PHADDSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PHADDSW", 3, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = std::clamp(DEST.i16[1] + DEST.i16[0], INT16_MIN, INT16_MAX);
        DEST.i16[1] = std::clamp(DEST.i16[3] + DEST.i16[2], INT16_MIN, INT16_MAX);
        DEST.i16[2] = std::clamp(DEST.i16[5] + DEST.i16[4], INT16_MIN, INT16_MAX);
        DEST.i16[3] = std::clamp(DEST.i16[7] + DEST.i16[6], INT16_MIN, INT16_MAX);
        DEST.i16[4] = std::clamp(SRC.i16[1] + SRC.i16[0], INT16_MIN, INT16_MAX);
        DEST.i16[5] = std::clamp(SRC.i16[3] + SRC.i16[2], INT16_MIN, INT16_MAX);
        DEST.i16[6] = std::clamp(SRC.i16[5] + SRC.i16[4], INT16_MIN, INT16_MAX);
        DEST.i16[7] = std::clamp(SRC.i16[7] + SRC.i16[6], INT16_MIN, INT16_MAX);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void ssse3_instruction::PHSUBW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PHSUBW", 3, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = DEST.i16[1] - DEST.i16[0];
        DEST.i16[1] = DEST.i16[3] - DEST.i16[2];
        DEST.i16[2] = DEST.i16[5] - DEST.i16[4];
        DEST.i16[3] = DEST.i16[7] - DEST.i16[6];
        DEST.i16[4] = SRC.i16[1] - SRC.i16[0];
        DEST.i16[5] = SRC.i16[3] - SRC.i16[2];
        DEST.i16[6] = SRC.i16[5] - SRC.i16[4];
        DEST.i16[7] = SRC.i16[7] - SRC.i16[6];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void ssse3_instruction::PHSUBD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PHSUBD", 3, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32[0] = DEST.i32[1] - DEST.i32[0];
        DEST.i32[1] = DEST.i32[3] - DEST.i32[2];
        DEST.i32[2] = SRC.i32[1] - SRC.i32[0];
        DEST.i32[3] = SRC.i32[3] - SRC.i32[2];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void ssse3_instruction::PHSUBSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PHSUBSW", 3, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = std::clamp(DEST.i16[1] - DEST.i16[0], INT16_MIN, INT16_MAX);
        DEST.i16[1] = std::clamp(DEST.i16[3] - DEST.i16[2], INT16_MIN, INT16_MAX);
        DEST.i16[2] = std::clamp(DEST.i16[5] - DEST.i16[4], INT16_MIN, INT16_MAX);
        DEST.i16[3] = std::clamp(DEST.i16[7] - DEST.i16[6], INT16_MIN, INT16_MAX);
        DEST.i16[4] = std::clamp(SRC.i16[1] - SRC.i16[0], INT16_MIN, INT16_MAX);
        DEST.i16[5] = std::clamp(SRC.i16[3] - SRC.i16[2], INT16_MIN, INT16_MAX);
        DEST.i16[6] = std::clamp(SRC.i16[5] - SRC.i16[4], INT16_MIN, INT16_MAX);
        DEST.i16[7] = std::clamp(SRC.i16[7] - SRC.i16[6], INT16_MIN, INT16_MAX);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void ssse3_instruction::PMADDUBSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMADDUBSW", 3, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = std::clamp(DEST.i8[1] * SRC.i8[1] + DEST.i8[0] * SRC.i8[0], INT16_MIN, INT16_MAX);
        DEST.i16[1] = std::clamp(DEST.i8[3] * SRC.i8[3] + DEST.i8[2] * SRC.i8[2], INT16_MIN, INT16_MAX);
        DEST.i16[2] = std::clamp(DEST.i8[5] * SRC.i8[5] + DEST.i8[4] * SRC.i8[4], INT16_MIN, INT16_MAX);
        DEST.i16[3] = std::clamp(DEST.i8[7] * SRC.i8[7] + DEST.i8[6] * SRC.i8[6], INT16_MIN, INT16_MAX);
        DEST.i16[4] = std::clamp(DEST.i8[9] * SRC.i8[9] + DEST.i8[8] * SRC.i8[8], INT16_MIN, INT16_MAX);
        DEST.i16[5] = std::clamp(DEST.i8[11] * SRC.i8[11] + DEST.i8[10] * SRC.i8[10], INT16_MIN, INT16_MAX);
        DEST.i16[6] = std::clamp(DEST.i8[13] * SRC.i8[13] + DEST.i8[12] * SRC.i8[12], INT16_MIN, INT16_MAX);
        DEST.i16[7] = std::clamp(DEST.i8[15] * SRC.i8[15] + DEST.i8[14] * SRC.i8[14], INT16_MIN, INT16_MAX);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void ssse3_instruction::PMULHRSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMULHRSW", 3, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = (DEST.i16[0] * SRC.i16[0]) >> 15;
        DEST.i16[1] = (DEST.i16[1] * SRC.i16[1]) >> 15;
        DEST.i16[2] = (DEST.i16[2] * SRC.i16[2]) >> 15;
        DEST.i16[3] = (DEST.i16[3] * SRC.i16[3]) >> 15;
        DEST.i16[4] = (DEST.i16[4] * SRC.i16[4]) >> 15;
        DEST.i16[5] = (DEST.i16[5] * SRC.i16[5]) >> 15;
        DEST.i16[6] = (DEST.i16[6] * SRC.i16[6]) >> 15;
        DEST.i16[7] = (DEST.i16[7] * SRC.i16[7]) >> 15;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void ssse3_instruction::PSHUFB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSHUFB", 3, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u8[0] = DEST.u8[SRC.u8[0] % 16];
        DEST.u8[1] = DEST.u8[SRC.u8[1] % 16];
        DEST.u8[2] = DEST.u8[SRC.u8[2] % 16];
        DEST.u8[3] = DEST.u8[SRC.u8[3] % 16];
        DEST.u8[4] = DEST.u8[SRC.u8[4] % 16];
        DEST.u8[5] = DEST.u8[SRC.u8[5] % 16];
        DEST.u8[6] = DEST.u8[SRC.u8[6] % 16];
        DEST.u8[7] = DEST.u8[SRC.u8[7] % 16];
        DEST.u8[8] = DEST.u8[SRC.u8[8] % 16];
        DEST.u8[9] = DEST.u8[SRC.u8[9] % 16];
        DEST.u8[10] = DEST.u8[SRC.u8[10] % 16];
        DEST.u8[11] = DEST.u8[SRC.u8[11] % 16];
        DEST.u8[12] = DEST.u8[SRC.u8[12] % 16];
        DEST.u8[13] = DEST.u8[SRC.u8[13] % 16];
        DEST.u8[14] = DEST.u8[SRC.u8[14] % 16];
        DEST.u8[15] = DEST.u8[SRC.u8[15] % 16];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void ssse3_instruction::PSIGNB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSIGNB", 3, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i8[0] = SRC.i8[0] <= 0 ? SRC.i8[0] < 0 ? -DEST.i8[0] : 0 : DEST.i8[0];
        DEST.i8[1] = SRC.i8[1] <= 0 ? SRC.i8[1] < 0 ? -DEST.i8[1] : 0 : DEST.i8[1];
        DEST.i8[2] = SRC.i8[2] <= 0 ? SRC.i8[2] < 0 ? -DEST.i8[2] : 0 : DEST.i8[2];
        DEST.i8[3] = SRC.i8[3] <= 0 ? SRC.i8[3] < 0 ? -DEST.i8[3] : 0 : DEST.i8[3];
        DEST.i8[4] = SRC.i8[4] <= 0 ? SRC.i8[4] < 0 ? -DEST.i8[4] : 0 : DEST.i8[4];
        DEST.i8[5] = SRC.i8[5] <= 0 ? SRC.i8[5] < 0 ? -DEST.i8[5] : 0 : DEST.i8[5];
        DEST.i8[6] = SRC.i8[6] <= 0 ? SRC.i8[6] < 0 ? -DEST.i8[6] : 0 : DEST.i8[6];
        DEST.i8[7] = SRC.i8[7] <= 0 ? SRC.i8[7] < 0 ? -DEST.i8[7] : 0 : DEST.i8[7];
        DEST.i8[8] = SRC.i8[8] <= 0 ? SRC.i8[8] < 0 ? -DEST.i8[8] : 0 : DEST.i8[8];
        DEST.i8[9] = SRC.i8[9] <= 0 ? SRC.i8[9] < 0 ? -DEST.i8[9] : 0 : DEST.i8[9];
        DEST.i8[10] = SRC.i8[10] <= 0 ? SRC.i8[10] < 0 ? -DEST.i8[10] : 0 : DEST.i8[10];
        DEST.i8[11] = SRC.i8[11] <= 0 ? SRC.i8[11] < 0 ? -DEST.i8[11] : 0 : DEST.i8[11];
        DEST.i8[12] = SRC.i8[12] <= 0 ? SRC.i8[12] < 0 ? -DEST.i8[12] : 0 : DEST.i8[12];
        DEST.i8[13] = SRC.i8[13] <= 0 ? SRC.i8[13] < 0 ? -DEST.i8[13] : 0 : DEST.i8[13];
        DEST.i8[14] = SRC.i8[14] <= 0 ? SRC.i8[14] < 0 ? -DEST.i8[14] : 0 : DEST.i8[14];
        DEST.i8[15] = SRC.i8[15] <= 0 ? SRC.i8[15] < 0 ? -DEST.i8[15] : 0 : DEST.i8[15];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void ssse3_instruction::PSIGNW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSIGNW", 3, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = SRC.i16[0] <= 0 ? SRC.i16[0] < 0 ? -DEST.i16[0] : 0 : DEST.i16[0];
        DEST.i16[1] = SRC.i16[1] <= 0 ? SRC.i16[1] < 0 ? -DEST.i16[1] : 0 : DEST.i16[1];
        DEST.i16[2] = SRC.i16[2] <= 0 ? SRC.i16[2] < 0 ? -DEST.i16[2] : 0 : DEST.i16[2];
        DEST.i16[3] = SRC.i16[3] <= 0 ? SRC.i16[3] < 0 ? -DEST.i16[3] : 0 : DEST.i16[3];
        DEST.i16[4] = SRC.i16[4] <= 0 ? SRC.i16[4] < 0 ? -DEST.i16[4] : 0 : DEST.i16[4];
        DEST.i16[5] = SRC.i16[5] <= 0 ? SRC.i16[5] < 0 ? -DEST.i16[5] : 0 : DEST.i16[5];
        DEST.i16[6] = SRC.i16[6] <= 0 ? SRC.i16[6] < 0 ? -DEST.i16[6] : 0 : DEST.i16[6];
        DEST.i16[7] = SRC.i16[7] <= 0 ? SRC.i16[7] < 0 ? -DEST.i16[7] : 0 : DEST.i16[7];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void ssse3_instruction::PSIGND(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSIGND", 3, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32[0] = SRC.i32[0] <= 0 ? SRC.i32[0] < 0 ? -DEST.i32[0] : 0 : DEST.i32[0];
        DEST.i32[1] = SRC.i32[1] <= 0 ? SRC.i32[1] < 0 ? -DEST.i32[1] : 0 : DEST.i32[1];
        DEST.i32[2] = SRC.i32[2] <= 0 ? SRC.i32[2] < 0 ? -DEST.i32[2] : 0 : DEST.i32[2];
        DEST.i32[3] = SRC.i32[3] <= 0 ? SRC.i32[3] < 0 ? -DEST.i32[3] : 0 : DEST.i32[3];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
