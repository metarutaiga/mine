//==============================================================================
// The RISC-V Instruction Set Manual
// Volume I: Unprivileged ISA
// Document Version 20191213
// December 13, 2019
//==============================================================================

#include "riscv_cpu.h"
#include "riscv_float.h"

#if RISCV_HAVE_SINGLE
//------------------------------------------------------------------------------
static bool issignaling(float f32)
{
    int u32 = (int&)f32;
    return (u32 > 0x7f800000 && u32 < 0x7fc00000) || (u32 > 0xff800000 && u32 < 0xffc00000);
}
//------------------------------------------------------------------------------
void riscv_cpu::FLW()
{
    f[rd].f = *(float*)(x[rs1] + simmI());
}
//------------------------------------------------------------------------------
void riscv_cpu::FSW()
{
    *(uint32_t*)(x[rs1] + simmS()) = f[rs2].u32;
}
//------------------------------------------------------------------------------
void riscv_cpu::FMADD_S()
{
    fclearexcept();
    f[rd].f = (f[rs1].f * f[rs2].f) + f[rs3].f;
    ftestexcept();
    if (isnan(f[rd].f))
    {
        f[rd].f = NAN;
        return;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::FMSUB_S()
{
    fclearexcept();
    f[rd].f = (f[rs1].f * f[rs2].f) - f[rs3].f;
    ftestexcept();
    if (isnan(f[rd].f))
    {
        f[rd].f = NAN;
        return;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::FNMSUB_S()
{
    fclearexcept();
    f[rd].f = -(f[rs1].f * f[rs2].f) + f[rs3].f;
    ftestexcept();
    if (isnan(f[rd].f))
    {
        f[rd].f = NAN;
        return;
    }

}
//------------------------------------------------------------------------------
void riscv_cpu::FNMADD_S()
{
    fclearexcept();
    f[rd].f = -(f[rs1].f * f[rs2].f) - f[rs3].f;
    ftestexcept();
    if (isnan(f[rd].f))
    {
        f[rd].f = NAN;
        return;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::FADD_S()
{
    fclearexcept();
    f[rd].f = f[rs1].f + f[rs2].f;
    ftestexcept();
    if (isnan(f[rd].f))
    {
        f[rd].f = NAN;
        return;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::FSUB_S()
{
    fclearexcept();
    f[rd].f = f[rs1].f - f[rs2].f;
    ftestexcept();
    if (isnan(f[rd].f))
    {
        f[rd].f = NAN;
        return;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::FMUL_S()
{
    fclearexcept();
    f[rd].f = f[rs1].f * f[rs2].f;
    ftestexcept();
    if (isnan(f[rd].f))
    {
        f[rd].f = NAN;
        return;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::FDIV_S()
{
    fclearexcept();
    f[rd].f = f[rs1].f / f[rs2].f;
    ftestexcept();
    if (isnan(f[rd].f))
    {
        f[rd].f = NAN;
        return;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::FSQRT_S()
{
    fclearexcept();
    f[rd].f = sqrtf(f[rs1].f);
    ftestexcept();
    if (isnan(f[rd].f))
    {
        f[rd].f = NAN;
        return;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::FSGNJ_S()
{
    float32_t value; value = (float)f[rs1].f;
    float32_t sign; sign = (float)f[rs2].f;
    f[rd].u = (value.u & ~0x80000000ull) | (sign.u & 0x80000000ull);
}
//------------------------------------------------------------------------------
void riscv_cpu::FSGNJN_S()
{
    float32_t value; value = (float)f[rs1].f;
    float32_t sign; sign = (float)f[rs2].f;
    f[rd].u = (value.u & ~0x80000000ull) | ((sign.u & 0x80000000ull) ^ 0x80000000ull);
}
//------------------------------------------------------------------------------
void riscv_cpu::FSGNJX_S()
{
    float32_t value; value = (float)f[rs1].f;
    float32_t sign; sign = (float)f[rs2].f;
    f[rd].u = value.u ^ (sign.u & 0x80000000ull);
}
//------------------------------------------------------------------------------
void riscv_cpu::FMIN_S()
{
    if (f[rs1].f == 0.0f && f[rs2].f == 0.0f)
    {
        f[rd].f = f[rs1].f * f[rs2].f;
        return;
    }
    if (isnan(f[rs1].f) || isnan(f[rs2].f))
    {
        if (isnan(f[rs1].f) && isnan(f[rs2].f))
        {
            f[rd].f = NAN;
        }
        else if (isnan(f[rs1].f))
        {
            f[rd].f = f[rs2].f;
        }
        else
        {
            f[rd].f = f[rs1].f;
        }
    }
    else
    {
        f[rd].f = fminf(f[rs1].f, f[rs2].f);
    }
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
    if (isnan(f[rs1].f) || isnan(f[rs2].f))
    {
        if (isnan(f[rs1].f) && isnan(f[rs2].f))
        {
            f[rd].f = NAN;
        }
        else if (isnan(f[rs1].f))
        {
            f[rd].f = f[rs2].f;
        }
        else
        {
            f[rd].f = f[rs1].f;
        }
    }
    else
    {
        f[rd].f = fmaxf(f[rs1].f, f[rs2].f);
    }
    fcsr.nv = (issignaling(f[rs1].f) || issignaling(f[rs2].f));
}
//------------------------------------------------------------------------------
void riscv_cpu::FCVT_W_S()
{
    float value = f[rs1].f;
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
void riscv_cpu::FCVT_WU_S()
{
    float value = f[rs1].f;
    if (value <= -1.0f || value == -INFINITY)
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
void riscv_cpu::FMV_X_W()
{
#if RISCV_HAVE_DOUBLE
    if ((f[rs1].u64 & 0xFFFFFFFF00000000ull) == 0xFFFFFFFF00000000ull)
    {
        if (isnan(f[rs1].f))
        {
            x[rd].u = int32_t(f[rs1].u & 0xFFC00000);
            return;
        }
    }
#endif
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
    float32_t value; value = f[rs1].f;
    uint32_t binary = uint32_t(value.u);
    int fclass = 0;
    if (binary <= 0xFFFFFFFF)   fclass = (1 << 9);
    if (binary <  0xFFC00000)   fclass = (1 << 8);
    if (binary == 0xFF800000)   fclass = (1 << 0);
    if (binary <  0xFF800000)   fclass = (1 << 1);
    if (binary <  0x80800000)   fclass = (1 << 2);
    if (binary == 0x80000000)   fclass = (1 << 3);
    if (binary <= 0x7FFFFFFF)   fclass = (1 << 9);
    if (binary <  0x7FC00000)   fclass = (1 << 8);
    if (binary == 0x7F800000)   fclass = (1 << 7);
    if (binary <  0x7F800000)   fclass = (1 << 6);
    if (binary <  0x00800000)   fclass = (1 << 5);
    if (binary == 0x00000000)   fclass = (1 << 4);
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
    f[rd].f = (float&)x[rs1].s32;
}
//------------------------------------------------------------------------------
#endif
