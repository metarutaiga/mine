//==============================================================================
// The RISC-V Instruction Set Manual
// Volume I: Unprivileged ISA
// Document Version 20191213
// December 13, 2019
//==============================================================================

#include "riscv_cpu.h"

//------------------------------------------------------------------------------
void riscv_cpu::LR_D()
{
    switch (funct7 & 0b11)
    {
    case 0b00:
    case 0b01:
        x[rd].u = __atomic_load_n((int64_t*)x[rs1].u, __ATOMIC_RELAXED);
        reservation = x[rs1].u;
        break;
    case 0b10:
    case 0b11:
        x[rd].u = __atomic_load_n((int64_t*)x[rs1].u, __ATOMIC_ACQUIRE);
        reservation = x[rs1].u;
        break;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::SC_D()
{
    switch (funct7 & 0b11)
    {
    case 0b00:
    case 0b10:
        if (reservation == x[rs1].u)
        {
            __atomic_store_n((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_RELAXED);
            reservation = 0;
            x[rd].u = 0;
        }
        else
        {
            x[rd].u = 1;
        }
        break;
    case 0b01:
    case 0b11:
        if (reservation == x[rs1].u)
        {
            __atomic_store_n((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_RELEASE);
            reservation = 0;
            x[rd].u = 0;
        }
        else
        {
            x[rd].u = 1;
        }
        break;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::AMOSWAP_D()
{
    switch (funct7 & 0b11)
    {
    case 0b00:
        x[rd].u = __atomic_exchange_n((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_RELAXED);
        break;
    case 0b01:
        x[rd].u = __atomic_exchange_n((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_RELEASE);
        break;
    case 0b10:
        x[rd].u = __atomic_exchange_n((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_ACQUIRE);
        break;
    case 0b11:
        x[rd].u = __atomic_exchange_n((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_ACQ_REL);
        break;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::AMOADD_D()
{
    switch (funct7 & 0b11)
    {
    case 0b00:
        x[rd].u = __atomic_fetch_add((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_RELAXED);
        break;
    case 0b01:
        x[rd].u = __atomic_fetch_add((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_RELEASE);
        break;
    case 0b10:
        x[rd].u = __atomic_fetch_add((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_ACQUIRE);
        break;
    case 0b11:
        x[rd].u = __atomic_fetch_add((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_ACQ_REL);
        break;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::AMOXOR_D()
{
    switch (funct7 & 0b11)
    {
    case 0b00:
        x[rd].u = __atomic_fetch_xor((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_RELAXED);
        break;
    case 0b01:
        x[rd].u = __atomic_fetch_xor((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_RELEASE);
        break;
    case 0b10:
        x[rd].u = __atomic_fetch_xor((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_ACQUIRE);
        break;
    case 0b11:
        x[rd].u = __atomic_fetch_xor((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_ACQ_REL);
        break;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::AMOAND_D()
{
    switch (funct7 & 0b11)
    {
    case 0b00:
        x[rd].u = __atomic_fetch_and((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_RELAXED);
        break;
    case 0b01:
        x[rd].u = __atomic_fetch_and((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_RELEASE);
        break;
    case 0b10:
        x[rd].u = __atomic_fetch_and((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_ACQUIRE);
        break;
    case 0b11:
        x[rd].u = __atomic_fetch_and((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_ACQ_REL);
        break;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::AMOOR_D()
{
    switch (funct7 & 0b11)
    {
    case 0b00:
        x[rd].u = __atomic_fetch_or((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_RELAXED);
        break;
    case 0b01:
        x[rd].u = __atomic_fetch_or((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_RELEASE);
        break;
    case 0b10:
        x[rd].u = __atomic_fetch_or((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_ACQUIRE);
        break;
    case 0b11:
        x[rd].u = __atomic_fetch_or((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_ACQ_REL);
        break;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::AMOMIN_D()
{
    switch (funct7 & 0b11)
    {
    case 0b00:
        x[rd].u = __atomic_fetch_min((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_RELAXED);
        break;
    case 0b01:
        x[rd].u = __atomic_fetch_min((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_RELEASE);
        break;
    case 0b10:
        x[rd].u = __atomic_fetch_min((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_ACQUIRE);
        break;
    case 0b11:
        x[rd].u = __atomic_fetch_min((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_ACQ_REL);
        break;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::AMOMAX_D()
{
    switch (funct7 & 0b11)
    {
    case 0b00:
        x[rd].u = __atomic_fetch_max((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_RELAXED);
        break;
    case 0b01:
        x[rd].u = __atomic_fetch_max((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_RELEASE);
        break;
    case 0b10:
        x[rd].u = __atomic_fetch_max((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_ACQUIRE);
        break;
    case 0b11:
        x[rd].u = __atomic_fetch_max((int64_t*)x[rs1].u, x[rs2].s64, __ATOMIC_ACQ_REL);
        break;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::AMOMINU_D()
{
    switch (funct7 & 0b11)
    {
    case 0b00:
        x[rd].u = __atomic_fetch_min((uint64_t*)x[rs1].u, x[rs2].u64, __ATOMIC_RELAXED);
        break;
    case 0b01:
        x[rd].u = __atomic_fetch_min((uint64_t*)x[rs1].u, x[rs2].u64, __ATOMIC_RELEASE);
        break;
    case 0b10:
        x[rd].u = __atomic_fetch_min((uint64_t*)x[rs1].u, x[rs2].u64, __ATOMIC_ACQUIRE);
        break;
    case 0b11:
        x[rd].u = __atomic_fetch_min((uint64_t*)x[rs1].u, x[rs2].u64, __ATOMIC_ACQ_REL);
        break;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::AMOMAXU_D()
{
    switch (funct7 & 0b11)
    {
    case 0b00:
        x[rd].u = __atomic_fetch_max((uint64_t*)x[rs1].u, x[rs2].u64, __ATOMIC_RELAXED);
        break;
    case 0b01:
        x[rd].u = __atomic_fetch_max((uint64_t*)x[rs1].u, x[rs2].u64, __ATOMIC_RELEASE);
        break;
    case 0b10:
        x[rd].u = __atomic_fetch_max((uint64_t*)x[rs1].u, x[rs2].u64, __ATOMIC_ACQUIRE);
        break;
    case 0b11:
        x[rd].u = __atomic_fetch_max((uint64_t*)x[rs1].u, x[rs2].u64, __ATOMIC_ACQ_REL);
        break;
    }
}
//------------------------------------------------------------------------------
