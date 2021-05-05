//==============================================================================
// MIPS(R) Architecture For Programmers Volume II-A: The MIPS64(R) Instruction Set Reference Manual
//
// Document Number: MD00087
// Revision 6.06
// December 15, 2016
//
// Copyright (C) Wave Computing, Inc. All rights reserved.
//==============================================================================

#include <float.h>
#include <math.h>
#include "platform.h"
#include "cpu.h"
#include "fpu.h"

//------------------------------------------------------------------------------
#define o &FPU::
#define x , o
//------------------------------------------------------------------------------
// Table A.17 MIPS64 COP1 Encoding of rs Field
//------------------------------------------------------------------------------
const FPU::FINSTRUCTION FPU::tableRS[4 * 8] =
{
    o MFC1  x DMFC1     x CFC1  x MFHC1 x MTC1  x DMTC1     x CTC1  x MTHC1
    x ____  x BC1EQZ    x ____  x ____  x ____  x BC1NEZ    x ____  x ____
    x S     x D         x ____  x ____  x W     x L         x ____  x ____
    x ____  x ____      x ____  x ____  x ____  x ____      x ____  x ____
};
//------------------------------------------------------------------------------
// Table A.18 MIPS64 COP1 Encoding of Function Field When rs=S
//------------------------------------------------------------------------------
const FPU::FINSTRUCTION FPU::tableS[8 * 8] =
{
    o ADD_S     x SUB_S     x MUL_S     x DIV_S     x SQRT_S    x ABS_S     x MOV_S     x NEG_S
    x ROUND_L_S x TRUNC_L_S x CEIL_L_S  x FLOOR_L_S x ROUND_W_S x TRUNC_W_S x CEIL_W_S  x FLOOR_W_S
    x SEL_S     x ____      x ____      x ____      x SELEQZ_S  x RECIP_S   x RSQRT_S   x SELNEZ_S
    x MADDF_S   x MSUBF_S   x RINT_S    x CLASS_S   x MIN_S     x MAX_S     x MINA_S    x MAXA_S
    x ____      x CVT_D_S   x ____      x ____      x CVT_W_S   x CVT_L_S   x ____      x ____
    x ____      x ____      x ____      x ____      x ____      x ____      x ____      x ____
    x ____      x ____      x ____      x ____      x ____      x ____      x ____      x ____
    x ____      x ____      x ____      x ____      x ____      x ____      x ____      x ____
};
//------------------------------------------------------------------------------
// Table A.19 MIPS64 COP1 Encoding of Function Field When rs=D
//------------------------------------------------------------------------------
const FPU::FINSTRUCTION FPU::tableD[8 * 8] =
{
    o ADD_D     x SUB_D     x MUL_D     x DIV_D     x SQRT_D    x ABS_D     x MOV_D     x NEG_D
    x ROUND_L_D x TRUNC_L_D x CEIL_L_D  x FLOOR_L_D x ROUND_W_D x TRUNC_W_D x CEIL_W_D  x FLOOR_W_D
    x SEL_D     x ____      x ____      x ____      x SELEQZ_D  x RECIP_D   x RSQRT_D   x SELNEZ_D
    x MADDF_D   x MSUBF_D   x RINT_D    x CLASS_D   x MIN_D     x MAX_D     x MINA_D    x MAXA_D
    x CVT_S_D   x ____      x ____      x ____      x CVT_W_D   x CVT_L_D   x ____      x ____
    x ____      x ____      x ____      x ____      x ____      x ____      x ____      x ____
    x ____      x ____      x ____      x ____      x ____      x ____      x ____      x ____
    x ____      x ____      x ____      x ____      x ____      x ____      x ____      x ____
};
//------------------------------------------------------------------------------
// Table A.20 MIPS64 COP1 Encoding of Function Field When rs=W or L
//------------------------------------------------------------------------------
const FPU::FINSTRUCTION FPU::tableW[8 * 8] =
{
    o CMP_cond_S    x CMP_cond_S    x CMP_cond_S    x CMP_cond_S    x CMP_cond_S    x CMP_cond_S    x CMP_cond_S    x CMP_cond_S
    x CMP_cond_S    x CMP_cond_S    x CMP_cond_S    x CMP_cond_S    x CMP_cond_S    x CMP_cond_S    x CMP_cond_S    x CMP_cond_S
    x ____          x CMP_cond_S    x CMP_cond_S    x CMP_cond_S    x ____          x ____          x ____          x ____
    x ____          x CMP_cond_S    x CMP_cond_S    x CMP_cond_S    x ____          x ____          x ____          x ____
    x CVT_S_W       x CVT_D_W       x ____          x ____          x ____          x ____          x ____          x ____
    x ____          x ____          x ____          x ____          x ____          x ____          x ____          x ____
    x ____          x ____          x ____          x ____          x ____          x ____          x ____          x ____
    x ____          x ____          x ____          x ____          x ____          x ____          x ____          x ____
};
const FPU::FINSTRUCTION FPU::tableL[8 * 8] =
{
    o CMP_cond_D    x CMP_cond_D    x CMP_cond_D    x CMP_cond_D    x CMP_cond_D    x CMP_cond_D    x CMP_cond_D    x CMP_cond_D
    x CMP_cond_D    x CMP_cond_D    x CMP_cond_D    x CMP_cond_D    x CMP_cond_D    x CMP_cond_D    x CMP_cond_D    x CMP_cond_D
    x ____          x CMP_cond_D    x CMP_cond_D    x CMP_cond_D    x ____          x ____          x ____          x ____
    x ____          x CMP_cond_D    x CMP_cond_D    x CMP_cond_D    x ____          x ____          x ____          x ____
    x CVT_S_L       x CVT_D_L       x ____          x ____          x ____          x ____          x ____          x ____
    x ____          x ____          x ____          x ____          x ____          x ____          x ____          x ____
    x ____          x ____          x ____          x ____          x ____          x ____          x ____          x ____
    x ____          x ____          x ____          x ____          x ____          x ____          x ____          x ____
};
#undef o
#undef x
//------------------------------------------------------------------------------
FPU::FPU(CPU& cpu) : COP(cpu)
{
    for (unsigned int i = 0; i < 32; ++i)
    {
        FPR[i].vs32[0] = 0;
        FPR[i].vs32[1] = 0;
    }
}
//------------------------------------------------------------------------------
FPU::~FPU()
{
}
//------------------------------------------------------------------------------
void FPU::Execute(unsigned int encode)
{
    Encode = encode;

    FINSTRUCTION RS = tableRS[rs];
    (this->*RS)();
}
//------------------------------------------------------------------------------
void FPU::LDC(unsigned int encode)
{
    Encode = encode;

    LDC1();
}
//------------------------------------------------------------------------------
void FPU::LWC(unsigned int encode)
{
    Encode = encode;

    LWC1();
}
//------------------------------------------------------------------------------
void FPU::SDC(unsigned int encode)
{
    Encode = encode;

    SDC1();
}
//------------------------------------------------------------------------------
void FPU::SWC(unsigned int encode)
{
    Encode = encode;

    SWC1();
}
//------------------------------------------------------------------------------
// Table 3.15 FPU Load, Store, and Memory Control Instructions
//------------------------------------------------------------------------------
void FPU::LWC1()
{
    intptr_t address = cpu.GPR[rs] + immediate;
    if (address & 3)
    {
        cpu.SignalException(CPU::AddressErrorLoad);
    }
    else
    {
        float load = *(float*)address;
        FPR[ft].f32 = load;
    }
}
//------------------------------------------------------------------------------
void FPU::LDC1()
{
    intptr_t address = cpu.GPR[rs] + immediate;
    if (address & 7)
    {
        cpu.SignalException(CPU::AddressErrorLoad);
    }
    else
    {
        double load = *(double*)address;
        FPR[ft].f64 = load;
    }
}
//------------------------------------------------------------------------------
void FPU::SWC1()
{
    intptr_t address = cpu.GPR[rs] + immediate;
    if (address & 3)
    {
        cpu.SignalException(CPU::AddressErrorStore);
    }
    else
    {
        float& store = *(float*)address;
        store = FPR[ft].f32;
    }
}
//------------------------------------------------------------------------------
void FPU::SDC1()
{
    intptr_t address = cpu.GPR[rs] + immediate;
    if (address & 7)
    {
        cpu.SignalException(CPU::AddressErrorStore);
    }
    else
    {
        double& store = *(double*)address;
        store = FPR[ft].f64;
    }
}
//------------------------------------------------------------------------------
// Table A.17 MIPS64 COP1 Encoding of rs Field
//------------------------------------------------------------------------------
void FPU::MFC1()
{
    cpu.GPR[rt] = FPR[fs].vs32[0];
}
//------------------------------------------------------------------------------
void FPU::DMFC1()
{
#if (MIPS_BITS >= 64)
    cpu.GPR[rt] = FPR[fs].vs64[0];
#endif
}
//------------------------------------------------------------------------------
void FPU::CFC1()
{
    int temp = 0;
    switch (fs)
    {
    case 0:
        break;
    case 25:
        break;
    case 26:
        break;
    case 28:
        break;
    case 31:
        break;
    default:
        cpu.SignalException(CPU::ReservedInstruction);
        break;
    }
    cpu.GPR[rt] = temp;
}
//------------------------------------------------------------------------------
void FPU::MFHC1()
{
    cpu.GPR[rt] = FPR[fs].vs32[1];
}
//------------------------------------------------------------------------------
void FPU::MTC1()
{
    FPR[fs].vs32[0] = cpu.GPR32[rt];
}
//------------------------------------------------------------------------------
void FPU::DMTC1()
{
#if (MIPS_BITS >= 64)
    FPR[fs].vs64[0] = cpu.GPR[rt];
#endif
}
//------------------------------------------------------------------------------
void FPU::CTC1()
{
    switch (fs)
    {
    case 1:
    case 4:
        if (rt == 0)
            cpu.SignalException(CPU::ReservedInstruction);
        break;
    case 25:
        break;
    case 26:
        break;
    case 28:
        break;
    case 31:
        break;
    default:
        break;
    }
}
//------------------------------------------------------------------------------
void FPU::MTHC1()
{
    FPR[fs].vs32[1] = cpu.GPR32[rt];
}
//------------------------------------------------------------------------------
void FPU::BC1EQZ()
{
    unsigned int temp = FPR[ft].u32;
    cpu.BranchDelaySlot();
    if ((temp & 1) == 0)
        cpu.PC += immediate << 2;
    else
        cpu.PC += 4;
    cpu.PC += 4;
}
//------------------------------------------------------------------------------
void FPU::BC1NEZ()
{
    unsigned int temp = FPR[ft].u32;
    cpu.BranchDelaySlot();
    if ((temp & 1) != 0)
        cpu.PC += immediate << 2;
    else
        cpu.PC += 4;
    cpu.PC += 4;
}
//------------------------------------------------------------------------------
void FPU::S()
{
    FINSTRUCTION S = tableS[function];
    (this->*S)();
}
//------------------------------------------------------------------------------
void FPU::D()
{
    FINSTRUCTION D = tableD[function];
    (this->*D)();
}
//------------------------------------------------------------------------------
void FPU::W()
{
    FINSTRUCTION W = tableW[function];
    (this->*W)();
}
//------------------------------------------------------------------------------
void FPU::L()
{
    FINSTRUCTION L = tableL[function];
    (this->*L)();
}
//------------------------------------------------------------------------------
// Table A.18 MIPS64 COP1 Encoding of Function Field When rs=S
//------------------------------------------------------------------------------
void FPU::ADD_S()
{
    FPR[fd].f32 = FPR[fs].f32 + FPR[ft].f32;
}
//------------------------------------------------------------------------------
void FPU::SUB_S()
{
    FPR[fd].f32 = FPR[fs].f32 - FPR[ft].f32;
}
//------------------------------------------------------------------------------
void FPU::MUL_S()
{
    FPR[fd].f32 = FPR[fs].f32 * FPR[ft].f32;
}
//------------------------------------------------------------------------------
void FPU::DIV_S()
{
    FPR[fd].f32 = FPR[fs].f32 / FPR[ft].f32;
}
//------------------------------------------------------------------------------
void FPU::SQRT_S()
{
    FPR[fd].f32 = sqrtf(FPR[fs].f32);
}
//------------------------------------------------------------------------------
void FPU::ABS_S()
{
    FPR[fd].f32 = fabsf(FPR[fs].f32);
}
//------------------------------------------------------------------------------
void FPU::MOV_S()
{
    FPR[fd].f32 = FPR[fs].f32;
}
//------------------------------------------------------------------------------
void FPU::NEG_S()
{
    FPR[fd].f32 = -FPR[fs].f32;
}
//------------------------------------------------------------------------------
void FPU::ROUND_L_S()
{
    FPR[fd].s64 = (signed long long)roundf(FPR[fs].f32);
}
//------------------------------------------------------------------------------
void FPU::TRUNC_L_S()
{
    FPR[fd].s64 = (signed long long)truncf(FPR[fs].f32);
}
//------------------------------------------------------------------------------
void FPU::CEIL_L_S()
{
    FPR[fd].s64 = (signed long long)ceilf(FPR[fs].f32);
}
//------------------------------------------------------------------------------
void FPU::FLOOR_L_S()
{
    FPR[fd].s64 = (signed long long)floorf(FPR[fs].f32);
}
//------------------------------------------------------------------------------
void FPU::ROUND_W_S()
{
    FPR[fd].s32 = (signed int)roundf(FPR[fs].f32);
}
//------------------------------------------------------------------------------
void FPU::TRUNC_W_S()
{
    FPR[fd].s32 = (signed int)truncf(FPR[fs].f32);
}
//------------------------------------------------------------------------------
void FPU::CEIL_W_S()
{
    FPR[fd].s32 = (signed int)ceilf(FPR[fs].f32);
}
//------------------------------------------------------------------------------
void FPU::FLOOR_W_S()
{
    FPR[fd].s32 = (signed int)floorf(FPR[fs].f32);
}
//------------------------------------------------------------------------------
void FPU::SEL_S()
{
    unsigned int temp = FPR[fd].u32;
    if (temp & 1)
        temp = FPR[ft].u32;
    else
        temp = FPR[fs].u32;
    FPR[fd].u32 = temp;
}
//------------------------------------------------------------------------------
void FPU::SELEQZ_S()
{
    unsigned int temp = FPR[ft].u32;
    if ((temp & 1) == 0)
        temp = FPR[fs].u32;
    else
        temp = 0;
    FPR[fd].u32 = temp;
}
//------------------------------------------------------------------------------
void FPU::RECIP_S()
{
    FPR[fd].f32 = 1.0f / (FPR[fs].f32);
}
//------------------------------------------------------------------------------
void FPU::RSQRT_S()
{
    FPR[fd].f32 = 1.0f / sqrtf(FPR[fs].f32);
}
//------------------------------------------------------------------------------
void FPU::SELNEZ_S()
{
    unsigned int temp = FPR[ft].u32;
    if ((temp & 1) != 0)
        temp = FPR[fs].u32;
    else
        temp = 0;
    FPR[fd].u32 = temp;
}
//------------------------------------------------------------------------------
void FPU::MADDF_S()
{
    FPR[fd].f32 = FPR[fd].f32 + (FPR[fs].f32 * FPR[ft].f32);
}
//------------------------------------------------------------------------------
void FPU::MSUBF_S()
{
    FPR[fd].f32 = FPR[fd].f32 - (FPR[fs].f32 * FPR[ft].f32);
}
//------------------------------------------------------------------------------
void FPU::RINT_S()
{
    FPR[fd].f32 = rintf(FPR[fs].f32);
}
//------------------------------------------------------------------------------
void FPU::CLASS_S()
{
    int classFP = fpclassify(FPR[fs].f32);
    FPR[fd].s32 = classFP;
}
//------------------------------------------------------------------------------
void FPU::MIN_S()
{
    FPR[fd].f32 = fminf(FPR[fs].f32, FPR[ft].f32);
}
//------------------------------------------------------------------------------
void FPU::MAX_S()
{
    FPR[fd].f32 = fmaxf(FPR[fs].f32, FPR[ft].f32);
}
//------------------------------------------------------------------------------
void FPU::MINA_S()
{
    FPR[fd].f32 = (FPR[fs].f32 < FPR[ft].f32) ? (FPR[fs].f32 - FPR[ft].f32) : (FPR[ft].f32 - FPR[fs].f32);
}
//------------------------------------------------------------------------------
void FPU::MAXA_S()
{
    FPR[fd].f32 = (FPR[fs].f32 > FPR[ft].f32) ? (FPR[fs].f32 - FPR[ft].f32) : (FPR[ft].f32 - FPR[fs].f32);
}
//------------------------------------------------------------------------------
void FPU::CVT_D_S()
{
    FPR[fd].f64 = FPR[fs].f32;
}
//------------------------------------------------------------------------------
void FPU::CVT_W_S()
{
    FPR[fd].s32 = (signed int)roundf(FPR[fs].f32);
}
//------------------------------------------------------------------------------
void FPU::CVT_L_S()
{
    FPR[fd].s64 = (signed long long)roundf(FPR[fs].f32);
}
//------------------------------------------------------------------------------
// Table A.19 MIPS64 COP1 Encoding of Function Field When rs=D
//------------------------------------------------------------------------------
void FPU::ADD_D()
{
    FPR[fd].f64 = FPR[fs].f64 + FPR[ft].f64;
}
//------------------------------------------------------------------------------
void FPU::SUB_D()
{
    FPR[fd].f64 = FPR[fs].f64 - FPR[ft].f64;
}
//------------------------------------------------------------------------------
void FPU::MUL_D()
{
    FPR[fd].f64 = FPR[fs].f64 * FPR[ft].f64;
}
//------------------------------------------------------------------------------
void FPU::DIV_D()
{
    FPR[fd].f64 = FPR[fs].f64 / FPR[ft].f64;
}
//------------------------------------------------------------------------------
void FPU::SQRT_D()
{
    FPR[fd].f64 = sqrt(FPR[fs].f64);
}
//------------------------------------------------------------------------------
void FPU::ABS_D()
{
    FPR[fd].f64 = fabs(FPR[fs].f64);
}
//------------------------------------------------------------------------------
void FPU::MOV_D()
{
    FPR[fd].f64 = FPR[fs].f64;
}
//------------------------------------------------------------------------------
void FPU::NEG_D()
{
    FPR[fd].f64 = -FPR[fs].f64;
}
//------------------------------------------------------------------------------
void FPU::ROUND_L_D()
{
    FPR[fd].s64 = (signed long long)round(FPR[fs].f64);
}
//------------------------------------------------------------------------------
void FPU::TRUNC_L_D()
{
    FPR[fd].s64 = (signed long long)trunc(FPR[fs].f64);
}
//------------------------------------------------------------------------------
void FPU::CEIL_L_D()
{
    FPR[fd].s64 = (signed long long)ceil(FPR[fs].f64);
}
//------------------------------------------------------------------------------
void FPU::FLOOR_L_D()
{
    FPR[fd].s64 = (signed long long)floor(FPR[fs].f64);
}
//------------------------------------------------------------------------------
void FPU::ROUND_W_D()
{
    FPR[fd].s32 = (signed int)round(FPR[fs].f64);
}
//------------------------------------------------------------------------------
void FPU::TRUNC_W_D()
{
    FPR[fd].s32 = (signed int)trunc(FPR[fs].f64);
}
//------------------------------------------------------------------------------
void FPU::CEIL_W_D()
{
    FPR[fd].s32 = (signed int)ceil(FPR[fs].f64);
}
//------------------------------------------------------------------------------
void FPU::FLOOR_W_D()
{
    FPR[fd].s32 = (signed int)floor(FPR[fs].f64);
}
//------------------------------------------------------------------------------
void FPU::SEL_D()
{
    unsigned long long temp = FPR[fd].u64;
    if (temp & 1)
        temp = FPR[ft].u64;
    else
        temp = FPR[fs].u64;
    FPR[fd].u64 = temp;
}
//------------------------------------------------------------------------------
void FPU::SELEQZ_D()
{
    unsigned long long temp = FPR[ft].u64;
    if ((temp & 1) == 0)
        temp = FPR[fs].u64;
    else
        temp = 0;
    FPR[fd].u64 = temp;
}
//------------------------------------------------------------------------------
void FPU::RECIP_D()
{
    FPR[fd].f64 = 1.0 / (FPR[fs].f64);
}
//------------------------------------------------------------------------------
void FPU::RSQRT_D()
{
    FPR[fd].f64 = 1.0 / sqrt(FPR[fs].f64);
}
//------------------------------------------------------------------------------
void FPU::SELNEZ_D()
{
    unsigned long long temp = FPR[ft].u64;
    if ((temp & 1) != 0)
        temp = FPR[fs].u64;
    else
        temp = 0;
    FPR[fd].u64 = temp;
}
//------------------------------------------------------------------------------
void FPU::MADDF_D()
{
    FPR[fd].f64 = FPR[fd].f64 + (FPR[fs].f64 * FPR[ft].f64);
}
//------------------------------------------------------------------------------
void FPU::MSUBF_D()
{
    FPR[fd].f64 = FPR[fd].f64 - (FPR[fs].f64 * FPR[ft].f64);
}
//------------------------------------------------------------------------------
void FPU::RINT_D()
{
    FPR[fd].f64 = rint(FPR[fs].f64);
}
//------------------------------------------------------------------------------
void FPU::CLASS_D()
{
    int classFP = fpclassify(FPR[fs].f64);
    FPR[fd].s32 = classFP;
}
//------------------------------------------------------------------------------
void FPU::MIN_D()
{
    FPR[fd].f64 = fmin(FPR[fs].f64, FPR[ft].f64);
}
//------------------------------------------------------------------------------
void FPU::MAX_D()
{
    FPR[fd].f64 = fmax(FPR[fs].f64, FPR[ft].f64);
}
//------------------------------------------------------------------------------
void FPU::MINA_D()
{
    FPR[fd].f64 = (FPR[fs].f64 < FPR[ft].f64) ? (FPR[fs].f64 - FPR[ft].f64) : (FPR[ft].f64 - FPR[fs].f64);
}
//------------------------------------------------------------------------------
void FPU::MAXA_D()
{
    FPR[fd].f64 = (FPR[fs].f64 > FPR[ft].f64) ? (FPR[fs].f64 - FPR[ft].f64) : (FPR[ft].f64 - FPR[fs].f64);
}
//------------------------------------------------------------------------------
void FPU::CVT_S_D()
{
    FPR[fd].f64 = FPR[fs].f64;
}
//------------------------------------------------------------------------------
void FPU::CVT_W_D()
{
    FPR[fd].s32 = (signed int)round(FPR[fs].f64);
}
//------------------------------------------------------------------------------
void FPU::CVT_L_D()
{
    FPR[fd].s64 = (signed long long)round(FPR[fs].f64);
}
//------------------------------------------------------------------------------
// Table A.20 MIPS64 COP1 Encoding of Function Field When rs=W
//------------------------------------------------------------------------------
void FPU::CMP_cond_S()
{
    int cond0 = (function >> 0) & 1;
    int cond1 = (function >> 1) & 1;
    int cond2 = (function >> 2) & 1;
    int cond3 = (function >> 3) & 1;
    int cond4 = (function >> 4) & 1;
    int less;
    int equal;
    int unordered;
    bool nan_fs = isnan(FPR[fs].f32);
    bool nan_ft = isnan(FPR[ft].f32);
    if (nan_fs || nan_ft)
    {
        less = 0;
        equal = 0;
        unordered = 1;
        if (cond3 & (nan_fs ^ nan_ft))
        {
            cpu.SignalException(CPU::InvalidOperation);
            return;
        }
    }
    else
    {
        less = (FPR[fs].f32 < FPR[ft].f32) ? 1 : 0;
        equal = (FPR[fs].f32 == FPR[ft].f32) ? 1 : 0;
        unordered = 0;
    }
    int condition = cond4 ^ ((cond2 & less) | (cond1 & equal) | (cond0 & unordered));
    FPR[rd].u32 = condition ? -1 : 0;
}
//------------------------------------------------------------------------------
void FPU::CVT_S_W()
{
    FPR[fd].f32 = (float)FPR[fs].s32;
}
//------------------------------------------------------------------------------
void FPU::CVT_D_W()
{
    FPR[fd].f64 = (double)FPR[fs].s32;
}
//------------------------------------------------------------------------------
// Table A.20 MIPS64 COP1 Encoding of Function Field When rs=L
//------------------------------------------------------------------------------
void FPU::CMP_cond_D()
{
    int cond0 = (function >> 0) & 1;
    int cond1 = (function >> 1) & 1;
    int cond2 = (function >> 2) & 1;
    int cond3 = (function >> 3) & 1;
    int cond4 = (function >> 4) & 1;
    int less;
    int equal;
    int unordered;
    bool nan_fs = isnan(FPR[fs].f64);
    bool nan_ft = isnan(FPR[ft].f64);
    if (nan_fs || nan_ft)
    {
        less = 0;
        equal = 0;
        unordered = 1;
        if (cond3 & (nan_fs ^ nan_ft))
        {
            cpu.SignalException(CPU::InvalidOperation);
            return;
        }
    }
    else
    {
        less = (FPR[fs].f64 < FPR[ft].f64) ? 1 : 0;
        equal = (FPR[fs].f64 == FPR[ft].f64) ? 1 : 0;
        unordered = 0;
    }
    int condition = cond4 ^ ((cond2 & less) | (cond1 & equal) | (cond0 & unordered));
    FPR[rd].u64 = condition ? -1 : 0;
}
//------------------------------------------------------------------------------
void FPU::CVT_S_L()
{
    FPR[fd].f32 = (float)FPR[fs].s64;
}
//------------------------------------------------------------------------------
void FPU::CVT_D_L()
{
    FPR[fd].f64 = (double)FPR[fs].s64;
}
//------------------------------------------------------------------------------
