//==============================================================================
// MIPS(R) Architecture For Programmers Volume II-A: The MIPS64(R) Instruction Set Reference Manual
//
// Document Number: MD00087
// Revision 6.06
// December 15, 2016
//
// Copyright (C) Wave Computing, Inc. All rights reserved.
//==============================================================================

#include <string.h>
#include "cpu.h"

//------------------------------------------------------------------------------
#define o &CPU::
#define x , o
//------------------------------------------------------------------------------
// Table A.27 Release 6 PC-relative family encoding
//------------------------------------------------------------------------------
const CPU::FINSTRUCTION CPU::tablePCREL[8 * 4] =
{
    o ADDIUPC   x ADDIUPC   x ADDIUPC   x ADDIUPC   x ADDIUPC   x ADDIUPC   x ADDIUPC   x ADDIUPC
    x LWPC      x LWPC      x LWPC      x LWPC      x LWPC      x LWPC      x LWPC      x LWPC
    x LWUPC     x LWUPC     x LWUPC     x LWUPC     x LWUPC     x LWUPC     x LWUPC     x LWUPC
    x LDPC      x LDPC      x LDPC      x LDPC      x ____      x ____      x AUIPC     x ALUIPC
};
//------------------------------------------------------------------------------
#undef o
#undef x
//------------------------------------------------------------------------------
void CPU::ADDIUPC()
{
    GPR[rs] = PC + (offset_19 << 2);
    MIPS_DEBUG("ADDIUPC", "GPR:%-2u[%016zX] = PC + IMM:%X", rs, GPR[rs], immediate);
}
//------------------------------------------------------------------------------
void CPU::LWPC()
{
    intptr_t address = PC + (offset_19 << 2);
    int memword;
    memcpy(&memword, (void*)address, sizeof(int));
    GPR[rs] = memword;
    MIPS_DEBUG("LWPC", "GPR:%-2u[%016zX] = PC + IMM:%X", rs, GPR[rs], offset_19);
}
//------------------------------------------------------------------------------
void CPU::LWUPC()
{
    intptr_t address = PC + (offset_19 << 2);
    unsigned int memword;
    memcpy(&memword, (void*)address, sizeof(int));
    GPR[rs] = memword;
    MIPS_DEBUG("LWUPC", "GPR:%-2u[%016zX] = PC + IMM:%X", rs, GPR[rs], offset_19);
}
//------------------------------------------------------------------------------
void CPU::LDPC()
{
    intptr_t address = (PC & 7) + offset_18;
    intptr_t memdoubleword;
    memcpy(&memdoubleword, (void*)address, sizeof(intptr_t));
    GPR[rs] = memdoubleword;
    MIPS_DEBUG("LDPC", "GPR:%-2u[%016zX] = PC + IMM:%X", rs, GPR[rs], offset_18);
}
//------------------------------------------------------------------------------
void CPU::AUIPC()
{
    GPR[rs] = PC + (immediate << 16);
    MIPS_DEBUG("AUIPC", "GPR:%-2u[%016zX] = PC + IMM:%X", rs, GPR[rs], immediate);
}
//------------------------------------------------------------------------------
void CPU::ALUIPC()
{
    GPR[rs] = ~0xFFFF & (PC + (immediate << 16));
    MIPS_DEBUG("ALUIPC", "GPR:%-2u[%016zX] = PC + IMM:%X", rs, GPR[rs], immediate);
}
//------------------------------------------------------------------------------
