//==============================================================================
// The RISC-V Instruction Set Manual
// Volume I: Unprivileged ISA
// Document Version 20191213
// December 13, 2019
//==============================================================================

#include "riscv_cpu.h"

//------------------------------------------------------------------------------
void riscv_cpu::LR_W()
{
    switch (funct7 & 0b11)
    {
    case 0b00:
    case 0b01:
        x[rd].u = __atomic_load_n((int32_t*)x[rs1].u, __ATOMIC_RELAXED);
        reservation = x[rs1].u;
        break;
    case 0b10:
    case 0b11:
        x[rd].u = __atomic_load_n((int32_t*)x[rs1].u, __ATOMIC_ACQUIRE);
        reservation = x[rs1].u;
        break;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::SC_W()
{
    switch (funct7 & 0b11)
    {
    case 0b00:
    case 0b10:
        if (reservation == x[rs1].u)
        {
            __atomic_store_n((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_RELAXED);
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
            __atomic_store_n((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_RELEASE);
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
void riscv_cpu::AMOSWAP_W()
{
    switch (funct7 & 0b11)
    {
    case 0b00:
        x[rd].u = __atomic_exchange_n((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_RELAXED);
        break;
    case 0b01:
        x[rd].u = __atomic_exchange_n((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_RELEASE);
        break;
    case 0b10:
        x[rd].u = __atomic_exchange_n((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_ACQUIRE);
        break;
    case 0b11:
        x[rd].u = __atomic_exchange_n((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_ACQ_REL);
        break;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::AMOADD_W()
{
    switch (funct7 & 0b11)
    {
    case 0b00:
        x[rd].u = __atomic_fetch_add((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_RELAXED);
        break;
    case 0b01:
        x[rd].u = __atomic_fetch_add((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_RELEASE);
        break;
    case 0b10:
        x[rd].u = __atomic_fetch_add((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_ACQUIRE);
        break;
    case 0b11:
        x[rd].u = __atomic_fetch_add((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_ACQ_REL);
        break;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::AMOXOR_W()
{
    switch (funct7 & 0b11)
    {
    case 0b00:
        x[rd].u = __atomic_fetch_xor((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_RELAXED);
        break;
    case 0b01:
        x[rd].u = __atomic_fetch_xor((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_RELEASE);
        break;
    case 0b10:
        x[rd].u = __atomic_fetch_xor((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_ACQUIRE);
        break;
    case 0b11:
        x[rd].u = __atomic_fetch_xor((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_ACQ_REL);
        break;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::AMOAND_W()
{
    switch (funct7 & 0b11)
    {
    case 0b00:
        x[rd].u = __atomic_fetch_and((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_RELAXED);
        break;
    case 0b01:
        x[rd].u = __atomic_fetch_and((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_RELEASE);
        break;
    case 0b10:
        x[rd].u = __atomic_fetch_and((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_ACQUIRE);
        break;
    case 0b11:
        x[rd].u = __atomic_fetch_and((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_ACQ_REL);
        break;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::AMOOR_W()
{
    switch (funct7 & 0b11)
    {
    case 0b00:
        x[rd].u = __atomic_fetch_or((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_RELAXED);
        break;
    case 0b01:
        x[rd].u = __atomic_fetch_or((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_RELEASE);
        break;
    case 0b10:
        x[rd].u = __atomic_fetch_or((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_ACQUIRE);
        break;
    case 0b11:
        x[rd].u = __atomic_fetch_or((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_ACQ_REL);
        break;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::AMOMIN_W()
{
    switch (funct7 & 0b11)
    {
    case 0b00:
        x[rd].u = __atomic_fetch_min((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_RELAXED);
        break;
    case 0b01:
        x[rd].u = __atomic_fetch_min((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_RELEASE);
        break;
    case 0b10:
        x[rd].u = __atomic_fetch_min((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_ACQUIRE);
        break;
    case 0b11:
        x[rd].u = __atomic_fetch_min((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_ACQ_REL);
        break;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::AMOMAX_W()
{
    switch (funct7 & 0b11)
    {
    case 0b00:
        x[rd].u = __atomic_fetch_max((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_RELAXED);
        break;
    case 0b01:
        x[rd].u = __atomic_fetch_max((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_RELEASE);
        break;
    case 0b10:
        x[rd].u = __atomic_fetch_max((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_ACQUIRE);
        break;
    case 0b11:
        x[rd].u = __atomic_fetch_max((int32_t*)x[rs1].u, x[rs2].s32, __ATOMIC_ACQ_REL);
        break;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::AMOMINU_W()
{
    switch (funct7 & 0b11)
    {
    case 0b00:
        x[rd].u = (int32_t)__atomic_fetch_min((uint32_t*)x[rs1].u, x[rs2].u32, __ATOMIC_RELAXED);
        break;
    case 0b01:
        x[rd].u = (int32_t)__atomic_fetch_min((uint32_t*)x[rs1].u, x[rs2].u32, __ATOMIC_RELEASE);
        break;
    case 0b10:
        x[rd].u = (int32_t)__atomic_fetch_min((uint32_t*)x[rs1].u, x[rs2].u32, __ATOMIC_ACQUIRE);
        break;
    case 0b11:
        x[rd].u = (int32_t)__atomic_fetch_min((uint32_t*)x[rs1].u, x[rs2].u32, __ATOMIC_ACQ_REL);
        break;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::AMOMAXU_W()
{
    switch (funct7 & 0b11)
    {
    case 0b00:
        x[rd].u = (int32_t)__atomic_fetch_max((uint32_t*)x[rs1].u, x[rs2].u32, __ATOMIC_RELAXED);
        break;
    case 0b01:
        x[rd].u = (int32_t)__atomic_fetch_max((uint32_t*)x[rs1].u, x[rs2].u32, __ATOMIC_RELEASE);
        break;
    case 0b10:
        x[rd].u = (int32_t)__atomic_fetch_max((uint32_t*)x[rs1].u, x[rs2].u32, __ATOMIC_ACQUIRE);
        break;
    case 0b11:
        x[rd].u = (int32_t)__atomic_fetch_max((uint32_t*)x[rs1].u, x[rs2].u32, __ATOMIC_ACQ_REL);
        break;
    }
}
//------------------------------------------------------------------------------
