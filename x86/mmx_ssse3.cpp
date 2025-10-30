#include <algorithm>
#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"
#include "mmx_register.h"
#include "mmx_register.inl"
#include "mmx_instruction.h"
#include "mmx_instruction.inl"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void mmx_instruction::PABSB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PABSB", 3, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i8[0] = abs(SRC.i8[0]);
        DEST.i8[1] = abs(SRC.i8[1]);
        DEST.i8[2] = abs(SRC.i8[2]);
        DEST.i8[3] = abs(SRC.i8[3]);
        DEST.i8[4] = abs(SRC.i8[4]);
        DEST.i8[5] = abs(SRC.i8[5]);
        DEST.i8[6] = abs(SRC.i8[6]);
        DEST.i8[7] = abs(SRC.i8[7]);
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PABSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PABSW", 3, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = abs(SRC.i16[0]);
        DEST.i16[1] = abs(SRC.i16[1]);
        DEST.i16[2] = abs(SRC.i16[2]);
        DEST.i16[3] = abs(SRC.i16[3]);
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PABSD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PABSD", 3, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32[0] = abs(SRC.i32[0]);
        DEST.i32[1] = abs(SRC.i32[1]);
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PALIGNR(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PALIGNR", 3, MMX_REGISTER | OPERAND_SIZE | DIRECTION | THREE_OPERAND | IMM_8BIT);

    BEGIN_OPERATION() {
        uint8_t COUNT = SRC2.u8[0];
        if (COUNT > 15) {
            for (int i = 0; i < 8; ++i) {
                DEST.u8[i] = 0;
            }
        }
        else {
            uint8_t TEMP[16];
            for (int i = 0; i < 8; ++i) {
                TEMP[i] = DEST.u8[i];
            }
            for (int i = 0; i < 8; ++i) {
                TEMP[16 + i] = SRC.u8[i];
            }
            for (int i = 0; i < 8; ++i) {
                DEST.u8[i] = TEMP[COUNT + i];
            }
        }
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PHADDW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PHADDW", 3, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16 = {
            int16_t(DEST.i16[1] + DEST.i16[0]),
            int16_t(DEST.i16[3] + DEST.i16[2]),
            int16_t(SRC.i16[1] + SRC.i16[0]),
            int16_t(SRC.i16[3] + SRC.i16[2]),
        };
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PHADDD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PHADDD", 3, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32 = {
            DEST.i32[1] + DEST.i32[0],
            SRC.i32[1] + SRC.i32[0],
        };
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PHADDSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PHADDSW", 3, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16 = {
            int16_t(std::clamp<int32_t>(DEST.i16[1] + DEST.i16[0], INT16_MIN, INT16_MAX)),
            int16_t(std::clamp<int32_t>(DEST.i16[3] + DEST.i16[2], INT16_MIN, INT16_MAX)),
            int16_t(std::clamp<int32_t>(SRC.i16[1] + SRC.i16[0], INT16_MIN, INT16_MAX)),
            int16_t(std::clamp<int32_t>(SRC.i16[3] + SRC.i16[2], INT16_MIN, INT16_MAX)),
        };
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PHSUBW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PHSUBW", 3, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16 = {
            int16_t(DEST.i16[1] - DEST.i16[0]),
            int16_t(DEST.i16[3] - DEST.i16[2]),
            int16_t(SRC.i16[1] - SRC.i16[0]),
            int16_t(SRC.i16[3] - SRC.i16[2]),
        };
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PHSUBD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PHSUBD", 3, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32 = {
            DEST.i32[1] - DEST.i32[0],
            SRC.i32[1] - SRC.i32[0],
        };
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PHSUBSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PHSUBSW", 3, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16 = {
            int16_t(std::clamp<int32_t>(DEST.i16[1] - DEST.i16[0], INT16_MIN, INT16_MAX)),
            int16_t(std::clamp<int32_t>(DEST.i16[3] - DEST.i16[2], INT16_MIN, INT16_MAX)),
            int16_t(std::clamp<int32_t>(SRC.i16[1] - SRC.i16[0], INT16_MIN, INT16_MAX)),
            int16_t(std::clamp<int32_t>(SRC.i16[3] - SRC.i16[2], INT16_MIN, INT16_MAX)),
        };
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PMADDUBSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMADDUBSW", 3, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16 = {
            int16_t(std::clamp<int32_t>(DEST.i8[1] * SRC.i8[1] + DEST.i8[0] * SRC.i8[0], INT16_MIN, INT16_MAX)),
            int16_t(std::clamp<int32_t>(DEST.i8[3] * SRC.i8[3] + DEST.i8[2] * SRC.i8[2], INT16_MIN, INT16_MAX)),
            int16_t(std::clamp<int32_t>(DEST.i8[5] * SRC.i8[5] + DEST.i8[4] * SRC.i8[4], INT16_MIN, INT16_MAX)),
            int16_t(std::clamp<int32_t>(DEST.i8[7] * SRC.i8[7] + DEST.i8[6] * SRC.i8[6], INT16_MIN, INT16_MAX)),
        };
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PMULHRSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMULHRSW", 3, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = (DEST.i16[0] * SRC.i16[0]) >> 15;
        DEST.i16[1] = (DEST.i16[1] * SRC.i16[1]) >> 15;
        DEST.i16[2] = (DEST.i16[2] * SRC.i16[2]) >> 15;
        DEST.i16[3] = (DEST.i16[3] * SRC.i16[3]) >> 15;
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PSHUFB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSHUFB", 3, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u8[0] = DEST.u8[SRC.u8[0] % 8];
        DEST.u8[1] = DEST.u8[SRC.u8[1] % 8];
        DEST.u8[2] = DEST.u8[SRC.u8[2] % 8];
        DEST.u8[3] = DEST.u8[SRC.u8[3] % 8];
        DEST.u8[4] = DEST.u8[SRC.u8[4] % 8];
        DEST.u8[5] = DEST.u8[SRC.u8[5] % 8];
        DEST.u8[6] = DEST.u8[SRC.u8[6] % 8];
        DEST.u8[7] = DEST.u8[SRC.u8[7] % 8];
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PSIGNB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSIGNB", 3, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i8 = CopySign(DEST.i8, SRC.i8);
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PSIGNW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSIGNW", 3, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16 = CopySign(DEST.i16, SRC.i16);
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PSIGND(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSIGND", 3, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32 = CopySign(DEST.i32, SRC.i32);
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
