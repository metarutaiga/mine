#include <math.h>
#include <algorithm>
#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"
#include "mmx_register.h"
#include "mmx_register.inl"
#include "mmx_instruction.h"
#include "mmx_instruction.inl"
#include "sse_register.h"
#include "sse_register.inl"
#include "sse_instruction.h"
#include "sse_instruction.inl"
#include "sse2_instruction.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void sse2_instruction::ADDPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "ADDPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = DEST.f64[0] + SRC.f64[0];
        DEST.f64[1] = DEST.f64[1] + SRC.f64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::ADDSD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "ADDSD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = DEST.f64[0] + SRC.f64[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::ANDNPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "ANDNPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = ~DEST.i64[0] & SRC.i64[0];
        DEST.i64[1] = ~DEST.i64[1] & SRC.i64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::ANDPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "ANDPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = DEST.i64[0] & SRC.i64[0];
        DEST.i64[1] = DEST.i64[1] & SRC.i64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::CLFLUSH(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CLFLUSH", 2);
    format.operand_count = 1;

    OPERATION() {};
}
//------------------------------------------------------------------------------
void sse2_instruction::CMPPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CMPPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION | THREE_OPERAND);

    switch (format.operand[2].displacement) {
    case 0:
        BEGIN_OPERATION() {
            DEST.i64[0] = (DEST.f64[0] == SRC.f64[0]) ? -1 : 0;
            DEST.i64[1] = (DEST.f64[1] == SRC.f64[1]) ? -1 : 0;
        } END_OPERATION_SSE;
        break;
    case 1:
        BEGIN_OPERATION() {
            DEST.i64[0] = (DEST.f64[0] < SRC.f64[0]) ? -1 : 0;
            DEST.i64[1] = (DEST.f64[1] < SRC.f64[1]) ? -1 : 0;
        } END_OPERATION_SSE;
        break;
    case 2:
        BEGIN_OPERATION() {
            DEST.i64[0] = (DEST.f64[0] <= SRC.f64[0]) ? -1 : 0;
            DEST.i64[1] = (DEST.f64[1] <= SRC.f64[1]) ? -1 : 0;
        } END_OPERATION_SSE;
        break;
    case 3:
        BEGIN_OPERATION() {
            DEST.i64[0] = isunordered(DEST.f64[0], SRC.f64[0]) ? -1 : 0;
            DEST.i64[1] = isunordered(DEST.f64[1], SRC.f64[1]) ? -1 : 0;
        } END_OPERATION_SSE;
        break;
    case 4:
        BEGIN_OPERATION() {
            DEST.i64[0] = (DEST.f64[0] != SRC.f64[0]) ? -1 : 0;
            DEST.i64[1] = (DEST.f64[1] != SRC.f64[1]) ? -1 : 0;
        } END_OPERATION_SSE;
        break;
    case 5:
        BEGIN_OPERATION() {
            DEST.i64[0] = (DEST.f64[0] >= SRC.f64[0]) ? -1 : 0;
            DEST.i64[1] = (DEST.f64[1] >= SRC.f64[1]) ? -1 : 0;
        } END_OPERATION_SSE;
        break;
    case 6:
        BEGIN_OPERATION() {
            DEST.i64[0] = (DEST.f64[0] > SRC.f64[0]) ? -1 : 0;
            DEST.i64[1] = (DEST.f64[1] > SRC.f64[1]) ? -1 : 0;
        } END_OPERATION_SSE;
        break;
    case 7:
        BEGIN_OPERATION() {
            DEST.i64[0] = isunordered(DEST.f64[0], SRC.f64[0]) == false ? -1 : 0;
            DEST.i64[1] = isunordered(DEST.f64[1], SRC.f64[1]) == false ? -1 : 0;
        } END_OPERATION_SSE;
        break;
    }
}
//------------------------------------------------------------------------------
void sse2_instruction::CMPSD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CMPSD", 2, 8, SSE_REGISTER | OPERAND_SIZE | DIRECTION | THREE_OPERAND);

    switch (format.operand[2].displacement) {
    case 0:
        BEGIN_OPERATION() {
            DEST.i64[0] = (DEST.f64[0] == SRC.f64[0]) ? -1 : 0;
        } END_OPERATION_SSE;
        break;
    case 1:
        BEGIN_OPERATION() {
            DEST.i64[0] = (DEST.f64[0] < SRC.f64[0]) ? -1 : 0;
        } END_OPERATION_SSE;
        break;
    case 2:
        BEGIN_OPERATION() {
            DEST.i64[0] = (DEST.f64[0] <= SRC.f64[0]) ? -1 : 0;
        } END_OPERATION_SSE;
        break;
    case 3:
        BEGIN_OPERATION() {
            DEST.i64[0] = isunordered(DEST.f64[0], SRC.f64[0]) ? -1 : 0;
        } END_OPERATION_SSE;
        break;
    case 4:
        BEGIN_OPERATION() {
            DEST.i64[0] = (DEST.f64[0] != SRC.f64[0]) ? -1 : 0;
        } END_OPERATION_SSE;
        break;
    case 5:
        BEGIN_OPERATION() {
            DEST.i64[0] = (DEST.f64[0] >= SRC.f64[0]) ? -1 : 0;
        } END_OPERATION_SSE;
        break;
    case 6:
        BEGIN_OPERATION() {
            DEST.i64[0] = (DEST.f64[0] > SRC.f64[0]) ? -1 : 0;
        } END_OPERATION_SSE;
        break;
    case 7:
        BEGIN_OPERATION() {
            DEST.i64[0] = isunordered(DEST.f64[0], SRC.f64[0]) == false ? -1 : 0;
        } END_OPERATION_SSE;
        break;
    }
}
//------------------------------------------------------------------------------
void sse2_instruction::COMISD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "COMISD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        if (isunordered(DEST.f64[0], SRC.f64[0])) {
            ZF = 0;
            PF = 1;
            CF = 1;
        }
        else if (DEST.f64[0] > SRC.f64[0]) {
            ZF = 0;
            PF = 0;
            CF = 0;
        }
        else if (DEST.f64[0] < SRC.f64[0]) {
            ZF = 0;
            PF = 0;
            CF = 1;
        }
        else {
            ZF = 1;
            PF = 0;
            CF = 0;
        }
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::CVTDQ2PD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTDQ2PD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = SRC.i64[0];
        DEST.f64[1] = SRC.i64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::CVTDQ2PS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTDQ2PS", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32[0] = SRC.i32[0];
        DEST.f32[1] = SRC.i32[1];
        DEST.f32[2] = SRC.i32[2];
        DEST.f32[3] = SRC.i32[3];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::CVTPD2DQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTPD2DQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        switch (MXCSR_RC) {
        case RoundNearest:
            DEST.i32[0] = round(SRC.f64[0]);
            DEST.i32[1] = round(SRC.f64[1]);
            break;
        case RoundDown:
            DEST.i32[0] = floor(SRC.f64[0]);
            DEST.i32[1] = floor(SRC.f64[1]);
            break;
        case RoundUp:
            DEST.i32[0] = ceil(SRC.f64[0]);
            DEST.i32[1] = ceil(SRC.f64[1]);
            break;
        case RoundChop:
            DEST.i32[0] = trunc(SRC.f64[0]);
            DEST.i32[1] = trunc(SRC.f64[1]);
            break;
        }
        DEST.i32[2] = 0;
        DEST.i32[3] = 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::CVTPD2PI(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTPD2PI", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);
    format.operand[0].type = Format::Operand::MMX;

    BEGIN_OPERATION() {
        switch (MXCSR_RC) {
        case RoundNearest:
            DEST.i32[0] = round(SRC.f64[0]);
            DEST.i32[1] = round(SRC.f64[1]);
            break;
        case RoundDown:
            DEST.i32[0] = floor(SRC.f64[0]);
            DEST.i32[1] = floor(SRC.f64[1]);
            break;
        case RoundUp:
            DEST.i32[0] = ceil(SRC.f64[0]);
            DEST.i32[1] = ceil(SRC.f64[1]);
            break;
        case RoundChop:
            DEST.i32[0] = trunc(SRC.f64[0]);
            DEST.i32[1] = trunc(SRC.f64[1]);
            break;
        }
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::CVTPD2PS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTPD2PS", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32[0] = SRC.f64[0];
        DEST.f32[1] = SRC.f64[1];
        DEST.f32[2] = 0;
        DEST.f32[3] = 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::CVTPI2PD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTPI2PD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);
    if (format.operand[1].type == Format::Operand::SSE)
        format.operand[1].type = Format::Operand::MMX;

    BEGIN_OPERATION() {
        DEST.f64[0] = SRC.i32[0];
        DEST.f64[1] = SRC.i32[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::CVTPS2DQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTPS2DQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        switch (MXCSR_RC) {
        case RoundNearest:
            DEST.i32[0] = roundf(SRC.f32[0]);
            DEST.i32[1] = roundf(SRC.f32[1]);
            DEST.i32[2] = roundf(SRC.f32[2]);
            DEST.i32[3] = roundf(SRC.f32[3]);
            break;
        case RoundDown:
            DEST.i32[0] = floorf(SRC.f32[0]);
            DEST.i32[1] = floorf(SRC.f32[1]);
            DEST.i32[2] = floorf(SRC.f32[2]);
            DEST.i32[3] = floorf(SRC.f32[3]);
            break;
        case RoundUp:
            DEST.i32[0] = ceilf(SRC.f32[0]);
            DEST.i32[1] = ceilf(SRC.f32[1]);
            DEST.i32[2] = ceilf(SRC.f32[2]);
            DEST.i32[3] = ceilf(SRC.f32[3]);
            break;
        case RoundChop:
            DEST.i32[0] = truncf(SRC.f32[0]);
            DEST.i32[1] = truncf(SRC.f32[1]);
            DEST.i32[2] = truncf(SRC.f32[2]);
            DEST.i32[3] = truncf(SRC.f32[3]);
            break;
        }
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::CVTPS2PD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTPS2PD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        auto TEMP = SRC;
        DEST.f64[0] = TEMP.f32[0];
        DEST.f64[1] = TEMP.f32[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::CVTSD2SI(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTSD2SI", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);
    format.operand[0].type = Format::Operand::REG;

    BEGIN_OPERATION() {
        switch (MXCSR_RC) {
        case RoundNearest:
            DEST.i32[0] = round(SRC.f64[0]);
            break;
        case RoundDown:
            DEST.i32[0] = floor(SRC.f64[0]);
            break;
        case RoundUp:
            DEST.i32[0] = ceil(SRC.f64[0]);
            break;
        case RoundChop:
            DEST.i32[0] = trunc(SRC.f64[0]);
            break;
        }
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::CVTSD2SS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTSD2SS", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32[0] = SRC.f64[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::CVTSI2SD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTSI2SD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);
    if (format.operand[1].type == Format::Operand::SSE)
        format.operand[1].type = Format::Operand::REG;

    BEGIN_OPERATION() {
        DEST.f64[0] = SRC.i32[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::CVTSS2SD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTSS2SD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = SRC.f32[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::CVTTPD2DQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTTPD2DQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32[0] = SRC.f64[0];
        DEST.i32[1] = SRC.f64[1];
        DEST.i32[2] = 0;
        DEST.i32[3] = 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::CVTTPD2PI(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTTPD2PI", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);
    format.operand[0].type = Format::Operand::MMX;

    BEGIN_OPERATION() {
        DEST.i32[0] = SRC.f64[0];
        DEST.i32[1] = SRC.f64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::CVTTPS2DQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTTPS2DQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32[0] = SRC.f32[0];
        DEST.i32[1] = SRC.f32[1];
        DEST.i32[2] = SRC.f32[2];
        DEST.i32[3] = SRC.f32[3];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::CVTTSD2SI(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTTSD2SI", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);
    format.operand[0].type = Format::Operand::REG;

    BEGIN_OPERATION() {
        DEST.i32[0] = SRC.f64[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::DIVPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "DIVPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = DEST.f64[0] / SRC.f64[0];
        DEST.f64[1] = DEST.f64[1] / SRC.f64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::DIVSD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "DIVSD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = DEST.f64[0] / SRC.f64[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::LFENCE(Format& format, const uint8_t* opcode)
{
    format.instruction = "LFENCE";
    format.operand_count = 0;

    OPERATION() {};
}
//------------------------------------------------------------------------------
void sse2_instruction::MASKMOVDQU(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MASKMOVDQU", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);
    format.operand[2] = format.operand[1];
    format.operand[1] = format.operand[0];
    format.operand[0].type = Format::Operand::ADR;
    format.operand[0].base = IndexREG(EDI);

    BEGIN_OPERATION() {
        for (int i = 0; i < 16; ++i) {
            if (SRC2.i8[i] == 1) {
                DEST.i8[i] = SRC1.i8[i];
            }
        }
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::MAXPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MAXPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = std::max(DEST.f64[0], SRC.f64[0]);
        DEST.f64[1] = std::max(DEST.f64[1], SRC.f64[1]);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::MAXSD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MAXSD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = std::max(DEST.f64[0], SRC.f64[0]);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::MFENCE(Format& format, const uint8_t* opcode)
{
    format.instruction = "MFENCE";
    format.operand_count = 0;

    OPERATION() {};
}
//------------------------------------------------------------------------------
void sse2_instruction::MINPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MINPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = std::min(DEST.f64[0], SRC.f64[0]);
        DEST.f64[1] = std::min(DEST.f64[1], SRC.f64[1]);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::MINSD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MINSD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = std::min(DEST.f64[0], SRC.f64[0]);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::MOVAPD(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x28:  Decode(format, opcode, "MOVAPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);    break;
    case 0x29:  Decode(format, opcode, "MOVAPD", 2, 0, SSE_REGISTER | OPERAND_SIZE);                break;
    }

    BEGIN_OPERATION() {
        DEST = SRC;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::MOVD(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x6E:
        Decode(format, opcode, "MOVD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);
        if (format.operand[1].type == Format::Operand::SSE)
            format.operand[1].type = Format::Operand::REG;
        break;
    case 0x7E:
        Decode(format, opcode, "MOVD", 2, 0, SSE_REGISTER | OPERAND_SIZE);
        if (format.operand[0].type == Format::Operand::SSE)
            format.operand[0].type = Format::Operand::REG;
        break;
    }

    if (format.operand[0].type == Format::Operand::SSE) {
        BEGIN_OPERATION() {
            DEST.u32[0] = SRC.u32[0];
            DEST.u32[1] = 0;
            DEST.u32[2] = 0;
            DEST.u32[3] = 0;
        } END_OPERATION_SSE;
    }
    else {
        BEGIN_OPERATION() {
            DEST.u32[0] = SRC.u32[0];
        } END_OPERATION_SSE;
    }
}
//------------------------------------------------------------------------------
void sse2_instruction::MOVDQ2Q(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVDQ2Q", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);
    format.operand[0].type = Format::Operand::MMX;

    BEGIN_OPERATION() {
        DEST.i64[0] = SRC.i64[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::MOVDQA(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x6F:  Decode(format, opcode, "MOVDQA", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);    break;
    case 0x7F:  Decode(format, opcode, "MOVDQA", 2, 0, SSE_REGISTER | OPERAND_SIZE);                break;
    }

    BEGIN_OPERATION() {
        DEST = SRC;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::MOVDQU(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x6F:  Decode(format, opcode, "MOVDQU", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);    break;
    case 0x7F:  Decode(format, opcode, "MOVDQU", 2, 0, SSE_REGISTER | OPERAND_SIZE);                break;
    }

    BEGIN_OPERATION() {
        DEST = SRC;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::MOVHPD(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x16:  Decode(format, opcode, "MOVHPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);    break;
    case 0x17:  Decode(format, opcode, "MOVHPD", 2, 0, SSE_REGISTER | OPERAND_SIZE);                break;
    }

    if (format.operand[0].type == Format::Operand::ADR) {
        BEGIN_OPERATION() {
            DEST.f64[0] = SRC.f64[1];
        } END_OPERATION_SSE;
    }
    else {
        BEGIN_OPERATION() {
            DEST.f64[1] = SRC.f64[0];
        } END_OPERATION_SSE;
    }
}
//------------------------------------------------------------------------------
void sse2_instruction::MOVLPD(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x12:  Decode(format, opcode, "MOVLPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);    break;
    case 0x13:  Decode(format, opcode, "MOVLPD", 2, 0, SSE_REGISTER | OPERAND_SIZE);                break;
    }

    BEGIN_OPERATION() {
        DEST.f64[0] = SRC.f64[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::MOVMSKPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVMSKPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);
    format.operand[0].type = Format::Operand::REG;

    BEGIN_OPERATION() {
        DEST.i32[0] = 0;
        DEST.i32[0] |= SRC.i64[0] & 0x8000000000000000ull ? 1 : 0;
        DEST.i32[0] |= SRC.i64[1] & 0x8000000000000000ull ? 2 : 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::MOVNTDQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVNTDQ", 2, 0, SSE_REGISTER | OPERAND_SIZE);

    BEGIN_OPERATION() {
        DEST = SRC;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::MOVNTI(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVNTI", 2);
    format.width = 32;

    BEGIN_OPERATION() {
        DEST = SRC;
    } END_OPERATION_RANGE(32, 32);
}
//------------------------------------------------------------------------------
void sse2_instruction::MOVNTPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVNTPD", 2, 0, SSE_REGISTER | OPERAND_SIZE);

    BEGIN_OPERATION() {
        DEST = SRC;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::MOVQ(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x7E:
        Decode(format, opcode, "MOVQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);
        if (format.operand[1].type == Format::Operand::SSE)
            format.operand[1].type = Format::Operand::REG;
        break;
    case 0xD6:
        Decode(format, opcode, "MOVQ", 2, 0, SSE_REGISTER | OPERAND_SIZE);
        if (format.operand[0].type == Format::Operand::SSE)
            format.operand[0].type = Format::Operand::REG;
        break;
    }

    if (format.operand[0].type == Format::Operand::SSE) {
        BEGIN_OPERATION() {
            DEST.u64[0] = SRC.u64[0];
            DEST.u64[1] = 0;
        } END_OPERATION_SSE;
    }
    else {
        BEGIN_OPERATION() {
            DEST.u64[0] = SRC.u64[0];
        } END_OPERATION_SSE;
    }
}
//------------------------------------------------------------------------------
void sse2_instruction::MOVQ2DQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVQ2DQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);
    format.operand[1].type = Format::Operand::MMX;

    BEGIN_OPERATION() {
        DEST.i64[0] = DEST.i64[0];
        DEST.i64[1] = 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::MOVSD(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x10:  Decode(format, opcode, "MOVSD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION); break;
    case 0x11:  Decode(format, opcode, "MOVSD", 2, 0, SSE_REGISTER | OPERAND_SIZE);             break;
    }

    if (format.operand[0].type == Format::Operand::SSE && format.operand[1].type != Format::Operand::SSE) {
        BEGIN_OPERATION() {
            DEST.f64[0] = SRC.f64[0];
            DEST.f64[1] = 0;
        } END_OPERATION_SSE;
    }
    else {
        BEGIN_OPERATION() {
            DEST.f64[0] = SRC.f64[0];
        } END_OPERATION_SSE;
    }
}
//------------------------------------------------------------------------------
void sse2_instruction::MOVUPD(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x10:  Decode(format, opcode, "MOVUPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);    break;
    case 0x11:  Decode(format, opcode, "MOVUPD", 2, 0, SSE_REGISTER | OPERAND_SIZE);                break;
    }

    BEGIN_OPERATION() {
        DEST = SRC;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::MULPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MULPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = DEST.f64[0] * SRC.f64[0];
        DEST.f64[1] = DEST.f64[1] * SRC.f64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::MULSD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MULSD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = DEST.f64[0] * SRC.f64[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::ORPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "ORPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = DEST.i64[0] | SRC.i64[0];
        DEST.i64[1] = DEST.i64[1] | SRC.i64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PACKSSDW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PACKSSDW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16 = {
            int16_t(std::clamp<int32_t>(DEST.i32[0], INT16_MIN, INT16_MAX)),
            int16_t(std::clamp<int32_t>(DEST.i32[1], INT16_MIN, INT16_MAX)),
            int16_t(std::clamp<int32_t>(SRC.i32[0], INT16_MIN, INT16_MAX)),
            int16_t(std::clamp<int32_t>(SRC.i32[1], INT16_MIN, INT16_MAX)),
            int16_t(std::clamp<int32_t>(DEST.i32[2], INT16_MIN, INT16_MAX)),
            int16_t(std::clamp<int32_t>(DEST.i32[3], INT16_MIN, INT16_MAX)),
            int16_t(std::clamp<int32_t>(SRC.i32[2], INT16_MIN, INT16_MAX)),
            int16_t(std::clamp<int32_t>(SRC.i32[3], INT16_MIN, INT16_MAX)),
        };
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PACKSSWB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PACKSSWB", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

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
            int8_t(std::clamp<int16_t>(DEST.i16[4], INT8_MIN, INT8_MAX)),
            int8_t(std::clamp<int16_t>(DEST.i16[5], INT8_MIN, INT8_MAX)),
            int8_t(std::clamp<int16_t>(DEST.i16[6], INT8_MIN, INT8_MAX)),
            int8_t(std::clamp<int16_t>(DEST.i16[7], INT8_MIN, INT8_MAX)),
            int8_t(std::clamp<int16_t>(SRC.i16[4], INT8_MIN, INT8_MAX)),
            int8_t(std::clamp<int16_t>(SRC.i16[5], INT8_MIN, INT8_MAX)),
            int8_t(std::clamp<int16_t>(SRC.i16[6], INT8_MIN, INT8_MAX)),
            int8_t(std::clamp<int16_t>(SRC.i16[7], INT8_MIN, INT8_MAX)),
        };
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PACKUSWB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PACKUSWB", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

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
            uint8_t(std::clamp<int16_t>(DEST.i16[4], 0, UINT8_MAX)),
            uint8_t(std::clamp<int16_t>(DEST.i16[5], 0, UINT8_MAX)),
            uint8_t(std::clamp<int16_t>(DEST.i16[6], 0, UINT8_MAX)),
            uint8_t(std::clamp<int16_t>(DEST.i16[7], 0, UINT8_MAX)),
            uint8_t(std::clamp<int16_t>(SRC.i16[4], 0, UINT8_MAX)),
            uint8_t(std::clamp<int16_t>(SRC.i16[5], 0, UINT8_MAX)),
            uint8_t(std::clamp<int16_t>(SRC.i16[6], 0, UINT8_MAX)),
            uint8_t(std::clamp<int16_t>(SRC.i16[7], 0, UINT8_MAX)),
        };
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PADDB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PADDB", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i8[0] = DEST.i8[0] + SRC.i8[0];
        DEST.i8[1] = DEST.i8[1] + SRC.i8[1];
        DEST.i8[2] = DEST.i8[2] + SRC.i8[2];
        DEST.i8[3] = DEST.i8[3] + SRC.i8[3];
        DEST.i8[4] = DEST.i8[4] + SRC.i8[4];
        DEST.i8[5] = DEST.i8[5] + SRC.i8[5];
        DEST.i8[6] = DEST.i8[6] + SRC.i8[6];
        DEST.i8[7] = DEST.i8[7] + SRC.i8[7];
        DEST.i8[8] = DEST.i8[8] + SRC.i8[8];
        DEST.i8[9] = DEST.i8[9] + SRC.i8[9];
        DEST.i8[10] = DEST.i8[10] + SRC.i8[10];
        DEST.i8[11] = DEST.i8[11] + SRC.i8[11];
        DEST.i8[12] = DEST.i8[12] + SRC.i8[12];
        DEST.i8[13] = DEST.i8[13] + SRC.i8[13];
        DEST.i8[14] = DEST.i8[14] + SRC.i8[14];
        DEST.i8[15] = DEST.i8[15] + SRC.i8[15];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PADDW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PADDW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = DEST.i16[0] + SRC.i16[0];
        DEST.i16[1] = DEST.i16[1] + SRC.i16[1];
        DEST.i16[2] = DEST.i16[2] + SRC.i16[2];
        DEST.i16[3] = DEST.i16[3] + SRC.i16[3];
        DEST.i16[4] = DEST.i16[4] + SRC.i16[4];
        DEST.i16[5] = DEST.i16[5] + SRC.i16[5];
        DEST.i16[6] = DEST.i16[6] + SRC.i16[6];
        DEST.i16[7] = DEST.i16[7] + SRC.i16[7];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PADDD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PADDD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32[0] = DEST.i32[0] + SRC.i32[0];
        DEST.i32[1] = DEST.i32[1] + SRC.i32[1];
        DEST.i32[2] = DEST.i32[2] + SRC.i32[2];
        DEST.i32[3] = DEST.i32[3] + SRC.i32[3];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PADDQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PADDQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = DEST.i64[0] + SRC.i64[0];
        DEST.i64[1] = DEST.i64[1] + SRC.i64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PADDSB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PADDSB", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i8[0] = std::clamp<int16_t>(DEST.i8[0] + SRC.i8[0], INT8_MIN, INT8_MAX);
        DEST.i8[1] = std::clamp<int16_t>(DEST.i8[1] + SRC.i8[1], INT8_MIN, INT8_MAX);
        DEST.i8[2] = std::clamp<int16_t>(DEST.i8[2] + SRC.i8[2], INT8_MIN, INT8_MAX);
        DEST.i8[3] = std::clamp<int16_t>(DEST.i8[3] + SRC.i8[3], INT8_MIN, INT8_MAX);
        DEST.i8[4] = std::clamp<int16_t>(DEST.i8[4] + SRC.i8[4], INT8_MIN, INT8_MAX);
        DEST.i8[5] = std::clamp<int16_t>(DEST.i8[5] + SRC.i8[5], INT8_MIN, INT8_MAX);
        DEST.i8[6] = std::clamp<int16_t>(DEST.i8[6] + SRC.i8[6], INT8_MIN, INT8_MAX);
        DEST.i8[7] = std::clamp<int16_t>(DEST.i8[7] + SRC.i8[7], INT8_MIN, INT8_MAX);
        DEST.i8[8] = std::clamp<int16_t>(DEST.i8[8] + SRC.i8[8], INT8_MIN, INT8_MAX);
        DEST.i8[9] = std::clamp<int16_t>(DEST.i8[9] + SRC.i8[9], INT8_MIN, INT8_MAX);
        DEST.i8[10] = std::clamp<int16_t>(DEST.i8[10] + SRC.i8[10], INT8_MIN, INT8_MAX);
        DEST.i8[11] = std::clamp<int16_t>(DEST.i8[11] + SRC.i8[11], INT8_MIN, INT8_MAX);
        DEST.i8[12] = std::clamp<int16_t>(DEST.i8[12] + SRC.i8[12], INT8_MIN, INT8_MAX);
        DEST.i8[13] = std::clamp<int16_t>(DEST.i8[13] + SRC.i8[13], INT8_MIN, INT8_MAX);
        DEST.i8[14] = std::clamp<int16_t>(DEST.i8[14] + SRC.i8[14], INT8_MIN, INT8_MAX);
        DEST.i8[15] = std::clamp<int16_t>(DEST.i8[15] + SRC.i8[15], INT8_MIN, INT8_MAX);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PADDSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PADDSW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = std::clamp<int16_t>(DEST.i16[0] + SRC.i16[0], INT16_MIN, INT16_MAX);
        DEST.i16[1] = std::clamp<int16_t>(DEST.i16[1] + SRC.i16[1], INT16_MIN, INT16_MAX);
        DEST.i16[2] = std::clamp<int16_t>(DEST.i16[2] + SRC.i16[2], INT16_MIN, INT16_MAX);
        DEST.i16[3] = std::clamp<int16_t>(DEST.i16[3] + SRC.i16[3], INT16_MIN, INT16_MAX);
        DEST.i16[4] = std::clamp<int16_t>(DEST.i16[4] + SRC.i16[4], INT16_MIN, INT16_MAX);
        DEST.i16[5] = std::clamp<int16_t>(DEST.i16[5] + SRC.i16[5], INT16_MIN, INT16_MAX);
        DEST.i16[6] = std::clamp<int16_t>(DEST.i16[6] + SRC.i16[6], INT16_MIN, INT16_MAX);
        DEST.i16[7] = std::clamp<int16_t>(DEST.i16[7] + SRC.i16[7], INT16_MIN, INT16_MAX);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PADDUSB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PADDUSB", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u8[0] = std::clamp<int16_t>(DEST.u8[0] + SRC.u8[0], 0, UINT8_MAX);
        DEST.u8[1] = std::clamp<int16_t>(DEST.u8[1] + SRC.u8[1], 0, UINT8_MAX);
        DEST.u8[2] = std::clamp<int16_t>(DEST.u8[2] + SRC.u8[2], 0, UINT8_MAX);
        DEST.u8[3] = std::clamp<int16_t>(DEST.u8[3] + SRC.u8[3], 0, UINT8_MAX);
        DEST.u8[4] = std::clamp<int16_t>(DEST.u8[4] + SRC.u8[4], 0, UINT8_MAX);
        DEST.u8[5] = std::clamp<int16_t>(DEST.u8[5] + SRC.u8[5], 0, UINT8_MAX);
        DEST.u8[6] = std::clamp<int16_t>(DEST.u8[6] + SRC.u8[6], 0, UINT8_MAX);
        DEST.u8[7] = std::clamp<int16_t>(DEST.u8[7] + SRC.u8[7], 0, UINT8_MAX);
        DEST.u8[8] = std::clamp<int16_t>(DEST.u8[8] + SRC.u8[8], 0, UINT8_MAX);
        DEST.u8[9] = std::clamp<int16_t>(DEST.u8[9] + SRC.u8[9], 0, UINT8_MAX);
        DEST.u8[10] = std::clamp<int16_t>(DEST.u8[10] + SRC.u8[10], 0, UINT8_MAX);
        DEST.u8[11] = std::clamp<int16_t>(DEST.u8[11] + SRC.u8[11], 0, UINT8_MAX);
        DEST.u8[12] = std::clamp<int16_t>(DEST.u8[12] + SRC.u8[12], 0, UINT8_MAX);
        DEST.u8[13] = std::clamp<int16_t>(DEST.u8[13] + SRC.u8[13], 0, UINT8_MAX);
        DEST.u8[14] = std::clamp<int16_t>(DEST.u8[14] + SRC.u8[14], 0, UINT8_MAX);
        DEST.u8[15] = std::clamp<int16_t>(DEST.u8[15] + SRC.u8[15], 0, UINT8_MAX);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PADDUSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PADDUSW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u16[0] = std::clamp<int32_t>(DEST.u16[0] + SRC.u16[0], 0, UINT16_MAX);
        DEST.u16[1] = std::clamp<int32_t>(DEST.u16[1] + SRC.u16[1], 0, UINT16_MAX);
        DEST.u16[2] = std::clamp<int32_t>(DEST.u16[2] + SRC.u16[2], 0, UINT16_MAX);
        DEST.u16[3] = std::clamp<int32_t>(DEST.u16[3] + SRC.u16[3], 0, UINT16_MAX);
        DEST.u16[4] = std::clamp<int32_t>(DEST.u16[4] + SRC.u16[4], 0, UINT16_MAX);
        DEST.u16[5] = std::clamp<int32_t>(DEST.u16[5] + SRC.u16[5], 0, UINT16_MAX);
        DEST.u16[6] = std::clamp<int32_t>(DEST.u16[6] + SRC.u16[6], 0, UINT16_MAX);
        DEST.u16[7] = std::clamp<int32_t>(DEST.u16[7] + SRC.u16[7], 0, UINT16_MAX);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PAND(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PAND", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = DEST.i64[0] & SRC.i64[0];
        DEST.i64[1] = DEST.i64[1] & SRC.i64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PANDN(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PANDN", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = ~DEST.i64[0] & SRC.i64[0];
        DEST.i64[1] = ~DEST.i64[1] & SRC.i64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PAUSE(Format& format, const uint8_t* opcode)
{
    format.instruction = "PAUSE";
    format.operand_count = 0;

    OPERATION() {};
}
//------------------------------------------------------------------------------
void sse2_instruction::PAVGB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PAVGB", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u8[0] = (DEST.u8[0] + SRC.u8[0]) / 2;
        DEST.u8[1] = (DEST.u8[1] + SRC.u8[1]) / 2;
        DEST.u8[2] = (DEST.u8[2] + SRC.u8[2]) / 2;
        DEST.u8[3] = (DEST.u8[3] + SRC.u8[3]) / 2;
        DEST.u8[4] = (DEST.u8[4] + SRC.u8[4]) / 2;
        DEST.u8[5] = (DEST.u8[5] + SRC.u8[5]) / 2;
        DEST.u8[6] = (DEST.u8[6] + SRC.u8[6]) / 2;
        DEST.u8[7] = (DEST.u8[7] + SRC.u8[7]) / 2;
        DEST.u8[8] = (DEST.u8[8] + SRC.u8[8]) / 2;
        DEST.u8[9] = (DEST.u8[9] + SRC.u8[9]) / 2;
        DEST.u8[10] = (DEST.u8[10] + SRC.u8[10]) / 2;
        DEST.u8[11] = (DEST.u8[11] + SRC.u8[11]) / 2;
        DEST.u8[12] = (DEST.u8[12] + SRC.u8[12]) / 2;
        DEST.u8[13] = (DEST.u8[13] + SRC.u8[13]) / 2;
        DEST.u8[14] = (DEST.u8[14] + SRC.u8[14]) / 2;
        DEST.u8[15] = (DEST.u8[15] + SRC.u8[15]) / 2;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PAVGW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PAVGW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u16[0] = (DEST.u16[0] + SRC.u16[0]) / 2;
        DEST.u16[1] = (DEST.u16[1] + SRC.u16[1]) / 2;
        DEST.u16[2] = (DEST.u16[2] + SRC.u16[2]) / 2;
        DEST.u16[3] = (DEST.u16[3] + SRC.u16[3]) / 2;
        DEST.u16[4] = (DEST.u16[4] + SRC.u16[4]) / 2;
        DEST.u16[5] = (DEST.u16[5] + SRC.u16[5]) / 2;
        DEST.u16[6] = (DEST.u16[6] + SRC.u16[6]) / 2;
        DEST.u16[7] = (DEST.u16[7] + SRC.u16[7]) / 2;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PCMPEQB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PCMPEQB", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i8[0] = (DEST.i8[0] == SRC.i8[0]) ? -1 : 0;
        DEST.i8[1] = (DEST.i8[1] == SRC.i8[1]) ? -1 : 0;
        DEST.i8[2] = (DEST.i8[2] == SRC.i8[2]) ? -1 : 0;
        DEST.i8[3] = (DEST.i8[3] == SRC.i8[3]) ? -1 : 0;
        DEST.i8[4] = (DEST.i8[4] == SRC.i8[4]) ? -1 : 0;
        DEST.i8[5] = (DEST.i8[5] == SRC.i8[5]) ? -1 : 0;
        DEST.i8[6] = (DEST.i8[6] == SRC.i8[6]) ? -1 : 0;
        DEST.i8[7] = (DEST.i8[7] == SRC.i8[7]) ? -1 : 0;
        DEST.i8[8] = (DEST.i8[8] == SRC.i8[8]) ? -1 : 0;
        DEST.i8[9] = (DEST.i8[9] == SRC.i8[9]) ? -1 : 0;
        DEST.i8[10] = (DEST.i8[10] == SRC.i8[10]) ? -1 : 0;
        DEST.i8[11] = (DEST.i8[11] == SRC.i8[11]) ? -1 : 0;
        DEST.i8[12] = (DEST.i8[12] == SRC.i8[12]) ? -1 : 0;
        DEST.i8[13] = (DEST.i8[13] == SRC.i8[13]) ? -1 : 0;
        DEST.i8[14] = (DEST.i8[14] == SRC.i8[14]) ? -1 : 0;
        DEST.i8[15] = (DEST.i8[15] == SRC.i8[15]) ? -1 : 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PCMPEQW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PCMPEQW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = (DEST.i16[0] == SRC.i16[0]) ? -1 : 0;
        DEST.i16[1] = (DEST.i16[1] == SRC.i16[1]) ? -1 : 0;
        DEST.i16[2] = (DEST.i16[2] == SRC.i16[2]) ? -1 : 0;
        DEST.i16[3] = (DEST.i16[3] == SRC.i16[3]) ? -1 : 0;
        DEST.i16[4] = (DEST.i16[4] == SRC.i16[4]) ? -1 : 0;
        DEST.i16[5] = (DEST.i16[5] == SRC.i16[5]) ? -1 : 0;
        DEST.i16[6] = (DEST.i16[6] == SRC.i16[6]) ? -1 : 0;
        DEST.i16[7] = (DEST.i16[7] == SRC.i16[7]) ? -1 : 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PCMPEQD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PCMPEQD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32[0] = (DEST.i32[0] == SRC.i32[0]) ? -1 : 0;
        DEST.i32[1] = (DEST.i32[1] == SRC.i32[1]) ? -1 : 0;
        DEST.i32[2] = (DEST.i32[2] == SRC.i32[2]) ? -1 : 0;
        DEST.i32[3] = (DEST.i32[3] == SRC.i32[3]) ? -1 : 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PCMPGTB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PCMPGTB", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i8[0] = (DEST.i8[0] > SRC.i8[0]) ? -1 : 0;
        DEST.i8[1] = (DEST.i8[1] > SRC.i8[1]) ? -1 : 0;
        DEST.i8[2] = (DEST.i8[2] > SRC.i8[2]) ? -1 : 0;
        DEST.i8[3] = (DEST.i8[3] > SRC.i8[3]) ? -1 : 0;
        DEST.i8[4] = (DEST.i8[4] > SRC.i8[4]) ? -1 : 0;
        DEST.i8[5] = (DEST.i8[5] > SRC.i8[5]) ? -1 : 0;
        DEST.i8[6] = (DEST.i8[6] > SRC.i8[6]) ? -1 : 0;
        DEST.i8[7] = (DEST.i8[7] > SRC.i8[7]) ? -1 : 0;
        DEST.i8[8] = (DEST.i8[8] > SRC.i8[8]) ? -1 : 0;
        DEST.i8[9] = (DEST.i8[9] > SRC.i8[9]) ? -1 : 0;
        DEST.i8[10] = (DEST.i8[10] > SRC.i8[10]) ? -1 : 0;
        DEST.i8[11] = (DEST.i8[11] > SRC.i8[11]) ? -1 : 0;
        DEST.i8[12] = (DEST.i8[12] > SRC.i8[12]) ? -1 : 0;
        DEST.i8[13] = (DEST.i8[13] > SRC.i8[13]) ? -1 : 0;
        DEST.i8[14] = (DEST.i8[14] > SRC.i8[14]) ? -1 : 0;
        DEST.i8[15] = (DEST.i8[15] > SRC.i8[15]) ? -1 : 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PCMPGTW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PCMPGTW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = (DEST.i16[0] > SRC.i16[0]) ? -1 : 0;
        DEST.i16[1] = (DEST.i16[1] > SRC.i16[1]) ? -1 : 0;
        DEST.i16[2] = (DEST.i16[2] > SRC.i16[2]) ? -1 : 0;
        DEST.i16[3] = (DEST.i16[3] > SRC.i16[3]) ? -1 : 0;
        DEST.i16[4] = (DEST.i16[4] > SRC.i16[4]) ? -1 : 0;
        DEST.i16[5] = (DEST.i16[5] > SRC.i16[5]) ? -1 : 0;
        DEST.i16[6] = (DEST.i16[6] > SRC.i16[6]) ? -1 : 0;
        DEST.i16[7] = (DEST.i16[7] > SRC.i16[7]) ? -1 : 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PCMPGTD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PCMPGTD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32[0] = (DEST.i32[0] > SRC.i32[0]) ? -1 : 0;
        DEST.i32[1] = (DEST.i32[1] > SRC.i32[1]) ? -1 : 0;
        DEST.i32[2] = (DEST.i32[2] > SRC.i32[2]) ? -1 : 0;
        DEST.i32[3] = (DEST.i32[3] > SRC.i32[3]) ? -1 : 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PEXTRW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PEXTRW", 2, 8, SSE_REGISTER | OPERAND_SIZE | DIRECTION | THREE_OPERAND);
    format.operand[0].type = Format::Operand::REG;

    BEGIN_OPERATION() {
        auto SEL = SRC2.u8[0] % 8;
        DEST.u32[0] = SRC.u16[SEL];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PINSRW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PINSRW", 2, 8, SSE_REGISTER | OPERAND_SIZE | DIRECTION | THREE_OPERAND);
    if (format.operand[1].type == Format::Operand::SSE)
        format.operand[1].type = Format::Operand::REG;

    BEGIN_OPERATION() {
        auto SEL = SRC2.u8[0] % 8;
        DEST.u16[SEL] = SRC.u16[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PMADDWD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMADDWD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32[0] = DEST.i16[0] * SRC.i16[0] + DEST.i16[1] * SRC.i16[1];
        DEST.i32[1] = DEST.i16[2] * SRC.i16[2] + DEST.i16[3] * SRC.i16[3];
        DEST.i32[2] = DEST.i16[4] * SRC.i16[4] + DEST.i16[5] * SRC.i16[5];
        DEST.i32[3] = DEST.i16[6] * SRC.i16[6] + DEST.i16[7] * SRC.i16[7];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PMAXSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMAXSW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = std::max(DEST.i16[0], SRC.i16[0]);
        DEST.i16[1] = std::max(DEST.i16[1], SRC.i16[1]);
        DEST.i16[2] = std::max(DEST.i16[2], SRC.i16[2]);
        DEST.i16[3] = std::max(DEST.i16[3], SRC.i16[3]);
        DEST.i16[4] = std::max(DEST.i16[4], SRC.i16[4]);
        DEST.i16[5] = std::max(DEST.i16[5], SRC.i16[5]);
        DEST.i16[6] = std::max(DEST.i16[6], SRC.i16[6]);
        DEST.i16[7] = std::max(DEST.i16[7], SRC.i16[7]);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PMAXUB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMAXUB", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u8[0] = std::max(DEST.u8[0], SRC.u8[0]);
        DEST.u8[1] = std::max(DEST.u8[1], SRC.u8[1]);
        DEST.u8[2] = std::max(DEST.u8[2], SRC.u8[2]);
        DEST.u8[3] = std::max(DEST.u8[3], SRC.u8[3]);
        DEST.u8[4] = std::max(DEST.u8[4], SRC.u8[4]);
        DEST.u8[5] = std::max(DEST.u8[5], SRC.u8[5]);
        DEST.u8[6] = std::max(DEST.u8[6], SRC.u8[6]);
        DEST.u8[7] = std::max(DEST.u8[7], SRC.u8[7]);
        DEST.u8[8] = std::max(DEST.u8[8], SRC.u8[8]);
        DEST.u8[9] = std::max(DEST.u8[9], SRC.u8[9]);
        DEST.u8[10] = std::max(DEST.u8[10], SRC.u8[10]);
        DEST.u8[11] = std::max(DEST.u8[11], SRC.u8[11]);
        DEST.u8[12] = std::max(DEST.u8[12], SRC.u8[12]);
        DEST.u8[13] = std::max(DEST.u8[13], SRC.u8[13]);
        DEST.u8[14] = std::max(DEST.u8[14], SRC.u8[14]);
        DEST.u8[15] = std::max(DEST.u8[15], SRC.u8[15]);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PMINSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMINSW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = std::min(DEST.i16[0], SRC.i16[0]);
        DEST.i16[1] = std::min(DEST.i16[1], SRC.i16[1]);
        DEST.i16[2] = std::min(DEST.i16[2], SRC.i16[2]);
        DEST.i16[3] = std::min(DEST.i16[3], SRC.i16[3]);
        DEST.i16[4] = std::min(DEST.i16[4], SRC.i16[4]);
        DEST.i16[5] = std::min(DEST.i16[5], SRC.i16[5]);
        DEST.i16[6] = std::min(DEST.i16[6], SRC.i16[6]);
        DEST.i16[7] = std::min(DEST.i16[7], SRC.i16[7]);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PMINUB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMINUB", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u8[0] = std::min(DEST.u8[0], SRC.u8[0]);
        DEST.u8[1] = std::min(DEST.u8[1], SRC.u8[1]);
        DEST.u8[2] = std::min(DEST.u8[2], SRC.u8[2]);
        DEST.u8[3] = std::min(DEST.u8[3], SRC.u8[3]);
        DEST.u8[4] = std::min(DEST.u8[4], SRC.u8[4]);
        DEST.u8[5] = std::min(DEST.u8[5], SRC.u8[5]);
        DEST.u8[6] = std::min(DEST.u8[6], SRC.u8[6]);
        DEST.u8[7] = std::min(DEST.u8[7], SRC.u8[7]);
        DEST.u8[8] = std::min(DEST.u8[8], SRC.u8[8]);
        DEST.u8[9] = std::min(DEST.u8[9], SRC.u8[9]);
        DEST.u8[10] = std::min(DEST.u8[10], SRC.u8[10]);
        DEST.u8[11] = std::min(DEST.u8[11], SRC.u8[11]);
        DEST.u8[12] = std::min(DEST.u8[12], SRC.u8[12]);
        DEST.u8[13] = std::min(DEST.u8[13], SRC.u8[13]);
        DEST.u8[14] = std::min(DEST.u8[14], SRC.u8[14]);
        DEST.u8[15] = std::min(DEST.u8[15], SRC.u8[15]);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PMOVMSKB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMOVMSKB", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u32[0] = 0;
        DEST.u32[0] |= SRC.u8[0] & 0x80 ? 0x0001 : 0;
        DEST.u32[0] |= SRC.u8[1] & 0x80 ? 0x0002 : 0;
        DEST.u32[0] |= SRC.u8[2] & 0x80 ? 0x0004 : 0;
        DEST.u32[0] |= SRC.u8[3] & 0x80 ? 0x0008 : 0;
        DEST.u32[0] |= SRC.u8[4] & 0x80 ? 0x0010 : 0;
        DEST.u32[0] |= SRC.u8[5] & 0x80 ? 0x0020 : 0;
        DEST.u32[0] |= SRC.u8[6] & 0x80 ? 0x0040 : 0;
        DEST.u32[0] |= SRC.u8[7] & 0x80 ? 0x0080 : 0;
        DEST.u32[0] |= SRC.u8[8] & 0x80 ? 0x0100 : 0;
        DEST.u32[0] |= SRC.u8[9] & 0x80 ? 0x0200 : 0;
        DEST.u32[0] |= SRC.u8[10] & 0x80 ? 0x0400 : 0;
        DEST.u32[0] |= SRC.u8[11] & 0x80 ? 0x0800 : 0;
        DEST.u32[0] |= SRC.u8[12] & 0x80 ? 0x1000 : 0;
        DEST.u32[0] |= SRC.u8[13] & 0x80 ? 0x2000 : 0;
        DEST.u32[0] |= SRC.u8[14] & 0x80 ? 0x4000 : 0;
        DEST.u32[0] |= SRC.u8[15] & 0x80 ? 0x8000 : 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PMULHUW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMULHUW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u16[0] = (DEST.u16[0] * SRC.u16[0]) >> 16;
        DEST.u16[1] = (DEST.u16[1] * SRC.u16[1]) >> 16;
        DEST.u16[2] = (DEST.u16[2] * SRC.u16[2]) >> 16;
        DEST.u16[3] = (DEST.u16[3] * SRC.u16[3]) >> 16;
        DEST.u16[4] = (DEST.u16[4] * SRC.u16[4]) >> 16;
        DEST.u16[5] = (DEST.u16[5] * SRC.u16[5]) >> 16;
        DEST.u16[6] = (DEST.u16[6] * SRC.u16[6]) >> 16;
        DEST.u16[7] = (DEST.u16[7] * SRC.u16[7]) >> 16;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PMULHW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMULHW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = (DEST.i16[0] * SRC.i16[0]) >> 16;
        DEST.i16[1] = (DEST.i16[1] * SRC.i16[1]) >> 16;
        DEST.i16[2] = (DEST.i16[2] * SRC.i16[2]) >> 16;
        DEST.i16[3] = (DEST.i16[3] * SRC.i16[3]) >> 16;
        DEST.i16[4] = (DEST.i16[4] * SRC.i16[4]) >> 16;
        DEST.i16[5] = (DEST.i16[5] * SRC.i16[5]) >> 16;
        DEST.i16[6] = (DEST.i16[6] * SRC.i16[6]) >> 16;
        DEST.i16[7] = (DEST.i16[7] * SRC.i16[7]) >> 16;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PMULLW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMULLW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = DEST.i16[0] * SRC.i16[0];
        DEST.i16[1] = DEST.i16[1] * SRC.i16[1];
        DEST.i16[2] = DEST.i16[2] * SRC.i16[2];
        DEST.i16[3] = DEST.i16[3] * SRC.i16[3];
        DEST.i16[4] = DEST.i16[4] * SRC.i16[4];
        DEST.i16[5] = DEST.i16[5] * SRC.i16[5];
        DEST.i16[6] = DEST.i16[6] * SRC.i16[6];
        DEST.i16[7] = DEST.i16[7] * SRC.i16[7];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PMULUDQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMULUDQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u64[0] = DEST.u32[0] * SRC.u32[0];
        DEST.u64[1] = DEST.u32[2] * SRC.u32[2];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::POR(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "POR", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32[0] = DEST.i32[0] | SRC.i32[0];
        DEST.i32[1] = DEST.i32[1] | SRC.i32[1];
        DEST.i32[2] = DEST.i32[2] | SRC.i32[2];
        DEST.i32[3] = DEST.i32[3] | SRC.i32[3];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PSADBW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSADBW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

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
        DEST.u16[4] = 0;
        DEST.u16[4] += abs(DEST.u8[8] - SRC.u8[8]);
        DEST.u16[4] += abs(DEST.u8[9] - SRC.u8[9]);
        DEST.u16[4] += abs(DEST.u8[10] - SRC.u8[10]);
        DEST.u16[4] += abs(DEST.u8[11] - SRC.u8[11]);
        DEST.u16[4] += abs(DEST.u8[12] - SRC.u8[12]);
        DEST.u16[4] += abs(DEST.u8[13] - SRC.u8[13]);
        DEST.u16[4] += abs(DEST.u8[14] - SRC.u8[14]);
        DEST.u16[4] += abs(DEST.u8[15] - SRC.u8[15]);
        DEST.u16[5] = 0;
        DEST.u16[6] = 0;
        DEST.u16[7] = 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PSHUFD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSHUFD", 2, 8, SSE_REGISTER | OPERAND_SIZE | DIRECTION | THREE_OPERAND);

    BEGIN_OPERATION() {
        auto SEL = SRC2.u8[0];
        DEST.i32[0] = SRC.i32[(SEL >> 0) & 0x3];
        DEST.i32[1] = SRC.i32[(SEL >> 2) & 0x3];
        DEST.i32[2] = SRC.i32[(SEL >> 4) & 0x3];
        DEST.i32[3] = SRC.i32[(SEL >> 6) & 0x3];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PSHUFHW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSHUFHW", 2, 8, SSE_REGISTER | OPERAND_SIZE | DIRECTION | THREE_OPERAND);

    BEGIN_OPERATION() {
        auto SEL = SRC2.u8[0];
        DEST.i16[4] = SRC.i16[(SEL >> 0) & 0x3 + 4];
        DEST.i16[5] = SRC.i16[(SEL >> 2) & 0x3 + 4];
        DEST.i16[6] = SRC.i16[(SEL >> 4) & 0x3 + 4];
        DEST.i16[7] = SRC.i16[(SEL >> 6) & 0x3 + 4];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PSHUFLW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSHUFLW", 2, 8, SSE_REGISTER | OPERAND_SIZE | DIRECTION | THREE_OPERAND);

    BEGIN_OPERATION() {
        auto SEL = SRC2.u8[0];
        DEST.i16[0] = SRC.i16[(SEL >> 0) & 0x3];
        DEST.i16[1] = SRC.i16[(SEL >> 2) & 0x3];
        DEST.i16[2] = SRC.i16[(SEL >> 4) & 0x3];
        DEST.i16[3] = SRC.i16[(SEL >> 6) & 0x3];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PSLLW(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x71:  Decode(format, opcode, "PSLLW", 2, 8, SSE_REGISTER | OPERAND_SIZE);             break;
    case 0xF1:  Decode(format, opcode, "PSLLW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION); break;
    }

    BEGIN_OPERATION() {
        auto COUNT = SRC.u8[0];
        DEST.i16[0] = COUNT < 16 ? DEST.i16[0] << COUNT : 0;
        DEST.i16[1] = COUNT < 16 ? DEST.i16[1] << COUNT : 0;
        DEST.i16[2] = COUNT < 16 ? DEST.i16[2] << COUNT : 0;
        DEST.i16[3] = COUNT < 16 ? DEST.i16[3] << COUNT : 0;
        DEST.i16[4] = COUNT < 16 ? DEST.i16[4] << COUNT : 0;
        DEST.i16[5] = COUNT < 16 ? DEST.i16[5] << COUNT : 0;
        DEST.i16[6] = COUNT < 16 ? DEST.i16[6] << COUNT : 0;
        DEST.i16[7] = COUNT < 16 ? DEST.i16[7] << COUNT : 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PSLLD(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x72:  Decode(format, opcode, "PSLLD", 2, 8, SSE_REGISTER | OPERAND_SIZE);             break;
    case 0xF2:  Decode(format, opcode, "PSLLD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION); break;
    }

    BEGIN_OPERATION() {
        auto COUNT = SRC.u8[0];
        DEST.i32[0] = COUNT < 32 ? DEST.i32[0] << COUNT : 0;
        DEST.i32[1] = COUNT < 32 ? DEST.i32[1] << COUNT : 0;
        DEST.i32[2] = COUNT < 32 ? DEST.i32[2] << COUNT : 0;
        DEST.i32[3] = COUNT < 32 ? DEST.i32[3] << COUNT : 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PSLLQ(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x73:  Decode(format, opcode, "PSLLQ", 2, 8, SSE_REGISTER | OPERAND_SIZE);             break;
    case 0xF3:  Decode(format, opcode, "PSLLQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION); break;
    }

    BEGIN_OPERATION() {
        auto COUNT = SRC.u8[0];
        DEST.i64[0] = COUNT < 64 ? DEST.i64[0] << COUNT : 0;
        DEST.i64[1] = COUNT < 64 ? DEST.i64[1] << COUNT : 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PSLLDQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSLLDQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        auto COUNT = SRC.u8[0];
        if (COUNT >= 16) {
            for (int i = 0; i < 16; ++i) {
                DEST.i8[i] = 0;
            }
        }
        else {
            for (int i = 15; i >= 0; --i) {
                int offset = i - COUNT;
                DEST.i8[i] = (offset >= 0) ? DEST.i8[offset] : 0;
            }
        }
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PSRAW(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x71:  Decode(format, opcode, "PSRAW", 2, 8, SSE_REGISTER | OPERAND_SIZE);             break;
    case 0xE1:  Decode(format, opcode, "PSRAW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION); break;
    }

    BEGIN_OPERATION() {
        auto COUNT = SRC.u8[0];
        DEST.i16[0] = COUNT < 16 ? DEST.i16[0] >> COUNT : 0;
        DEST.i16[1] = COUNT < 16 ? DEST.i16[1] >> COUNT : 0;
        DEST.i16[2] = COUNT < 16 ? DEST.i16[2] >> COUNT : 0;
        DEST.i16[3] = COUNT < 16 ? DEST.i16[3] >> COUNT : 0;
        DEST.i16[4] = COUNT < 16 ? DEST.i16[4] >> COUNT : 0;
        DEST.i16[5] = COUNT < 16 ? DEST.i16[5] >> COUNT : 0;
        DEST.i16[6] = COUNT < 16 ? DEST.i16[6] >> COUNT : 0;
        DEST.i16[7] = COUNT < 16 ? DEST.i16[7] >> COUNT : 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PSRAD(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x72:  Decode(format, opcode, "PSRAD", 2, 8, SSE_REGISTER | OPERAND_SIZE);             break;
    case 0xE2:  Decode(format, opcode, "PSRAD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION); break;
    }

    BEGIN_OPERATION() {
        auto COUNT = SRC.u8[0];
        DEST.i32[0] = COUNT < 32 ? DEST.i32[0] >> COUNT : 0;
        DEST.i32[1] = COUNT < 32 ? DEST.i32[1] >> COUNT : 0;
        DEST.i32[2] = COUNT < 32 ? DEST.i32[2] >> COUNT : 0;
        DEST.i32[3] = COUNT < 32 ? DEST.i32[3] >> COUNT : 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PSRLW(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x71:  Decode(format, opcode, "PSRLW", 2, 8, SSE_REGISTER | OPERAND_SIZE);             break;
    case 0xD1:  Decode(format, opcode, "PSRLW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION); break;
    }

    BEGIN_OPERATION() {
        auto COUNT = SRC.u8[0];
        DEST.u16[0] = COUNT < 16 ? DEST.u16[0] >> COUNT : 0;
        DEST.u16[1] = COUNT < 16 ? DEST.u16[1] >> COUNT : 0;
        DEST.u16[2] = COUNT < 16 ? DEST.u16[2] >> COUNT : 0;
        DEST.u16[3] = COUNT < 16 ? DEST.u16[3] >> COUNT : 0;
        DEST.u16[4] = COUNT < 16 ? DEST.u16[4] >> COUNT : 0;
        DEST.u16[5] = COUNT < 16 ? DEST.u16[5] >> COUNT : 0;
        DEST.u16[6] = COUNT < 16 ? DEST.u16[6] >> COUNT : 0;
        DEST.u16[7] = COUNT < 16 ? DEST.u16[7] >> COUNT : 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PSRLD(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x72:  Decode(format, opcode, "PSRLD", 2, 8, SSE_REGISTER | OPERAND_SIZE);             break;
    case 0xD2:  Decode(format, opcode, "PSRLD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION); break;
    }

    BEGIN_OPERATION() {
        auto COUNT = SRC.u8[0];
        DEST.u32[0] = COUNT < 32 ? DEST.u32[0] >> COUNT : 0;
        DEST.u32[1] = COUNT < 32 ? DEST.u32[1] >> COUNT : 0;
        DEST.u32[2] = COUNT < 32 ? DEST.u32[2] >> COUNT : 0;
        DEST.u32[3] = COUNT < 32 ? DEST.u32[3] >> COUNT : 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PSRLQ(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x73:  Decode(format, opcode, "PSRLQ", 2, 8, SSE_REGISTER | OPERAND_SIZE);             break;
    case 0xD3:  Decode(format, opcode, "PSRLQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION); break;
    }

    BEGIN_OPERATION() {
        auto COUNT = SRC.u8[0];
        DEST.u64[0] = COUNT < 64 ? DEST.u64[0] >> COUNT : 0;
        DEST.u64[1] = COUNT < 64 ? DEST.u64[1] >> COUNT : 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PSRLDQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSRLDQ", 2, 8, SSE_REGISTER | OPERAND_SIZE);

    BEGIN_OPERATION() {
        auto COUNT = SRC.u8[0];
        if (COUNT >= 16) {
            for (int i = 0; i < 16; ++i) {
                DEST.i8[i] = 0;
            }
        }
        else {
            for (int i = 0; i < 16; ++i) {
                int src_index = i + COUNT;
                DEST.i8[i] = (src_index < 16) ? DEST.i8[src_index] : 0;
            }
        }
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PSUBB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSUBB", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i8[0] = DEST.i8[0] - SRC.i8[0];
        DEST.i8[1] = DEST.i8[1] - SRC.i8[1];
        DEST.i8[2] = DEST.i8[2] - SRC.i8[2];
        DEST.i8[3] = DEST.i8[3] - SRC.i8[3];
        DEST.i8[4] = DEST.i8[4] - SRC.i8[4];
        DEST.i8[5] = DEST.i8[5] - SRC.i8[5];
        DEST.i8[6] = DEST.i8[6] - SRC.i8[6];
        DEST.i8[7] = DEST.i8[7] - SRC.i8[7];
        DEST.i8[8] = DEST.i8[8] - SRC.i8[8];
        DEST.i8[9] = DEST.i8[9] - SRC.i8[9];
        DEST.i8[10] = DEST.i8[10] - SRC.i8[10];
        DEST.i8[11] = DEST.i8[11] - SRC.i8[11];
        DEST.i8[12] = DEST.i8[12] - SRC.i8[12];
        DEST.i8[13] = DEST.i8[13] - SRC.i8[13];
        DEST.i8[14] = DEST.i8[14] - SRC.i8[14];
        DEST.i8[15] = DEST.i8[15] - SRC.i8[15];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PSUBW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSUBW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = DEST.i16[0] - SRC.i16[0];
        DEST.i16[1] = DEST.i16[1] - SRC.i16[1];
        DEST.i16[2] = DEST.i16[2] - SRC.i16[2];
        DEST.i16[3] = DEST.i16[3] - SRC.i16[3];
        DEST.i16[4] = DEST.i16[4] - SRC.i16[4];
        DEST.i16[5] = DEST.i16[5] - SRC.i16[5];
        DEST.i16[6] = DEST.i16[6] - SRC.i16[6];
        DEST.i16[7] = DEST.i16[7] - SRC.i16[7];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PSUBD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSUBD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32[0] = DEST.i32[0] - SRC.i32[0];
        DEST.i32[1] = DEST.i32[1] - SRC.i32[1];
        DEST.i32[2] = DEST.i32[2] - SRC.i32[2];
        DEST.i32[3] = DEST.i32[3] - SRC.i32[3];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PSUBQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSUBQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = DEST.i64[0] - SRC.i64[0];
        DEST.i64[1] = DEST.i64[1] - SRC.i64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PSUBSB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSUBSB", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i8[0] = std::clamp<int16_t>(DEST.i8[0] - SRC.i8[0], INT8_MIN, INT8_MAX);
        DEST.i8[1] = std::clamp<int16_t>(DEST.i8[1] - SRC.i8[1], INT8_MIN, INT8_MAX);
        DEST.i8[2] = std::clamp<int16_t>(DEST.i8[2] - SRC.i8[2], INT8_MIN, INT8_MAX);
        DEST.i8[3] = std::clamp<int16_t>(DEST.i8[3] - SRC.i8[3], INT8_MIN, INT8_MAX);
        DEST.i8[4] = std::clamp<int16_t>(DEST.i8[4] - SRC.i8[4], INT8_MIN, INT8_MAX);
        DEST.i8[5] = std::clamp<int16_t>(DEST.i8[5] - SRC.i8[5], INT8_MIN, INT8_MAX);
        DEST.i8[6] = std::clamp<int16_t>(DEST.i8[6] - SRC.i8[6], INT8_MIN, INT8_MAX);
        DEST.i8[7] = std::clamp<int16_t>(DEST.i8[7] - SRC.i8[7], INT8_MIN, INT8_MAX);
        DEST.i8[8] = std::clamp<int16_t>(DEST.i8[8] - SRC.i8[8], INT8_MIN, INT8_MAX);
        DEST.i8[9] = std::clamp<int16_t>(DEST.i8[9] - SRC.i8[9], INT8_MIN, INT8_MAX);
        DEST.i8[10] = std::clamp<int16_t>(DEST.i8[10] - SRC.i8[10], INT8_MIN, INT8_MAX);
        DEST.i8[11] = std::clamp<int16_t>(DEST.i8[11] - SRC.i8[11], INT8_MIN, INT8_MAX);
        DEST.i8[12] = std::clamp<int16_t>(DEST.i8[12] - SRC.i8[12], INT8_MIN, INT8_MAX);
        DEST.i8[13] = std::clamp<int16_t>(DEST.i8[13] - SRC.i8[13], INT8_MIN, INT8_MAX);
        DEST.i8[14] = std::clamp<int16_t>(DEST.i8[14] - SRC.i8[14], INT8_MIN, INT8_MAX);
        DEST.i8[15] = std::clamp<int16_t>(DEST.i8[15] - SRC.i8[15], INT8_MIN, INT8_MAX);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PSUBSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSUBSW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = std::clamp<int32_t>(DEST.i16[0] - SRC.i16[0], INT16_MIN, INT16_MAX);
        DEST.i16[1] = std::clamp<int32_t>(DEST.i16[1] - SRC.i16[1], INT16_MIN, INT16_MAX);
        DEST.i16[2] = std::clamp<int32_t>(DEST.i16[2] - SRC.i16[2], INT16_MIN, INT16_MAX);
        DEST.i16[3] = std::clamp<int32_t>(DEST.i16[3] - SRC.i16[3], INT16_MIN, INT16_MAX);
        DEST.i16[4] = std::clamp<int32_t>(DEST.i16[4] - SRC.i16[4], INT16_MIN, INT16_MAX);
        DEST.i16[5] = std::clamp<int32_t>(DEST.i16[5] - SRC.i16[5], INT16_MIN, INT16_MAX);
        DEST.i16[6] = std::clamp<int32_t>(DEST.i16[6] - SRC.i16[6], INT16_MIN, INT16_MAX);
        DEST.i16[7] = std::clamp<int32_t>(DEST.i16[7] - SRC.i16[7], INT16_MIN, INT16_MAX);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PSUBUSB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSUBUSB", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u8[0] = std::clamp<int16_t>(DEST.u8[0] - SRC.u8[0], 0, UINT8_MAX);
        DEST.u8[1] = std::clamp<int16_t>(DEST.u8[1] - SRC.u8[1], 0, UINT8_MAX);
        DEST.u8[2] = std::clamp<int16_t>(DEST.u8[2] - SRC.u8[2], 0, UINT8_MAX);
        DEST.u8[3] = std::clamp<int16_t>(DEST.u8[3] - SRC.u8[3], 0, UINT8_MAX);
        DEST.u8[4] = std::clamp<int16_t>(DEST.u8[4] - SRC.u8[4], 0, UINT8_MAX);
        DEST.u8[5] = std::clamp<int16_t>(DEST.u8[5] - SRC.u8[5], 0, UINT8_MAX);
        DEST.u8[6] = std::clamp<int16_t>(DEST.u8[6] - SRC.u8[6], 0, UINT8_MAX);
        DEST.u8[7] = std::clamp<int16_t>(DEST.u8[7] - SRC.u8[7], 0, UINT8_MAX);
        DEST.u8[8] = std::clamp<int16_t>(DEST.u8[8] - SRC.u8[8], 0, UINT8_MAX);
        DEST.u8[9] = std::clamp<int16_t>(DEST.u8[9] - SRC.u8[9], 0, UINT8_MAX);
        DEST.u8[10] = std::clamp<int16_t>(DEST.u8[10] - SRC.u8[10], 0, UINT8_MAX);
        DEST.u8[11] = std::clamp<int16_t>(DEST.u8[11] - SRC.u8[11], 0, UINT8_MAX);
        DEST.u8[12] = std::clamp<int16_t>(DEST.u8[12] - SRC.u8[12], 0, UINT8_MAX);
        DEST.u8[13] = std::clamp<int16_t>(DEST.u8[13] - SRC.u8[13], 0, UINT8_MAX);
        DEST.u8[14] = std::clamp<int16_t>(DEST.u8[14] - SRC.u8[14], 0, UINT8_MAX);
        DEST.u8[15] = std::clamp<int16_t>(DEST.u8[15] - SRC.u8[15], 0, UINT8_MAX);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PSUBUSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSUBUSW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u16[0] = std::clamp<int32_t>(DEST.u16[0] - SRC.u16[0], 0, UINT16_MAX);
        DEST.u16[1] = std::clamp<int32_t>(DEST.u16[1] - SRC.u16[1], 0, UINT16_MAX);
        DEST.u16[2] = std::clamp<int32_t>(DEST.u16[2] - SRC.u16[2], 0, UINT16_MAX);
        DEST.u16[3] = std::clamp<int32_t>(DEST.u16[3] - SRC.u16[3], 0, UINT16_MAX);
        DEST.u16[4] = std::clamp<int32_t>(DEST.u16[4] - SRC.u16[4], 0, UINT16_MAX);
        DEST.u16[5] = std::clamp<int32_t>(DEST.u16[5] - SRC.u16[5], 0, UINT16_MAX);
        DEST.u16[6] = std::clamp<int32_t>(DEST.u16[6] - SRC.u16[6], 0, UINT16_MAX);
        DEST.u16[7] = std::clamp<int32_t>(DEST.u16[7] - SRC.u16[7], 0, UINT16_MAX);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PUNPCKHBW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PUNPCKHBW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i8 = {
            DEST.i8[8],
            SRC.i8[8],
            DEST.i8[9],
            SRC.i8[9],
            DEST.i8[10],
            SRC.i8[10],
            DEST.i8[11],
            SRC.i8[11],
            DEST.i8[12],
            SRC.i8[12],
            DEST.i8[13],
            SRC.i8[13],
            DEST.i8[14],
            SRC.i8[14],
            DEST.i8[15],
            SRC.i8[15],
        };
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PUNPCKHWD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PUNPCKHWD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16 = {
            DEST.i16[4],
            SRC.i16[4],
            DEST.i16[5],
            SRC.i16[5],
            DEST.i16[6],
            SRC.i16[6],
            DEST.i16[7],
            SRC.i16[7],
        };
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PUNPCKHDQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PUNPCKHDQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32 = {
            DEST.i32[2],
            SRC.i32[2],
            DEST.i32[3],
            SRC.i32[3],
        };
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PUNPCKHQDQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PUNPCKHQDQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64 = {
            DEST.i64[1],
            SRC.i64[1],
        };
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PUNPCKLBW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PUNPCKLBW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

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
            DEST.i8[4],
            SRC.i8[4],
            DEST.i8[5],
            SRC.i8[5],
            DEST.i8[6],
            SRC.i8[6],
            DEST.i8[7],
            SRC.i8[7],
        };
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PUNPCKLWD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PUNPCKLWD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16 = {
            DEST.i16[0],
            SRC.i16[0],
            DEST.i16[1],
            SRC.i16[1],
            DEST.i16[2],
            SRC.i16[2],
            DEST.i16[3],
            SRC.i16[3],
        };
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PUNPCKLDQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PUNPCKLDQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32 = {
            DEST.i32[0],
            SRC.i32[0],
            DEST.i32[1],
            SRC.i32[1],
        };
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PUNPCKLQDQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PUNPCKLQDQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64 = {
            DEST.i64[0],
            SRC.i64[0],
        };
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::PXOR(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PXOR", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32[0] = DEST.i32[0] ^ SRC.i32[0];
        DEST.i32[1] = DEST.i32[1] ^ SRC.i32[1];
        DEST.i32[2] = DEST.i32[2] ^ SRC.i32[2];
        DEST.i32[3] = DEST.i32[3] ^ SRC.i32[3];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::SHUFPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "SHUFPD", 2, 8, SSE_REGISTER | OPERAND_SIZE | DIRECTION | THREE_OPERAND);

    BEGIN_OPERATION() {
        auto SEL = SRC2.u8[0];
        DEST.f64[0] = SRC.f64[(SEL >> 0) & 0x1];
        DEST.f64[1] = SRC.f64[(SEL >> 1) & 0x1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::SQRTPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "SQRTPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = sqrt(SRC.f64[0]);
        DEST.f64[1] = sqrt(SRC.f64[1]);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::SQRTSD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "SQRTSD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = sqrt(SRC.f64[0]);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::SUBPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "SUBPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = DEST.f64[0] - SRC.f64[0];
        DEST.f64[1] = DEST.f64[1] - SRC.f64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::SUBSD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "SUBSD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = DEST.f64[0] - SRC.f64[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::UCOMISD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "UCOMISD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        if (isunordered(DEST.f64[0], SRC.f64[0])) {
            ZF = 1;
            PF = 1;
            CF = 1;
        }
        else if (DEST.f64[0] > SRC.f64[0]) {
            ZF = 0;
            PF = 0;
            CF = 0;
        }
        else if (DEST.f64[0] < SRC.f64[0]) {
            ZF = 0;
            PF = 0;
            CF = 1;
        }
        else {
            ZF = 1;
            PF = 0;
            CF = 0;
        }
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::UNPCKHPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "UNPCKHPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64 = {
            DEST.f64[1],
            SRC.f64[1],
        };
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::UNPCKLPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "UNPCKLPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64 = {
            DEST.f64[0],
            SRC.f64[0],
        };
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse2_instruction::XORPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "XORPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = DEST.i64[0] ^ SRC.i64[0];
        DEST.i64[1] = DEST.i64[1] ^ SRC.i64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
