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
    Decode(format, opcode, "MASKMOVQ", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);
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
    Decode(format, opcode, "MOVNTQ", 2, 0, MMX_REGISTER | OPERAND_SIZE);

    BEGIN_OPERATION() {
        DEST = SRC;
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PAVGB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PAVGB", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u8[0] = (DEST.u8[0] + SRC.u8[0]) / 2;
        DEST.u8[1] = (DEST.u8[1] + SRC.u8[1]) / 2;
        DEST.u8[2] = (DEST.u8[2] + SRC.u8[2]) / 2;
        DEST.u8[3] = (DEST.u8[3] + SRC.u8[3]) / 2;
        DEST.u8[4] = (DEST.u8[4] + SRC.u8[4]) / 2;
        DEST.u8[5] = (DEST.u8[5] + SRC.u8[5]) / 2;
        DEST.u8[6] = (DEST.u8[6] + SRC.u8[6]) / 2;
        DEST.u8[7] = (DEST.u8[7] + SRC.u8[7]) / 2;
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PAVGW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PAVGW", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u16[0] = (DEST.u16[0] + SRC.u16[0]) / 2;
        DEST.u16[1] = (DEST.u16[1] + SRC.u16[1]) / 2;
        DEST.u16[2] = (DEST.u16[2] + SRC.u16[2]) / 2;
        DEST.u16[3] = (DEST.u16[3] + SRC.u16[3]) / 2;
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PEXTRW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PEXTRW", 2, 8, MMX_REGISTER | OPERAND_SIZE | DIRECTION | THREE_OPERAND);
    format.operand[0].type = Format::Operand::REG;

    BEGIN_OPERATION() {
        auto SEL = SRC2.u8[0] % 4;
        DEST.u32[0] = SRC.u16[SEL];
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PINSRW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PINSRW", 2, 8, MMX_REGISTER | OPERAND_SIZE | DIRECTION | THREE_OPERAND);
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
    Decode(format, opcode, "PMAXSW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = std::max(DEST.i16[0], SRC.i16[0]);
        DEST.i16[1] = std::max(DEST.i16[1], SRC.i16[1]);
        DEST.i16[2] = std::max(DEST.i16[2], SRC.i16[2]);
        DEST.i16[3] = std::max(DEST.i16[3], SRC.i16[3]);
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PMAXUB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMAXUB", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u8[0] = std::max(DEST.u8[0], SRC.u8[0]);
        DEST.u8[1] = std::max(DEST.u8[1], SRC.u8[1]);
        DEST.u8[2] = std::max(DEST.u8[2], SRC.u8[2]);
        DEST.u8[3] = std::max(DEST.u8[3], SRC.u8[3]);
        DEST.u8[4] = std::max(DEST.u8[4], SRC.u8[4]);
        DEST.u8[5] = std::max(DEST.u8[5], SRC.u8[5]);
        DEST.u8[6] = std::max(DEST.u8[6], SRC.u8[6]);
        DEST.u8[7] = std::max(DEST.u8[7], SRC.u8[7]);
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PMINSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMINSW", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = std::min(DEST.i16[0], SRC.i16[0]);
        DEST.i16[1] = std::min(DEST.i16[1], SRC.i16[1]);
        DEST.i16[2] = std::min(DEST.i16[2], SRC.i16[2]);
        DEST.i16[3] = std::min(DEST.i16[3], SRC.i16[3]);
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PMINUB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMINUB", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u8[0] = std::min(DEST.u8[0], SRC.u8[0]);
        DEST.u8[1] = std::min(DEST.u8[1], SRC.u8[1]);
        DEST.u8[2] = std::min(DEST.u8[2], SRC.u8[2]);
        DEST.u8[3] = std::min(DEST.u8[3], SRC.u8[3]);
        DEST.u8[4] = std::min(DEST.u8[4], SRC.u8[4]);
        DEST.u8[5] = std::min(DEST.u8[5], SRC.u8[5]);
        DEST.u8[6] = std::min(DEST.u8[6], SRC.u8[6]);
        DEST.u8[7] = std::min(DEST.u8[7], SRC.u8[7]);
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PMOVMSKB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMOVMSKB", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);
    format.operand[0].type = Format::Operand::REG;

    BEGIN_OPERATION() {
        DEST.u32[0] = 0;
        DEST.u32[0] |= SRC.u8[0] & 0x80 ? 0x01 : 0;
        DEST.u32[0] |= SRC.u8[1] & 0x80 ? 0x02 : 0;
        DEST.u32[0] |= SRC.u8[2] & 0x80 ? 0x04 : 0;
        DEST.u32[0] |= SRC.u8[3] & 0x80 ? 0x08 : 0;
        DEST.u32[0] |= SRC.u8[4] & 0x80 ? 0x10 : 0;
        DEST.u32[0] |= SRC.u8[5] & 0x80 ? 0x20 : 0;
        DEST.u32[0] |= SRC.u8[6] & 0x80 ? 0x40 : 0;
        DEST.u32[0] |= SRC.u8[7] & 0x80 ? 0x80 : 0;
    } END_OPERATION_MMX;
}
//-----------------------------------------------------------------------------
void mmx_instruction::PMULHUW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMULHUW", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u16[0] = (DEST.u16[0] * SRC.u16[0]) >> 16;
        DEST.u16[1] = (DEST.u16[1] * SRC.u16[1]) >> 16;
        DEST.u16[2] = (DEST.u16[2] * SRC.u16[2]) >> 16;
        DEST.u16[3] = (DEST.u16[3] * SRC.u16[3]) >> 16;
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PSADBW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSADBW", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u16[0] = 0;
        DEST.u16[0] += abs(DEST.u8[0] - SRC.u8[0]);
        DEST.u16[0] += abs(DEST.u8[1] - SRC.u8[1]);
        DEST.u16[0] += abs(DEST.u8[2] - SRC.u8[2]);
        DEST.u16[0] += abs(DEST.u8[3] - SRC.u8[3]);
        DEST.u16[0] += abs(DEST.u8[4] - SRC.u8[4]);
        DEST.u16[0] += abs(DEST.u8[5] - SRC.u8[5]);
        DEST.u16[0] += abs(DEST.u8[6] - SRC.u8[6]);
        DEST.u16[0] += abs(DEST.u8[7] - SRC.u8[7]);
        DEST.u16[1] = 0;
        DEST.u16[2] = 0;
        DEST.u16[3] = 0;
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PSHUFW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSHUFW", 2, 8, MMX_REGISTER | OPERAND_SIZE | DIRECTION | THREE_OPERAND);

    BEGIN_OPERATION() {
        auto SEL = SRC2.u8[0];
        DEST.i16[0] = SRC.i16[(SEL >> 0) & 0x3];
        DEST.i16[1] = SRC.i16[(SEL >> 2) & 0x3];
        DEST.i16[2] = SRC.i16[(SEL >> 4) & 0x3];
        DEST.i16[3] = SRC.i16[(SEL >> 6) & 0x3];
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
