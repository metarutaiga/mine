//==============================================================================
// The RISC-V Instruction Set Manual
// Volume I: Unprivileged ISA
// Document Version 20191213
// December 13, 2019
//==============================================================================

#include <math.h>
#include <float.h>
#include "riscv_cpu.h"

//------------------------------------------------------------------------------
void riscv_cpu::FCVT_L_S()
{
    switch (funct3)
    {
    case 0b000:
        x[rd].s = int64_t(lround(f[rs1].f));
        break;
    case 0b001:
        x[rd].s = int64_t(f[rs1].f);
        break;
    case 0b010:
        x[rd].s = int64_t(floorf(f[rs1].f));
        break;
    case 0b011:
        x[rd].s = int64_t(ceilf(f[rs1].f));
        break;
    case 0b100:
        x[rd].s = int64_t(f[rs1].f + 0.5f);
        break;
    case 0b101:
    case 0b110:
        HINT();
        break;
    case 0b111:
        x[rd].s = int64_t(lrintf(f[rs1].f));
        break;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::FCVT_LU_S()
{
    switch (funct3)
    {
    case 0b000:
        x[rd].u = uint64_t(lround(f[rs1].f));
        break;
    case 0b001:
        x[rd].u = uint64_t(f[rs1].f);
        break;
    case 0b010:
        x[rd].u = uint64_t(floorf(f[rs1].f));
        break;
    case 0b011:
        x[rd].u = uint64_t(ceilf(f[rs1].f));
        break;
    case 0b100:
        x[rd].u = uint64_t(f[rs1].f + 0.5f);
        break;
    case 0b101:
    case 0b110:
        HINT();
        break;
    case 0b111:
        x[rd].u = uint64_t(lrintf(f[rs1].f));
        break;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::FCVT_S_L()
{
    
}
//------------------------------------------------------------------------------
void riscv_cpu::FCVT_S_LU()
{
    
}
//------------------------------------------------------------------------------
