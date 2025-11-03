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
void sse_instruction::ADDPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "ADDPS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32 = DEST.f32 + SRC.f32;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::ADDSS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "ADDSS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32[0] = DEST.f32[0] + SRC.f32[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::ANDNPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "ANDNPS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32 = ~DEST.i32 & SRC.i32;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::ANDPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "ANDPS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32 = DEST.i32 & SRC.i32;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::CMPPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CMPPS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION | THREE_OPERAND | IMM_8BIT);

    switch (format.operand[2].displacement) {
    case 0: BEGIN_OPERATION() { DEST.i32 = (DEST.f32 == SRC.f32);   } END_OPERATION_SSE;    break;
    case 1: BEGIN_OPERATION() { DEST.i32 = (DEST.f32 < SRC.f32);    } END_OPERATION_SSE;    break;
    case 2: BEGIN_OPERATION() { DEST.i32 = (DEST.f32 <= SRC.f32);   } END_OPERATION_SSE;    break;
    case 3:
        BEGIN_OPERATION() {
            DEST.i32[0] = isunordered(DEST.f32[0], SRC.f32[0]) ? -1 : 0;
            DEST.i32[1] = isunordered(DEST.f32[1], SRC.f32[1]) ? -1 : 0;
            DEST.i32[2] = isunordered(DEST.f32[2], SRC.f32[2]) ? -1 : 0;
            DEST.i32[3] = isunordered(DEST.f32[3], SRC.f32[3]) ? -1 : 0;
        } END_OPERATION_SSE;
        break;
    case 4: BEGIN_OPERATION() { DEST.i32 = (DEST.f32 != SRC.f32);   } END_OPERATION_SSE;    break;
    case 5: BEGIN_OPERATION() { DEST.i32 = (DEST.f32 >= SRC.f32);   } END_OPERATION_SSE;    break;
    case 6: BEGIN_OPERATION() { DEST.i32 = (DEST.f32 > SRC.f32);    } END_OPERATION_SSE;    break;
    case 7:
        BEGIN_OPERATION() {
            DEST.i32[0] = isunordered(DEST.f32[0], SRC.f32[0]) == false ? -1 : 0;
            DEST.i32[1] = isunordered(DEST.f32[1], SRC.f32[1]) == false ? -1 : 0;
            DEST.i32[2] = isunordered(DEST.f32[2], SRC.f32[2]) == false ? -1 : 0;
            DEST.i32[3] = isunordered(DEST.f32[3], SRC.f32[3]) == false ? -1 : 0;
        } END_OPERATION_SSE;
        break;
    }
}
//------------------------------------------------------------------------------
void sse_instruction::CMPSS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CMPSS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION | THREE_OPERAND | IMM_8BIT);

    switch (format.operand[2].displacement) {
    case 0: BEGIN_OPERATION() { DEST.i32[0] = (DEST.f32[0] == SRC.f32[0]) ? -1 : 0;                     } END_OPERATION_SSE;    break;
    case 1: BEGIN_OPERATION() { DEST.i32[0] = (DEST.f32[0] < SRC.f32[0]) ? -1 : 0;                      } END_OPERATION_SSE;    break;
    case 2: BEGIN_OPERATION() { DEST.i32[0] = (DEST.f32[0] <= SRC.f32[0]) ? -1 : 0;                     } END_OPERATION_SSE;    break;
    case 3: BEGIN_OPERATION() { DEST.i32[0] = isunordered(DEST.f32[0], SRC.f32[0]) ? -1 : 0;            } END_OPERATION_SSE;    break;
    case 4: BEGIN_OPERATION() { DEST.i32[0] = (DEST.f32[0] != SRC.f32[0]) ? -1 : 0;                     } END_OPERATION_SSE;    break;
    case 5: BEGIN_OPERATION() { DEST.i32[0] = (DEST.f32[0] >= SRC.f32[0]) ? -1 : 0;                     } END_OPERATION_SSE;    break;
    case 6: BEGIN_OPERATION() { DEST.i32[0] = (DEST.f32[0] > SRC.f32[0]) ? -1 : 0;                      } END_OPERATION_SSE;    break;
    case 7: BEGIN_OPERATION() { DEST.i32[0] = isunordered(DEST.f32[0], SRC.f32[0]) == false ? -1 : 0;   } END_OPERATION_SSE;    break;
    }
}
//------------------------------------------------------------------------------
void sse_instruction::COMISS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "COMISS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        if (isunordered(DEST.f32[0], SRC.f32[0])) {
            ZF = 0;
            PF = 1;
            CF = 1;
        }
        else if (DEST.f32[0] > SRC.f32[0]) {
            ZF = 0;
            PF = 0;
            CF = 0;
        }
        else if (DEST.f32[0] < SRC.f32[0]) {
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
void sse_instruction::CVTPI2PS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTPI2PS", 2, MMX_REGISTER | OPERAND_SIZE | DIRECTION);
    format.width = 64;
    format.operand[0].type = Format::Operand::SSE;

    BEGIN_OPERATION() {
        DEST.f32 = {
            float(SRC.i32[0]),
            float(SRC.i32[1]),
        };
    } END_OPERATION_SSE_MMX;
}
//------------------------------------------------------------------------------
void sse_instruction::CVTSI2SS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTSI2SS", 2, OPERAND_SIZE | DIRECTION);
    format.width = 32;
    format.operand[0].type = Format::Operand::SSE;

    BEGIN_OPERATION() {
        DEST.f32[0] = SRC.i32[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::CVTPS2PI(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTPS2PI", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);
    format.width = 64;
    format.operand[0].type = Format::Operand::MMX;

    BEGIN_OPERATION() {
        switch (MXCSR_RC) {
        case RoundNearest:
            DEST.i32 = {
                int32_t(roundf(SRC.f32[0])),
                int32_t(roundf(SRC.f32[1])),
            };
            break;
        case RoundDown:
            DEST.i32 = {
                int32_t(floorf(SRC.f32[0])),
                int32_t(floorf(SRC.f32[1])),
            };
            break;
        case RoundUp:
            DEST.i32 = {
                int32_t(ceilf(SRC.f32[0])),
                int32_t(ceilf(SRC.f32[1])),
            };
            break;
        case RoundChop:
            DEST.i32 = {
                int32_t(truncf(SRC.f32[0])),
                int32_t(truncf(SRC.f32[1])),
            };
            break;
        }
    } END_OPERATION_MMX_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::CVTSS2SI(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTSS2SI", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);
    format.width = 32;
    format.operand[0].type = Format::Operand::REG;

    BEGIN_OPERATION() {
        switch (MXCSR_RC) {
        case RoundNearest:
            DEST.d = roundf(SRC.f32[0]);
            break;
        case RoundDown:
            DEST.d = floorf(SRC.f32[0]);
            break;
        case RoundUp:
            DEST.d = ceilf(SRC.f32[0]);
            break;
        case RoundChop:
            DEST.d = truncf(SRC.f32[0]);
            break;
        }
    } END_OPERATION_REG_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::CVTTPS2PI(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTTPS2PI", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);
    format.width = 64;
    format.operand[0].type = Format::Operand::MMX;

    BEGIN_OPERATION() {
        DEST.i32 = {
            int32_t(SRC.f32[0]),
            int32_t(SRC.f32[1]),
        };
    } END_OPERATION_MMX_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::CVTTSS2SI(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTTSS2SI", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);
    format.width = 32;
    format.operand[0].type = Format::Operand::REG;

    BEGIN_OPERATION() {
        DEST.d = SRC.f32[0];
    } END_OPERATION_REG_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::DIVPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "DIVPS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32 = DEST.f32 / SRC.f32;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::DIVSS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "DIVSS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32[0] = DEST.f32[0] / SRC.f32[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::LDMXCSR(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "LDMXCSR", 2, OPERAND_SIZE | DIRECTION);
    format.operand[0].flags = Format::Operand::HIDE;

    BEGIN_OPERATION() {
        MXCSR = uint32_t(SRC);
    } END_OPERATION_RANGE(32, 32);
}
//------------------------------------------------------------------------------
void sse_instruction::MAXPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MAXPS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32 = Max(DEST.f32, SRC.f32);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MAXSS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MAXSS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32[0] = Max(DEST.f32[0], SRC.f32[0]);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MINPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MINPS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32 = Min(DEST.f32, SRC.f32);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MINSS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MINSS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32[0] = Min(DEST.f32[0], SRC.f32[0]);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MOVAPS(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x28:  Decode(format, opcode, "MOVAPS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);    break;
    case 0x29:  Decode(format, opcode, "MOVAPS", 2, SSE_REGISTER | OPERAND_SIZE);                break;
    }

    BEGIN_OPERATION() {
        DEST = SRC;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MOVHPS(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x16:  Decode(format, opcode, "MOVHPS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);    break;
    case 0x17:  Decode(format, opcode, "MOVHPS", 2, SSE_REGISTER | OPERAND_SIZE);                break;
    }

    // Since Katmai B0 Stepping
    if (format.operand[0].type == Format::Operand::SSE && format.operand[1].type == Format::Operand::SSE) {
        format.instruction = "MOVLHPS";
    }

    if (format.operand[0].type == Format::Operand::ADR) {
        BEGIN_OPERATION() {
            DEST.f32[0] = SRC.f32[2];
            DEST.f32[1] = SRC.f32[3];
        } END_OPERATION_SSE;
    }
    else {
        BEGIN_OPERATION() {
            DEST.f32[2] = SRC.f32[0];
            DEST.f32[3] = SRC.f32[1];
        } END_OPERATION_SSE;
    }
}
//------------------------------------------------------------------------------
void sse_instruction::MOVLPS(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x12:  Decode(format, opcode, "MOVLPS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);    break;
    case 0x13:  Decode(format, opcode, "MOVLPS", 2, SSE_REGISTER | OPERAND_SIZE);                break;
    }

    // Since Katmai B0 Stepping
    if (format.operand[0].type == Format::Operand::SSE && format.operand[1].type == Format::Operand::SSE) {
        format.instruction = "MOVHLPS";
    }

    if (format.operand[0].type == Format::Operand::SSE && format.operand[1].type == Format::Operand::SSE) {
        BEGIN_OPERATION() {
            DEST.f32[0] = SRC.f32[2];
            DEST.f32[1] = SRC.f32[3];
        } END_OPERATION_SSE;
    }
    else {
        BEGIN_OPERATION() {
            DEST.f32[0] = SRC.f32[0];
            DEST.f32[1] = SRC.f32[1];
        } END_OPERATION_SSE;
    }
}
//------------------------------------------------------------------------------
void sse_instruction::MOVMSKPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVMSKPS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);
    format.operand[0].type = Format::Operand::REG;

    BEGIN_OPERATION() {
        DEST.d = 0;
        DEST.d |= SRC.i32[0] & 0x80000000 ? 1 : 0;
        DEST.d |= SRC.i32[1] & 0x80000000 ? 2 : 0;
        DEST.d |= SRC.i32[2] & 0x80000000 ? 4 : 0;
        DEST.d |= SRC.i32[3] & 0x80000000 ? 8 : 0;
    } END_OPERATION_REG_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MOVNTPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVNTPS", 2, SSE_REGISTER | OPERAND_SIZE);

    BEGIN_OPERATION() {
        DEST = SRC;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MOVSS(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x10:  Decode(format, opcode, "MOVSS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION); break;
    case 0x11:  Decode(format, opcode, "MOVSS", 2, SSE_REGISTER | OPERAND_SIZE);             break;
    }

    if (format.operand[0].type == Format::Operand::SSE && format.operand[1].type != Format::Operand::SSE) {
        BEGIN_OPERATION() {
            DEST.f32 = { SRC.f32[0] };
        } END_OPERATION_SSE;
    }
    else {
        BEGIN_OPERATION() {
            DEST.f32[0] = SRC.f32[0];
        } END_OPERATION_SSE;
    }
}
//------------------------------------------------------------------------------
void sse_instruction::MOVUPS(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x10:  Decode(format, opcode, "MOVUPS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);    break;
    case 0x11:  Decode(format, opcode, "MOVUPS", 2, SSE_REGISTER | OPERAND_SIZE);                break;
    }

    BEGIN_OPERATION() {
        DEST = SRC;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MULPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MULPS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32 = DEST.f32 * SRC.f32;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::MULSS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MULSS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32[0] = DEST.f32[0] * SRC.f32[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::ORPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "ORPS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32 = DEST.i32 | SRC.i32;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::PREFETCH0(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PREFETCH0", 2);
    format.operand_count = 1;

    OPERATION() {};
}
//------------------------------------------------------------------------------
void sse_instruction::PREFETCH1(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PREFETCH1", 2);
    format.operand_count = 1;

    OPERATION() {};
}
//------------------------------------------------------------------------------
void sse_instruction::PREFETCH2(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PREFETCH2", 2);
    format.operand_count = 1;

    OPERATION() {};
}
//------------------------------------------------------------------------------
void sse_instruction::PREFETCHNTA(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PREFETCHNTA", 2);
    format.operand_count = 1;

    OPERATION() {};
}
//------------------------------------------------------------------------------
void sse_instruction::RCPPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "RCPPS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32 = 1.0f / SRC.f32;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::RCPSS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "RCPSS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32[0] = 1.0f / SRC.f32[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::RSQRTPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "RSQRTPS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32 = 1.0f / Sqrt(SRC.f32);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::RSQRTSS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "RSQRTSS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32[0] = 1.0f / sqrtf(SRC.f32[0]);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::SFENCE(Format& format, const uint8_t* opcode)
{
    format.instruction = "SFENCE";
    format.length = 3;
    format.operand_count = 0;

    OPERATION() {};
}
//------------------------------------------------------------------------------
void sse_instruction::SHUFPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "SHUFPS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION | THREE_OPERAND | IMM_8BIT);

    BEGIN_OPERATION() {
        auto SEL = SRC2.u8[0];
        DEST.f32 = {
            SRC.f32[(SEL >> 0) & 0x3],
            SRC.f32[(SEL >> 2) & 0x3],
            SRC.f32[(SEL >> 4) & 0x3],
            SRC.f32[(SEL >> 6) & 0x3],
        };
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::SQRTPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "SQRTPS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32 = Sqrt(SRC.f32);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::SQRTSS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "SQRTSS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32[0] = sqrtf(SRC.f32[0]);
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::STMXCSR(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "STMXCSR", 2, OPERAND_SIZE);
    format.operand_count = 1;

    BEGIN_OPERATION() {
        DEST = MXCSR;
    } END_OPERATION_RANGE(32, 32);
}
//------------------------------------------------------------------------------
void sse_instruction::SUBPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "SUBPS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32 = DEST.f32 - SRC.f32;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::SUBSS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "SUBSS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32[0] = DEST.f32[0] - SRC.f32[0];
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::UCOMISS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "UCOMISS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        if (isunordered(DEST.f32[0], SRC.f32[0])) {
            ZF = 1;
            PF = 1;
            CF = 1;
        }
        else if (DEST.f32[0] > SRC.f32[0]) {
            ZF = 0;
            PF = 0;
            CF = 0;
        }
        else if (DEST.f32[0] < SRC.f32[0]) {
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
void sse_instruction::UNPCKHPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "UNPCKHPS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32 = {
            DEST.f32[2],
            SRC.f32[2],
            DEST.f32[3],
            SRC.f32[3],
        };
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::UNPCKLPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "UNPCKLPS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.f32 = {
            DEST.f32[0],
            SRC.f32[0],
            DEST.f32[1],
            SRC.f32[1],
        };
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
void sse_instruction::XORPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "XORPS", 2, SSE_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i32 = DEST.i32 ^ SRC.i32;
    } END_OPERATION_SSE;
}
//------------------------------------------------------------------------------
