//==============================================================================
// The RISC-V Instruction Set Manual
// Volume I: Unprivileged ISA
// Document Version 20191213
// December 13, 2019
//==============================================================================

#include "riscv_cpu.h"
#include "riscv_float.h"

#if RISCV_HAVE_DOUBLE
//------------------------------------------------------------------------------
void riscv_cpu::FCVT_L_D()
{
    double value = f[rs1].d;
    if (value <= INT64_MIN || value == -INFINITY)
    {
        x[rd].s = int64_t(INT64_MIN);
        fcsr.fflags = 0;
        fcsr.nv = true;
        return;
    }
    if (value >= INT64_MAX || value == INFINITY || isnan(value))
    {
        x[rd].s = int64_t(INT64_MAX);
        fcsr.fflags = 0;
        fcsr.nv = true;
        return;
    }
    fclearexcept();
    int frm = (funct3 == 0b111) ? fcsr.frm : funct3;
    switch (frm)
    {
    case 0b000:
        x[rd].s = fromfp(value, FP_INT_TONEAREST, 64);
        break;
    case 0b001:
        x[rd].s = fromfp(value, FP_INT_TOWARDZERO, 64);
        break;
    case 0b010:
        x[rd].s = fromfp(value, FP_INT_DOWNWARD, 64);
        break;
    case 0b011:
        x[rd].s = fromfp(value, FP_INT_UPWARD, 64);
        break;
    case 0b100:
        x[rd].s = fromfp(value, FP_INT_TONEARESTFROMZERO, 64);
        break;
    case 0b101:
    case 0b110:
    case 0b111:
        HINT();
        break;
    }
    ftestexcept();
}
//------------------------------------------------------------------------------
void riscv_cpu::FCVT_LU_D()
{
    double value = f[rs1].d;
    if (value <= -1.0f || value == -INFINITY)
    {
        x[rd].u = uint64_t(0);
        fcsr.fflags = 0;
        fcsr.nv = true;
        return;
    }
    if (value >= UINT64_MAX || value == INFINITY || isnan(value))
    {
        x[rd].u = uint64_t(UINT64_MAX);
        fcsr.fflags = 0;
        fcsr.nv = true;
        return;
    }
    fclearexcept();
    int frm = (funct3 == 0b111) ? fcsr.frm : funct3;
    switch (frm)
    {
    case 0b000:
        x[rd].u = ufromfp(value, FP_INT_TONEAREST, 64);
        break;
    case 0b001:
        x[rd].u = ufromfp(value, FP_INT_TOWARDZERO, 64);
        break;
    case 0b010:
        x[rd].u = ufromfp(value, FP_INT_DOWNWARD, 64);
        break;
    case 0b011:
        x[rd].u = ufromfp(value, FP_INT_UPWARD, 64);
        break;
    case 0b100:
        x[rd].u = ufromfp(value, FP_INT_TONEARESTFROMZERO, 64);
        break;
    case 0b101:
    case 0b110:
    case 0b111:
        HINT();
        break;
    }
    ftestexcept();
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
