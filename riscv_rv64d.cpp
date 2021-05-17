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

#if RISCV_HAVE_DOUBLE
//------------------------------------------------------------------------------
void riscv_cpu::FCVT_L_D()
{
    if (f[rs1].d <= INT64_MIN || f[rs1].d == -INFINITY)
    {
        x[rd].s = int64_t(INT64_MIN);
        fcsr.fflags = 0;
        fcsr.nv = true;
        return;
    }
    if (f[rs1].d >= INT64_MAX || f[rs1].d == INFINITY || isnan(f[rs1].d))
    {
        x[rd].s = int64_t(INT64_MAX);
        fcsr.fflags = 0;
        fcsr.nv = true;
        return;
    }
    double value = f[rs1].d;
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
void riscv_cpu::FCVT_LU_D()
{
    if (f[rs1].d <= -1.0f || f[rs1].d == -INFINITY)
    {
        x[rd].u = uint64_t(0);
        fcsr.fflags = 0;
        fcsr.nv = true;
        return;
    }
    if (f[rs1].d >= UINT64_MAX || f[rs1].d == INFINITY || isnan(f[rs1].d))
    {
        x[rd].u = uint64_t(UINT64_MAX);
        fcsr.fflags = 0;
        fcsr.nv = true;
        return;
    }
    double value = f[rs1].d;
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
void riscv_cpu::FCVT_D_L()
{
    f[rd].d = x[rs1].s64;
}
//------------------------------------------------------------------------------
void riscv_cpu::FCVT_D_LU()
{
    f[rd].d = x[rs1].u64;
}
//------------------------------------------------------------------------------
#endif
