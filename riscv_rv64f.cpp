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

//------------------------------------------------------------------------------
void riscv_cpu::FCVT_L_S()
{
    if (f[rs1].f <= INT64_MIN || f[rs1].f == -INFINITY)
    {
        x[rd].s = int64_t(INT64_MIN);
        fcsr.fflags = 0x10;
        return;
    }
    if (f[rs1].f >= INT64_MAX || (isfinite(f[rs1].f) == false))
    {
        x[rd].s = int64_t(INT64_MAX);
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
    x[rd].s = int64_t(lrint(value));
    ftestexcept();
    fesetround(round);
}
//------------------------------------------------------------------------------
void riscv_cpu::FCVT_LU_S()
{
    if (f[rs1].f <= -1.0f || f[rs1].f == -INFINITY)
    {
        x[rd].u = uint64_t(0);
        fcsr.fflags = 0x10;
        return;
    }
    if (f[rs1].f >= UINT64_MAX || (isfinite(f[rs1].f) == false))
    {
        x[rd].u = uint64_t(UINT64_MAX);
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
    x[rd].u = uint64_t(lrint(value));
    ftestexcept();
    fesetround(round);
}
//------------------------------------------------------------------------------
void riscv_cpu::FCVT_S_L()
{
    f[rd].f = x[rs1].s64;
}
//------------------------------------------------------------------------------
void riscv_cpu::FCVT_S_LU()
{
    f[rd].f = x[rs1].u64;
}
//------------------------------------------------------------------------------
