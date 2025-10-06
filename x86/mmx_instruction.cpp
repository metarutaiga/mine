#include <algorithm>
#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"
#include "x87_register.h"
#include "x87_register.inl"
#include "mmx_register.h"
#include "mmx_register.inl"
#include "mmx_instruction.h"
#include "mmx_instruction.inl"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void mmx_instruction::EMMS(Format& format, const uint8_t* opcode)
{
    format.instruction = "EMMS";

    OPERATION() {
        TOP = 0;
    };
}
//------------------------------------------------------------------------------
void mmx_instruction::MOVD(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x6E:
        Decode(format, opcode, "MOVD", 2, 0, OPERAND_SIZE | DIRECTION);
        format.operand[0].type = Format::Operand::MMX;
        break;
    case 0x7E:
        Decode(format, opcode, "MOVD", 2, 0, OPERAND_SIZE);
        format.operand[1].type = Format::Operand::MMX;
        break;
    }

    if (format.operand[0].type == Format::Operand::MMX && format.operand[1].type != Format::Operand::MMX) {
        BEGIN_OPERATION() {
            DEST.u32[0] = SRC.u32[0];
            DEST.u32[1] = 0;
        } END_OPERATION_MMX;
    }
    else {
        BEGIN_OPERATION() {
            DEST.u32[0] = SRC.u32[0];
        } END_OPERATION_MMX;
    }
}
//------------------------------------------------------------------------------
void mmx_instruction::MOVQ(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x6F:  Decode(format, opcode, "MOVQ", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION); break;
    case 0x7F:  Decode(format, opcode, "MOVQ", 2, 0, MMX_REGISTER | OPERAND_SIZE);             break;
    }

    BEGIN_OPERATION() {
        DEST.u64[0] = SRC.u64[0];
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PACKSSDW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PACKSSDW", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16 = {
            int16_t(std::clamp<int32_t>(DEST.i32[0], INT16_MIN, INT16_MAX)),
            int16_t(std::clamp<int32_t>(DEST.i32[1], INT16_MIN, INT16_MAX)),
            int16_t(std::clamp<int32_t>(SRC.i32[0], INT16_MIN, INT16_MAX)),
            int16_t(std::clamp<int32_t>(SRC.i32[1], INT16_MIN, INT16_MAX)),
        };
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PACKSSWB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PACKSSWB", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i8 = {
            int8_t(std::clamp<int16_t>(DEST.i16[0], INT8_MIN, INT8_MAX)),
            int8_t(std::clamp<int16_t>(DEST.i16[1], INT8_MIN, INT8_MAX)),
            int8_t(std::clamp<int16_t>(DEST.i16[2], INT8_MIN, INT8_MAX)),
            int8_t(std::clamp<int16_t>(DEST.i16[3], INT8_MIN, INT8_MAX)),
            int8_t(std::clamp<int16_t>(SRC.i16[0], INT8_MIN, INT8_MAX)),
            int8_t(std::clamp<int16_t>(SRC.i16[1], INT8_MIN, INT8_MAX)),
            int8_t(std::clamp<int16_t>(SRC.i16[2], INT8_MIN, INT8_MAX)),
            int8_t(std::clamp<int16_t>(SRC.i16[3], INT8_MIN, INT8_MAX)),
        };
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PACKUSWB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PACKUSWB", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u8 = {
            uint8_t(std::clamp<int16_t>(DEST.i16[0], 0, UINT8_MAX)),
            uint8_t(std::clamp<int16_t>(DEST.i16[1], 0, UINT8_MAX)),
            uint8_t(std::clamp<int16_t>(DEST.i16[2], 0, UINT8_MAX)),
            uint8_t(std::clamp<int16_t>(DEST.i16[3], 0, UINT8_MAX)),
            uint8_t(std::clamp<int16_t>(SRC.i16[0], 0, UINT8_MAX)),
            uint8_t(std::clamp<int16_t>(SRC.i16[1], 0, UINT8_MAX)),
            uint8_t(std::clamp<int16_t>(SRC.i16[2], 0, UINT8_MAX)),
            uint8_t(std::clamp<int16_t>(SRC.i16[3], 0, UINT8_MAX)),
        };
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PADDB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PADDB", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i8[0] = DEST.i8[0] + SRC.i8[0];
        DEST.i8[1] = DEST.i8[1] + SRC.i8[1];
        DEST.i8[2] = DEST.i8[2] + SRC.i8[2];
        DEST.i8[3] = DEST.i8[3] + SRC.i8[3];
        DEST.i8[4] = DEST.i8[4] + SRC.i8[4];
        DEST.i8[5] = DEST.i8[5] + SRC.i8[5];
        DEST.i8[6] = DEST.i8[6] + SRC.i8[6];
        DEST.i8[7] = DEST.i8[7] + SRC.i8[7];
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PADDW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PADDW", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = DEST.i16[0] + SRC.i16[0];
        DEST.i16[1] = DEST.i16[1] + SRC.i16[1];
        DEST.i16[2] = DEST.i16[2] + SRC.i16[2];
        DEST.i16[3] = DEST.i16[3] + SRC.i16[3];
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PADDD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PADDD", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32[0] = DEST.i32[0] + SRC.i32[0];
        DEST.i32[1] = DEST.i32[1] + SRC.i32[1];
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PADDSB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PADDSB", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i8[0] = std::clamp<int16_t>(DEST.i8[0] + SRC.i8[0], INT8_MIN, INT8_MAX);
        DEST.i8[1] = std::clamp<int16_t>(DEST.i8[1] + SRC.i8[1], INT8_MIN, INT8_MAX);
        DEST.i8[2] = std::clamp<int16_t>(DEST.i8[2] + SRC.i8[2], INT8_MIN, INT8_MAX);
        DEST.i8[3] = std::clamp<int16_t>(DEST.i8[3] + SRC.i8[3], INT8_MIN, INT8_MAX);
        DEST.i8[4] = std::clamp<int16_t>(DEST.i8[4] + SRC.i8[4], INT8_MIN, INT8_MAX);
        DEST.i8[5] = std::clamp<int16_t>(DEST.i8[5] + SRC.i8[5], INT8_MIN, INT8_MAX);
        DEST.i8[6] = std::clamp<int16_t>(DEST.i8[6] + SRC.i8[6], INT8_MIN, INT8_MAX);
        DEST.i8[7] = std::clamp<int16_t>(DEST.i8[7] + SRC.i8[7], INT8_MIN, INT8_MAX);
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PADDSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PADDSW", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = std::clamp<int32_t>(DEST.i16[0] + SRC.i16[0], INT16_MIN, INT16_MAX);
        DEST.i16[1] = std::clamp<int32_t>(DEST.i16[1] + SRC.i16[1], INT16_MIN, INT16_MAX);
        DEST.i16[2] = std::clamp<int32_t>(DEST.i16[2] + SRC.i16[2], INT16_MIN, INT16_MAX);
        DEST.i16[3] = std::clamp<int32_t>(DEST.i16[3] + SRC.i16[3], INT16_MIN, INT16_MAX);
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PADDUSB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PADDUSB", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u8[0] = std::clamp<int16_t>(DEST.u8[0] + SRC.u8[0], 0, UINT8_MAX);
        DEST.u8[1] = std::clamp<int16_t>(DEST.u8[1] + SRC.u8[1], 0, UINT8_MAX);
        DEST.u8[2] = std::clamp<int16_t>(DEST.u8[2] + SRC.u8[2], 0, UINT8_MAX);
        DEST.u8[3] = std::clamp<int16_t>(DEST.u8[3] + SRC.u8[3], 0, UINT8_MAX);
        DEST.u8[4] = std::clamp<int16_t>(DEST.u8[4] + SRC.u8[4], 0, UINT8_MAX);
        DEST.u8[5] = std::clamp<int16_t>(DEST.u8[5] + SRC.u8[5], 0, UINT8_MAX);
        DEST.u8[6] = std::clamp<int16_t>(DEST.u8[6] + SRC.u8[6], 0, UINT8_MAX);
        DEST.u8[7] = std::clamp<int16_t>(DEST.u8[7] + SRC.u8[7], 0, UINT8_MAX);
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PADDUSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PADDUSW", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u16[0] = std::clamp<int32_t>(DEST.u16[0] + SRC.u16[0], 0, UINT16_MAX);
        DEST.u16[1] = std::clamp<int32_t>(DEST.u16[1] + SRC.u16[1], 0, UINT16_MAX);
        DEST.u16[2] = std::clamp<int32_t>(DEST.u16[2] + SRC.u16[2], 0, UINT16_MAX);
        DEST.u16[3] = std::clamp<int32_t>(DEST.u16[3] + SRC.u16[3], 0, UINT16_MAX);
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PAND(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PAND", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = DEST.i64[0] & SRC.i64[0];
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PANDN(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PANDN", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = ~DEST.i64[0] & SRC.i64[0];
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PCMPEQB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PCMPEQB", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i8[0] = (DEST.i8[0] == SRC.i8[0]) ? -1 : 0;
        DEST.i8[1] = (DEST.i8[1] == SRC.i8[1]) ? -1 : 0;
        DEST.i8[2] = (DEST.i8[2] == SRC.i8[2]) ? -1 : 0;
        DEST.i8[3] = (DEST.i8[3] == SRC.i8[3]) ? -1 : 0;
        DEST.i8[4] = (DEST.i8[4] == SRC.i8[4]) ? -1 : 0;
        DEST.i8[5] = (DEST.i8[5] == SRC.i8[5]) ? -1 : 0;
        DEST.i8[6] = (DEST.i8[6] == SRC.i8[6]) ? -1 : 0;
        DEST.i8[7] = (DEST.i8[7] == SRC.i8[7]) ? -1 : 0;
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PCMPEQW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PCMPEQW", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = (DEST.i16[0] == SRC.i16[0]) ? -1 : 0;
        DEST.i16[1] = (DEST.i16[1] == SRC.i16[1]) ? -1 : 0;
        DEST.i16[2] = (DEST.i16[2] == SRC.i16[2]) ? -1 : 0;
        DEST.i16[3] = (DEST.i16[3] == SRC.i16[3]) ? -1 : 0;
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PCMPEQD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PCMPEQD", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32[0] = (DEST.i32[0] == SRC.i32[0]) ? -1 : 0;
        DEST.i32[1] = (DEST.i32[1] == SRC.i32[1]) ? -1 : 0;
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PCMPGTB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PCMPGTB", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i8[0] = (DEST.i8[0] > SRC.i8[0]) ? -1 : 0;
        DEST.i8[1] = (DEST.i8[1] > SRC.i8[1]) ? -1 : 0;
        DEST.i8[2] = (DEST.i8[2] > SRC.i8[2]) ? -1 : 0;
        DEST.i8[3] = (DEST.i8[3] > SRC.i8[3]) ? -1 : 0;
        DEST.i8[4] = (DEST.i8[4] > SRC.i8[4]) ? -1 : 0;
        DEST.i8[5] = (DEST.i8[5] > SRC.i8[5]) ? -1 : 0;
        DEST.i8[6] = (DEST.i8[6] > SRC.i8[6]) ? -1 : 0;
        DEST.i8[7] = (DEST.i8[7] > SRC.i8[7]) ? -1 : 0;
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PCMPGTW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PCMPGTW", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = (DEST.i16[0] > SRC.i16[0]) ? -1 : 0;
        DEST.i16[1] = (DEST.i16[1] > SRC.i16[1]) ? -1 : 0;
        DEST.i16[2] = (DEST.i16[2] > SRC.i16[2]) ? -1 : 0;
        DEST.i16[3] = (DEST.i16[3] > SRC.i16[3]) ? -1 : 0;
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PCMPGTD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PCMPGTD", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32[0] = (DEST.i32[0] > SRC.i32[0]) ? -1 : 0;
        DEST.i32[1] = (DEST.i32[1] > SRC.i32[1]) ? -1 : 0;
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PMADDWD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMADDWD", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32 = {
            DEST.i16[0] * SRC.i16[0] + DEST.i16[1] * SRC.i16[1],
            DEST.i16[2] * SRC.i16[2] + DEST.i16[3] * SRC.i16[3],
        };
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PMULHW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMULHW", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = (DEST.i16[0] * SRC.i16[0]) >> 16;
        DEST.i16[1] = (DEST.i16[1] * SRC.i16[1]) >> 16;
        DEST.i16[2] = (DEST.i16[2] * SRC.i16[2]) >> 16;
        DEST.i16[3] = (DEST.i16[3] * SRC.i16[3]) >> 16;
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PMULLW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMULLW", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = DEST.i16[0] * SRC.i16[0];
        DEST.i16[1] = DEST.i16[1] * SRC.i16[1];
        DEST.i16[2] = DEST.i16[2] * SRC.i16[2];
        DEST.i16[3] = DEST.i16[3] * SRC.i16[3];
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::POR(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "POR", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = DEST.i64[0] | SRC.i64[0];
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PSLLW(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x71:  Decode(format, opcode, "PSLLW", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);
                format.operand[1].type = Format::Operand::IMM;
                format.operand[1].displacement = IMM8(opcode, 3);                               break;
    case 0xF1:  Decode(format, opcode, "PSLLW", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION); break;
    }

    BEGIN_OPERATION() {
        auto COUNT = SRC.u8[0];
        DEST.i16[0] = COUNT < 16 ? DEST.i16[0] << COUNT : 0;
        DEST.i16[1] = COUNT < 16 ? DEST.i16[1] << COUNT : 0;
        DEST.i16[2] = COUNT < 16 ? DEST.i16[2] << COUNT : 0;
        DEST.i16[3] = COUNT < 16 ? DEST.i16[3] << COUNT : 0;
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PSLLD(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x72:  Decode(format, opcode, "PSLLD", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);
                format.operand[1].type = Format::Operand::IMM;
                format.operand[1].displacement = IMM8(opcode, 3);                               break;
    case 0xF2:  Decode(format, opcode, "PSLLD", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION); break;
    }

    BEGIN_OPERATION() {
        auto COUNT = SRC.u8[0];
        DEST.i32[0] = COUNT < 32 ? DEST.i32[0] << COUNT : 0;
        DEST.i32[1] = COUNT < 32 ? DEST.i32[1] << COUNT : 0;
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PSLLQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSLLQ", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        auto COUNT = SRC.u8[0];
        DEST.i64[0] = COUNT < 64 ? DEST.i64[0] << COUNT : 0;
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PSRAW(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x71:  Decode(format, opcode, "PSRAW", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);
                format.operand[1].type = Format::Operand::IMM;
                format.operand[1].displacement = IMM8(opcode, 3);                               break;
    case 0xE1:  Decode(format, opcode, "PSRAW", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION); break;
    }

    BEGIN_OPERATION() {
        auto COUNT = SRC.u8[0];
        DEST.i16[0] = COUNT < 16 ? DEST.i16[0] >> COUNT : 0;
        DEST.i16[1] = COUNT < 16 ? DEST.i16[1] >> COUNT : 0;
        DEST.i16[2] = COUNT < 16 ? DEST.i16[2] >> COUNT : 0;
        DEST.i16[3] = COUNT < 16 ? DEST.i16[3] >> COUNT : 0;
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PSRAD(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x72:  Decode(format, opcode, "PSRAD", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);
                format.operand[1].type = Format::Operand::IMM;
                format.operand[1].displacement = IMM8(opcode, 3);                               break;
    case 0xE2:  Decode(format, opcode, "PSRAD", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION); break;
    }

    BEGIN_OPERATION() {
        auto COUNT = SRC.u8[0];
        DEST.i32[0] = COUNT < 32 ? DEST.i32[0] >> COUNT : 0;
        DEST.i32[1] = COUNT < 32 ? DEST.i32[1] >> COUNT : 0;
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PSRLW(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x71:  Decode(format, opcode, "PSRLW", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);
                format.operand[1].type = Format::Operand::IMM;
                format.operand[1].displacement = IMM8(opcode, 3);                               break;
    case 0xD1:  Decode(format, opcode, "PSRLW", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION); break;
    }

    BEGIN_OPERATION() {
        auto COUNT = SRC.u8[0];
        DEST.u16[0] = COUNT < 16 ? DEST.u16[0] >> COUNT : 0;
        DEST.u16[1] = COUNT < 16 ? DEST.u16[1] >> COUNT : 0;
        DEST.u16[2] = COUNT < 16 ? DEST.u16[2] >> COUNT : 0;
        DEST.u16[3] = COUNT < 16 ? DEST.u16[3] >> COUNT : 0;
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PSRLD(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x72:  Decode(format, opcode, "PSRLD", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);
                format.operand[1].type = Format::Operand::IMM;
                format.operand[1].displacement = IMM8(opcode, 3);                               break;
    case 0xD2:  Decode(format, opcode, "PSRLD", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION); break;
    }

    BEGIN_OPERATION() {
        auto COUNT = SRC.u8[0];
        DEST.u32[0] = COUNT < 32 ? DEST.u32[0] >> COUNT : 0;
        DEST.u32[1] = COUNT < 32 ? DEST.u32[1] >> COUNT : 0;
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PSRLQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSRLQ", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        auto COUNT = SRC.u8[0];
        DEST.u64[0] = COUNT < 64 ? DEST.u64[0] >> COUNT : 0;
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PSUBB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSUBB", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i8[0] = DEST.i8[0] - SRC.i8[0];
        DEST.i8[1] = DEST.i8[1] - SRC.i8[1];
        DEST.i8[2] = DEST.i8[2] - SRC.i8[2];
        DEST.i8[3] = DEST.i8[3] - SRC.i8[3];
        DEST.i8[4] = DEST.i8[4] - SRC.i8[4];
        DEST.i8[5] = DEST.i8[5] - SRC.i8[5];
        DEST.i8[6] = DEST.i8[6] - SRC.i8[6];
        DEST.i8[7] = DEST.i8[7] - SRC.i8[7];
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PSUBW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSUBW", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = DEST.i16[0] - SRC.i16[0];
        DEST.i16[1] = DEST.i16[1] - SRC.i16[1];
        DEST.i16[2] = DEST.i16[2] - SRC.i16[2];
        DEST.i16[3] = DEST.i16[3] - SRC.i16[3];
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PSUBD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSUBD", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32[0] = DEST.i32[0] - SRC.i32[0];
        DEST.i32[1] = DEST.i32[1] - SRC.i32[1];
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PSUBSB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSUBSB", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i8[0] = std::clamp<int16_t>(DEST.i8[0] - SRC.i8[0], INT8_MIN, INT8_MAX);
        DEST.i8[1] = std::clamp<int16_t>(DEST.i8[1] - SRC.i8[1], INT8_MIN, INT8_MAX);
        DEST.i8[2] = std::clamp<int16_t>(DEST.i8[2] - SRC.i8[2], INT8_MIN, INT8_MAX);
        DEST.i8[3] = std::clamp<int16_t>(DEST.i8[3] - SRC.i8[3], INT8_MIN, INT8_MAX);
        DEST.i8[4] = std::clamp<int16_t>(DEST.i8[4] - SRC.i8[4], INT8_MIN, INT8_MAX);
        DEST.i8[5] = std::clamp<int16_t>(DEST.i8[5] - SRC.i8[5], INT8_MIN, INT8_MAX);
        DEST.i8[6] = std::clamp<int16_t>(DEST.i8[6] - SRC.i8[6], INT8_MIN, INT8_MAX);
        DEST.i8[7] = std::clamp<int16_t>(DEST.i8[7] - SRC.i8[7], INT8_MIN, INT8_MAX);
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PSUBSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSUBSW", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = std::clamp<int32_t>(DEST.i16[0] - SRC.i16[0], INT16_MIN, INT16_MAX);
        DEST.i16[1] = std::clamp<int32_t>(DEST.i16[1] - SRC.i16[1], INT16_MIN, INT16_MAX);
        DEST.i16[2] = std::clamp<int32_t>(DEST.i16[2] - SRC.i16[2], INT16_MIN, INT16_MAX);
        DEST.i16[3] = std::clamp<int32_t>(DEST.i16[3] - SRC.i16[3], INT16_MIN, INT16_MAX);
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PSUBUSB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSUBUSB", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i8[0] = std::clamp<int16_t>(DEST.i8[0] - SRC.i8[0], 0, UINT8_MAX);
        DEST.i8[1] = std::clamp<int16_t>(DEST.i8[1] - SRC.i8[1], 0, UINT8_MAX);
        DEST.i8[2] = std::clamp<int16_t>(DEST.i8[2] - SRC.i8[2], 0, UINT8_MAX);
        DEST.i8[3] = std::clamp<int16_t>(DEST.i8[3] - SRC.i8[3], 0, UINT8_MAX);
        DEST.i8[4] = std::clamp<int16_t>(DEST.i8[4] - SRC.i8[4], 0, UINT8_MAX);
        DEST.i8[5] = std::clamp<int16_t>(DEST.i8[5] - SRC.i8[5], 0, UINT8_MAX);
        DEST.i8[6] = std::clamp<int16_t>(DEST.i8[6] - SRC.i8[6], 0, UINT8_MAX);
        DEST.i8[7] = std::clamp<int16_t>(DEST.i8[7] - SRC.i8[7], 0, UINT8_MAX);
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PSUBUSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSUBUSW", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = std::clamp<int32_t>(DEST.i16[0] - SRC.i16[0], 0, UINT16_MAX);
        DEST.i16[1] = std::clamp<int32_t>(DEST.i16[1] - SRC.i16[1], 0, UINT16_MAX);
        DEST.i16[2] = std::clamp<int32_t>(DEST.i16[2] - SRC.i16[2], 0, UINT16_MAX);
        DEST.i16[3] = std::clamp<int32_t>(DEST.i16[3] - SRC.i16[3], 0, UINT16_MAX);
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PUNPCKHBW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PUNPCKHBW", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i8 = {
            DEST.i8[4],
            SRC.i8[4],
            DEST.i8[5],
            SRC.i8[5],
            DEST.i8[6],
            SRC.i8[6],
            DEST.i8[7],
            SRC.i8[7],
        };
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PUNPCKHWD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PUNPCKHWD", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16 = {
            DEST.i16[2],
            SRC.i16[2],
            DEST.i16[3],
            SRC.i16[3],
        };
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PUNPCKHDQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PUNPCKHDQ", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32 = {
            DEST.i32[1],
            SRC.i32[1],
        };
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PUNPCKLBW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PUNPCKLBW", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i8 = {
            DEST.i8[0],
            SRC.i8[0],
            DEST.i8[1],
            SRC.i8[1],
            DEST.i8[2],
            SRC.i8[2],
            DEST.i8[3],
            SRC.i8[3],
        };
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PUNPCKLWD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PUNPCKLWD", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16 = {
            DEST.i16[0],
            SRC.i16[0],
            DEST.i16[1],
            SRC.i16[1],
        };
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PUNPCKLDQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PUNPCKLDQ", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32 = {
            DEST.i32[0],
            SRC.i32[0],
        };
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PXOR(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PXOR", 2, 0, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = DEST.i64[0] ^ SRC.i64[0];
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
