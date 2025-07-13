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
// Table A.6 MIPS64 SPECIAL3 Encoding of Function Field
//------------------------------------------------------------------------------
const CPU::FINSTRUCTION CPU::tableSPECIAL3[8 * 8] =
{
    o EXT   x DEXTM x DEXTU x DEXT      x INS       x DINSM x DINSU x DINS
    x ____  x ____  x ____  x ____      x ____      x ____  x ____  x ____
    x ____  x ____  x ____  x ____      x ____      x ____  x ____  x ____
    x ____  x ____  x ____  x CACHEE    x SBE       x SHE   x SCE   x SWE
    x BSHFL x ____  x ____  x PREFE     x DBSHFL    x CACHE x SC    x SCD
    x LBUE  x LHUE  x ____  x ____      x LBE       x LHE   x LLE   x LWE
    x ____  x ____  x ____  x ____      x ____      x PREF  x LL    x LLD
    x ____  x ____  x ____  x RDHWR     x ____      x ____  x ____  x ____
};
//------------------------------------------------------------------------------
// Table A.13 MIPS64 BSHFL and DBSHFL Encoding of sa Field
//------------------------------------------------------------------------------
const CPU::FINSTRUCTION CPU::tableBSHFL[4 * 8] =
{
    o BITSWAP   x ____  x WSBH  x ____  x ____  x ____  x ____  x ____
    x ALIGN     x ALIGN x ALIGN x ALIGN x ____  x ____  x ____  x ____
    x SEB       x ____  x ____  x ____  x ____  x ____  x ____  x ____
    x SEH       x ____  x ____  x ____  x ____  x ____  x ____  x ____
};
const CPU::FINSTRUCTION CPU::tableDBSHFL[4 * 8] =
{
    o DBITSWAP  x ____      x DSBH      x ____      x ____      x DSHD      x ____      x ____
    x DALIGN    x DALIGN    x DALIGN    x DALIGN    x DALIGN    x DALIGN    x DALIGN    x DALIGN
    x ____      x ____      x ____      x ____      x ____      x ____      x ____      x ____
    x ____      x ____      x ____      x ____      x ____      x ____      x ____      x ____
};
#undef o
#undef x
//------------------------------------------------------------------------------
void CPU::EXT()
{
    int left = 32 - rd;
    int right = sa + left;
    uGPR[rt] = ((uGPR[rs] << left) >> right);
    MIPS_DEBUG("EXT", "%s", "");
}
//------------------------------------------------------------------------------
void CPU::DEXTM()
{
#if (MIPS_BITS >= 64)
    MIPS_DEBUG("DEXTM", "%s", "");
#endif
}
//------------------------------------------------------------------------------
void CPU::DEXTU()
{
#if (MIPS_BITS >= 64)
    MIPS_DEBUG("DEXTU", "%s", "");
#endif
}
//------------------------------------------------------------------------------
void CPU::DEXT()
{
#if (MIPS_BITS >= 64)
    MIPS_DEBUG("DEXT", "%s", "");
#endif
}
//------------------------------------------------------------------------------
void CPU::INS()
{
    int left = 32 - rd + sa;
    int right = 32 - sa;
    int mask = ((~0 << left) >> right);
    uGPR[rt] = (uGPR[rt] & ~mask) | ((uGPR[rs] << left) >> right);
    MIPS_DEBUG("INS", "%s", "");
}
//------------------------------------------------------------------------------
void CPU::DINSM()
{
#if (MIPS_BITS >= 64)
    MIPS_DEBUG("DINSM", "%s", "");
#endif
}
//------------------------------------------------------------------------------
void CPU::DINSU()
{
#if (MIPS_BITS >= 64)
    MIPS_DEBUG("DINSU", "%s", "");
#endif
}
//------------------------------------------------------------------------------
void CPU::DINS()
{
#if (MIPS_BITS >= 64)
    MIPS_DEBUG("DINS", "%s", "");
#endif
}
//------------------------------------------------------------------------------
void CPU::CACHEE()
{
    CACHE();
}
//------------------------------------------------------------------------------
void CPU::SBE()
{
    SB();
}
//------------------------------------------------------------------------------
void CPU::SHE()
{
    SH();
}
//------------------------------------------------------------------------------
void CPU::SCE()
{
    SC();
}
//------------------------------------------------------------------------------
void CPU::SWE()
{
    SW();
}
//------------------------------------------------------------------------------
void CPU::BSHFL()
{
    FINSTRUCTION BSHFL = tableBSHFL[sa];
    (this->*BSHFL)();
}
//------------------------------------------------------------------------------
// Table A.13 MIPS64 BSHFL and DBSHFL Encoding of sa Field
//------------------------------------------------------------------------------
void CPU::BITSWAP()
{
    GPR[rd] = __builtin_bswap32(GPR32[rt]);
    MIPS_DEBUG("BITSWAP", "GPR:%-2u[%016zX] = GPR:%-2u", rd, GPR[rd], rt);
}
//------------------------------------------------------------------------------
void CPU::WSBH()
{
    int temp = 0;
    temp |= (uGPR32[rt] << 8) & 0xFF00FF00;
    temp |= (uGPR32[rt] >> 8) & 0x00FF00FF;
    GPR[rd] = temp;
    MIPS_DEBUG("WSBH", "GPR:%-2u[%016zX] = GPR:%-2u", rd, GPR[rd], rt);
}
//------------------------------------------------------------------------------
void CPU::ALIGN()
{
    int bp = (sa & 3);
    int tmp_rt_hi = (uGPR32[rt] << (8 * bp));
    int tmp_rs_lo = (uGPR32[rs] >> (8 * (4 - bp)));
    int tmp = tmp_rt_hi | tmp_rs_lo;
    GPR[rd] = tmp;
    MIPS_DEBUG("ALIGN", "GPR:%-2u[%016zX] = GPR:%-2u GPR:%-2u BP:%-2u", rd, GPR[rd], rt, rs, bp);
}
//------------------------------------------------------------------------------
void CPU::SEB()
{
    GPR[rd] = char(GPR[rt]);
    MIPS_DEBUG("SEB", "GPR:%-2u[%016zX] = GPR:%-2u", rd, GPR[rd], rt);
}
//------------------------------------------------------------------------------
void CPU::SEH()
{
    GPR[rd] = short(GPR[rt]);
    MIPS_DEBUG("SEH", "GPR:%-2u[%016zX] = GPR:%-2u", rd, GPR[rd], rt);
}
//------------------------------------------------------------------------------
void CPU::PREFE()
{
    PREF();
}
//------------------------------------------------------------------------------
void CPU::DBSHFL()
{
#if (MIPS_BITS >= 64)
    FINSTRUCTION DBSHFL = tableDBSHFL[sa];
    (this->*DBSHFL)();
#endif
}
//------------------------------------------------------------------------------
// Table A.13 MIPS64 BSHFL and DBSHFL Encoding of sa Field
//------------------------------------------------------------------------------
void CPU::DBITSWAP()
{
#if (MIPS_BITS >= 64)
    GPR[rd] = __builtin_bswap64(GPR32[rt]);
    MIPS_DEBUG("DBITSWAP", "GPR:%-2u[%016zX] = GPR:%-2u", rd, GPR[rd], rt);
#endif
}
//------------------------------------------------------------------------------
void CPU::DSBH()
{
#if (MIPS_BITS >= 64)
    intptr_t temp = 0;
    temp |= (uGPR[rt] << 8) & 0xFF00FF00FF00FF00;
    temp |= (uGPR[rt] >> 8) & 0x00FF00FF00FF00FF;
    uGPR[rd] = temp;
    MIPS_DEBUG("DSBH", "GPR:%-2u[%016zX] = GPR:%-2u", rd, GPR[rd], rt);
#endif
}
//------------------------------------------------------------------------------
void CPU::DSHD()
{
#if (MIPS_BITS >= 64)
    intptr_t temp = 0;
    temp |= (uGPR[rt] << 48) & 0xFFFF000000000000;
    temp |= (uGPR[rt] << 16) & 0x0000FFFF00000000;
    temp |= (uGPR[rt] >> 16) & 0x00000000FFFF0000;
    temp |= (uGPR[rt] >> 48) & 0x000000000000FFFF;
    uGPR[rd] = temp;
    MIPS_DEBUG("DSHD", "GPR:%-2u[%016zX] = GPR:%-2u", rd, GPR[rd], rt);
#endif
}
//------------------------------------------------------------------------------
void CPU::DALIGN()
{
#if (MIPS_BITS >= 64)
    int bp = (sa & 7);
    intptr_t tmp_rt_hi = (uGPR[rt] << (8 * bp));
    intptr_t tmp_rs_lo = (uGPR[rs] >> (8 * (8 - bp)));
    intptr_t tmp = tmp_rt_hi | tmp_rs_lo;
    GPR[rd] = tmp;
    MIPS_DEBUG("DALIGN", "GPR:%-2u[%016zX] = GPR:%-2u GPR:%-2u BP:%-2u", rd, GPR[rd], rt, rs, bp);
#endif
}
//------------------------------------------------------------------------------
void CPU::CACHE()
{
}
//------------------------------------------------------------------------------
void CPU::SC()
{
}
//------------------------------------------------------------------------------
void CPU::SCD()
{
}
//------------------------------------------------------------------------------
void CPU::LBUE()
{
    LBU();
}
//------------------------------------------------------------------------------
void CPU::LHUE()
{
    LHU();
}
//------------------------------------------------------------------------------
void CPU::LBE()
{
    LB();
}
//------------------------------------------------------------------------------
void CPU::LHE()
{
    LH();
}
//------------------------------------------------------------------------------
void CPU::LLE()
{
    LL();
}
//------------------------------------------------------------------------------
void CPU::LWE()
{
    LW();
}
//------------------------------------------------------------------------------
void CPU::PREF()
{
}
//------------------------------------------------------------------------------
void CPU::LL()
{
}
//------------------------------------------------------------------------------
void CPU::LLD()
{
}
//------------------------------------------------------------------------------
void CPU::RDHWR()
{
    switch (rd)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 29:
        GPR[rt] = 0;
        break;
    default:
        SignalException(ReservedInstruction);
        break;
    }
}
//------------------------------------------------------------------------------
