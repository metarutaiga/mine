#pragma once

#define REG(i)          x86.regs[i & 0b111]
#define AL              x86.regs[0].l
#define AH              x86.regs[0].h
#define AX              x86.regs[0].w
#define EAX             x86.regs[0].d
#define RAX             x86.regs[0].q
#define CL              x86.regs[1].l
#define CH              x86.regs[1].h
#define CX              x86.regs[1].w
#define ECX             x86.regs[1].d
#define RCX             x86.regs[1].q
#define DL              x86.regs[2].l
#define DH              x86.regs[2].h
#define DX              x86.regs[2].w
#define EDX             x86.regs[2].d
#define RDX             x86.regs[2].q
#define BL              x86.regs[3].l
#define BH              x86.regs[3].h
#define BX              x86.regs[3].w
#define EBX             x86.regs[3].d
#define RBX             x86.regs[3].q
#define SP              x86.regs[4].w
#define ESP             x86.regs[4].d
#define RSP             x86.regs[4].q
#define BP              x86.regs[5].w
#define EBP             x86.regs[5].d
#define RBP             x86.regs[5].q
#define SI              x86.regs[6].w
#define ESI             x86.regs[6].d
#define RSI             x86.regs[6].q
#define DI              x86.regs[7].w
#define EDI             x86.regs[7].d
#define RDI             x86.regs[7].q
#define IP              x86.ip.w
#define EIP             x86.ip.d
#define RIP             x86.ip.q
#define EFLAGS          x86.flags.d
#define FLAGS           x86.flags.w
#define CF              x86.flags._CF
#define PF              x86.flags._PF
#define AF              x86.flags._AF
#define ZF              x86.flags._ZF
#define SF              x86.flags._SF
#define DF              x86.flags._DF
#define OF              x86.flags._OF
//------------------------------------------------------------------------------
namespace internal { static x86_register x86; };
//------------------------------------------------------------------------------
#define IndexREG(reg)   ((int)((& internal :: reg - & internal :: EAX) / (& internal :: ECX - & internal :: EAX)))
//------------------------------------------------------------------------------
#define _____C  0b000001
#define ____P_  0b000010
#define ___A__  0b000100
#define __Z___  0b001000
#define _S____  0b010000
#define O_____  0b100000
#define _SZ_P_  0b011010
#define OSZAP_  0b111110
#define OSZAPC  0b111111
#define CARRY   false
#define BORROW  true
//------------------------------------------------------------------------------
template<int F, bool B = false, typename L, typename R, typename X, typename Y>
inline void x86_register::UpdateFlags(x86_register& x86, L& DEST, R TEMP, X SRC1, Y SRC2)
{
    uint64_t bc = ( TEMP & (~SRC1 | SRC2)) | (~SRC1 & SRC2);
    uint64_t cc = (~TEMP & ( SRC1 | SRC2)) | ( SRC1 & SRC2);
    uint64_t pp = __builtin_popcount((uint8_t)TEMP) ^ 1;
    uint64_t bits = sizeof(L) * 8;
    uint64_t sign = (uint64_t)1 << (bits - 1);
    uint64_t sign2 = (uint64_t)1 << (bits - 2);
    uint64_t c = B ? bc : cc;
    if (F & _____C) CF = c &               sign ? 1 : 0;
    if (F & ____P_) PF = pp &                 1 ? 1 : 0;
    if (F & ___A__) AF = c &                  8 ? 1 : 0;
    if (F & __Z___) ZF = TEMP ==              0 ? 1 : 0;
    if (F & _S____) SF = TEMP &            sign ? 1 : 0;
    if (F & O_____) OF = (c ^ (c >> 1)) & sign2 ? 1 : 0;
    DEST = TEMP;
}
//------------------------------------------------------------------------------
