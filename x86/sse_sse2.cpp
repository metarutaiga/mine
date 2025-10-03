#include <math.h>
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

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void sse_instruction::ADDPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "ADDPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = DEST.f64[0] + SRC.f64[0];
        DEST.f64[1] = DEST.f64[1] + SRC.f64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::ADDSD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "ADDSD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = DEST.f64[0] + SRC.f64[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::ANDNPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "ANDNPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = ~DEST.i64[0] & SRC.i64[0];
        DEST.i64[1] = ~DEST.i64[1] & SRC.i64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::ANDPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "ANDPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = DEST.i64[0] & SRC.i64[0];
        DEST.i64[1] = DEST.i64[1] & SRC.i64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::CLFLUSH(Format& format, const uint8_t* opcode)
{
    format.instruction = "CLFLUSH";
    format.operand_count = 0;

    OPERATION() {};
}
//------------------------------------------------------------------------------
void sse_instruction::CMPPD(Format& format, const uint8_t* opcode)
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
            DEST.i64[0] = (DEST.f64[0] == SRC.f64[0]) ? -1 : 0;
            DEST.i64[1] = (DEST.f64[1] == SRC.f64[1]) ? -1 : 0;
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
            DEST.i64[0] = (DEST.f64[0] != SRC.f64[0]) ? -1 : 0;
            DEST.i64[1] = (DEST.f64[1] != SRC.f64[1]) ? -1 : 0;
        } END_OPERATION_SSE;
        break;
    }
}
//------------------------------------------------------------------------------
void sse_instruction::CMPSD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CMPSD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION | THREE_OPERAND);

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
            DEST.i64[0] = (DEST.f64[0] == SRC.f64[0]) ? -1 : 0;
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
            DEST.i64[0] = (DEST.f64[0] != SRC.f64[0]) ? -1 : 0;
        } END_OPERATION_SSE;
        break;
    }
}
//------------------------------------------------------------------------------
void sse_instruction::COMISD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "COMISD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        OF = 0;
        SF = 0;
        AF = 0;
        if (DEST.f64[0] == SRC.f64[0]) {
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
void sse_instruction::CVTDQ2PD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTDQ2PD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = SRC.i64[0];
        DEST.f64[1] = SRC.i64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::CVTDQ2PS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTDQ2PS", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32[0] = SRC.i64[0];
        DEST.f32[1] = SRC.i64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::CVTPD2DQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTPD2DQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = SRC.f64[0];
        DEST.i64[1] = SRC.f64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::CVTPD2PI(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTPD2PI", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32[0] = SRC.f64[0];
        DEST.i32[1] = SRC.f64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::CVTPD2PS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTPD2PS", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32[0] = SRC.f64[0];
        DEST.f32[1] = SRC.f64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::CVTPI2PD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTPI2PD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = SRC.i32[0];
        DEST.f64[1] = SRC.i32[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::CVTPS2DQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTPS2DQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = SRC.f32[0];
        DEST.i64[1] = SRC.f32[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::CVTPS2PD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTPS2PD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = SRC.f32[0];
        DEST.f64[1] = SRC.f32[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::CVTSD2SI(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTSD2SI", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32[0] = SRC.f64[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::CVTSD2SS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTSD2SS", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32[0] = SRC.f64[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::CVTSI2SD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTSI2SD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = SRC.i32[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::CVTSS2SD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTSS2SD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = SRC.f32[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::CVTTPD2DQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTTPD2DQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = SRC.f64[0];
        DEST.i64[1] = SRC.f64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::CVTTPD2PI(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTTPD2PI", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = SRC.f64[0];
        DEST.i64[1] = SRC.f64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::CVTTPS2DQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTTPS2DQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = SRC.f32[0];
        DEST.i64[1] = SRC.f32[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::CVTTSD2SI(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTTSD2SI", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32[0] = SRC.f64[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::DIVPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "DIVPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = DEST.f64[0] / SRC.f64[0];
        DEST.f64[1] = DEST.f64[1] / SRC.f64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::DIVSD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "DIVSD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = DEST.f64[0] / SRC.f64[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::LFENCE(Format& format, const uint8_t* opcode)
{
    format.instruction = "LFENCE";
    format.operand_count = 0;

    OPERATION() {};
}
//------------------------------------------------------------------------------
void sse_instruction::MASKMOVDQU(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MASKMOVDQU", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);
    format.operand[2] = format.operand[1];
    format.operand[1] = format.operand[0];
    format.operand[0].type = Format::Operand::ADR;
    format.operand[0].base = IndexREG(EDI);

    BEGIN_OPERATION() {
        DEST.i8[0] = (SRC2.i8[0] == 1) ? SRC1.i8[0] : 0;
        DEST.i8[1] = (SRC2.i8[1] == 1) ? SRC1.i8[1] : 0;
        DEST.i8[2] = (SRC2.i8[2] == 1) ? SRC1.i8[2] : 0;
        DEST.i8[3] = (SRC2.i8[3] == 1) ? SRC1.i8[3] : 0;
        DEST.i8[4] = (SRC2.i8[4] == 1) ? SRC1.i8[4] : 0;
        DEST.i8[5] = (SRC2.i8[5] == 1) ? SRC1.i8[5] : 0;
        DEST.i8[6] = (SRC2.i8[6] == 1) ? SRC1.i8[6] : 0;
        DEST.i8[7] = (SRC2.i8[7] == 1) ? SRC1.i8[7] : 0;
        DEST.i8[8] = (SRC2.i8[8] == 1) ? SRC1.i8[8] : 0;
        DEST.i8[9] = (SRC2.i8[9] == 1) ? SRC1.i8[9] : 0;
        DEST.i8[10] = (SRC2.i8[10] == 1) ? SRC1.i8[10] : 0;
        DEST.i8[11] = (SRC2.i8[11] == 1) ? SRC1.i8[11] : 0;
        DEST.i8[12] = (SRC2.i8[12] == 1) ? SRC1.i8[12] : 0;
        DEST.i8[13] = (SRC2.i8[13] == 1) ? SRC1.i8[13] : 0;
        DEST.i8[14] = (SRC2.i8[14] == 1) ? SRC1.i8[14] : 0;
        DEST.i8[15] = (SRC2.i8[15] == 1) ? SRC1.i8[15] : 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MAXPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MAXPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = std::max(DEST.f64[0], SRC.f64[0]);
        DEST.f64[1] = std::max(DEST.f64[1], SRC.f64[1]);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MAXSD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MAXSD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = std::max(DEST.f64[0], SRC.f64[0]);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MFENCE(Format& format, const uint8_t* opcode)
{
    format.instruction = "MFENCE";
    format.operand_count = 0;

    OPERATION() {};
}
//------------------------------------------------------------------------------
void sse_instruction::MINPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MINPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = std::min(DEST.f64[0], SRC.f64[0]);
        DEST.f64[1] = std::min(DEST.f64[1], SRC.f64[1]);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MINSD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MINSD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = std::min(DEST.f64[0], SRC.f64[0]);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MOVAPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVAPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST = SRC;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MOVD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32[0] = SRC.i32[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MOVDQ2Q(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVDQ2Q", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);
    format.operand[0].type = Format::Operand::MMX;

    BEGIN_OPERATION() {
        DEST.i64[0] = SRC.i64[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MOVDQA(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVDQA", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST = SRC;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MOVDQU(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVDQU", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST = SRC;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MOVHPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVHPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[1] = SRC.f64[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MOVLPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVLPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = SRC.f64[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MOVMSKPD(Format& format, const uint8_t* opcode)
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
void sse_instruction::MOVNTDQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVNTDQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST = SRC;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MOVNTI(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVNTI", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32[0] = SRC.i32[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MOVNTPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVNTPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST = SRC;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MOVQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = SRC.i64[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MOVQ2DQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVQ2DQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = DEST.f64[0];
        DEST.f64[1] = 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MOVSD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVSD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = SRC.f64[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MOVUPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVUPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST = SRC;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MULPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MULPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = DEST.f64[0] * SRC.f64[0];
        DEST.f64[1] = DEST.f64[1] * SRC.f64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MULSD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MULSD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = DEST.f64[0] * SRC.f64[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::ORPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "ORPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = DEST.i64[0] | SRC.i64[0];
        DEST.i64[1] = DEST.i64[1] | SRC.i64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PACKSSDW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PACKSSDW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        auto TEMP = DEST;
        DEST.i16[0] = std::clamp<int32_t>(TEMP.i32[0], INT16_MIN, INT16_MAX);
        DEST.i16[1] = std::clamp<int32_t>(TEMP.i32[1], INT16_MIN, INT16_MAX);
        DEST.i16[2] = std::clamp<int32_t>(SRC.i32[0], INT16_MIN, INT16_MAX);
        DEST.i16[3] = std::clamp<int32_t>(SRC.i32[1], INT16_MIN, INT16_MAX);
        DEST.i16[4] = std::clamp<int32_t>(TEMP.i32[2], INT16_MIN, INT16_MAX);
        DEST.i16[5] = std::clamp<int32_t>(TEMP.i32[3], INT16_MIN, INT16_MAX);
        DEST.i16[6] = std::clamp<int32_t>(SRC.i32[2], INT16_MIN, INT16_MAX);
        DEST.i16[7] = std::clamp<int32_t>(SRC.i32[3], INT16_MIN, INT16_MAX);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PACKSSWB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PACKSSWB", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        auto TEMP = DEST;
        DEST.i8[0] = std::clamp<int16_t>(TEMP.i16[0], INT8_MIN, INT8_MAX);
        DEST.i8[1] = std::clamp<int16_t>(TEMP.i16[1], INT8_MIN, INT8_MAX);
        DEST.i8[2] = std::clamp<int16_t>(TEMP.i16[2], INT8_MIN, INT8_MAX);
        DEST.i8[3] = std::clamp<int16_t>(TEMP.i16[3], INT8_MIN, INT8_MAX);
        DEST.i8[4] = std::clamp<int16_t>(SRC.i16[0], INT8_MIN, INT8_MAX);
        DEST.i8[5] = std::clamp<int16_t>(SRC.i16[1], INT8_MIN, INT8_MAX);
        DEST.i8[6] = std::clamp<int16_t>(SRC.i16[2], INT8_MIN, INT8_MAX);
        DEST.i8[7] = std::clamp<int16_t>(SRC.i16[3], INT8_MIN, INT8_MAX);
        DEST.i8[8] = std::clamp<int16_t>(TEMP.i16[4], INT8_MIN, INT8_MAX);
        DEST.i8[9] = std::clamp<int16_t>(TEMP.i16[5], INT8_MIN, INT8_MAX);
        DEST.i8[10] = std::clamp<int16_t>(TEMP.i16[6], INT8_MIN, INT8_MAX);
        DEST.i8[11] = std::clamp<int16_t>(TEMP.i16[7], INT8_MIN, INT8_MAX);
        DEST.i8[12] = std::clamp<int16_t>(SRC.i16[4], INT8_MIN, INT8_MAX);
        DEST.i8[13] = std::clamp<int16_t>(SRC.i16[5], INT8_MIN, INT8_MAX);
        DEST.i8[14] = std::clamp<int16_t>(SRC.i16[6], INT8_MIN, INT8_MAX);
        DEST.i8[15] = std::clamp<int16_t>(SRC.i16[7], INT8_MIN, INT8_MAX);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PACKUSWB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PACKUSWB", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        auto TEMP = DEST;
        DEST.i8[0] = std::clamp<int16_t>(TEMP.i16[0], 0, UINT8_MAX);
        DEST.i8[1] = std::clamp<int16_t>(TEMP.i16[1], 0, UINT8_MAX);
        DEST.i8[2] = std::clamp<int16_t>(TEMP.i16[2], 0, UINT8_MAX);
        DEST.i8[3] = std::clamp<int16_t>(TEMP.i16[3], 0, UINT8_MAX);
        DEST.i8[4] = std::clamp<int16_t>(SRC.i16[0], 0, UINT8_MAX);
        DEST.i8[5] = std::clamp<int16_t>(SRC.i16[1], 0, UINT8_MAX);
        DEST.i8[6] = std::clamp<int16_t>(SRC.i16[2], 0, UINT8_MAX);
        DEST.i8[7] = std::clamp<int16_t>(SRC.i16[3], 0, UINT8_MAX);
        DEST.i8[8] = std::clamp<int16_t>(TEMP.i16[4], 0, UINT8_MAX);
        DEST.i8[9] = std::clamp<int16_t>(TEMP.i16[5], 0, UINT8_MAX);
        DEST.i8[10] = std::clamp<int16_t>(TEMP.i16[6], 0, UINT8_MAX);
        DEST.i8[11] = std::clamp<int16_t>(TEMP.i16[7], 0, UINT8_MAX);
        DEST.i8[12] = std::clamp<int16_t>(SRC.i16[4], 0, UINT8_MAX);
        DEST.i8[13] = std::clamp<int16_t>(SRC.i16[5], 0, UINT8_MAX);
        DEST.i8[14] = std::clamp<int16_t>(SRC.i16[6], 0, UINT8_MAX);
        DEST.i8[15] = std::clamp<int16_t>(SRC.i16[7], 0, UINT8_MAX);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PADDB(Format& format, const uint8_t* opcode)
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
void sse_instruction::PADDW(Format& format, const uint8_t* opcode)
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
void sse_instruction::PADDD(Format& format, const uint8_t* opcode)
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
void sse_instruction::PADDQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PADDQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = DEST.i64[0] + SRC.i64[0];
        DEST.i64[1] = DEST.i64[1] + SRC.i64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PADDSB(Format& format, const uint8_t* opcode)
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
void sse_instruction::PADDSW(Format& format, const uint8_t* opcode)
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
void sse_instruction::PADDUSB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PADDUSB", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i8[0] = std::clamp<uint8_t>(DEST.i8[0] + SRC.i8[0], 0, UINT8_MAX);
        DEST.i8[1] = std::clamp<uint8_t>(DEST.i8[1] + SRC.i8[1], 0, UINT8_MAX);
        DEST.i8[2] = std::clamp<uint8_t>(DEST.i8[2] + SRC.i8[2], 0, UINT8_MAX);
        DEST.i8[3] = std::clamp<uint8_t>(DEST.i8[3] + SRC.i8[3], 0, UINT8_MAX);
        DEST.i8[4] = std::clamp<uint8_t>(DEST.i8[4] + SRC.i8[4], 0, UINT8_MAX);
        DEST.i8[5] = std::clamp<uint8_t>(DEST.i8[5] + SRC.i8[5], 0, UINT8_MAX);
        DEST.i8[6] = std::clamp<uint8_t>(DEST.i8[6] + SRC.i8[6], 0, UINT8_MAX);
        DEST.i8[7] = std::clamp<uint8_t>(DEST.i8[7] + SRC.i8[7], 0, UINT8_MAX);
        DEST.i8[8] = std::clamp<uint8_t>(DEST.i8[8] + SRC.i8[8], 0, UINT8_MAX);
        DEST.i8[9] = std::clamp<uint8_t>(DEST.i8[9] + SRC.i8[9], 0, UINT8_MAX);
        DEST.i8[10] = std::clamp<uint8_t>(DEST.i8[10] + SRC.i8[10], 0, UINT8_MAX);
        DEST.i8[11] = std::clamp<uint8_t>(DEST.i8[11] + SRC.i8[11], 0, UINT8_MAX);
        DEST.i8[12] = std::clamp<uint8_t>(DEST.i8[12] + SRC.i8[12], 0, UINT8_MAX);
        DEST.i8[13] = std::clamp<uint8_t>(DEST.i8[13] + SRC.i8[13], 0, UINT8_MAX);
        DEST.i8[14] = std::clamp<uint8_t>(DEST.i8[14] + SRC.i8[14], 0, UINT8_MAX);
        DEST.i8[15] = std::clamp<uint8_t>(DEST.i8[15] + SRC.i8[15], 0, UINT8_MAX);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PADDUSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PADDUSW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = std::clamp<uint16_t>(DEST.i16[0] + SRC.i16[0], 0, UINT16_MAX);
        DEST.i16[1] = std::clamp<uint16_t>(DEST.i16[1] + SRC.i16[1], 0, UINT16_MAX);
        DEST.i16[2] = std::clamp<uint16_t>(DEST.i16[2] + SRC.i16[2], 0, UINT16_MAX);
        DEST.i16[3] = std::clamp<uint16_t>(DEST.i16[3] + SRC.i16[3], 0, UINT16_MAX);
        DEST.i16[4] = std::clamp<uint16_t>(DEST.i16[4] + SRC.i16[4], 0, UINT16_MAX);
        DEST.i16[5] = std::clamp<uint16_t>(DEST.i16[5] + SRC.i16[5], 0, UINT16_MAX);
        DEST.i16[6] = std::clamp<uint16_t>(DEST.i16[6] + SRC.i16[6], 0, UINT16_MAX);
        DEST.i16[7] = std::clamp<uint16_t>(DEST.i16[7] + SRC.i16[7], 0, UINT16_MAX);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PAND(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PAND", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = DEST.i64[0] & SRC.i64[0];
        DEST.i64[1] = DEST.i64[1] & SRC.i64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PANDN(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PANDN", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = ~DEST.i64[0] & SRC.i64[0];
        DEST.i64[1] = ~DEST.i64[1] & SRC.i64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PAUSE(Format& format, const uint8_t* opcode)
{
    format.instruction = "PAUSE";
    format.operand_count = 0;

    OPERATION() {};
}
//------------------------------------------------------------------------------
void sse_instruction::PAVGB(Format& format, const uint8_t* opcode)
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
void sse_instruction::PAVGW(Format& format, const uint8_t* opcode)
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
void sse_instruction::PCMPEQB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "ADDPS", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i8[0] = (DEST.i8[0] == SRC.i8[0]) ? 0xFF : 0;
        DEST.i8[1] = (DEST.i8[1] == SRC.i8[1]) ? 0xFF : 0;
        DEST.i8[2] = (DEST.i8[2] == SRC.i8[2]) ? 0xFF : 0;
        DEST.i8[3] = (DEST.i8[3] == SRC.i8[3]) ? 0xFF : 0;
        DEST.i8[4] = (DEST.i8[4] == SRC.i8[4]) ? 0xFF : 0;
        DEST.i8[5] = (DEST.i8[5] == SRC.i8[5]) ? 0xFF : 0;
        DEST.i8[6] = (DEST.i8[6] == SRC.i8[6]) ? 0xFF : 0;
        DEST.i8[7] = (DEST.i8[7] == SRC.i8[7]) ? 0xFF : 0;
        DEST.i8[8] = (DEST.i8[8] == SRC.i8[8]) ? 0xFF : 0;
        DEST.i8[9] = (DEST.i8[9] == SRC.i8[9]) ? 0xFF : 0;
        DEST.i8[10] = (DEST.i8[10] == SRC.i8[10]) ? 0xFF : 0;
        DEST.i8[11] = (DEST.i8[11] == SRC.i8[11]) ? 0xFF : 0;
        DEST.i8[12] = (DEST.i8[12] == SRC.i8[12]) ? 0xFF : 0;
        DEST.i8[13] = (DEST.i8[13] == SRC.i8[13]) ? 0xFF : 0;
        DEST.i8[14] = (DEST.i8[14] == SRC.i8[14]) ? 0xFF : 0;
        DEST.i8[15] = (DEST.i8[15] == SRC.i8[15]) ? 0xFF : 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PCMPEQW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PCMPEQW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = (DEST.i16[0] == SRC.i16[0]) ? 0xFFFF : 0;
        DEST.i16[1] = (DEST.i16[1] == SRC.i16[1]) ? 0xFFFF : 0;
        DEST.i16[2] = (DEST.i16[2] == SRC.i16[2]) ? 0xFFFF : 0;
        DEST.i16[3] = (DEST.i16[3] == SRC.i16[3]) ? 0xFFFF : 0;
        DEST.i16[4] = (DEST.i16[4] == SRC.i16[4]) ? 0xFFFF : 0;
        DEST.i16[5] = (DEST.i16[5] == SRC.i16[5]) ? 0xFFFF : 0;
        DEST.i16[6] = (DEST.i16[6] == SRC.i16[6]) ? 0xFFFF : 0;
        DEST.i16[7] = (DEST.i16[7] == SRC.i16[7]) ? 0xFFFF : 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PCMPEQD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PCMPEQD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32[0] = (DEST.i32[0] == SRC.i32[0]) ? 0xFFFFFFFF : 0;
        DEST.i32[1] = (DEST.i32[1] == SRC.i32[1]) ? 0xFFFFFFFF : 0;
        DEST.i32[2] = (DEST.i32[2] == SRC.i32[2]) ? 0xFFFFFFFF : 0;
        DEST.i32[3] = (DEST.i32[3] == SRC.i32[3]) ? 0xFFFFFFFF : 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PCMPGTB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PCMPGTB", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i8[0] = (DEST.i8[0] > SRC.i8[0]) ? 0xFF : 0;
        DEST.i8[1] = (DEST.i8[1] > SRC.i8[1]) ? 0xFF : 0;
        DEST.i8[2] = (DEST.i8[2] > SRC.i8[2]) ? 0xFF : 0;
        DEST.i8[3] = (DEST.i8[3] > SRC.i8[3]) ? 0xFF : 0;
        DEST.i8[4] = (DEST.i8[4] > SRC.i8[4]) ? 0xFF : 0;
        DEST.i8[5] = (DEST.i8[5] > SRC.i8[5]) ? 0xFF : 0;
        DEST.i8[6] = (DEST.i8[6] > SRC.i8[6]) ? 0xFF : 0;
        DEST.i8[7] = (DEST.i8[7] > SRC.i8[7]) ? 0xFF : 0;
        DEST.i8[8] = (DEST.i8[8] > SRC.i8[8]) ? 0xFF : 0;
        DEST.i8[9] = (DEST.i8[9] > SRC.i8[9]) ? 0xFF : 0;
        DEST.i8[10] = (DEST.i8[10] > SRC.i8[10]) ? 0xFF : 0;
        DEST.i8[11] = (DEST.i8[11] > SRC.i8[11]) ? 0xFF : 0;
        DEST.i8[12] = (DEST.i8[12] > SRC.i8[12]) ? 0xFF : 0;
        DEST.i8[13] = (DEST.i8[13] > SRC.i8[13]) ? 0xFF : 0;
        DEST.i8[14] = (DEST.i8[14] > SRC.i8[14]) ? 0xFF : 0;
        DEST.i8[15] = (DEST.i8[15] > SRC.i8[15]) ? 0xFF : 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PCMPGTW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PCMPGTW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = (DEST.i16[0] > SRC.i16[0]) ? 0xFFFF : 0;
        DEST.i16[1] = (DEST.i16[1] > SRC.i16[1]) ? 0xFFFF : 0;
        DEST.i16[2] = (DEST.i16[2] > SRC.i16[2]) ? 0xFFFF : 0;
        DEST.i16[3] = (DEST.i16[3] > SRC.i16[3]) ? 0xFFFF : 0;
        DEST.i16[4] = (DEST.i16[4] > SRC.i16[4]) ? 0xFFFF : 0;
        DEST.i16[5] = (DEST.i16[5] > SRC.i16[5]) ? 0xFFFF : 0;
        DEST.i16[6] = (DEST.i16[6] > SRC.i16[6]) ? 0xFFFF : 0;
        DEST.i16[7] = (DEST.i16[7] > SRC.i16[7]) ? 0xFFFF : 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PCMPGTD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PCMPGTD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32[0] = (DEST.i32[0] > SRC.i32[0]) ? 0xFFFFFFFF : 0;
        DEST.i32[1] = (DEST.i32[1] > SRC.i32[1]) ? 0xFFFFFFFF : 0;
        DEST.i32[2] = (DEST.i32[2] > SRC.i32[2]) ? 0xFFFFFFFF : 0;
        DEST.i32[3] = (DEST.i32[3] > SRC.i32[3]) ? 0xFFFFFFFF : 0;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PEXTRW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PEXTRW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION | THREE_OPERAND);
    format.operand[0].type = Format::Operand::REG;

    BEGIN_OPERATION() {
        auto SEL = SRC2.u8[0] % 8;
        DEST.u16[0] = SRC.u16[SEL];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PINSRW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PINSRW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION | THREE_OPERAND);
    format.operand[0].type = Format::Operand::REG;

    BEGIN_OPERATION() {
        auto SEL = SRC2.u8[0] % 8;
        DEST.u16[SEL] = SRC.u16[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PMADDWD(Format& format, const uint8_t* opcode)
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
void sse_instruction::PMAXSW(Format& format, const uint8_t* opcode)
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
void sse_instruction::PMAXUB(Format& format, const uint8_t* opcode)
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
void sse_instruction::PMINSW(Format& format, const uint8_t* opcode)
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
void sse_instruction::PMINUB(Format& format, const uint8_t* opcode)
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
void sse_instruction::PMOVMSKB(Format& format, const uint8_t* opcode)
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
void sse_instruction::PMULHUW(Format& format, const uint8_t* opcode)
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
void sse_instruction::PMULHW(Format& format, const uint8_t* opcode)
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
void sse_instruction::PMULLW(Format& format, const uint8_t* opcode)
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
void sse_instruction::PMULUDQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMULUDQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = DEST.i32[0] * SRC.f32[0];
        DEST.i64[1] = DEST.i32[2] * SRC.f32[2];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::POR(Format& format, const uint8_t* opcode)
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
void sse_instruction::PSADBW(Format& format, const uint8_t* opcode)
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
void sse_instruction::PSHUFD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSHUFD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        auto SEL = SRC2.u8[0];
        DEST.i32[0] = SRC.i32[(SEL >> 0) & 0x3];
        DEST.i32[1] = SRC.i32[(SEL >> 2) & 0x3];
        DEST.i32[2] = SRC.i32[(SEL >> 4) & 0x3];
        DEST.i32[3] = SRC.i32[(SEL >> 6) & 0x3];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PSHUFHW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSHUFHW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        auto SEL = SRC2.u8[0];
        DEST.i16[4] = SRC.i16[(SEL >> 0) & 0x3 + 4];
        DEST.i16[5] = SRC.i16[(SEL >> 2) & 0x3 + 4];
        DEST.i16[6] = SRC.i16[(SEL >> 4) & 0x3 + 4];
        DEST.i16[7] = SRC.i16[(SEL >> 6) & 0x3 + 4];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PSHUFLW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSHUFLW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        auto SEL = SRC2.u8[0];
        DEST.i16[0] = SRC.i16[(SEL >> 0) & 0x3];
        DEST.i16[1] = SRC.i16[(SEL >> 2) & 0x3];
        DEST.i16[2] = SRC.i16[(SEL >> 4) & 0x3];
        DEST.i16[3] = SRC.i16[(SEL >> 6) & 0x3];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PSLLW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSLLW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        auto COUNT = (SRC.u8[0] % 16);
        DEST.i16[0] = DEST.i16[0] << COUNT;
        DEST.i16[1] = DEST.i16[1] << COUNT;
        DEST.i16[2] = DEST.i16[2] << COUNT;
        DEST.i16[3] = DEST.i16[3] << COUNT;
        DEST.i16[4] = DEST.i16[4] << COUNT;
        DEST.i16[5] = DEST.i16[5] << COUNT;
        DEST.i16[6] = DEST.i16[6] << COUNT;
        DEST.i16[7] = DEST.i16[7] << COUNT;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PSLLD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSLLD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        auto COUNT = (SRC.u8[0] % 32);
        DEST.i32[0] = DEST.i32[0] << COUNT;
        DEST.i32[1] = DEST.i32[1] << COUNT;
        DEST.i32[2] = DEST.i32[2] << COUNT;
        DEST.i32[3] = DEST.i32[3] << COUNT;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PSLLQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSLLQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        auto COUNT = (SRC.u8[0] % 64);
        DEST.i64[0] = DEST.i64[0] << COUNT;
        DEST.i64[1] = DEST.i64[1] << COUNT;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PSLLDQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSLLDQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        int COUNT = (SRC.u8[0] % 16);
        for (int i = 15; i > 0; --i) {
            int offset = i - COUNT;
            DEST.i8[i] = offset >= 0 ? DEST.i8[offset] : 0;
        }
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PSRAW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSRAW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        auto COUNT = (SRC.u8[0] % 16);
        DEST.i16[0] = DEST.i16[0] >> COUNT;
        DEST.i16[1] = DEST.i16[1] >> COUNT;
        DEST.i16[2] = DEST.i16[2] >> COUNT;
        DEST.i16[3] = DEST.i16[3] >> COUNT;
        DEST.i16[4] = DEST.i16[4] >> COUNT;
        DEST.i16[5] = DEST.i16[5] >> COUNT;
        DEST.i16[6] = DEST.i16[6] >> COUNT;
        DEST.i16[7] = DEST.i16[7] >> COUNT;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PSRAD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSRAD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        auto COUNT = (SRC.u8[0] % 32);
        DEST.i32[0] = DEST.i32[0] >> COUNT;
        DEST.i32[1] = DEST.i32[1] >> COUNT;
        DEST.i32[2] = DEST.i32[2] >> COUNT;
        DEST.i32[3] = DEST.i32[3] >> COUNT;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PSRLW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSRLW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        auto COUNT = (SRC.u8[0] % 16);
        DEST.u16[0] = DEST.u16[0] >> COUNT;
        DEST.u16[1] = DEST.u16[1] >> COUNT;
        DEST.u16[2] = DEST.u16[2] >> COUNT;
        DEST.u16[3] = DEST.u16[3] >> COUNT;
        DEST.u16[4] = DEST.u16[4] >> COUNT;
        DEST.u16[5] = DEST.u16[5] >> COUNT;
        DEST.u16[6] = DEST.u16[6] >> COUNT;
        DEST.u16[7] = DEST.u16[7] >> COUNT;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PSRLD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSRLD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        auto COUNT = (SRC.u8[0] % 32);
        DEST.u32[0] = DEST.u32[0] >> COUNT;
        DEST.u32[1] = DEST.u32[1] >> COUNT;
        DEST.u32[2] = DEST.u32[2] >> COUNT;
        DEST.u32[3] = DEST.u32[3] >> COUNT;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PSRLDQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSRLDQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        auto COUNT = (SRC.u8[0] % 64);
        DEST.u64[0] = DEST.u64[0] >> COUNT;
        DEST.u64[1] = DEST.u64[1] >> COUNT;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PSUBB(Format& format, const uint8_t* opcode)
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
void sse_instruction::PSUBW(Format& format, const uint8_t* opcode)
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
void sse_instruction::PSUBD(Format& format, const uint8_t* opcode)
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
void sse_instruction::PSUBQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSUBQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = DEST.i64[0] - SRC.i64[0];
        DEST.i64[1] = DEST.i64[1] - SRC.i64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PSUBSB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSUBSB", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i8[0] = std::clamp<int8_t>(DEST.i8[0] - SRC.i8[0], INT8_MIN, INT8_MAX);
        DEST.i8[1] = std::clamp<int8_t>(DEST.i8[1] - SRC.i8[1], INT8_MIN, INT8_MAX);
        DEST.i8[2] = std::clamp<int8_t>(DEST.i8[2] - SRC.i8[2], INT8_MIN, INT8_MAX);
        DEST.i8[3] = std::clamp<int8_t>(DEST.i8[3] - SRC.i8[3], INT8_MIN, INT8_MAX);
        DEST.i8[4] = std::clamp<int8_t>(DEST.i8[4] - SRC.i8[4], INT8_MIN, INT8_MAX);
        DEST.i8[5] = std::clamp<int8_t>(DEST.i8[5] - SRC.i8[5], INT8_MIN, INT8_MAX);
        DEST.i8[6] = std::clamp<int8_t>(DEST.i8[6] - SRC.i8[6], INT8_MIN, INT8_MAX);
        DEST.i8[7] = std::clamp<int8_t>(DEST.i8[7] - SRC.i8[7], INT8_MIN, INT8_MAX);
        DEST.i8[8] = std::clamp<int8_t>(DEST.i8[8] - SRC.i8[8], INT8_MIN, INT8_MAX);
        DEST.i8[9] = std::clamp<int8_t>(DEST.i8[9] - SRC.i8[9], INT8_MIN, INT8_MAX);
        DEST.i8[10] = std::clamp<int8_t>(DEST.i8[10] - SRC.i8[10], INT8_MIN, INT8_MAX);
        DEST.i8[11] = std::clamp<int8_t>(DEST.i8[11] - SRC.i8[11], INT8_MIN, INT8_MAX);
        DEST.i8[12] = std::clamp<int8_t>(DEST.i8[12] - SRC.i8[12], INT8_MIN, INT8_MAX);
        DEST.i8[13] = std::clamp<int8_t>(DEST.i8[13] - SRC.i8[13], INT8_MIN, INT8_MAX);
        DEST.i8[14] = std::clamp<int8_t>(DEST.i8[14] - SRC.i8[14], INT8_MIN, INT8_MAX);
        DEST.i8[15] = std::clamp<int8_t>(DEST.i8[15] - SRC.i8[15], INT8_MIN, INT8_MAX);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PSUBSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSUBSW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = std::clamp<int16_t>(DEST.i16[0] - SRC.i16[0], INT16_MIN, INT16_MAX);
        DEST.i16[1] = std::clamp<int16_t>(DEST.i16[1] - SRC.i16[1], INT16_MIN, INT16_MAX);
        DEST.i16[2] = std::clamp<int16_t>(DEST.i16[2] - SRC.i16[2], INT16_MIN, INT16_MAX);
        DEST.i16[3] = std::clamp<int16_t>(DEST.i16[3] - SRC.i16[3], INT16_MIN, INT16_MAX);
        DEST.i16[4] = std::clamp<int16_t>(DEST.i16[4] - SRC.i16[4], INT16_MIN, INT16_MAX);
        DEST.i16[5] = std::clamp<int16_t>(DEST.i16[5] - SRC.i16[5], INT16_MIN, INT16_MAX);
        DEST.i16[6] = std::clamp<int16_t>(DEST.i16[6] - SRC.i16[6], INT16_MIN, INT16_MAX);
        DEST.i16[7] = std::clamp<int16_t>(DEST.i16[7] - SRC.i16[7], INT16_MIN, INT16_MAX);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PSUBUSB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSUBUSB", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i8[0] = std::clamp<int16_t>(DEST.i8[0] - SRC.i8[0], 0, UINT8_MAX);
        DEST.i8[1] = std::clamp<int16_t>(DEST.i8[1] - SRC.i8[1], 0, UINT8_MAX);
        DEST.i8[2] = std::clamp<int16_t>(DEST.i8[2] - SRC.i8[2], 0, UINT8_MAX);
        DEST.i8[3] = std::clamp<int16_t>(DEST.i8[3] - SRC.i8[3], 0, UINT8_MAX);
        DEST.i8[4] = std::clamp<int16_t>(DEST.i8[4] - SRC.i8[4], 0, UINT8_MAX);
        DEST.i8[5] = std::clamp<int16_t>(DEST.i8[5] - SRC.i8[5], 0, UINT8_MAX);
        DEST.i8[6] = std::clamp<int16_t>(DEST.i8[6] - SRC.i8[6], 0, UINT8_MAX);
        DEST.i8[7] = std::clamp<int16_t>(DEST.i8[7] - SRC.i8[7], 0, UINT8_MAX);
        DEST.i8[8] = std::clamp<int16_t>(DEST.i8[8] - SRC.i8[8], 0, UINT8_MAX);
        DEST.i8[9] = std::clamp<int16_t>(DEST.i8[9] - SRC.i8[9], 0, UINT8_MAX);
        DEST.i8[10] = std::clamp<int16_t>(DEST.i8[10] - SRC.i8[10], 0, UINT8_MAX);
        DEST.i8[11] = std::clamp<int16_t>(DEST.i8[11] - SRC.i8[11], 0, UINT8_MAX);
        DEST.i8[12] = std::clamp<int16_t>(DEST.i8[12] - SRC.i8[12], 0, UINT8_MAX);
        DEST.i8[13] = std::clamp<int16_t>(DEST.i8[13] - SRC.i8[13], 0, UINT8_MAX);
        DEST.i8[14] = std::clamp<int16_t>(DEST.i8[14] - SRC.i8[14], 0, UINT8_MAX);
        DEST.i8[15] = std::clamp<int16_t>(DEST.i8[15] - SRC.i8[15], 0, UINT8_MAX);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PSUBUSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSUBUSW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = std::clamp<int32_t>(DEST.i16[0] - SRC.i16[0], 0, UINT16_MAX);
        DEST.i16[1] = std::clamp<int32_t>(DEST.i16[1] - SRC.i16[1], 0, UINT16_MAX);
        DEST.i16[2] = std::clamp<int32_t>(DEST.i16[2] - SRC.i16[2], 0, UINT16_MAX);
        DEST.i16[3] = std::clamp<int32_t>(DEST.i16[3] - SRC.i16[3], 0, UINT16_MAX);
        DEST.i16[4] = std::clamp<int32_t>(DEST.i16[4] - SRC.i16[4], 0, UINT16_MAX);
        DEST.i16[5] = std::clamp<int32_t>(DEST.i16[5] - SRC.i16[5], 0, UINT16_MAX);
        DEST.i16[6] = std::clamp<int32_t>(DEST.i16[6] - SRC.i16[6], 0, UINT16_MAX);
        DEST.i16[7] = std::clamp<int32_t>(DEST.i16[7] - SRC.i16[7], 0, UINT16_MAX);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PUNPCKHBW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PUNPCKHBW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i8[0] = DEST.i8[8];
        DEST.i8[1] = SRC.i8[8];
        DEST.i8[2] = DEST.i8[9];
        DEST.i8[3] = SRC.i8[9];
        DEST.i8[4] = DEST.i8[10];
        DEST.i8[5] = SRC.i8[10];
        DEST.i8[6] = DEST.i8[11];
        DEST.i8[7] = SRC.i8[11];
        DEST.i8[8] = DEST.i8[12];
        DEST.i8[9] = SRC.i8[12];
        DEST.i8[10] = DEST.i8[13];
        DEST.i8[11] = SRC.i8[13];
        DEST.i8[12] = DEST.i8[14];
        DEST.i8[13] = SRC.i8[14];
        DEST.i8[14] = DEST.i8[15];
        DEST.i8[15] = SRC.i8[15];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PUNPCKHWD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PUNPCKHWD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = DEST.i16[4];
        DEST.i16[1] = SRC.i16[4];
        DEST.i16[2] = DEST.i16[5];
        DEST.i16[3] = SRC.i16[5];
        DEST.i16[4] = DEST.i16[6];
        DEST.i16[5] = SRC.i16[6];
        DEST.i16[6] = DEST.i16[7];
        DEST.i16[7] = SRC.i16[7];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PUNPCKHDQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PUNPCKHDQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32[0] = DEST.i32[2];
        DEST.i32[1] = SRC.i32[2];
        DEST.i32[2] = DEST.i32[3];
        DEST.i32[3] = SRC.i32[3];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PUNPCKHQDQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PUNPCKHQDQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = DEST.i64[1];
        DEST.i64[1] = SRC.i64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PUNPCKLBW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PUNPCKLBW", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i8[0] = DEST.i8[0];
        DEST.i8[2] = DEST.i8[1];
        DEST.i8[4] = DEST.i8[2];
        DEST.i8[6] = DEST.i8[3];
        DEST.i8[8] = DEST.i8[4];
        DEST.i8[10] = DEST.i8[5];
        DEST.i8[12] = DEST.i8[6];
        DEST.i8[14] = DEST.i8[7];
        DEST.i8[1] = SRC.i8[0];
        DEST.i8[3] = SRC.i8[1];
        DEST.i8[5] = SRC.i8[2];
        DEST.i8[7] = SRC.i8[3];
        DEST.i8[9] = SRC.i8[4];
        DEST.i8[11] = SRC.i8[5];
        DEST.i8[13] = SRC.i8[6];
        DEST.i8[15] = SRC.i8[7];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PUNPCKLWD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PUNPCKLWD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i16[0] = DEST.i16[0];
        DEST.i16[2] = DEST.i16[1];
        DEST.i16[4] = DEST.i16[2];
        DEST.i16[6] = DEST.i16[3];
        DEST.i16[1] = SRC.i16[0];
        DEST.i16[3] = SRC.i16[1];
        DEST.i16[5] = SRC.i16[2];
        DEST.i16[7] = SRC.i16[3];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PUNPCKLDQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PUNPCKLDQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32[0] = DEST.i32[0];
        DEST.i32[2] = DEST.i32[1];
        DEST.i32[1] = SRC.i32[0];
        DEST.i32[3] = SRC.i32[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PUNPCKLQDQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PUNPCKLQDQ", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = DEST.i64[0];
        DEST.i64[1] = SRC.i64[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PXOR(Format& format, const uint8_t* opcode)
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
void sse_instruction::SHUFPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "SHUFPD", 2, 8, SSE_REGISTER | OPERAND_SIZE | DIRECTION | THREE_OPERAND);

    BEGIN_OPERATION() {
        auto SEL = SRC2.u8[0];
        DEST.f64[0] = SRC.f64[(SEL >> 0) & 0x1];
        DEST.f64[1] = SRC.f64[(SEL >> 1) & 0x1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::SQRTPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "SQRTPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = sqrt(SRC.f64[0]);
        DEST.f64[1] = sqrt(SRC.f64[1]);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::SQRTSD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "SQRTSD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = sqrt(SRC.f64[0]);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::SUBPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "SUBPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = DEST.f64[0] - SRC.f64[0];
        DEST.f64[1] = DEST.f64[1] - SRC.f64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::SUBSD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "SUBSD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = DEST.f64[0] - SRC.f64[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::UCOMISD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "UCOMISD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        OF = 0;
        SF = 0;
        AF = 0;
        if (DEST.f64[0] == SRC.f64[0]) {
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
void sse_instruction::UNPCKHPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "UNPCKHPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = DEST.f64[1];
        DEST.f64[1] = SRC.f64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::UNPCKLPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "UNPCKLPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f64[0] = DEST.f64[0];
        DEST.f64[1] = SRC.f64[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::XORPD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "XORPD", 2, 0, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64[0] = DEST.i64[0] ^ SRC.i64[0];
        DEST.i64[1] = DEST.i64[1] ^ SRC.i64[1];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
