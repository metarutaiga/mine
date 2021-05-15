//==============================================================================
// The RISC-V Instruction Set Manual
// Volume I: Unprivileged ISA
// Document Version 20191213
// December 13, 2019
//==============================================================================

#include <fenv.h>
#include <math.h>
#include <float.h>
#include "riscv_cpu.h"
static int issignaling(float f32)
{
    int u32 = (int&)f32;
    return (u32 > 0x7f800000 && u32 < 0x7fc00000) || (u32 > 0xff800000 && u32 < 0xffc00000);
}
static int issubnormal(float f32)
{
    int u32 = (int&)f32;
    return (u32 > 0x00000000 && u32 < 0x00800000) || (u32 > 0x80000000 && u32 < 0x80800000);
}

//------------------------------------------------------------------------------
void riscv_cpu::FLW()
{
    f[rd].f = *(float32_t*)(x[rs1] + simmI());
}
//------------------------------------------------------------------------------
void riscv_cpu::FSW()
{
    *(float32_t*)(x[rs1] + simmS()) = f[rs2].f;
}
//------------------------------------------------------------------------------
void riscv_cpu::FMADD_S()
{
    fclearexcept();
    f[rd].f = (f[rs1].f * f[rs2].f) + f[rs3].f;
    ftestexcept();
}
//------------------------------------------------------------------------------
void riscv_cpu::FMSUB_S()
{
    fclearexcept();
    f[rd].f = (f[rs1].f * f[rs2].f) - f[rs3].f;
    ftestexcept();
}
//------------------------------------------------------------------------------
void riscv_cpu::FNMSUB_S()
{
    fclearexcept();
    f[rd].f = -(f[rs1].f * f[rs2].f) + f[rs3].f;
    ftestexcept();
}
//------------------------------------------------------------------------------
void riscv_cpu::FNMADD_S()
{
    fclearexcept();
    f[rd].f = -(f[rs1].f * f[rs2].f) - f[rs3].f;
    ftestexcept();
}
//------------------------------------------------------------------------------
void riscv_cpu::FADD_S()
{
    fclearexcept();
    f[rd].f = f[rs1].f + f[rs2].f;
    ftestexcept();
}
//------------------------------------------------------------------------------
void riscv_cpu::FSUB_S()
{
    fclearexcept();
    f[rd].f = f[rs1].f - f[rs2].f;
    ftestexcept();
}
//------------------------------------------------------------------------------
void riscv_cpu::FMUL_S()
{
    fclearexcept();
    f[rd].f = f[rs1].f * f[rs2].f;
    ftestexcept();
}
//------------------------------------------------------------------------------
void riscv_cpu::FDIV_S()
{
    fclearexcept();
    f[rd].f = f[rs1].f / f[rs2].f;
    ftestexcept();
}
//------------------------------------------------------------------------------
void riscv_cpu::FSQRT_S()
{
    fclearexcept();
    f[rd].f = sqrtf(f[rs1].f);
    ftestexcept();
}
//------------------------------------------------------------------------------
void riscv_cpu::FSGNJ_S()
{
    f[rd].f = copysignf(f[rs1].f, f[rs2].f);
}
//------------------------------------------------------------------------------
void riscv_cpu::FSGNJN_S()
{
    f[rd].f = copysignf(f[rs1].f, f[rs2].f * -1.0f);
}
//------------------------------------------------------------------------------
void riscv_cpu::FSGNJX_S()
{
    f[rd].f = copysignf(f[rs1].f, f[rs1].f * copysignf(1.0f, f[rs2].f));
}
//------------------------------------------------------------------------------
void riscv_cpu::FMIN_S()
{
    if (f[rs1].f == 0.0f && f[rs2].f == 0.0f)
    {
        f[rd].f = f[rs1].f * f[rs2].f;
        return;
    }
    f[rd].f = fminf(f[rs1].f, f[rs2].f);
    fcsr.nv = (issignaling(f[rs1].f) || issignaling(f[rs2].f));
}
//------------------------------------------------------------------------------
void riscv_cpu::FMAX_S()
{
    if (f[rs1].f == 0.0f && f[rs2].f == 0.0f)
    {
        f[rd].f = f[rs1].f * f[rs2].f * -1.0f;
        return;
    }
    f[rd].f = fmaxf(f[rs1].f, f[rs2].f);
    fcsr.nv = (issignaling(f[rs1].f) || issignaling(f[rs2].f));
}
//------------------------------------------------------------------------------
void riscv_cpu::FCVT_W_S()
{
    if (f[rs1].f <= INT32_MIN || f[rs1].f == -INFINITY)
    {
        x[rd].s = int32_t(INT32_MIN);
        fcsr.fflags = 0x10;
        return;
    }
    if (f[rs1].f >= INT32_MAX || (isfinite(f[rs1].f) == false))
    {
        x[rd].s = int32_t(INT32_MAX);
        fcsr.fflags = 0x10;
        return;
    }
    float value = f[rs1].f;
    int round = fegetround();
    fclearexcept();
    int frm = (funct3 == 0b111) ? fcsr.frm : funct3;
    switch (frm)
    {
    case 0b000:
        fesetround(FE_TONEAREST);
        break;
    case 0b001:
        fesetround(FE_TOWARDZERO);
        break;
    case 0b010:
        fesetround(FE_DOWNWARD);
        break;
    case 0b011:
        fesetround(FE_UPWARD);
        break;
    case 0b100:
        fesetround(FE_TOWARDZERO);
        value += 0.5f;
        break;
    case 0b101:
    case 0b110:
    case 0b111:
        HINT();
        break;
    }
    x[rd].s = int32_t(lrint(value));
    ftestexcept();
    fesetround(round);
}
//------------------------------------------------------------------------------
void riscv_cpu::FCVT_WU_S()
{
    if (f[rs1].f <= -1.0f || f[rs1].f == -INFINITY)
    {
        x[rd].u = 0;
        fcsr.fflags = 0x10;
        return;
    }
    if (f[rs1].f >= UINT32_MAX || (isfinite(f[rs1].f) == false))
    {
        x[rd].u = int32_t(UINT32_MAX);
        fcsr.fflags = 0x10;
        return;
    }
    float value = f[rs1].f;
    int round = fegetround();
    fclearexcept();
    int frm = (funct3 == 0b111) ? fcsr.frm : funct3;
    switch (frm)
    {
    case 0b000:
        fesetround(FE_TONEAREST);
        break;
    case 0b001:
        fesetround(FE_TOWARDZERO);
        break;
    case 0b010:
        fesetround(FE_DOWNWARD);
        break;
    case 0b011:
        fesetround(FE_UPWARD);
        break;
    case 0b100:
        fesetround(FE_TOWARDZERO);
        value += 0.5f;
        break;
    case 0b101:
    case 0b110:
    case 0b111:
        HINT();
        break;
    }
    x[rd].u = int32_t(uint32_t(lrint(value)));
    ftestexcept();
    fesetround(round);
}
//------------------------------------------------------------------------------
void riscv_cpu::FMV_X_W()
{
    if (isnan(f[rs1].f))
    {
        x[rd].u = 0x7fc00000;
        return;
    }
    x[rd].s = f[rs1].s32;
}
//------------------------------------------------------------------------------
void riscv_cpu::FEQ_S()
{
    x[rd].u = (f[rs1].f == f[rs2].f);
    fcsr.nv = (issignaling(f[rs1].f) || issignaling(f[rs2].f));
}
//------------------------------------------------------------------------------
void riscv_cpu::FLT_S()
{
    x[rd].u = (f[rs1].f < f[rs2].f);
    fcsr.nv = (isnan(f[rs1].f) || isnan(f[rs2].f));
}
//------------------------------------------------------------------------------
void riscv_cpu::FLE_S()
{
    x[rd].u = (f[rs1].f <= f[rs2].f);
    fcsr.nv = (isnan(f[rs1].f) || isnan(f[rs2].f));
}
//------------------------------------------------------------------------------
void riscv_cpu::FCLASS_S()
{
    int fclass = 0;
    fclass |= (f[rs1].f == -INFINITY) << 0;
    fclass |= (f[rs1].f < 0.0f && isnormal(f[rs1].f) == true) << 1;
    fclass |= (f[rs1].f < 0.0f && issubnormal(f[rs1].f) == true) << 2;
    fclass |= (f[rs1].f == 0.0f && (f[rs1].u32 & 0x80000000)) << 3;
    fclass |= (f[rs1].f == 0.0f && ((f[rs1].u32 & 0x80000000) == 0)) << 4;
    fclass |= (f[rs1].f > 0.0f && issubnormal(f[rs1].f) == true) << 5;
    fclass |= (f[rs1].f > 0.0f && isnormal(f[rs1].f) == true) << 6;
    fclass |= (f[rs1].f == INFINITY) << 7;
    fclass |= (isnan(f[rs1].f) && issignaling(f[rs1].f) == true) << 8;
    fclass |= (isnan(f[rs1].f) && issignaling(f[rs1].f) == false) << 9;
    x[rd].u = fclass;
}
//------------------------------------------------------------------------------
void riscv_cpu::FCVT_S_W()
{
    f[rd].f = x[rs1].s32;
}
//------------------------------------------------------------------------------
void riscv_cpu::FCVT_S_WU()
{
    f[rd].f = x[rs1].u32;
}
//------------------------------------------------------------------------------
void riscv_cpu::FMV_W_X()
{
    f[rd].s32 = x[rs1].s32;
}
//------------------------------------------------------------------------------
