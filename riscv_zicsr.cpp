//==============================================================================
// The RISC-V Instruction Set Manual
// Volume I: Unprivileged ISA
// Document Version 20191213
// December 13, 2019
//==============================================================================

#include "riscv_cpu.h"

//------------------------------------------------------------------------------
void riscv_cpu::CSRRW()
{
    switch (immI())
    {
    case 0x001:
        x[rd] = fcsr.fflags;
        if (rs1 == 0)
            break;
        fcsr.fflags = x[rs1].u32;
        break;
    case 0x002:
        x[rd] = fcsr.frm;
        if (rs1 == 0)
            break;
        fcsr.frm = x[rs1].u32;
        break;
    case 0x003:
        x[rd] = fcsr.u32;
        if (rs1 == 0)
            break;
        fcsr.fflags = x[rs1].fflags;
        fcsr.frm = x[rs1].frm;
        break;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::CSRRS()
{
    switch (immI())
    {
    case 0x001:
        x[rd] = fcsr.fflags;
        if (rs1 == 0)
            break;
        fcsr.fflags |= x[rs1].u32;
        break;
    case 0x002:
        x[rd] = fcsr.frm;
        if (rs1 == 0)
            break;
        fcsr.frm |= x[rs1].u32;
        break;
    case 0x003:
        x[rd] = fcsr.u32;
        if (rs1 == 0)
            break;
        fcsr.u32 |= x[rs1].u32;
        break;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::CSRRC()
{
    switch (immI())
    {
    case 0x001:
        x[rd] = fcsr.fflags;
        if (rs1 == 0)
            break;
        fcsr.fflags &= ~(x[rs1].u32);
        break;
    case 0x002:
        x[rd] = fcsr.frm;
        if (rs1 == 0)
            break;
        fcsr.frm &= ~(x[rs1].u32);
        break;
    case 0x003:
        x[rd] = fcsr.u32;
        if (rs1 == 0)
            break;
        fcsr.u32 &= ~(x[rs1].u32);
        break;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::CSRRWI()
{
    switch (immI())
    {
    case 0x001:
        x[rd] = fcsr.fflags;
        fcsr.fflags = rs1;
        break;
    case 0x002:
        x[rd] = fcsr.frm;
        fcsr.frm = rs1;
        break;
    case 0x003:
        x[rd] = fcsr.u32;
        fcsr.u32 = rs1;
        break;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::CSRRSI()
{
    switch (immI())
    {
    case 0x001:
        x[rd] = fcsr.fflags;
        fcsr.fflags |= rs1;
        break;
    case 0x002:
        x[rd] = fcsr.frm;
        fcsr.frm |= rs1;
        break;
    case 0x003:
        x[rd] = fcsr.u32;
        fcsr.u32 |= rs1;
        break;
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::CSRRCI()
{
    switch (immI())
    {
    case 0x001:
        x[rd] = fcsr.fflags;
        fcsr.fflags &= ~(rs1);
        break;
    case 0x002:
        x[rd] = fcsr.frm;
        fcsr.frm &= ~(rs1);
        break;
    case 0x003:
        x[rd] = fcsr.u32;
        fcsr.u32 &= ~(rs1);
        break;
    }
}
//------------------------------------------------------------------------------
