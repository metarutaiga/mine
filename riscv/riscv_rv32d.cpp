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
static bool issignaling(double d64)
{
    uint64_t u64 = (uint64_t&)d64;
    return (u64 > 0x7ff0000000000000ull && u64 < 0x7ff8000000000000ull) || (u64 > 0xfff0000000000000ull && u64 < 0xfff8000000000000ull);
}
//------------------------------------------------------------------------------
void riscv_cpu::FLD()
{
    f[rd].d = *(double*)(x[rs1] + simmI());
}
//------------------------------------------------------------------------------
void riscv_cpu::FSD()
{
    *(uint64_t*)(x[rs1] + simmS()) = f[rs2].u64;
}
//------------------------------------------------------------------------------
void riscv_cpu::FMADD_D()
{
    fclearexcept();
    f[rd].d = fma(f[rs1].d, f[rs2].d, f[rs3].d);
    ftestexcept();
    if (isnan(f[rd].d))
    {
        f[rd].d = NAN;
        return;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::FMSUB_D()
{
    fclearexcept();
    f[rd].d = fma(f[rs1].d, f[rs2].d, -f[rs3].d);
    ftestexcept();
    if (isnan(f[rd].d))
    {
        f[rd].d = NAN;
        return;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::FNMSUB_D()
{
    fclearexcept();
    f[rd].d = -fma(f[rs1].d, f[rs2].d, -f[rs3].d);
    ftestexcept();
    if (isnan(f[rd].d))
    {
        f[rd].d = NAN;
        return;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::FNMADD_D()
{
    fclearexcept();
    f[rd].d = -fma(f[rs1].d, f[rs2].d, f[rs3].d);
    ftestexcept();
    if (isnan(f[rd].d))
    {
        f[rd].d = NAN;
        return;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::FADD_D()
{
    fclearexcept();
    f[rd].d = f[rs1].d + f[rs2].d;
    ftestexcept();
    if (isnan(f[rd].d))
    {
        f[rd].d = NAN;
        return;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::FSUB_D()
{
    fclearexcept();
    f[rd].d = f[rs1].d - f[rs2].d;
    ftestexcept();
    if (isnan(f[rd].d))
    {
        f[rd].d = NAN;
        return;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::FMUL_D()
{
    fclearexcept();
    f[rd].d = f[rs1].d * f[rs2].d;
    ftestexcept();
    if (isnan(f[rd].d))
    {
        f[rd].d = NAN;
        return;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::FDIV_D()
{
    fclearexcept();
    f[rd].d = f[rs1].d / f[rs2].d;
    ftestexcept();
    if (isnan(f[rd].d))
    {
        f[rd].d = NAN;
        return;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::FSQRT_D()
{
    fclearexcept();
    f[rd].d = sqrt(f[rs1].d);
    ftestexcept();
    if (isnan(f[rd].d))
    {
        f[rd].d = NAN;
        return;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::FSGNJ_D()
{
    f[rd].u = (f[rs1].u & ~0x8000000000000000ull) | (f[rs2].u & 0x8000000000000000ull);
}
//------------------------------------------------------------------------------
void riscv_cpu::FSGNJN_D()
{
    f[rd].u = (f[rs1].u & ~0x8000000000000000ull) | ((f[rs2].u & 0x8000000000000000ull) ^ 0x8000000000000000ull);
}
//------------------------------------------------------------------------------
void riscv_cpu::FSGNJX_D()
{
    f[rd].u = f[rs1].u ^ (f[rs2].u & 0x8000000000000000ull);
}
//------------------------------------------------------------------------------
void riscv_cpu::FMIN_D()
{
    if (f[rs1].d == 0.0 && f[rs2].d == 0.0)
    {
        f[rd].d = f[rs1].d * f[rs2].d;
        return;
    }
    if (isnan(f[rs1].d) || isnan(f[rs2].d))
    {
        if (isnan(f[rs1].d) && isnan(f[rs2].d))
        {
            f[rd].d = NAN;
        }
        else if (isnan(f[rs1].d))
        {
            f[rd].d = f[rs2].d;
        }
        else
        {
            f[rd].d = f[rs1].d;
        }
    }
    else
    {
        f[rd].d = fmin(f[rs1].d, f[rs2].d);
    }
    fcsr.nv = (issignaling(f[rs1].d) || issignaling(f[rs2].d));
}
//------------------------------------------------------------------------------
void riscv_cpu::FMAX_D()
{
    if (f[rs1].d == 0.0 && f[rs2].d == 0.0)
    {
        f[rd].d = f[rs1].d * f[rs2].d * -1.0;
        return;
    }
    if (isnan(f[rs1].d) || isnan(f[rs2].d))
    {
        if (isnan(f[rs1].d) && isnan(f[rs2].d))
        {
            f[rd].d = NAN;
        }
        else if (isnan(f[rs1].d))
        {
            f[rd].d = f[rs2].d;
        }
        else
        {
            f[rd].d = f[rs1].d;
        }
    }
    else
    {
        f[rd].d = fmax(f[rs1].d, f[rs2].d);
    }
    fcsr.nv = (issignaling(f[rs1].d) || issignaling(f[rs2].d));
}
//------------------------------------------------------------------------------
void riscv_cpu::FCVT_S_D()
{
    f[rd].f = f[rs1].d;
    if (isnan(f[rd].f))
    {
        f[rd].f = NAN;
        return;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::FCVT_D_S()
{
    f[rd].d = f[rs1].f;
    if (isnan(f[rd].d))
    {
        f[rd].d = NAN;
        return;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::FCVT_W_D()
{
    double value = f[rs1].d;
    if (value <= INT32_MIN || value == -INFINITY)
    {
        x[rd].s = int32_t(INT32_MIN);
        fcsr.fflags = 0;
        fcsr.nv = true;
        return;
    }
    if (value >= INT32_MAX || value == INFINITY || isnan(value))
    {
        x[rd].s = int32_t(INT32_MAX);
        fcsr.fflags = 0;
        fcsr.nv = true;
        return;
    }
    fclearexcept();
    int frm = (funct3 == 0b111) ? fcsr.frm : funct3;
    switch (frm)
    {
    case 0b000:
        x[rd].s = fromfp(value, FP_INT_TONEAREST, 32);
        break;
    case 0b001:
        x[rd].s = fromfp(value, FP_INT_TOWARDZERO, 32);
        break;
    case 0b010:
        x[rd].s = fromfp(value, FP_INT_DOWNWARD, 32);
        break;
    case 0b011:
        x[rd].s = fromfp(value, FP_INT_UPWARD, 32);
        break;
    case 0b100:
        x[rd].s = fromfp(value, FP_INT_TONEARESTFROMZERO, 32);
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
void riscv_cpu::FCVT_WU_D()
{
    double value = f[rs1].d;
    if (value <= -1.0 || value == -INFINITY)
    {
        x[rd].u = 0;
        fcsr.fflags = 0;
        fcsr.nv = true;
        return;
    }
    if (value >= UINT32_MAX || value == INFINITY || isnan(value))
    {
        x[rd].u = int32_t(UINT32_MAX);
        fcsr.fflags = 0;
        fcsr.nv = true;
        return;
    }
    fclearexcept();
    int frm = (funct3 == 0b111) ? fcsr.frm : funct3;
    switch (frm)
    {
    case 0b000:
        x[rd].u = int32_t(ufromfp(value, FP_INT_TONEAREST, 32));
        break;
    case 0b001:
        x[rd].u = int32_t(ufromfp(value, FP_INT_TOWARDZERO, 32));
        break;
    case 0b010:
        x[rd].u = int32_t(ufromfp(value, FP_INT_DOWNWARD, 32));
        break;
    case 0b011:
        x[rd].u = int32_t(ufromfp(value, FP_INT_UPWARD, 32));
        break;
    case 0b100:
        x[rd].u = int32_t(ufromfp(value, FP_INT_TONEARESTFROMZERO, 32));
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
void riscv_cpu::FMV_X_D()
{
    if ((f[rs1].u64 & 0xFFFFFFFF00000000ull) == 0xFFFFFFFF00000000ull)
    {
        if (isnan(f[rs1].f))
        {
            x[rd].u = f[rs1].u64 & 0xFFFFFFFFFFC00000ull;
            return;
        }
    }
    x[rd].u = f[rs1].u64;
}
//------------------------------------------------------------------------------
void riscv_cpu::FEQ_D()
{
    x[rd].u = (f[rs1].d == f[rs2].d);
    fcsr.nv = (issignaling(f[rs1].d) || issignaling(f[rs2].d));
}
//------------------------------------------------------------------------------
void riscv_cpu::FLT_D()
{
    x[rd].u = (f[rs1].d < f[rs2].d);
    fcsr.nv = (isnan(f[rs1].d) || isnan(f[rs2].d));
}
//------------------------------------------------------------------------------
void riscv_cpu::FLE_D()
{
    x[rd].u = (f[rs1].d <= f[rs2].d);
    fcsr.nv = (isnan(f[rs1].d) || isnan(f[rs2].d));
}
//------------------------------------------------------------------------------
void riscv_cpu::FCLASS_D()
{
    float64_t value; value = f[rs1].d;
    uint64_t binary = uint64_t(value.u);
    int fclass = 0;
    if (binary <= 0xFFFFFFFFFFFFFFFFull)    fclass = (1 << 9);
    if (binary <  0xFFF8000000000000ull)    fclass = (1 << 8);
    if (binary == 0xFFF0000000000000ull)    fclass = (1 << 0);
    if (binary <  0xFFF0000000000000ull)    fclass = (1 << 1);
    if (binary <  0x8010000000000000ull)    fclass = (1 << 2);
    if (binary == 0x8000000000000000ull)    fclass = (1 << 3);
    if (binary <= 0x7FFFFFFFFFFFFFFFull)    fclass = (1 << 9);
    if (binary <  0x7FF8000000000000ull)    fclass = (1 << 8);
    if (binary == 0x7FF0000000000000ull)    fclass = (1 << 7);
    if (binary <  0x7FF0000000000000ull)    fclass = (1 << 6);
    if (binary <  0x0010000000000000ull)    fclass = (1 << 5);
    if (binary == 0x0000000000000000ull)    fclass = (1 << 4);
    x[rd].u = fclass;
}
//------------------------------------------------------------------------------
void riscv_cpu::FCVT_D_W()
{
    f[rd].d = x[rs1].s32;
}
//------------------------------------------------------------------------------
void riscv_cpu::FCVT_D_WU()
{
    f[rd].d = x[rs1].u32;
}
//------------------------------------------------------------------------------
void riscv_cpu::FMV_D_X()
{
    f[rd].u64 = x[rs1].u64;
}
//------------------------------------------------------------------------------
#endif
