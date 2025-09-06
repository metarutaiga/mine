#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"
#include "mmx_register.h"
#include "mmx_register.inl"
#include "mmx_instruction.h"
#include "sse_register.h"
#include "sse_register.inl"
#include "sse_instruction.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void sse_instruction::ADDPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "ADDPS", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = XMM(format.operand[0].base);
        auto SRC = CastXMM(format.operand[1]);
        DEST.f32[0] = DEST.f32[0] + SRC.f32[0];
        DEST.f32[1] = DEST.f32[1] + SRC.f32[1];
        DEST.f32[2] = DEST.f32[2] + SRC.f32[2];
        DEST.f32[3] = DEST.f32[3] + SRC.f32[3];
    };
}
//------------------------------------------------------------------------------
void sse_instruction::ANDNPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "ANDNPS", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = XMM(format.operand[0].base);
        auto SRC = CastXMM(format.operand[1]);
        DEST.i32[0] = ~DEST.i32[0] & SRC.i32[0];
        DEST.i32[1] = ~DEST.i32[1] & SRC.i32[1];
        DEST.i32[2] = ~DEST.i32[2] & SRC.i32[2];
        DEST.i32[3] = ~DEST.i32[3] & SRC.i32[3];
    };
}
//------------------------------------------------------------------------------
void sse_instruction::ANDPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "ANDPS", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = XMM(format.operand[0].base);
        auto SRC = CastXMM(format.operand[1]);
        DEST.i32[0] = DEST.i32[0] & SRC.i32[0];
        DEST.i32[1] = DEST.i32[1] & SRC.i32[1];
        DEST.i32[2] = DEST.i32[2] & SRC.i32[2];
        DEST.i32[3] = DEST.i32[3] & SRC.i32[3];
    };
}
//------------------------------------------------------------------------------
void sse_instruction::CMPPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CMPPS", 2, 8, OPERAND_SIZE | DIRECTION);

    switch (format.operand[2].displacement) {
    case 0:
        OPERATION() {
            auto& DEST = XMM(format.operand[0].base);
            auto SRC = CastXMM(format.operand[1]);
            DEST.f32[0] = (DEST.f32[0] == SRC.f32[0]) ? 0xFFFFFFFF : 0;
            DEST.f32[1] = (DEST.f32[1] == SRC.f32[1]) ? 0xFFFFFFFF : 0;
            DEST.f32[2] = (DEST.f32[2] == SRC.f32[2]) ? 0xFFFFFFFF : 0;
            DEST.f32[3] = (DEST.f32[3] == SRC.f32[3]) ? 0xFFFFFFFF : 0;
        };
        break;
    case 1:
        OPERATION() {
            auto& DEST = XMM(format.operand[0].base);
            auto SRC = CastXMM(format.operand[1]);
            DEST.f32[0] = (DEST.f32[0] < SRC.f32[0]) ? 0xFFFFFFFF : 0;
            DEST.f32[1] = (DEST.f32[1] < SRC.f32[1]) ? 0xFFFFFFFF : 0;
            DEST.f32[2] = (DEST.f32[2] < SRC.f32[2]) ? 0xFFFFFFFF : 0;
            DEST.f32[3] = (DEST.f32[3] < SRC.f32[3]) ? 0xFFFFFFFF : 0;
        };
        break;
    case 2:
        OPERATION() {
            auto& DEST = XMM(format.operand[0].base);
            auto SRC = CastXMM(format.operand[1]);
            DEST.f32[0] = (DEST.f32[0] <= SRC.f32[0]) ? 0xFFFFFFFF : 0;
            DEST.f32[1] = (DEST.f32[1] <= SRC.f32[1]) ? 0xFFFFFFFF : 0;
            DEST.f32[2] = (DEST.f32[2] <= SRC.f32[2]) ? 0xFFFFFFFF : 0;
            DEST.f32[3] = (DEST.f32[3] <= SRC.f32[3]) ? 0xFFFFFFFF : 0;
        };
        break;
    case 3:
        OPERATION() {
            auto& DEST = XMM(format.operand[0].base);
            auto SRC = CastXMM(format.operand[1]);
            DEST.f32[0] = (DEST.f32[0] == SRC.f32[0]) ? 0xFFFFFFFF : 0;
            DEST.f32[1] = (DEST.f32[1] == SRC.f32[1]) ? 0xFFFFFFFF : 0;
            DEST.f32[2] = (DEST.f32[2] == SRC.f32[2]) ? 0xFFFFFFFF : 0;
            DEST.f32[3] = (DEST.f32[3] == SRC.f32[3]) ? 0xFFFFFFFF : 0;
        };
        break;
    case 4:
        OPERATION() {
            auto& DEST = XMM(format.operand[0].base);
            auto SRC = CastXMM(format.operand[1]);
            DEST.f32[0] = (DEST.f32[0] != SRC.f32[0]) ? 0xFFFFFFFF : 0;
            DEST.f32[1] = (DEST.f32[1] != SRC.f32[1]) ? 0xFFFFFFFF : 0;
            DEST.f32[2] = (DEST.f32[2] != SRC.f32[2]) ? 0xFFFFFFFF : 0;
            DEST.f32[3] = (DEST.f32[3] != SRC.f32[3]) ? 0xFFFFFFFF : 0;
        };
        break;
    case 5:
        OPERATION() {
            auto& DEST = XMM(format.operand[0].base);
            auto SRC = CastXMM(format.operand[1]);
            DEST.f32[0] = (DEST.f32[0] >= SRC.f32[0]) ? 0xFFFFFFFF : 0;
            DEST.f32[1] = (DEST.f32[1] >= SRC.f32[1]) ? 0xFFFFFFFF : 0;
            DEST.f32[2] = (DEST.f32[2] >= SRC.f32[2]) ? 0xFFFFFFFF : 0;
            DEST.f32[3] = (DEST.f32[3] >= SRC.f32[3]) ? 0xFFFFFFFF : 0;
        };
        break;
    case 6:
        OPERATION() {
            auto& DEST = XMM(format.operand[0].base);
            auto SRC = CastXMM(format.operand[1]);
            DEST.f32[0] = (DEST.f32[0] > SRC.f32[0]) ? 0xFFFFFFFF : 0;
            DEST.f32[1] = (DEST.f32[1] > SRC.f32[1]) ? 0xFFFFFFFF : 0;
            DEST.f32[2] = (DEST.f32[2] > SRC.f32[2]) ? 0xFFFFFFFF : 0;
            DEST.f32[3] = (DEST.f32[3] > SRC.f32[3]) ? 0xFFFFFFFF : 0;
        };
        break;
    case 7:
        OPERATION() {
            auto& DEST = XMM(format.operand[0].base);
            auto SRC = CastXMM(format.operand[1]);
            DEST.f32[0] = (DEST.f32[0] != SRC.f32[0]) ? 0xFFFFFFFF : 0;
            DEST.f32[1] = (DEST.f32[1] != SRC.f32[1]) ? 0xFFFFFFFF : 0;
            DEST.f32[2] = (DEST.f32[2] != SRC.f32[2]) ? 0xFFFFFFFF : 0;
            DEST.f32[3] = (DEST.f32[3] != SRC.f32[3]) ? 0xFFFFFFFF : 0;
        };
        break;
    }
}
//------------------------------------------------------------------------------
void sse_instruction::COMISS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "COMISS", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = XMM(format.operand[0].base);
        auto SRC = CastXMM(format.operand[1]);
        OF = 0;
        SF = 0;
        AF = 0;
        if (DEST.f32[0] == SRC.f32[0]) {
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
    };
}
//------------------------------------------------------------------------------
void sse_instruction::CVTPI2PS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTPI2PS", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = XMM(format.operand[0].base);
        auto SRC = CastMM(format.operand[1]);
        DEST.f32[0] = SRC.i32[0];
        DEST.f32[1] = SRC.i32[1];
    };
}
//------------------------------------------------------------------------------
void sse_instruction::CVTPS2PI(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTPS2PI", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = MM(format.operand[0].base);
        auto SRC = CastXMM(format.operand[1]);
        DEST.i32[0] = SRC.f32[0];
        DEST.i32[1] = SRC.f32[1];
    };
}
//------------------------------------------------------------------------------
void sse_instruction::CVTTPS2PI(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CVTTPS2PI", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = MM(format.operand[0].base);
        auto SRC = CastXMM(format.operand[1]);
        DEST.i32[0] = SRC.f32[0];
        DEST.i32[1] = SRC.f32[1];
    };
}
//------------------------------------------------------------------------------
void sse_instruction::DIVPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "DIVPS", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = XMM(format.operand[0].base);
        auto SRC = CastXMM(format.operand[1]);
        DEST.f32[0] = DEST.f32[0] / SRC.f32[0];
        DEST.f32[1] = DEST.f32[1] / SRC.f32[1];
        DEST.f32[2] = DEST.f32[2] / SRC.f32[2];
        DEST.f32[3] = DEST.f32[3] / SRC.f32[3];
    };
}
//------------------------------------------------------------------------------
void sse_instruction::LDMXCSR(Format& format, const uint8_t* opcode)
{
    format.length = 3;
    format.instruction = "LDMXCSR";
    format.operand[0].type = Format::Operand::REG;
    format.operand[0].base = opcode[2] & 0b111;

    OPERATION() {
        MXCSR.d = REG(format.operand[0].base).d;
    };
}
//------------------------------------------------------------------------------
void sse_instruction::MASKMOVQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MASKMOVQ", 2, 0, OPERAND_SIZE | DIRECTION);
    format.operand[2] = format.operand[1];
    format.operand[1] = format.operand[0];
    format.operand[0].type = Format::Operand::ADR;
    format.operand[0].base = IndexREG(EDI);

    OPERATION() {
        auto& DEST = CastMM(format.operand[0]);
        auto SRC1 = MM(format.operand[1].base);
        auto SRC2 = MM(format.operand[2].base);
        DEST.i8[0] = (SRC2.i8[0] == 1) ? SRC1.i8[0] : 0;
        DEST.i8[1] = (SRC2.i8[1] == 1) ? SRC1.i8[1] : 0;
        DEST.i8[2] = (SRC2.i8[2] == 1) ? SRC1.i8[2] : 0;
        DEST.i8[3] = (SRC2.i8[3] == 1) ? SRC1.i8[3] : 0;
        DEST.i8[4] = (SRC2.i8[4] == 1) ? SRC1.i8[4] : 0;
        DEST.i8[5] = (SRC2.i8[5] == 1) ? SRC1.i8[5] : 0;
        DEST.i8[6] = (SRC2.i8[6] == 1) ? SRC1.i8[6] : 0;
        DEST.i8[7] = (SRC2.i8[7] == 1) ? SRC1.i8[7] : 0;
    };
}
//------------------------------------------------------------------------------
void sse_instruction::MAXPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MAXPS", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = XMM(format.operand[0].base);
        auto SRC = CastXMM(format.operand[1]);
        DEST.f32[0] = std::max(DEST.f32[0], SRC.f32[0]);
        DEST.f32[1] = std::max(DEST.f32[1], SRC.f32[1]);
        DEST.f32[2] = std::max(DEST.f32[2], SRC.f32[2]);
        DEST.f32[3] = std::max(DEST.f32[3], SRC.f32[3]);
    };
}
//------------------------------------------------------------------------------
void sse_instruction::MINPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MINPS", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = XMM(format.operand[0].base);
        auto SRC = CastXMM(format.operand[1]);
        DEST.f32[0] = std::min(DEST.f32[0], SRC.f32[0]);
        DEST.f32[1] = std::min(DEST.f32[1], SRC.f32[1]);
        DEST.f32[2] = std::min(DEST.f32[2], SRC.f32[2]);
        DEST.f32[3] = std::min(DEST.f32[3], SRC.f32[3]);
    };
}
//------------------------------------------------------------------------------
void sse_instruction::MOVAPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVAPS", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = CastXMM(format.operand[0]);
        auto SRC = CastXMM(format.operand[1]);
        DEST = SRC;
    };
}
//------------------------------------------------------------------------------
void sse_instruction::MOVHPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVHPS", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = XMM(format.operand[0].base);
        auto SRC = CastXMM(format.operand[1]);
        DEST.f32[2] = SRC.f32[0];
        DEST.f32[3] = SRC.f32[1];
    };
}
//------------------------------------------------------------------------------
void sse_instruction::MOVLPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVLPS", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = XMM(format.operand[0].base);
        auto SRC = CastXMM(format.operand[1]);
        DEST.f32[0] = SRC.f32[0];
        DEST.f32[1] = SRC.f32[1];
    };
}
//------------------------------------------------------------------------------
void sse_instruction::MOVMSKPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVMSKPS", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = REG(format.operand[0].base).d;
        auto SRC = XMM(format.operand[1].base);
        DEST = 0;
        DEST |= SRC.i32[0] & 0x80000000 ? 1 : 0;
        DEST |= SRC.i32[1] & 0x80000000 ? 2 : 0;
        DEST |= SRC.i32[2] & 0x80000000 ? 4 : 0;
        DEST |= SRC.i32[3] & 0x80000000 ? 8 : 0;
    };
}
//------------------------------------------------------------------------------
void sse_instruction::MOVNTPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVNTPS", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = CastXMM(format.operand[0]);
        auto SRC = CastXMM(format.operand[1]);
        DEST = SRC;
    };
}
//------------------------------------------------------------------------------
void sse_instruction::MOVNTQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVNTQ", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = CastMM(format.operand[0]);
        auto SRC = CastMM(format.operand[1]);
        DEST = SRC;
    };
}
//------------------------------------------------------------------------------
void sse_instruction::MOVUPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVUPS", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = CastXMM(format.operand[0]);
        auto SRC = CastXMM(format.operand[1]);
        DEST = SRC;
    };
}
//------------------------------------------------------------------------------
void sse_instruction::MULPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MULPS", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = XMM(format.operand[0].base);
        auto SRC = CastXMM(format.operand[1]);
        DEST.f32[0] = DEST.f32[0] * SRC.f32[0];
        DEST.f32[1] = DEST.f32[1] * SRC.f32[1];
        DEST.f32[2] = DEST.f32[2] * SRC.f32[2];
        DEST.f32[3] = DEST.f32[3] * SRC.f32[3];
    };
}
//------------------------------------------------------------------------------
void sse_instruction::ORPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "ORPS", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = XMM(format.operand[0].base);
        auto SRC = CastXMM(format.operand[1]);
        DEST.i32[0] = DEST.i32[0] | SRC.i32[0];
        DEST.i32[1] = DEST.i32[1] | SRC.i32[1];
        DEST.i32[2] = DEST.i32[2] | SRC.i32[2];
        DEST.i32[3] = DEST.i32[3] | SRC.i32[3];
    };
}
//------------------------------------------------------------------------------
void sse_instruction::PAVGB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PAVGB", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = MM(format.operand[0].base);
        auto SRC = CastMM(format.operand[1]);
        DEST.u8[0] = (DEST.u8[0] + SRC.u8[0]) / 2;
        DEST.u8[1] = (DEST.u8[1] + SRC.u8[1]) / 2;
        DEST.u8[2] = (DEST.u8[2] + SRC.u8[2]) / 2;
        DEST.u8[3] = (DEST.u8[3] + SRC.u8[3]) / 2;
        DEST.u8[4] = (DEST.u8[4] + SRC.u8[4]) / 2;
        DEST.u8[5] = (DEST.u8[5] + SRC.u8[5]) / 2;
        DEST.u8[6] = (DEST.u8[6] + SRC.u8[6]) / 2;
        DEST.u8[7] = (DEST.u8[7] + SRC.u8[7]) / 2;
    };
}
//------------------------------------------------------------------------------
void sse_instruction::PAVGW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PAVGW", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = MM(format.operand[0].base);
        auto SRC = CastMM(format.operand[1]);
        DEST.u16[0] = (DEST.u16[0] + SRC.u16[0]) / 2;
        DEST.u16[1] = (DEST.u16[1] + SRC.u16[1]) / 2;
        DEST.u16[2] = (DEST.u16[2] + SRC.u16[2]) / 2;
        DEST.u16[3] = (DEST.u16[3] + SRC.u16[3]) / 2;
    };
}
//------------------------------------------------------------------------------
void sse_instruction::PEXTRW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PEXTRW", 2, 8, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = REG(format.operand[0].base).d;
        auto SRC = MM(format.operand[1].base);
        auto SEL = format.operand[2].displacement % 4;
        DEST = SRC.u16[SEL];
    };
}
//------------------------------------------------------------------------------
void sse_instruction::PINSRW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PINSRW", 2, 8, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto DEST = MM(format.operand[0].base);
        auto SRC = REG(format.operand[1].base).d;
        auto SEL = format.operand[2].displacement % 4;
        DEST.u16[SEL] = SRC;
    };
}
//------------------------------------------------------------------------------
void sse_instruction::PMAXSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMAXSW", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = MM(format.operand[0].base);
        auto SRC = CastMM(format.operand[1]);
        DEST.i16[0] = std::max(DEST.i16[0], SRC.i16[0]);
        DEST.i16[1] = std::max(DEST.i16[1], SRC.i16[1]);
        DEST.i16[2] = std::max(DEST.i16[2], SRC.i16[2]);
        DEST.i16[3] = std::max(DEST.i16[3], SRC.i16[3]);
    };
}
//------------------------------------------------------------------------------
void sse_instruction::PMAXUB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMAXUB", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = MM(format.operand[0].base);
        auto SRC = CastMM(format.operand[1]);
        DEST.u8[0] = std::max(DEST.u8[0], SRC.u8[0]);
        DEST.u8[1] = std::max(DEST.u8[1], SRC.u8[1]);
        DEST.u8[2] = std::max(DEST.u8[2], SRC.u8[2]);
        DEST.u8[3] = std::max(DEST.u8[3], SRC.u8[3]);
        DEST.u8[4] = std::max(DEST.u8[4], SRC.u8[4]);
        DEST.u8[5] = std::max(DEST.u8[5], SRC.u8[5]);
        DEST.u8[6] = std::max(DEST.u8[6], SRC.u8[6]);
        DEST.u8[7] = std::max(DEST.u8[7], SRC.u8[7]);
    };
}
//------------------------------------------------------------------------------
void sse_instruction::PMINSW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMINSW", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = MM(format.operand[0].base);
        auto SRC = CastMM(format.operand[1]);
        DEST.i16[0] = std::min(DEST.i16[0], SRC.i16[0]);
        DEST.i16[1] = std::min(DEST.i16[1], SRC.i16[1]);
        DEST.i16[2] = std::min(DEST.i16[2], SRC.i16[2]);
        DEST.i16[3] = std::min(DEST.i16[3], SRC.i16[3]);
    };
}
//------------------------------------------------------------------------------
void sse_instruction::PMINUB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMINUB", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = MM(format.operand[0].base);
        auto SRC = CastMM(format.operand[1]);
        DEST.u8[0] = std::min(DEST.u8[0], SRC.u8[0]);
        DEST.u8[1] = std::min(DEST.u8[1], SRC.u8[1]);
        DEST.u8[2] = std::min(DEST.u8[2], SRC.u8[2]);
        DEST.u8[3] = std::min(DEST.u8[3], SRC.u8[3]);
        DEST.u8[4] = std::min(DEST.u8[4], SRC.u8[4]);
        DEST.u8[5] = std::min(DEST.u8[5], SRC.u8[5]);
        DEST.u8[6] = std::min(DEST.u8[6], SRC.u8[6]);
        DEST.u8[7] = std::min(DEST.u8[7], SRC.u8[7]);
    };
}
//------------------------------------------------------------------------------
void sse_instruction::PMOVMSKB(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMOVMSKB", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = REG(format.operand[0].base).d;
        auto SRC = MM(format.operand[1].base);
        DEST = 0;
        DEST |= SRC.u8[0] & 0x80 ? 0x01 : 0;
        DEST |= SRC.u8[1] & 0x80 ? 0x02 : 0;
        DEST |= SRC.u8[2] & 0x80 ? 0x04 : 0;
        DEST |= SRC.u8[3] & 0x80 ? 0x08 : 0;
        DEST |= SRC.u8[4] & 0x80 ? 0x10 : 0;
        DEST |= SRC.u8[5] & 0x80 ? 0x20 : 0;
        DEST |= SRC.u8[6] & 0x80 ? 0x40 : 0;
        DEST |= SRC.u8[7] & 0x80 ? 0x80 : 0;
    };
}
//-----------------------------------------------------------------------------
void sse_instruction::PMULHUW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMULHUW", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = MM(format.operand[0].base);
        auto SRC = CastMM(format.operand[1]);
        DEST.u16[0] = (DEST.u16[0] * SRC.u16[0]) >> 16;
        DEST.u16[1] = (DEST.u16[1] * SRC.u16[1]) >> 16;
        DEST.u16[2] = (DEST.u16[2] * SRC.u16[2]) >> 16;
        DEST.u16[3] = (DEST.u16[3] * SRC.u16[3]) >> 16;
    };
}
//------------------------------------------------------------------------------
void sse_instruction::PREFETCH0(Format& format, const uint8_t* opcode)
{
    format.length = 3;
    format.instruction = "PREFETCH0";

    OPERATION() {};
}
//------------------------------------------------------------------------------
void sse_instruction::PREFETCH1(Format& format, const uint8_t* opcode)
{
    format.length = 3;
    format.instruction = "PREFETCH1";

    OPERATION() {};
}
//------------------------------------------------------------------------------
void sse_instruction::PREFETCH2(Format& format, const uint8_t* opcode)
{
    format.length = 3;
    format.instruction = "PREFETCH2";

    OPERATION() {};
}
//------------------------------------------------------------------------------
void sse_instruction::PREFETCHNTA(Format& format, const uint8_t* opcode)
{
    format.length = 3;
    format.instruction = "PREFETCHNTA";

    OPERATION() {};
}
//------------------------------------------------------------------------------
void sse_instruction::PSADBW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSADBW", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = MM(format.operand[0].base);
        auto SRC = CastMM(format.operand[1]);
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
    };
}
//------------------------------------------------------------------------------
void sse_instruction::PSHUFW(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSHUFW", 2, 8, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = MM(format.operand[0].base);
        auto SRC = CastMM(format.operand[1]);
        auto SEL = format.operand[2].displacement;
        DEST.i16[0] = SRC.i16[(SEL >> 0) & 0x3];
        DEST.i16[1] = SRC.i16[(SEL >> 2) & 0x3];
        DEST.i16[2] = SRC.i16[(SEL >> 4) & 0x3];
        DEST.i16[3] = SRC.i16[(SEL >> 6) & 0x3];
    };
}
//------------------------------------------------------------------------------
void sse_instruction::RCPPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "RCPPS", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = XMM(format.operand[0].base);
        auto SRC = CastXMM(format.operand[1]);
        DEST.f32[0] = 1.0f / SRC.f32[0];
        DEST.f32[1] = 1.0f / SRC.f32[1];
        DEST.f32[2] = 1.0f / SRC.f32[2];
        DEST.f32[3] = 1.0f / SRC.f32[3];
    };
}
//------------------------------------------------------------------------------
void sse_instruction::RSQRTPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "RSQRTPS", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = XMM(format.operand[0].base);
        auto SRC = CastXMM(format.operand[1]);
        DEST.f32[0] = 1.0f / sqrtf(SRC.f32[0]);
        DEST.f32[1] = 1.0f / sqrtf(SRC.f32[1]);
        DEST.f32[2] = 1.0f / sqrtf(SRC.f32[2]);
        DEST.f32[3] = 1.0f / sqrtf(SRC.f32[3]);
    };
}
//------------------------------------------------------------------------------
void sse_instruction::SFENCE(Format& format, const uint8_t* opcode)
{
    format.length = 3;
    format.instruction = "SFENCE";

    OPERATION() {};
}
//------------------------------------------------------------------------------
void sse_instruction::SHUFPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "SHUFPS", 2, 8, OPERAND_SIZE | DIRECTION);

    OPERATION() {};
}
//------------------------------------------------------------------------------
void sse_instruction::SQRTPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "SQRTPS", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = XMM(format.operand[0].base);
        auto SRC = CastXMM(format.operand[1]);
        DEST.f32[0] = sqrtf(SRC.f32[0]);
        DEST.f32[1] = sqrtf(SRC.f32[1]);
        DEST.f32[2] = sqrtf(SRC.f32[2]);
        DEST.f32[3] = sqrtf(SRC.f32[3]);
    };
}
//------------------------------------------------------------------------------
void sse_instruction::STMXCSR(Format& format, const uint8_t* opcode)
{
    format.length = 3;
    format.instruction = "STMXCSR";
    format.operand[0].type = Format::Operand::REG;
    format.operand[0].base = opcode[2] & 0b111;

    OPERATION() {
        REG(format.operand[0].base).d = MXCSR.d;
    };
}
//------------------------------------------------------------------------------
void sse_instruction::SUBPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "SUBPS", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = XMM(format.operand[0].base);
        auto SRC = CastXMM(format.operand[1]);
        DEST.f32[0] = DEST.f32[0] - SRC.f32[0];
        DEST.f32[1] = DEST.f32[1] - SRC.f32[1];
        DEST.f32[2] = DEST.f32[2] - SRC.f32[2];
        DEST.f32[3] = DEST.f32[3] - SRC.f32[3];
    };
}
//------------------------------------------------------------------------------
void sse_instruction::UCOMISS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "UCOMISS", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = XMM(format.operand[0].base);
        auto SRC = CastXMM(format.operand[1]);
        OF = 0;
        SF = 0;
        AF = 0;
        if (DEST.f32[0] == SRC.f32[0]) {
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
    };
}
//------------------------------------------------------------------------------
void sse_instruction::UNPCKHPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "UNPCKHPS", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = XMM(format.operand[0].base);
        auto SRC = CastXMM(format.operand[1]);
        DEST.f32[0] = DEST.f32[2];
        DEST.f32[2] = DEST.f32[3];
        DEST.f32[1] = SRC.f32[2];
        DEST.f32[3] = SRC.f32[3];
    };
}
//------------------------------------------------------------------------------
void sse_instruction::UNPCKLPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "UNPCKLPS", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = XMM(format.operand[0].base);
        auto SRC = CastXMM(format.operand[1]);
        DEST.f32[0] = DEST.f32[0];
        DEST.f32[2] = DEST.f32[1];
        DEST.f32[1] = SRC.f32[0];
        DEST.f32[3] = SRC.f32[1];
    };
}
//------------------------------------------------------------------------------
void sse_instruction::XORPS(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "XORPS", 2, 0, OPERAND_SIZE | DIRECTION);

    OPERATION() {
        auto& DEST = XMM(format.operand[0].base);
        auto SRC = CastXMM(format.operand[1]);
        DEST.i32[0] = DEST.i32[0] ^ SRC.i32[0];
        DEST.i32[1] = DEST.i32[1] ^ SRC.i32[1];
        DEST.i32[2] = DEST.i32[2] ^ SRC.i32[2];
        DEST.i32[3] = DEST.i32[3] ^ SRC.i32[3];
    };
}
//------------------------------------------------------------------------------
