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
// Table A.2 MIPS64 Encoding of the Opcode Field
//------------------------------------------------------------------------------
const CPU::FINSTRUCTION CPU::tableOPCODE[8 * 8] =
{
    o SPECIAL   x REGIMM    x J     x JAL   x BEQ   x BNE   x POP06 x POP07
    x POP10     x ADDIU     x SLTI  x SLTIU x ANDI  x ORI   x XORI  x AUI
    x COP0      x COP1      x COP2  x ____  x ____  x ____  x POP26 x POP27
    x POP30     x DADDIU    x ____  x ____  x ____  x DAUI  x ____  x SPECIAL3
    x LB        x LH        x ____  x LW    x LBU   x LHU   x ____  x LWU
    x SB        x SH        x ____  x SW    x ____  x ____  x ____  x ____
    x ____      x LWC1      x BC    x ____  x ____  x LDC1  x POP66 x LD
    x ____      x SWC1      x BALC  x PCREL x ____  x SDC1  x POP76 x SD
};
#undef o
#undef x
//------------------------------------------------------------------------------
// Table A.29 B*C compact branch encodings
//------------------------------------------------------------------------------
void CPU::POP06()
{
    if (rs == rt)       (rs == 0) ? BLEZ()      : BGEZALC();
    else if (rs < rt)   (rs == 0) ? BLEZALC()   : BGEUC();
    else if (rs > rt)   (rt == 0) ? BLEZ()      : BGEUC();
}
//------------------------------------------------------------------------------
void CPU::POP07()
{
    if (rs == rt)       (rs == 0) ? BGTZ()      : BLTZALC();
    else if (rs < rt)   (rs == 0) ? BGTZALC()   : BLTUC();
    else if (rs > rt)   (rt == 0) ? BGTZ()      : BLTUC();
}
//------------------------------------------------------------------------------
void CPU::POP10()
{
    if (rs < rt)        (rs == 0) ? BEQZALC()   : BEQC();
    else if (rs == rt)  (rs == 0) ? BOVC()      : BOVC();
    else if (rs > rt)   (rt == 0) ? BOVC()      : BOVC();
}
//------------------------------------------------------------------------------
void CPU::POP26()
{
    if (rs == rt)       (rs == 0) ? ____()      : BGEZC();
    else if (rs < rt)   (rs == 0) ? BLEZC()     : BGEC();
    else if (rs > rt)   (rt == 0) ? ____()      : BGEC();
}
//------------------------------------------------------------------------------
void CPU::POP27()
{
    if (rs == rt)       (rs == 0) ? ____()      : BLTZC();
    else if (rs < rt)   (rs == 0) ? BGTZC()     : BLTC();
    else if (rs > rt)   (rt == 0) ? ____()      : BLTC();
}
//------------------------------------------------------------------------------
void CPU::POP30()
{
    if (rs < rt)        (rs == 0) ? BNEZALC()   : BNEC();
    else if (rs == rt)  (rs == 0) ? BNVC()      : BNVC();
    else if (rs > rt)   (rt == 0) ? BNVC()      : BNVC();
}
//------------------------------------------------------------------------------
void CPU::POP66()
{
    if (rs < rt)        (rs == 0) ? JIC()       : BEQZC();
    else if (rs == rt)  (rs == 0) ? JIC()       : BEQZC();
    else if (rs > rt)   (rt == 0) ? BEQZC()     : BEQZC();
}
//------------------------------------------------------------------------------
void CPU::POP76()
{
    if (rs < rt)        (rs == 0) ? JIALC()     : BNEZC();
    else if (rs == rt)  (rs == 0) ? JIALC()     : BNEZC();
    else if (rs > rt)   (rt == 0) ? BNEZC()     : BNEZC();
}
//------------------------------------------------------------------------------
// Table A.2 MIPS64 Encoding of the Opcode Field
//------------------------------------------------------------------------------
void CPU::BC()
{
    intptr_t target_offset = offset_26 << 2;
    PC = PC + 4 + target_offset;
    MIPS_DEBUG("BC", "PC: [%016zX]", PC);
}
//------------------------------------------------------------------------------
void CPU::BALC()
{
    intptr_t target_offset = offset_26 << 2;
    GPR[31] = PC + 4;
    PC = PC + 4 + target_offset;
    MIPS_DEBUG("BALC", "PC: [%016zX]", PC);
}
//------------------------------------------------------------------------------
void CPU::JIC()
{
    intptr_t temp = GPR[rt] + immediate;
    PC = temp;
    MIPS_DEBUG("JIC", "PC: [%016zX]", PC);
}
//------------------------------------------------------------------------------
void CPU::JIALC()
{
    intptr_t temp = GPR[rt] + immediate;
    GPR[31] = PC + 4;
    PC = temp;
    MIPS_DEBUG("JIALC", "PC: [%016zX]", PC);
}
//------------------------------------------------------------------------------
void CPU::BEQZC()
{
    intptr_t target_offset = offset_21 << 2;
    if (GPR[rs] == 0)
        PC = PC + 4 + target_offset;
    MIPS_DEBUG("BEQZC", "PC: [%016zX]", PC);
}
//------------------------------------------------------------------------------
void CPU::BNEZC()
{
    intptr_t target_offset = offset_21 << 2;
    if (GPR[rs] != 0)
        PC = PC + 4 + target_offset;
    MIPS_DEBUG("BNEZC", "PC: [%016zX]", PC);
}
//------------------------------------------------------------------------------
void CPU::BLEZC()
{
    intptr_t target_offset = offset_16 << 2;
    if (GPR[rt] <= 0)
        PC = PC + 4 + target_offset;
    MIPS_DEBUG("BLEZC", "PC: [%016zX]", PC);
}
//------------------------------------------------------------------------------
void CPU::BGEZC()
{
    intptr_t target_offset = offset_16 << 2;
    if (GPR[rt] >= 0)
        PC = PC + 4 + target_offset;
    MIPS_DEBUG("BGEZC", "PC: [%016zX]", PC);
}
//------------------------------------------------------------------------------
void CPU::BGTZC()
{
    intptr_t target_offset = offset_16 << 2;
    if (GPR[rt] > 0)
        PC = PC + 4 + target_offset;
    MIPS_DEBUG("BGTZC", "PC: [%016zX]", PC);
}
//------------------------------------------------------------------------------
void CPU::BLTZC()
{
    intptr_t target_offset = offset_16 << 2;
    if (GPR[rt] < 0)
        PC = PC + 4 + target_offset;
    MIPS_DEBUG("BLTZC", "PC: [%016zX]", PC);
}
//------------------------------------------------------------------------------
void CPU::BEQZALC()
{
    GPR[31] = PC + 4;
    intptr_t target_offset = offset_16 << 2;
    if (GPR[rt] == 0)
        PC = PC + 4 + target_offset;
    MIPS_DEBUG("BEQZALC", "PC: [%016zX]", PC);
}
//------------------------------------------------------------------------------
void CPU::BNEZALC()
{
    GPR[31] = PC + 4;
    intptr_t target_offset = offset_16 << 2;
    if (GPR[rt] != 0)
        PC = PC + 4 + target_offset;
    MIPS_DEBUG("BNEZALC", "PC: [%016zX]", PC);
}
//------------------------------------------------------------------------------
void CPU::BLEZALC()
{
    GPR[31] = PC + 4;
    intptr_t target_offset = offset_16 << 2;
    if (GPR[rt] <= 0)
        PC = PC + 4 + target_offset;
    MIPS_DEBUG("BLEZALC", "PC: [%016zX]", PC);
}
//------------------------------------------------------------------------------
void CPU::BGEZALC()
{
    GPR[31] = PC + 4;
    intptr_t target_offset = offset_16 << 2;
    if (GPR[rt] >= 0)
        PC = PC + 4 + target_offset;
    MIPS_DEBUG("BGEZALC", "PC: [%016zX]", PC);
}
//------------------------------------------------------------------------------
void CPU::BGTZALC()
{
    GPR[31] = PC + 4;
    intptr_t target_offset = offset_16 << 2;
    if (GPR[rt] > 0)
        PC = PC + 4 + target_offset;
    MIPS_DEBUG("BGTZALC", "PC: [%016zX]", PC);
}
//------------------------------------------------------------------------------
void CPU::BLTZALC()
{
    GPR[31] = PC + 4;
    intptr_t target_offset = offset_16 << 2;
    if (GPR[rt] < 0)
        PC = PC + 4 + target_offset;
    MIPS_DEBUG("BLTZALC", "PC: [%016zX]", PC);
}
//------------------------------------------------------------------------------
void CPU::BEQC()
{
    intptr_t target_offset = offset_16 << 2;
    if (GPR[rs] == GPR[rt])
        PC = PC + 4 + target_offset;
    MIPS_DEBUG("BEQC", "PC: [%016zX]", PC);
}
//------------------------------------------------------------------------------
void CPU::BNEC()
{
    intptr_t target_offset = offset_16 << 2;
    if (GPR[rs] != GPR[rt])
        PC = PC + 4 + target_offset;
    MIPS_DEBUG("BNEC", "PC: [%016zX]", PC);
}
//------------------------------------------------------------------------------
void CPU::BGEC()
{
    intptr_t target_offset = offset_16 << 2;
    if (GPR[rs] >= GPR[rt])
        PC = PC + 4 + target_offset;
    MIPS_DEBUG("BGEC", "PC: [%016zX]", PC);
}
//------------------------------------------------------------------------------
void CPU::BLTC()
{
    intptr_t target_offset = offset_16 << 2;
    if (GPR[rs] < GPR[rt])
        PC = PC + 4 + target_offset;
    MIPS_DEBUG("BLTC", "PC: [%016zX]", PC);
}
//------------------------------------------------------------------------------
void CPU::BGEUC()
{
    intptr_t target_offset = offset_16 << 2;
    if (uGPR[rs] >= uGPR[rt])
        PC = PC + 4 + target_offset;
    MIPS_DEBUG("BGEUC", "PC: [%016zX]", PC);
}
//------------------------------------------------------------------------------
void CPU::BLTUC()
{
    intptr_t target_offset = offset_16 << 2;
    if (uGPR[rs] < uGPR[rt])
        PC = PC + 4 + target_offset;
    MIPS_DEBUG("BLTUC", "PC: [%016zX]", PC);
}
//------------------------------------------------------------------------------
void CPU::BLEC()
{
    BGEC();
}
//------------------------------------------------------------------------------
void CPU::BGTC()
{
    BLTC();
}
//------------------------------------------------------------------------------
void CPU::BLEUC()
{
    BGEUC();
}
//------------------------------------------------------------------------------
void CPU::BGTUC()
{
    BLTUC();
}
//------------------------------------------------------------------------------
void CPU::BOVC()
{
    bool input_overflow = (GPR[rs] != GPR32[rs]) | (GPR[rt] != GPR32[rt]);

    int temp1 = GPR32[rs];
    int temp2 = GPR32[rt];
    int tempd = temp1 + temp2;
    bool sum_overflow = (tempd ^ (tempd << 1)) & 0x80000000;

    if (input_overflow || sum_overflow)
    {
        intptr_t target_offset = offset_16 << 2;
        PC = PC + 4 + target_offset;
    }
    MIPS_DEBUG("BOVC", "PC: [%016zX]", PC);
}
//------------------------------------------------------------------------------
void CPU::BNVC()
{
    bool input_overflow = (GPR[rs] != GPR32[rs]) | (GPR[rt] != GPR32[rt]);

    int temp1 = GPR32[rs];
    int temp2 = GPR32[rt];
    int tempd = temp1 + temp2;
    bool sum_overflow = (tempd ^ (tempd << 1)) & 0x80000000;

    if (!input_overflow && !sum_overflow)
    {
        intptr_t target_offset = offset_16 << 2;
        PC = PC + 4 + target_offset;
    }
    MIPS_DEBUG("BNVC", "PC: [%016zX]", PC);
}
//------------------------------------------------------------------------------
// Table A.2 MIPS64 Encoding of the Opcode Field
//------------------------------------------------------------------------------
void CPU::SPECIAL()
{
    FINSTRUCTION SPECIAL = tableSPECIAL[function];
    (this->*SPECIAL)();
}
//------------------------------------------------------------------------------
void CPU::REGIMM()
{
    FINSTRUCTION REGIMM = tableREGIMM[rt];
    (this->*REGIMM)();
}
//------------------------------------------------------------------------------
void CPU::J()
{
    BranchDelaySlot();
    PC = (PC & 0xF0000000) | (instr_index << 2);
    MIPS_DEBUG("J", "PC: [%016zX]", PC);
}
//------------------------------------------------------------------------------
void CPU::JAL()
{
    BranchDelaySlot();
    GPR[31] = PC + 8;
    PC = (PC & 0xF0000000) | (instr_index << 2);
    MIPS_DEBUG("JAL", "PC: [%016zX]", PC);
}
//------------------------------------------------------------------------------
void CPU::BEQ()
{
    bool condition = (GPR[rs] == GPR[rt]);
    BranchDelaySlot();
    if (condition)
        PC += immediate << 2;
    else
        PC += 4;
    PC += 4;
    if (rs == rt && rt == 0)
    {
        MIPS_DEBUG("B", "PC: [%016zX]", PC);
    }
    else if (rt == 0)
    {
        MIPS_DEBUG("BEQZ", "PC: [%016zX] = GPR:%-2u[%016zX] == 0", PC, rs, GPR[rs]);
    }
    else
    {
        MIPS_DEBUG("BEQ", "PC: [%016zX] = GPR:%-2u[%016zX] == GPR:%-2u[%016zX]", PC, rs, GPR[rs], rt, GPR[rt]);
    }
}
//------------------------------------------------------------------------------
void CPU::BNE()
{
    bool condition = (GPR[rs] != GPR[rt]);
    BranchDelaySlot();
    if (condition)
        PC += immediate << 2;
    else
        PC += 4;
    PC += 4;
    if (rt == 0)
    {
        MIPS_DEBUG("BNEZ", "PC: [%016zX] = GPR:%-2u[%016zX] != 0", PC, rs, GPR[rs]);
    }
    else
    {
        MIPS_DEBUG("BNE", "PC: [%016zX] = GPR:%-2u[%016zX] != GPR:%-2u[%016zX]", PC, rs, GPR[rs], rt, GPR[rt]);
    }
}
//------------------------------------------------------------------------------
void CPU::BLEZ()
{
    bool condition = (GPR[rs] <= 0);
    BranchDelaySlot();
    if (condition)
        PC += immediate << 2;
    else
        PC += 4;
    PC += 4;
    MIPS_DEBUG("BLEZ", "PC: [%016zX] = GPR:%-2u[%016zX] <= 0", PC, rs, GPR[rs]);
}
//------------------------------------------------------------------------------
void CPU::BGTZ()
{
    bool condition = (GPR[rs] > 0);
    BranchDelaySlot();
    if (condition)
        PC += immediate << 2;
    else
        PC += 4;
    PC += 4;
    MIPS_DEBUG("BGTZ", "PC: [%016zX] = GPR:%-2u[%016zX] > 0", PC, rs, GPR[rs]);
}
//------------------------------------------------------------------------------
void CPU::ADDIU()
{
    GPR[rt] = GPR32[rs] + immediate;
    MIPS_DEBUG("ADDIU", "GPR:%-2u[%016zX] = GPR:%-2u[%016zX] + IMM:%X", rt, GPR[rt], rs, GPR[rs], immediate);
}
//------------------------------------------------------------------------------
void CPU::SLTI()
{
    GPR[rt] = (GPR[rs] < immediate) ? 1 : 0;
    MIPS_DEBUG("SLTI", "GPR:%-2u[%016zX] = GPR:%-2u[%016zX] < IMM:%X", rt, GPR[rt], rs, GPR[rs], immediate);
}
//------------------------------------------------------------------------------
void CPU::SLTIU()
{
    uGPR[rt] = (uGPR[rs] < immediate_unsigned) ? 1 : 0;
    MIPS_DEBUG("SLTIU", "GPR:%-2u[%016zX] = GPR:%-2u[%016zX] < IMM:%X", rt, uGPR[rt], rs, GPR[rs], immediate_unsigned);
}
//------------------------------------------------------------------------------
void CPU::ANDI()
{
    GPR[rt] = GPR[rs] & immediate_unsigned;
    MIPS_DEBUG("ANDI", "GPR:%-2u[%016zX] = GPR:%-2u[%016zX] & IMM:%X", rt, GPR[rt], rs, GPR[rs], immediate_unsigned);
}
//------------------------------------------------------------------------------
void CPU::ORI()
{
    GPR[rt] = GPR[rs] | immediate_unsigned;
    MIPS_DEBUG("ORI", "GPR:%-2u[%016zX] = GPR:%-2u[%016zX] | IMM:%X", rt, GPR[rt], rs, GPR[rs], immediate_unsigned);
}
//------------------------------------------------------------------------------
void CPU::XORI()
{
    GPR[rt] = GPR[rs] ^ immediate_unsigned;
    MIPS_DEBUG("XORI", "GPR:%-2u[%016zX] = GPR:%-2u[%016zX] ^ IMM:%X", rt, GPR[rt], rs, GPR[rs], immediate_unsigned);
}
//------------------------------------------------------------------------------
void CPU::AUI()
{
    GPR[rt] = GPR32[rs] + (immediate << 16);
    if (rs == 0)
    {
        MIPS_DEBUG("LUI", "GPR:%-2u[%016zX] = IMM:%X", rt, GPR[rt], immediate);
    }
    else
    {
        MIPS_DEBUG("AUI", "GPR:%-2u[%016zX] = GPR:%-2u[%016zX] + IMM:%X", rt, GPR[rt], rs, GPR[rs], immediate);
    }
}
//------------------------------------------------------------------------------
void CPU::COP0()
{
    cop0.Execute(Encode);
}
//------------------------------------------------------------------------------
void CPU::COP1()
{
    cop1.Execute(Encode);
}
//------------------------------------------------------------------------------
void CPU::COP2()
{
    cop2.Execute(Encode);
}
//------------------------------------------------------------------------------
void CPU::DADDIU()
{
#if (MIPS_BITS >= 64)
    GPR[rt] = GPR[rs] + immediate;
    MIPS_DEBUG("DADDIU", "GPR:%-2u[%016zX] = GPR:%-2u[%016zX] + IMM:%X", rt, GPR[rt], rs, GPR[rs], immediate);
#endif
}
//------------------------------------------------------------------------------
void CPU::DAUI()
{
#if (MIPS_BITS >= 64)
    GPR[rt] = GPR[rs] + (immediate << 16);
    MIPS_DEBUG("DAUI", "GPR:%-2u[%016zX] = GPR:%-2u[%016zX] + IMM:%X", rt, GPR[rt], rs, GPR[rs], immediate);
#endif
}
//------------------------------------------------------------------------------
void CPU::SPECIAL3()
{
    FINSTRUCTION SPECIAL3 = tableSPECIAL3[function];
    (this->*SPECIAL3)();
}
//------------------------------------------------------------------------------
void CPU::PCREL()
{
    FINSTRUCTION PCREL = tablePCREL[rt];
    (this->*PCREL)();
}
//------------------------------------------------------------------------------
void CPU::LB()
{
    intptr_t address = GPR[rs] + immediate;
    signed char load = *(signed char*)address;
    GPR[rt] = load;
    MIPS_DEBUG("LB", "GPR:%-2u[%016zX] = GPR:%-2u[%016zX] + IMM:%X", rt, GPR[rt], rs, GPR[rs], immediate);
}
//------------------------------------------------------------------------------
void CPU::LH()
{
    intptr_t address = GPR[rs] + immediate;
    signed short load;
    memcpy(&load, (void*)address, sizeof(short));
    GPR[rt] = load;
    MIPS_DEBUG("LH", "GPR:%-2u[%016zX] = GPR:%-2u[%016zX] + IMM:%X", rt, GPR[rt], rs, GPR[rs], immediate);
}
//------------------------------------------------------------------------------
void CPU::LW()
{
    intptr_t address = GPR[rs] + immediate;
    signed int load;
    memcpy(&load, (void*)address, sizeof(int));
    GPR[rt] = load;
    MIPS_DEBUG("LW", "GPR:%-2u[%016zX] = GPR:%-2u[%016zX] + IMM:%X[%016zX]", rt, GPR[rt], rs, GPR[rs], immediate, nAddress);
}
//------------------------------------------------------------------------------
void CPU::LBU()
{
    intptr_t address = GPR[rs] + immediate;
    unsigned char load = *(char*)address;
    GPR[rt] = load;
    MIPS_DEBUG("LBU", "GPR:%-2u[%016zX] = GPR:%-2u[%016zX] + IMM:%X", rt, GPR[rt], rs, GPR[rs], immediate);
}
//------------------------------------------------------------------------------
void CPU::LHU()
{
    intptr_t address = GPR[rs] + immediate;
    unsigned short load;
    memcpy(&load, (void*)address, sizeof(short));
    GPR[rt] = load;
    MIPS_DEBUG("LHU", "GPR:%-2u[%016zX] = GPR:%-2u[%016zX] + IMM:%X", rt, GPR[rt], rs, GPR[rs], immediate);
}
//------------------------------------------------------------------------------
void CPU::LWU()
{
#if (MIPS_BITS >= 64)
    intptr_t address = GPR[rs] + immediate;
    unsigned int load;
    memcpy(&load, (void*)address, sizeof(int));
    GPR[rt] = load;
    MIPS_DEBUG("LWU", "GPR:%-2u[%016zX] = GPR:%-2u[%016zX] + IMM:%X", rt, GPR[rt], rs, GPR[rs], immediate);
#endif
}
//------------------------------------------------------------------------------
void CPU::SB()
{
    intptr_t address = GPR[rs] + immediate;
    char& store = *(char*)address;
    store = (char)GPR[rt];
    MIPS_DEBUG("SB", "GPR:%-2u[%016zX] + IMM:%X = GPR:%-2u[%016zX]", rs, GPR[rs], immediate, rt, GPR[rt]);
}
//------------------------------------------------------------------------------
void CPU::SH()
{
    intptr_t address = GPR[rs] + immediate;
    memcpy((void*)address, &GPR[rt], sizeof(short));
    MIPS_DEBUG("SH", "GPR:%-2u[%016zX] + IMM:%X = GPR:%-2u[%016zX]", rs, GPR[rs], immediate, rt, GPR[rt]);
}
//------------------------------------------------------------------------------
void CPU::SW()
{
    intptr_t address = GPR[rs] + immediate;
    memcpy((void*)address, &GPR[rt], sizeof(int));
    MIPS_DEBUG("SW", "GPR:%-2u[%016zX] + IMM:%X = GPR:%-2u[%016zX]", rs, GPR[rs], immediate, rt, GPR[rt]);
}
//------------------------------------------------------------------------------
void CPU::LWC1()
{
    cop1.LWC(Encode);
}
//------------------------------------------------------------------------------
void CPU::LDC1()
{
    cop1.LDC(Encode);
}
//------------------------------------------------------------------------------
void CPU::LD()
{
#if (MIPS_BITS >= 64)
    intptr_t address = GPR[rs] + immediate;
    intptr_t load;
    memcpy (&load, (void*)address, sizeof (intptr_t));
    GPR[rt] = load;
    MIPS_DEBUG("LD", "GPR:%-2u[%016zX] = GPR:%-2u[%016zX] + IMM:%X[%016zX]", rt, GPR[rt], rs, GPR[rs], immediate, nAddress);
#endif
}
//------------------------------------------------------------------------------
void CPU::SWC1()
{
    cop1.SWC(Encode);
}
//------------------------------------------------------------------------------
void CPU::SDC1()
{
    cop1.SDC(Encode);
}
//------------------------------------------------------------------------------
void CPU::SD()
{
#if (MIPS_BITS >= 64)
    intptr_t address = GPR[rs] + immediate;
    memcpy((void*)address, &GPR[rt], sizeof (intptr_t));
    MIPS_DEBUG("SD", "GPR:%-2u[%016zX] + IMM:%X = GPR:%-2u[%016zX]", rs, GPR[rs], immediate, rt, GPR[rt]);
#endif
}
//------------------------------------------------------------------------------
