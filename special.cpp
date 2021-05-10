//==============================================================================
// MIPS(R) Architecture For Programmers Volume II-A: The MIPS64(R) Instruction Set Reference Manual
//
// Document Number: MD00087
// Revision 6.06
// December 15, 2016
//
// Copyright (C) Wave Computing, Inc. All rights reserved.
//==============================================================================

#include "platform.h"
#include "cpu.h"

//------------------------------------------------------------------------------
#define o &CPU::
#define x , o
//------------------------------------------------------------------------------
// Table A.3 MIPS64 SPECIAL Opcode Encoding of Function Field
//------------------------------------------------------------------------------
const CPU::FINSTRUCTION CPU::tableSPECIAL[8 * 8] =
{
    o SLL   x ____  x SRL   x SRA   x SLLV      x LSA       x SRLV      x SRAV
    x ____  x JALR  x ____  x ____  x SYSCALL   x BREAK     x SDBBP     x SYNC
    x CLZ   x CLO   x DCLZ  x DCLO  x DSLLV     x DLSA      x DSRLV     x DSRAV
    x SOP3x x SOP3x x SOP3x x SOP3x x SOP3x     x SOP3x     x SOP3x     x SOP3x
    x ADD   x ADDU  x SUB   x SUBU  x AND       x OR        x XOR       x NOR
    x ____  x ____  x SLT   x SLTU  x DADD      x DADDU     x DSUB      x DSUBU
    x TGE   x TGEU  x TLT   x TLTU  x TEQ       x SELEQZ    x TNE       x SELNEZ
    x DSLL  x ____  x DSRL  x DSRA  x DSLL32    x ____      x DSRL32    x DSRA32
};
//------------------------------------------------------------------------------
// Table A.26 Release 6 MUL/DIV encodings
//------------------------------------------------------------------------------
const CPU::FINSTRUCTION CPU::tableMULDIV[8 * 4] =
{
    o ____  x ____  x ____  x ____  x ____  x ____  x ____  x ____
    x ____  x ____  x ____  x ____  x ____  x ____  x ____  x ____
    x MUL   x MULU  x DIV   x DIVU  x DMUL  x DMULU x DDIV  x DDIVU
    x MUH   x MUHU  x MOD   x MODU  x DMUH  x DMUHU x DMOD  x DMODU
};
//------------------------------------------------------------------------------
#undef o
#undef x
//------------------------------------------------------------------------------
void CPU::SOP3x()
{
    FINSTRUCTION MULDIV = tableMULDIV[(immediate & 0x7) | ((immediate >> 3) & 18)];
    (this->*MULDIV)();
}
//------------------------------------------------------------------------------
void CPU::SLL()
{
    GPR[rd] = GPR32[rt] << sa;
    if (rd == 0 && rt == 0 && sa == 0)
    {
        MIPS_DEBUG("NOP", "%s", "");
    }
    else
    {
        MIPS_DEBUG("SLL", "GPR:%-2u[%016zX] = GPR:%-2u << %u", rd, GPR[rd], rt, sa);
    }
}
//------------------------------------------------------------------------------
void CPU::SRL()
{
    if (rs & 1)
        ROTR();
    else
        GPR[rd] = uGPR32[rt] >> sa;
}
//------------------------------------------------------------------------------
void CPU::ROTR()
{
    GPR[rd] = (GPR32[rt] << (32 - sa)) | (uGPR32[rt] >> sa);
    MIPS_DEBUG("ROTR", "GPR:%-2u[%016zX] = GPR:%-2u rot %u", rd, GPR[rd], rt, sa);
}
//------------------------------------------------------------------------------
void CPU::SRA()
{
    GPR[rd] = GPR32[rt] >> sa;
    MIPS_DEBUG("SRA", "GPR:%-2u[%016zX] = GPR:%-2u >> %u", rd, GPR[rd], rt, sa);
}
//------------------------------------------------------------------------------
void CPU::SLLV()
{
    GPR[rd] = GPR32[rt] << GPR32[rs];
    MIPS_DEBUG("SLLV", "GPR:%-2u[%016zX] = GPR:%-2u << GPR:%-2u", rd, GPR[rd], rt, rs);
}
//------------------------------------------------------------------------------
void CPU::LSA()
{
    GPR[rd] = (GPR32[rs] << ((sa & 3) + 1)) + GPR32[rt];
    MIPS_DEBUG("LSA", "GPR:%-2u[%016zX] = GPR:%-2u << SA:%-2u + GPR:%-2u", rd, GPR[rd], rs, sa, rt);
}
//------------------------------------------------------------------------------
void CPU::SRLV()
{
    if (sa & 1)
        ROTRV();
    else
        uGPR[rd] = uGPR32[rt] >> uGPR32[rs];
}
//------------------------------------------------------------------------------
void CPU::ROTRV()
{
    GPR[rd] = (GPR32[rt] << (32 - GPR32[rs])) | (uGPR32[rt] >> uGPR32[rs]);
    MIPS_DEBUG("ROTRV", "GPR:%-2u[%016zX] = GPR:%-2u rot GPR:%-2u", rd, GPR[rd], rt, rs);
}
//------------------------------------------------------------------------------
void CPU::SRAV()
{
    GPR[rd] = GPR32[rt] >> GPR32[rs];
    MIPS_DEBUG("SRA", "GPR:%-2u[%016zX] = GPR:%-2u >> GPR:%-2u", rd, GPR[rd], rt, rs);
}
//------------------------------------------------------------------------------
void CPU::JR()
{
    intptr_t temp = GPR[rs];
    BranchDelaySlot();
    PC = temp;
    MIPS_DEBUG("JR", "PC: [%016zX] = GPR:%-2u", PC, rs);
}
//------------------------------------------------------------------------------
void CPU::JALR()
{
    if (rd == 0)
    {
        JR();
    }
    else
    {
        intptr_t temp = GPR[rs];
        BranchDelaySlot();
        GPR[rd] = PC + 8;
        PC = temp;
        MIPS_DEBUG("JALR", "PC: [%016zX] = GPR:%-2u", PC, rs);
    }
}
//------------------------------------------------------------------------------
void CPU::SYSCALL()
{
    unsigned int code = instr_index >> 6;
    SignalException(SystemCall, code);
    MIPS_DEBUG("SYSCALL", "Code:%X", code);
}
//------------------------------------------------------------------------------
void CPU::BREAK()
{
    unsigned int code = instr_index >> 6;
    SignalException(Breakpoint, code);
    MIPS_DEBUG("BREAK", "Code:%X", code);
}
//------------------------------------------------------------------------------
void CPU::SDBBP()
{
    unsigned int code = instr_index >> 6;
    SignalException(DebugBreakpoint, code);
    MIPS_DEBUG("SDBBP", "Code:%X", code);
}
//------------------------------------------------------------------------------
void CPU::SYNC()
{
    MIPS_DEBUG("SYNC", "%s", "");
}
//------------------------------------------------------------------------------
void CPU::CLZ()
{
    GPR[rd] = __builtin_clzl(GPR32[rs]);
    MIPS_DEBUG("CLZ", "GPR:%-2u:[%016zX] = GPR:%-2u", rd, GPR[rd], rs);
}
//------------------------------------------------------------------------------
void CPU::CLO()
{
    GPR[rd] = __builtin_clzl(~GPR32[rs]);
    MIPS_DEBUG("CLO", "GPR:%-2u:[%016zX] = GPR:%-2u", rd, GPR[rd], rs);
}
//------------------------------------------------------------------------------
void CPU::DCLZ()
{
#if (MIPS_BITS >= 64)
    GPR[rd] = __builtin_clzll(GPR[rs]);
    MIPS_DEBUG("DCLZ", "GPR:%-2u:[%016zX] = GPR:%-2u", rd, GPR[rd], rs);
#endif
}
//------------------------------------------------------------------------------
void CPU::DCLO()
{
#if (MIPS_BITS >= 64)
    GPR[rd] = __builtin_clzll(~GPR[rs]);
    MIPS_DEBUG("DCLO", "GPR:%-2u:[%016zX] = GPR:%-2u", rd, GPR[rd], rs);
#endif
}
//------------------------------------------------------------------------------
void CPU::DSLLV()
{
#if (MIPS_BITS >= 64)
    GPR[rd] = GPR[rt] << GPR[rs];
    MIPS_DEBUG("DSLLV", "GPR:%-2u[%016zX] = GPR:%-2u << GPR:%-2u", rd, GPR[rd], rt, rs);
#endif
}
//------------------------------------------------------------------------------
void CPU::DLSA()
{
    GPR[rd] = (GPR[rs] << ((sa & 3) + 1)) + GPR[rt];
    MIPS_DEBUG("DLSA", "GPR:%-2u[%016zX] = GPR:%-2u << SA:%-2u + GPR:%-2u", rd, GPR[rd], rs, sa, rt);
}
//------------------------------------------------------------------------------
void CPU::DSRLV()
{
#if (MIPS_BITS >= 64)
    if (sa & 1)
        DROTRV();
    else
        uGPR[rd] = uGPR[rt] >> uGPR[rs];
#endif
}
//------------------------------------------------------------------------------
void CPU::DROTRV()
{
#if (MIPS_BITS >= 64)
    GPR[rd] = (GPR[rt] << (64 - GPR[rs])) | (uGPR[rt] >> uGPR[rs]);
    MIPS_DEBUG("DROTRV", "GPR:%-2u[%016zX] = GPR:%-2u rot GPR:%-2u", rd, GPR[rd], rt, rs);
#endif
}
//------------------------------------------------------------------------------
void CPU::DSRAV()
{
#if (MIPS_BITS >= 64)
    GPR[rd] = GPR[rt] >> GPR[rs];
    MIPS_DEBUG("DSRAV", "GPR:%-2u[%016zX] = GPR:%-2u >> GPR:%-2u", rd, GPR[rd], rt, rs);
#endif
}
//------------------------------------------------------------------------------
void CPU::MUL()
{
    int32_t UNUSED hi;
    int32_t UNUSED lo = _mul64(GPR32[rs], GPR32[rt], &hi);
    GPR[rd] = lo;
    MIPS_DEBUG("MUL", "GPR:%-2u[%016zX] = GPR:%-2u / GPR:%-2u", rd, GPR[rd], rs, rt);
}
//------------------------------------------------------------------------------
void CPU::MUH()
{
    int32_t UNUSED hi;
    int32_t UNUSED lo = _mul64(GPR32[rs], GPR32[rt], &hi);
    GPR[rd] = hi;
    MIPS_DEBUG("MUH", "GPR:%-2u[%016zX] = GPR:%-2u / GPR:%-2u", rd, GPR[rd], rs, rt);
}
//------------------------------------------------------------------------------
void CPU::MULU()
{
    uint32_t UNUSED hi;
    uint32_t UNUSED lo = _umul64(uGPR32[rs], uGPR32[rt], &hi);
    GPR[rd] = lo;
    MIPS_DEBUG("MULU", "GPR:%-2u[%016zX] = GPR:%-2u / GPR:%-2u", rd, GPR[rd], rs, rt);
}
//------------------------------------------------------------------------------
void CPU::MUHU()
{
    uint32_t UNUSED hi;
    uint32_t UNUSED lo = _umul64(uGPR32[rs], uGPR32[rt], &hi);
    GPR[rd] = hi;
    MIPS_DEBUG("MUHU", "GPR:%-2u[%016zX] = GPR:%-2u / GPR:%-2u", rd, GPR[rd], rs, rt);
}
//------------------------------------------------------------------------------
void CPU::DMUL()
{
#if (MIPS_BITS >= 64)
    int64_t UNUSED hi;
    int64_t UNUSED lo = _mul128(GPR[rs], GPR[rt], &hi);
    GPR[rd] = lo;
    MIPS_DEBUG("DMUL", "GPR:%-2u[%016zX] = GPR:%-2u / GPR:%-2u", rd, GPR[rd], rs, rt);
#endif
}
//------------------------------------------------------------------------------
void CPU::DMUH()
{
#if (MIPS_BITS >= 64)
    int64_t UNUSED hi;
    int64_t UNUSED lo = _mul128(GPR[rs], GPR[rt], &hi);
    GPR[rd] = hi;
    MIPS_DEBUG("DMUH", "GPR:%-2u[%016zX] = GPR:%-2u / GPR:%-2u", rd, GPR[rd], rs, rt);
#endif
}
//------------------------------------------------------------------------------
void CPU::DMULU()
{
#if (MIPS_BITS >= 64)
    uint64_t UNUSED hi;
    uint64_t UNUSED lo = _umul128(uGPR[rs], uGPR[rt], &hi);
    GPR[rd] = lo;
    MIPS_DEBUG("DMULU", "GPR:%-2u[%016zX] = GPR:%-2u / GPR:%-2u", rd, GPR[rd], rs, rt);
#endif
}
//------------------------------------------------------------------------------
void CPU::DMUHU()
{
#if (MIPS_BITS >= 64)
    uint64_t UNUSED hi;
    uint64_t UNUSED lo = _umul128(uGPR[rs], uGPR[rt], &hi);
    GPR[rd] = hi;
    MIPS_DEBUG("DMUHU", "GPR:%-2u[%016zX] = GPR:%-2u / GPR:%-2u", rd, GPR[rd], rs, rt);
#endif
}
//------------------------------------------------------------------------------
void CPU::DIV()
{
    if (GPR32[rt] != 0)
        GPR[rd] = GPR32[rs] / GPR32[rt];
    MIPS_DEBUG("DIV", "GPR:%-2u[%016zX] = GPR:%-2u / GPR:%-2u", rd, GPR[rd], rs, rt);
}
//------------------------------------------------------------------------------
void CPU::MOD()
{
    if (GPR32[rt] != 0)
        GPR[rd] = GPR32[rs] % GPR32[rt];
    MIPS_DEBUG("MOD", "GPR:%-2u[%016zX] = GPR:%-2u %% GPR:%-2u", rd, GPR[rd], rs, rt);
}
//------------------------------------------------------------------------------
void CPU::DIVU()
{
    if (GPR32[rt] != 0)
        GPR[rd] = uGPR32[rs] / uGPR32[rt];
    MIPS_DEBUG("DIVU", "GPR:%-2u[%016zX] = GPR:%-2u / GPR:%-2u", rd, GPR[rd], rs, rt);
}
//------------------------------------------------------------------------------
void CPU::MODU()
{
    if (GPR32[rt] != 0)
        GPR[rd] = uGPR32[rs] % uGPR32[rt];
    MIPS_DEBUG("MODU", "GPR:%-2u[%016zX] = GPR:%-2u %% GPR:%-2u", rd, GPR[rd], rs, rt);
}
//------------------------------------------------------------------------------
void CPU::DDIV()
{
#if (MIPS_BITS >= 64)
    if (GPR[rt] != 0)
        GPR[rd] = GPR[rs] / GPR[rt];
    MIPS_DEBUG("DDIV", "GPR:%-2u[%016zX] = GPR:%-2u / GPR:%-2u", rd, GPR[rd], rs, rt);
#endif
}
//------------------------------------------------------------------------------
void CPU::DMOD()
{
#if (MIPS_BITS >= 64)
    if (GPR[rt] != 0)
        GPR[rd] = GPR[rs] % GPR[rt];
    MIPS_DEBUG("DMOD", "GPR:%-2u[%016zX] = GPR:%-2u %% GPR:%-2u", rd, GPR[rd], rs, rt);
#endif
}
//------------------------------------------------------------------------------
void CPU::DDIVU()
{
#if (MIPS_BITS >= 64)
    if (GPR[rt] != 0)
        GPR[rd] = uGPR[rs] / uGPR[rt];
    MIPS_DEBUG("DDIVU", "GPR:%-2u[%016zX] = GPR:%-2u / GPR:%-2u", rd, GPR[rd], rs, rt);
#endif
}
//------------------------------------------------------------------------------
void CPU::DMODU()
{
#if (MIPS_BITS >= 64)
    if (GPR[rt] != 0)
        GPR[rd] = uGPR[rs] % uGPR[rt];
    MIPS_DEBUG("DMODU", "GPR:%-2u[%016zX] = GPR:%-2u %% GPR:%-2u", rd, GPR[rd], rs, rt);
#endif
}
//------------------------------------------------------------------------------
void CPU::ADD()
{
    int temp = GPR32[rs] + GPR32[rt];

    // Exception
    if ((temp ^ (temp << 1)) & 0x80000000)
        SignalException(IntegerOverflow);
    else
        GPR[rd] = temp;
    MIPS_DEBUG("ADD", "GPR:%-2u[%016zX] = GPR:%-2u + GPR:%-2u", rd, GPR[rd], rs, rt);
}
//------------------------------------------------------------------------------
void CPU::ADDU()
{
    if (rt == 0)
    {
        GPR[rd] = GPR32[rs];
        MIPS_DEBUG("MOVE", "GPR:%-2u[%016zX] = GPR:%-2u", rd, GPR[rd], rs);
    }
    else
    {
        GPR[rd] = GPR32[rs] + GPR32[rt];
        MIPS_DEBUG("ADDU", "GPR:%-2u[%016zX] = GPR:%-2u + GPR:%-2u", rd, GPR[rd], rs, rt);
    }
}
//------------------------------------------------------------------------------
void CPU::SUB()
{
    int temp = GPR32[rs] - GPR32[rt];

    // Exception
    if ((temp ^ (temp << 1)) & 0x80000000)
        SignalException(IntegerOverflow);
    else
        GPR[rd] = temp;
    MIPS_DEBUG("SUB", "GPR:%-2u[%016zX] = GPR:%-2u - GPR:%-2u", rd, GPR[rd], rs, rt);
}
//------------------------------------------------------------------------------
void CPU::SUBU()
{
    GPR[rd] = GPR32[rs] - GPR32[rt];
    MIPS_DEBUG("SUBU", "GPR:%-2u[%016zX] = GPR:%-2u - GPR:%-2u", rd, GPR[rd], rs, rt);
}
//------------------------------------------------------------------------------
void CPU::AND()
{
    GPR[rd] = GPR[rs] & GPR[rt];
    MIPS_DEBUG("AND", "GPR:%-2u[%016zX] = GPR:%-2u & GPR:%-2u", rd, GPR[rd], rs, rt);
}
//------------------------------------------------------------------------------
void CPU::OR()
{
    GPR[rd] = GPR[rs] | GPR[rt];
    MIPS_DEBUG("OR", "GPR:%-2u[%016zX] = GPR:%-2u | GPR:%-2u", rd, GPR[rd], rs, rt);
}
//------------------------------------------------------------------------------
void CPU::XOR()
{
    GPR[rd] = GPR[rs] ^ GPR[rt];
    MIPS_DEBUG("XOR", "GPR:%-2u[%016zX] = GPR:%-2u xor GPR:%-2u", rd, GPR[rd], rs, rt);
}
//------------------------------------------------------------------------------
void CPU::NOR()
{
    GPR[rd] = ~(GPR[rs] | GPR[rt]);
    MIPS_DEBUG("NOR", "GPR:%-2u[%016zX] = GPR:%-2u nor GPR:%-2u", rd, GPR[rd], rs, rt);
}
//------------------------------------------------------------------------------
void CPU::SLT()
{
    GPR[rd] = (GPR[rs] < GPR[rt]) ? 1 : 0;
    MIPS_DEBUG("SLT", "GPR:%-2u[%016zX] = GPR:%-2u < GPR:%-2u", rd, GPR[rd], rs, rt);
}
//------------------------------------------------------------------------------
void CPU::SLTU()
{
    uGPR[rd] = (uGPR[rs] < uGPR[rt]) ? 1 : 0;
    MIPS_DEBUG("SLTU", "GPR:%-2u[%016zX] = GPR:%-2u < GPR:%-2u", rd, GPR[rd], rs, rt);
}
//------------------------------------------------------------------------------
void CPU::DADD()
{
#if (MIPS_BITS >= 64)
    intptr_t temp = GPR[rs] + GPR[rt];

    // Exception
    if ((temp ^ (temp << 1)) & 0x8000000000000000)
    {
        SignalException(IntegerOverflow);
    }
    else
    {
        GPR[rd] = temp;
    }
    MIPS_DEBUG("DADD", "GPR:%-2u[%016zX] = GPR:%-2u + GPR:%-2u", rd, GPR[rd], rs, rt);
#endif
}
//------------------------------------------------------------------------------
void CPU::DADDU()
{
#if (MIPS_BITS >= 64)
    GPR[rd] = GPR[rs] + GPR[rt];
    if (rt == 0)
    {
        MIPS_DEBUG("MOVE", "GPR:%-2u[%016zX] = GPR:%-2u", rd, GPR[rd], rs);
    }
    else
    {
        MIPS_DEBUG("DADDU", "GPR:%-2u[%016zX] = GPR:%-2u + GPR:%-2u", rd, GPR[rd], rs, rt);
    }
#endif
}
//------------------------------------------------------------------------------
void CPU::DSUB()
{
#if (MIPS_BITS >= 64)
    intptr_t nTemp = GPR[rs] - GPR[rt];

    // Exception
    if ((nTemp ^ (nTemp << 1)) & 0x8000000000000000)
    {
        SignalException(IntegerOverflow);
    }
    else
    {
        GPR[rd] = nTemp;
    }
    MIPS_DEBUG("DSUB", "GPR:%-2u[%016zX] = GPR:%-2u - GPR:%-2u", rd, GPR[rd], rs, rt);
#endif
}
//------------------------------------------------------------------------------
void CPU::DSUBU()
{
#if (MIPS_BITS >= 64)
    GPR[rd] = GPR[rs] - GPR[rt];
    MIPS_DEBUG("DSUBU", "GPR:%-2u[%016zX] = GPR:%-2u - GPR:%-2u", rd, GPR[rd], rs, rt);
#endif
}
//------------------------------------------------------------------------------
void CPU::TGE()
{
    if (GPR[rs] >= GPR[rt])
    {
        unsigned int code = immediate >> 6;
        SignalException(Trap, code);
    }
    MIPS_DEBUG("TGE", "GPR:%-2u >= GPR:%-2u Code:%X", rs, rt, immediate >> 6);
}
//------------------------------------------------------------------------------
void CPU::TGEU()
{
    if (uGPR[rs] >= uGPR[rt])
    {
        unsigned int code = immediate >> 6;
        SignalException(Trap, code);
    }
    MIPS_DEBUG("TGEU", "GPR:%-2u >= GPR:%-2u Code:%X", rs, rt, immediate >> 6);
}
//------------------------------------------------------------------------------
void CPU::TLT()
{
    if (GPR[rs] < GPR[rt])
    {
        unsigned int code = immediate >> 6;
        SignalException(Trap, code);
    }
    MIPS_DEBUG("TLT", "GPR:%-2u < GPR:%-2u Code:%X", rs, rt, immediate >> 6);
}
//------------------------------------------------------------------------------
void CPU::TLTU()
{
    if (uGPR[rs] < uGPR[rt])
    {
        unsigned int code = immediate >> 6;
        SignalException(Trap, code);
    }
    MIPS_DEBUG("TLTU", "GPR:%-2u < GPR:%-2u Code:%X", rs, rt, immediate >> 6);
}
//------------------------------------------------------------------------------
void CPU::TEQ()
{
    if (GPR[rs] == GPR[rt])
    {
        unsigned int code = immediate >> 6;
        SignalException(Trap, code);
    }
    MIPS_DEBUG("TEQ", "GPR:%-2u == GPR:%-2u Code:%X", rs, rt, immediate >> 6);
}
//------------------------------------------------------------------------------
void CPU::SELEQZ()
{
    GPR[rd] = GPR[rt] ? 0 : GPR[rs];
    MIPS_DEBUG("SELEQZ", "GPR:%-2u[%016zX] = GPR:%-2u ? 0 : GPR:%-2u", rd, GPR[rd], rt, rs);
}
//------------------------------------------------------------------------------
void CPU::TNE()
{
    if (GPR[rs] != GPR[rt])
    {
        unsigned int code = immediate >> 6;
        SignalException(Trap, code);
    }
    MIPS_DEBUG("TNE", "GPR:%-2u != GPR:%-2u Code:%X", rs, rt, immediate >> 6);
}
//------------------------------------------------------------------------------
void CPU::SELNEZ()
{
    GPR[rd] = GPR[rt] ? GPR[rs] : 0;
    MIPS_DEBUG("SELNEZ", "GPR:%-2u[%016zX] = GPR:%-2u ? GPR:%-2u : 0", rd, GPR[rd], rt, rs);
}
//------------------------------------------------------------------------------
void CPU::DSLL()
{
#if (MIPS_BITS >= 64)
    GPR[rd] = GPR[rt] << sa;
    MIPS_DEBUG("DSLL", "GPR:%-2u[%016zX] = GPR:%-2u << %u", rd, GPR[rd], rt, sa);
#endif
}
//------------------------------------------------------------------------------
void CPU::DSRL()
{
#if (MIPS_BITS >= 64)
    if (rs & 1)
    {
        DROTR();
    }
    else
    {
        GPR[rd] = uGPR[rt] >> sa;
        MIPS_DEBUG("DSRL", "GPR:%-2u[%016zX] = GPR:%-2u >> %u", rd, GPR[rd], rt, sa);
    }
#endif
}
//------------------------------------------------------------------------------
void CPU::DROTR()
{
#if (MIPS_BITS >= 64)
    GPR[rd] = (GPR[rt] << (64 - sa)) | (uGPR[rt] >> sa);
    MIPS_DEBUG("DROTR", "GPR:%-2u[%016zX] = GPR:%-2u rot %u", rd, GPR[rd], rt, sa);
#endif
}
//------------------------------------------------------------------------------
void CPU::DSRA()
{
#if (MIPS_BITS >= 64)
    GPR[rd] = GPR[rt] >> sa;
    MIPS_DEBUG("DSRA", "GPR:%-2u[%016zX] = GPR:%-2u >> %u", rd, GPR[rd], rt, sa);
#endif
}
//------------------------------------------------------------------------------
void CPU::DSLL32()
{
#if (MIPS_BITS >= 64)
    GPR[rd] = GPR[rt] << (32 + sa);
    MIPS_DEBUG("DSLL32", "GPR:%-2u[%016zX] = GPR:%-2u << %u", rd, GPR[rd], rt, sa);
#endif
}
//------------------------------------------------------------------------------
void CPU::DSRL32()
{
#if (MIPS_BITS >= 64)
    if (rs & 1)
    {
        DROTR32();
    }
    else
    {
        GPR[rd] = uGPR[rt] >> (32 + sa);
        MIPS_DEBUG("DSRL32", "GPR:%-2u[%016zX] = GPR:%-2u >> %u", rd, GPR[rd], rt, sa);
    }
#endif
}
//------------------------------------------------------------------------------
void CPU::DROTR32()
{
#if (MIPS_BITS >= 64)
    GPR[rd] = (GPR[rt] << (64 - (32 + sa))) | (uGPR[rt] >> (32 + sa));
    MIPS_DEBUG("DROTR32", "GPR:%-2u[%016zX] = GPR:%-2u rot %u", rd, GPR[rd], rt, sa);
#endif
}
//------------------------------------------------------------------------------
void CPU::DSRA32()
{
#if (MIPS_BITS >= 64)
    GPR[rd] = GPR[rt] >> (32 + sa);
    MIPS_DEBUG("DSRA32", "GPR:%-2u[%016zX] = GPR:%-2u >> %u", rd, GPR[rd], rt, sa);
#endif
}
//------------------------------------------------------------------------------

