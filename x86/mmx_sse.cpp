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
void mmx_instruction::MASKMOVQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MASKMOVQ", 2, MMX_REGISTER | OPERAND_SIZE | DIRECTION);
    format.operand[2] = format.operand[1];
    format.operand[1] = format.operand[0];
    format.operand[0].type = Format::Operand::ADR;
    format.operand[0].base = IndexREG(EDI);

    BEGIN_OPERATION() {
        for (int i = 0; i < 8; ++i) {
            if (SRC2.i8[i] == 1) {
                DEST.i8[i] = SRC1.i8[i];
            }
        }
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::MOVNTQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVNTQ", 2, MMX_REGISTER | OPERAND_SIZE);

    BEGIN_OPERATION() {
        DEST = SRC;
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PAVGB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PAVGB", 2, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u8 = {
            uint8_t((DEST.u8[0] + SRC.u8[0]) / 2),
            uint8_t((DEST.u8[1] + SRC.u8[1]) / 2),
            uint8_t((DEST.u8[2] + SRC.u8[2]) / 2),
            uint8_t((DEST.u8[3] + SRC.u8[3]) / 2),
            uint8_t((DEST.u8[4] + SRC.u8[4]) / 2),
            uint8_t((DEST.u8[5] + SRC.u8[5]) / 2),
            uint8_t((DEST.u8[6] + SRC.u8[6]) / 2),
            uint8_t((DEST.u8[7] + SRC.u8[7]) / 2),
        };
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PAVGW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PAVGW", 2, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u16 = {
            uint16_t((DEST.u16[0] + SRC.u16[0]) / 2),
            uint16_t((DEST.u16[1] + SRC.u16[1]) / 2),
            uint16_t((DEST.u16[2] + SRC.u16[2]) / 2),
            uint16_t((DEST.u16[3] + SRC.u16[3]) / 2),
        };
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PEXTRW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PEXTRW", 2, MMX_REGISTER | OPERAND_SIZE | DIRECTION | THREE_OPERAND | IMM_8BIT);
    format.operand[0].type = Format::Operand::REG;

    BEGIN_OPERATION() {
        auto SEL = SRC2.u8[0] % 4;
        DEST.u32[0] = SRC.u16[SEL];
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PINSRW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PINSRW", 2, MMX_REGISTER | OPERAND_SIZE | DIRECTION | THREE_OPERAND | IMM_8BIT);
    if (format.operand[1].type == Format::Operand::MMX)
        format.operand[1].type = Format::Operand::REG;

    BEGIN_OPERATION() {
        auto SEL = SRC2.u8[0] % 4;
        DEST.u16[SEL] = SRC.u16[0];
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PMAXSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMAXSW", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16 = Max(DEST.i16, SRC.i16);
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PMAXUB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMAXUB", 2, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u8 = Max(DEST.u8, SRC.u8);
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PMINSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMINSW", 2, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16 = Min(DEST.i16, SRC.i16);
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PMINUB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMINUB", 2, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u8 = Min(DEST.u8, SRC.u8);
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PMOVMSKB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMOVMSKB", 2, MMX_REGISTER | OPERAND_SIZE | DIRECTION);
    format.operand[0].type = Format::Operand::REG;

    BEGIN_OPERATION() {
        DEST.d = 0;
        DEST.d |= SRC.u8[0] & 0x80 ? 0x01 : 0;
        DEST.d |= SRC.u8[1] & 0x80 ? 0x02 : 0;
        DEST.d |= SRC.u8[2] & 0x80 ? 0x04 : 0;
        DEST.d |= SRC.u8[3] & 0x80 ? 0x08 : 0;
        DEST.d |= SRC.u8[4] & 0x80 ? 0x10 : 0;
        DEST.d |= SRC.u8[5] & 0x80 ? 0x20 : 0;
        DEST.d |= SRC.u8[6] & 0x80 ? 0x40 : 0;
        DEST.d |= SRC.u8[7] & 0x80 ? 0x80 : 0;
    } END_OPERATION_REG_MMX;
}
//-----------------------------------------------------------------------------
void mmx_instruction::PMULHUW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMULHUW", 2, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u16 = {
            uint16_t((DEST.u16[0] * SRC.u16[0]) >> 16),
            uint16_t((DEST.u16[1] * SRC.u16[1]) >> 16),
            uint16_t((DEST.u16[2] * SRC.u16[2]) >> 16),
            uint16_t((DEST.u16[3] * SRC.u16[3]) >> 16),
        };
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PSADBW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSADBW", 2, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u16 = {
            uint16_t(abs(DEST.u8[0] - SRC.u8[0]) +
                     abs(DEST.u8[1] - SRC.u8[1]) +
                     abs(DEST.u8[2] - SRC.u8[2]) +
                     abs(DEST.u8[3] - SRC.u8[3]) +
                     abs(DEST.u8[4] - SRC.u8[4]) +
                     abs(DEST.u8[5] - SRC.u8[5]) +
                     abs(DEST.u8[6] - SRC.u8[6]) +
                     abs(DEST.u8[7] - SRC.u8[7])),
            0,
            0,
            0,
        };
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PSHUFW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSHUFW", 2, MMX_REGISTER | OPERAND_SIZE | DIRECTION | THREE_OPERAND | IMM_8BIT);

    BEGIN_OPERATION() {
        auto SEL = SRC2.u8[0];
        DEST.i16 = {
            SRC.i16[(SEL >> 0) & 0x3],
            SRC.i16[(SEL >> 2) & 0x3],
            SRC.i16[(SEL >> 4) & 0x3],
            SRC.i16[(SEL >> 6) & 0x3],
        };
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
