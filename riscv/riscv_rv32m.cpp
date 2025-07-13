//==============================================================================
// The RISC-V Instruction Set Manual
// Volume I: Unprivileged ISA
// Document Version 20191213
// December 13, 2019
//==============================================================================

#include "riscv_cpu.h"
#if defined(__LP64__)
typedef signed __int128 int128_t;
typedef unsigned __int128 uint128_t;
#endif

//------------------------------------------------------------------------------
void riscv_cpu::MUL()
{
    x[rd] = x[rs1].s * x[rs2].s;
}
//------------------------------------------------------------------------------
void riscv_cpu::MULH()
{
#if defined(__LP64__)
    x[rd] = ((int128_t)x[rs1].s * (int128_t)x[rs2].s) >> 64;
#else
    x[rd] = ((int64_t)x[rs1].s * (int64_t)x[rs2].s) >> 32;
#endif
}
//------------------------------------------------------------------------------
void riscv_cpu::MULHSU()
{
#if defined(__LP64__)
    x[rd] = ((int128_t)x[rs1].s * (uint128_t)x[rs2].u) >> 64;
#else
    x[rd] = ((int64_t)x[rs1].s * (uint64_t)x[rs2].u) >> 32;
#endif
}
//------------------------------------------------------------------------------
void riscv_cpu::MULHU()
{
#if defined(__LP64__)
    x[rd] = ((uint128_t)x[rs1].u * (uint128_t)x[rs2].u) >> 64;
#else
    x[rd] = ((uint64_t)x[rs1].u * (uint64_t)x[rs2].u) >> 32;
#endif
}
//------------------------------------------------------------------------------
void riscv_cpu::DIV()
{
#if defined(__i386__) || defined(__amd64__)
    if (x[rs1].s == INTPTR_MIN && x[rs2].s == -1)
    {
        x[rd] = x[rs1].s;
        return;
    }
#endif
    x[rd] = x[rs2].s ? x[rs1].s / x[rs2].s : -1;
}
//------------------------------------------------------------------------------
void riscv_cpu::DIVU()
{
    x[rd] = x[rs2].u ? x[rs1].u / x[rs2].u : UINTPTR_MAX;
}
//------------------------------------------------------------------------------
void riscv_cpu::REM()
{
#if defined(__i386__) || defined(__amd64__)
    if (x[rs1].s == INTPTR_MIN && x[rs2].s == -1)
    {
        x[rd] = 0;
        return;
    }
#endif
    x[rd] = x[rs2].s ? x[rs1].s % x[rs2].s : x[rs1];
}
//------------------------------------------------------------------------------
void riscv_cpu::REMU()
{
    x[rd] = x[rs2].u ? x[rs1].u % x[rs2].u : x[rs1];
}
//------------------------------------------------------------------------------
