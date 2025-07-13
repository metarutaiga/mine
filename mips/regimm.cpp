//==============================================================================
// MIPS(R) Architecture For Programmers Volume II-A: The MIPS64(R) Instruction Set Reference Manual
//
// Document Number: MD00087
// Revision 6.06
// December 15, 2016
//
// Copyright (C) Wave Computing, Inc. All rights reserved.
//==============================================================================

#include "cpu.h"

//------------------------------------------------------------------------------
#define o &CPU::
#define x , o
//------------------------------------------------------------------------------
// Table A.4 MIPS64 REGIMM Encoding of rt Field
//------------------------------------------------------------------------------
const CPU::FINSTRUCTION CPU::tableREGIMM[4 * 8] =
{
    o BLTZ  x BGEZ  x ____  x ____  x ____  x ____  x DAHI  x ____
    x ____  x ____  x ____  x ____  x ____  x ____  x ____  x ____
    x NAL   x BAL   x ____  x ____  x ____  x ____  x ____  x SIGRIE
    x ____  x ____  x ____  x ____  x ____  x ____  x DATI  x SYNCI
};
//------------------------------------------------------------------------------
#undef o
#undef x
//------------------------------------------------------------------------------
void CPU::BLTZ()
{
    bool condition = (GPR[rs] < 0);
    BranchDelaySlot();
    if (condition)
        PC += immediate << 2;
    else
        PC += 4;
    PC += 4;
    MIPS_DEBUG("BLTZ", "PC: [%016zX] = GPR:%-2u < 0", PC, rs);
}
//------------------------------------------------------------------------------
void CPU::BGEZ()
{
    bool condition = (GPR[rs] >= 0);
    BranchDelaySlot();
    if (condition)
        PC += immediate << 2;
    else
        PC += 4;
    PC += 4;
    MIPS_DEBUG("BGEZ", "PC: [%016zX] = GPR:%-2u >= 0", PC, rs);
}
//------------------------------------------------------------------------------
void CPU::DAHI()
{
#if (MIPS_BITS >= 64)
    GPR[rs] = GPR[rs] + ((intptr_t)immediate << 32);
    MIPS_DEBUG("DAHI", "GPR:%-2u[%016zX] = GPR:%-2u + IMM:%X", rs, GPR[rs], rs, immediate);
#endif
}
//------------------------------------------------------------------------------
void CPU::DATI()
{
#if (MIPS_BITS >= 64)
    GPR[rs] = GPR[rs] + ((intptr_t)immediate << 48);
    MIPS_DEBUG("DATI", "GPR:%-2u[%016zX] = GPR:%-2u + IMM:%X", rs, GPR[rs], rs, immediate);
#endif
}
//------------------------------------------------------------------------------
void CPU::NAL()
{
    GPR[31] = PC + 8;
    MIPS_DEBUG("NAL", "GPR:%-2u[%016zX] = PC", 31, GPR[31]);
}
//------------------------------------------------------------------------------
void CPU::BAL()
{
    BranchDelaySlot();
    intptr_t target_offset = offset_16 << 2;
    GPR[31] = PC + 8;
    PC = PC + target_offset;
    MIPS_DEBUG("BAL", "PC: [%016zX]", PC);
}
//------------------------------------------------------------------------------
void CPU::SIGRIE()
{
    SignalException(ReservedInstruction, immediate_unsigned);
    MIPS_DEBUG("SIGRIE", "Code:%X", immediate_unsigned);
}
//------------------------------------------------------------------------------
void CPU::SYNCI()
{
    MIPS_DEBUG("SYNCI", "%s", "");
}
//------------------------------------------------------------------------------
