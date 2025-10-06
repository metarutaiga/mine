//==============================================================================
// IA-32 Intel (R) Architecture Software Developer’s Manual
//
// 2004
//==============================================================================
#include "x86_ia32.h"
#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"
#include "mmx_instruction.h"
#include "ssse3_instruction.h"

//------------------------------------------------------------------------------
#define X x86_ia32::
#define x , X
#define M mmx_instruction::
#define m , M
#define S ssse3_instruction::
#define s , S
//------------------------------------------------------------------------------
// One-Byte Opcode Map
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_ia32::one[256] =
{      // 0        1       2       3      4       5       6       7       8       9       A       B       C        D       E       F
/* 0 */ X ADD    x ADD   x ADD   x ADD  x ADD   x ADD   x _     x _     x OR    x OR    x OR    x OR    x OR     x OR    x _     x TWO
/* 1 */ x ADC    x ADC   x ADC   x ADC  x ADC   x ADC   x _     x _     x SBB   x SBB   x SBB   x SBB   x SBB    x SBB   x _     x _
/* 2 */ x AND    x AND   x AND   x AND  x AND   x AND   x ES    x _     x SUB   x SUB   x SUB   x SUB   x SUB    x SUB   x CS    x _
/* 3 */ x XOR    x XOR   x XOR   x XOR  x XOR   x XOR   x SS    x _     x CMP   x CMP   x CMP   x CMP   x CMP    x CMP   x DS    x _
/* 4 */ x INC    x INC   x INC   x INC  x INC   x INC   x INC   x INC   x DEC   x DEC   x DEC   x DEC   x DEC    x DEC   x DEC   x DEC
/* 5 */ x PUSH   x PUSH  x PUSH  x PUSH x PUSH  x PUSH  x PUSH  x PUSH  x POP   x POP   x POP   x POP   x POP    x POP   x POP   x POP
/* 6 */ x PUSHAD x POPAD x _     x _    x FS    x GS    x OSIZE x ASIZE x PUSH  x IMUL  x PUSH  x IMUL  x _      x _     x _     x _
/* 7 */ x Jcc    x Jcc   x Jcc   x Jcc  x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc    x Jcc   x Jcc   x Jcc
/* 8 */ x grp1   x grp1  x _     x grp1 x TEST  x TEST  x XCHG  x XCHG  x MOV   x MOV   x MOV   x MOV   x MOV    x LEA   x MOV   x POP
/* 9 */ x NOP    x XCHG  x XCHG  x XCHG x XCHG  x XCHG  x XCHG  x XCHG  x CBW   x CWD   x _     x WAIT  x PUSHFD x POPFD x SAHF  x LAHF
/* A */ x MOV    x MOV   x MOV   x MOV  x MOVSx x MOVSx x CMPSx x CMPSx x TEST  x TEST  x STOSx x STOSx x LODSx  x LODSx x SCASx x SCASx
/* B */ x MOV    x MOV   x MOV   x MOV  x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV    x MOV   x MOV   x MOV
/* C */ x grp2   x grp2  x RET   x RET  x _     x _     x MOV   x MOV   x ENTER x LEAVE x _     x _     x _      x _     x _     x _
/* D */ x grp2   x grp2  x grp2  x grp2 x _     x _     x _     x XLAT  x ESC   x ESC   x ESC   x ESC   x ESC    x ESC   x ESC   x ESC
/* E */ x LOOP   x LOOP  x LOOP  x Jcc  x _     x _     x _     x _     x CALL  x JMP   x _     x JMP   x _      x _     x _     x _
/* F */ x _      x _     x REPNE x REPE x _     x CMC   x grp3  x grp3  x CLC   x STC   x _     x _     x CLD    x STD   x grp4  x grp5
};
//------------------------------------------------------------------------------
// Two-Byte Opcode Map
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_ia32::two[256] =
{      // 0           1           2           3          4           5          6           7            8           9           A           B           C            D            E         F
/* 0 */ X grp6      x grp7      x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 1 */ s MOVUPS    s MOVUPS    s MOVLPS    s MOVLPS   s UNPCKLPS  s UNPCKHPS s MOVHPS    s MOVHPS     x grp16     x _         x _         x _         x _          x _          x _       x NOP
/* 2 */ x _         x _         x _         x _        x _         x _        x _         x _          s MOVAPS    s MOVAPS    s CVTPI2PS  s MOVNTPS   s CVTTPS2PI  s CVTPS2PI   s UCOMISS s COMISS
/* 3 */ x _         x RDTSC     x _         x RDPMC    x _         x _        x _         x _          x THREE38   x _         x THREE3A   x _         x _          x _          x _       x _
/* 4 */ x CMOVcc    x CMOVcc    x CMOVcc    x CMOVcc   x CMOVcc    x CMOVcc   x CMOVcc    x CMOVcc     x CMOVcc    x CMOVcc    x CMOVcc    x CMOVcc    x CMOVcc     x CMOVcc     x CMOVcc  x CMOVcc
/* 5 */ s MOVMSKPS  s SQRTPS    s RSQRTPS   s RCPPS    s ANDPS     s ANDNPS   s ORPS      s XORPS      s ADDPS     s MULPS     x _         x _         s SUBPS      s MINPS      s DIVPS   s MAXPS
/* 6 */ m PUNPCKLBW m PUNPCKLWD m PUNPCKLDQ m PACKSSWB m PCMPGTB   m PCMPGTW  m PCMPGTD   m PACKUSWB   m PUNPCKHBW m PUNPCKHWD m PUNPCKHDQ m PACKSSDW  x _          x _          m MOVD    m MOVQ
/* 7 */ m PSHUFW    x grp12     x grp13     x grp14    m PCMPEQB   m PCMPEQW  m PCMPEQD   m EMMS       x _         x _         x _         x _         x _          x _          m MOVD    m MOVQ
/* 8 */ x Jcc       x Jcc       x Jcc       x Jcc      x Jcc       x Jcc      x Jcc       x Jcc        x Jcc       x Jcc       x Jcc       x Jcc       x Jcc        x Jcc        x Jcc     x Jcc
/* 9 */ x SETcc     x SETcc     x SETcc     x SETcc    x SETcc     x SETcc    x SETcc     x SETcc      x SETcc     x SETcc     x SETcc     x SETcc     x SETcc      x SETcc      x SETcc   x SETcc
/* A */ x _         x _         x CPUID     x BT       x SHxD      x SHxD     x _         x _          x _         x _         x _         x BTS       x SHxD       x SHxD       x grp15   x IMUL
/* B */ x CMPXCHG   x CMPXCHG   x _         x BTR      x _         x _        x MOVZX     x MOVZX      x _         x _         x grp8      x BTC       x BSF        x BSR        x MOVSX   x MOVSX
/* C */ x XADD      x XADD      s CMPPS     x _        m PINSRW    m PEXTRW   s SHUFPS    x grp9       x BSWAP     x BSWAP     x BSWAP     x BSWAP     x BSWAP      x BSWAP      x BSWAP   x BSWAP
/* D */ x _         m PSRLW     m PSRLD     m PSRLQ    x _         m PMULLW   x _         m PMOVMSKB   m PSUBUSB   m PSUBUSW   m PMINUB    m PAND      m PADDUSB    m PADDUSW    m PMAXUB  m PANDN
/* E */ m PAVGB     m PSRAW     m PSRAD     m PAVGW    m PMULHUW   m PMULHW   x _         m MOVNTQ     m PSUBSB    m PSUBSW    m PMINSW    m POR       m PADDSB     m PADDSW     m PMAXSW  m PXOR
/* F */ x _         m PSLLW     m PSLLD     m PSLLQ    x _         m PMADDWD  m PSADBW    x _          m PSUBB     m PSUBW     m PSUBD     x _         m PADDB      m PADDW      m PADDD   x _
};
//------------------------------------------------------------------------------
// Two-Byte Opcode Map - 66
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_ia32::two66[256] =
{      // 0           1           2           3          4           5          6           7            8           9           A           B           C            D            E         F
/* 0 */ X _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 1 */ s MOVUPD    s MOVUPD    s MOVLPD    s MOVLPD   s UNPCKLPD  s UNPCKHPD s MOVHPD    s MOVHPD     s MOVAPD    s MOVAPD    s CVTPI2PD  s MOVNTPD   s CVTTPD2PI  s CVTPD2PI   s UCOMISD s COMISD
/* 2 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 3 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 4 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 5 */ s MOVMSKPD  s SQRTPD    x _         x _        s ANDPD     s ANDNPD   s ORPD      s XORPD      s ADDPD     s MULPD     s CVTPD2PS  s CVTPS2DQ  s SUBPD      s MINPD      s DIVPD   s MAXPD
/* 6 */ s PUNPCKLBW s PUNPCKLWD s PUNPCKLDQ s PACKSSWB s PCMPGTB   s PCMPGTW  s PCMPGTD   s PACKUSWB   s PUNPCKHBW s PUNPCKHWD s PUNPCKHDQ s PACKSSDW  s PUNPCKLQDQ s PUNPCKHQDQ s MOVD    s MOVDQA
/* 7 */ s PSHUFD    x _         x _         x _        s PCMPEQB   s PCMPEQW  s PCMPEQD   x _          x _         x _         x _         x _         s HADDPD     s HSUBPD     s MOVD    s MOVDQA
/* 8 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 9 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* A */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* B */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* C */ x _         x _         s CMPPD     x _        s PINSRW    s PEXTRW   s SHUFPD    x _          x _         x _         x _         x _         x _          x _          x _       x _
/* D */ s ADDSUBPD  s PSRLW     s PSRLD     s PSRLQ    s PADDQ     s PMULLW   s MOVQ      s PMOVMSKB   s PSUBUSB   s PSUBUSW   s PMINUB    s PAND      s PADDUSB    s PADDUSW    s PMAXUB  s PANDN
/* E */ s PAVGB     s PSRAW     s PSRAD     s PAVGW    s PMULHUW   s PMULHW   s CVTTPD2DQ s MOVNTDQ    s PSUBSB    s PSUBSW    s PMINSW    s POR       s PADDSB     s PADDSW     s PMAXSW  s PXOR
/* F */ x _         s PSLLW     s PSLLD     s PSLLQ    s PMULUDQ   s PMADDWD  s PSADBW    s MASKMOVDQU s PSUBB     s PSUBW     s PSUBD     s PSUBQ     s PADDB      s PADDW      s PADDD   x _
};
//------------------------------------------------------------------------------
// Two-Byte Opcode Map - F2
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_ia32::twoF2[256] =
{      // 0           1           2           3          4           5          6           7            8           9           A           B           C            D            E         F
/* 0 */ X _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 1 */ s MOVSD     s MOVSD     s MOVDDUP   x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 2 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         s CVTSI2SD  x _         s CVTTSD2SI  s CVTSD2SI   x _       x _
/* 3 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 4 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 5 */ x _         s SQRTSD    x _         x _        x _         x _        x _         x _          s ADDSD     s MULSD     s CVTSD2SS  x _         s SUBSD      s MINSD      s DIVSD   s MAXSD
/* 6 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 7 */ s PSHUFLW   x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         s HADDPS     s HSUBPS     x _       x _
/* 8 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 9 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* A */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* B */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* C */ x _         x _         s CMPSD     x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* D */ x _         x _         x _         x _        x _         x _        s MOVDQ2Q   x _          x _         x _         x _         x _         x _          x _          x _       x _
/* E */ x _         x _         x _         x _        x _         x _        s CVTPD2DQ  x _          x _         x _         x _         x _         x _          x _          x _       x _
/* F */ s LDDQU     x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
};
//------------------------------------------------------------------------------
// Two-Byte Opcode Map - F3
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_ia32::twoF3[256] =
{      // 0           1           2           3          4           5          6           7            8           9           A           B           C            D            E         F
/* 0 */ X _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 1 */ s MOVSS     s MOVSS     s MOVSLDUP  x _        x _         x _        s MOVSHDUP  x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 2 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         s CVTSI2SS  x _         s CVTTSS2SI  s CVTSS2SI   x _       x _
/* 3 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 4 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 5 */ x _         s SQRTSS    s RSQRTSS   s RCPSS    x _         x _        x _         x _          s ADDSS     s MULSS     s CVTSS2SD  s CVTTPS2DQ s SUBSS      s MINSS      s DIVSS   s MAXSS
/* 6 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       s MOVDQU
/* 7 */ s PSHUFHW   x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          s MOVQ    s MOVDQU
/* 8 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 9 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* A */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* B */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* C */ x _         x _         s CMPSS     x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* D */ x _         x _         x _         x _        x _         x _        s MOVQ2DQ   x _          x _         x _         x _         x _         x _          x _          x _       x _
/* E */ x _         x _         x _         x _        x _         x _        s CVTDQ2PD  x _          x _         x _         x _         x _         x _          x _          x _       x _
/* F */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
};
//------------------------------------------------------------------------------
// Three-Byte Opcode Map - 38
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_ia32::three38[256] =
{      // 0           1           2           3          4           5          6           7            8           9           A           B           C            D            E         F
/* 0 */ M PSHUFB    m PHADDW    m PHADDD    m PHADDSW  m PMADDUBSW m PHSUBW   m PHSUBD    m PHSUBSW    m PSIGNB    m PSIGNW    m PSIGND    m PMULHRSW  x _          x _          x _       x _
/* 1 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         m PABSB      m PABSW      m PABSD   x _
/* 2 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 3 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 4 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 5 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 6 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 7 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 8 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 9 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* A */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* B */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* C */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* D */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* E */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* F */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
};
//------------------------------------------------------------------------------
// Three-Byte Opcode Map - 3A
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_ia32::three3A[256] =
{      // 0           1           2           3          4           5          6           7            8           9           A           B           C            D            E         F
/* 0 */ X _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       m PALIGNR
/* 1 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 2 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 3 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 4 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 5 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 6 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 7 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 8 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 9 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* A */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* B */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* C */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* D */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* E */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* F */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
};
//------------------------------------------------------------------------------
// Three-Byte Opcode Map - 66 38
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_ia32::three6638[256] =
{      // 0           1           2           3          4           5          6           7            8           9           A           B           C            D            E         F
/* 0 */ S PSHUFB    s PHADDW    s PHADDD    s PHADDSW  s PMADDUBSW s PHSUBW   s PHSUBD    s PHSUBSW    s PSIGNB    s PSIGNW    s PSIGND    s PMULHRSW  x _          x _          x _       x _
/* 1 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         s PABSB      s PABSW      s PABSD   x _
/* 2 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 3 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 4 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 5 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 6 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 7 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 8 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 9 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* A */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* B */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* C */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* D */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* E */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* F */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
};
//------------------------------------------------------------------------------
// Three-Byte Opcode Map - 66 3A
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_ia32::three663A[256] =
{      // 0           1           2           3          4           5          6           7            8           9           A           B           C            D            E         F
/* 0 */ X _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       s PALIGNR
/* 1 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 2 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 3 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 4 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 5 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 6 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 7 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 8 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 9 */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* A */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* B */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* C */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* D */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* E */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* F */ x _         x _         x _         x _        x _         x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
};
//------------------------------------------------------------------------------
// Opcodes determined by bits 5,4,3 of modR/M byte
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_ia32::group[17][2][8] =
{              // 0             1           2           3           4       5        6        7
/*  0 mem */{ { X _           x _         x _         x _         x _     x _      x _      x _       },
/*  0 11B */  { X _           x _         x _         x _         x _     x _      x _      x _       }, },
/*  1 mem */{ { X ADD         x OR        x ADC       x SBB       x AND   x SUB    x XOR    x CMP     },
/*  1 11B */  { X ADD         x OR        x ADC       x SBB       x AND   x SUB    x XOR    x CMP     }, },
/*  2 mem */{ { X Rxx         x Rxx       x Rxx       x Rxx       x Sxx   x Sxx    x _      x Sxx     },
/*  2 11B */  { X Rxx         x Rxx       x Rxx       x Rxx       x Sxx   x Sxx    x _      x Sxx     }, },
/*  3 mem */{ { X TEST        x _         x NOT       x NEG       x MUL   x IMUL   x DIV    x IDIV    },
/*  3 11B */  { X TEST        x _         x NOT       x NEG       x MUL   x IMUL   x DIV    x IDIV    }, },
/*  4 mem */{ { X INC         x DEC       x _         x _         x _     x _      x _      x _       },
/*  4 11B */  { X INC         x DEC       x _         x _         x _     x _      x _      x _       }, },
/*  5 mem */{ { X INC         x DEC       x CALL      x _         x JMP   x _      x PUSH   x _       },
/*  5 11B */  { X INC         x DEC       x CALL      x _         x JMP   x _      x PUSH   x _       }, },
/*  6 mem */{ { X _           x _         x _         x _         x _     x _      x _      x _       },
/*  6 11B */  { X _           x _         x _         x _         x _     x _      x _      x _       }, },
/*  7 mem */{ { X _           x _         x _         x _         x _     x _      x _      x _       },
/*  7 11B */  { X _           s MONITOR   x _         x _         x _     x _      x _      x _       }, },
/*  8 mem */{ { X _           x _         x _         x _         x BT    x BTS    x BTR    x BTC     },
/*  8 11B */  { X _           x _         x _         x _         x BT    x BTS    x BTR    x BTC     }, },
/*  9 mem */{ { X _           x CMPXCHG8B x _         x _         x _     x _      x _      x _       },
/*  9 11B */  { X _           x _         x _         x _         x _     x _      x _      x _       }, },
/* 10 mem */{ { X _           x _         x _         x _         x _     x _      x _      x _       },
/* 10 11B */  { X _           x _         x _         x _         x _     x _      x _      x _       }, },
/* 11 mem */{ { X _           x _         x _         x _         x _     x _      x _      x _       },
/* 11 11B */  { X _           x _         x _         x _         x _     x _      x _      x _       }, },
/* 12 mem */{ { X _           x _         x _         x _         x _     x _      x _      x _       },
/* 12 11B */  { X _           x _         m PSRLW     x _         m PSRAW x _      m PSLLW  x _       }, },
/* 13 mem */{ { X _           x _         x _         x _         x _     x _      x _      x _       },
/* 13 11B */  { X _           x _         m PSRLD     x _         m PSRAD x _      m PSLLD  x _       }, },
/* 14 mem */{ { X _           x _         x _         x _         x _     x _      x _      x _       },
/* 14 11B */  { X _           x _         m PSRLQ     x _         x _     x _      m PSLLQ  x _       }, },
/* 15 mem */{ { X _           x _         s LDMXCSR   s STMXCSR   x _     x _      x _      s CLFLUSH },
/* 15 11B */  { X _           x _         x _         x _         x _     s LFENCE s MFENCE s SFENCE  }, },
/* 16 mem */{ { S PREFETCHNTA s PREFETCH0 s PREFETCH1 s PREFETCH2 x _     x _      x _      x _       },
/* 16 11B */  { X _           x _         x _         x _         x _     x _      x _      x _       }, },
};
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_ia32::group66[17][2][8] =
{              // 0             1           2           3           4       5        6        7
/*  0 mem */{ { X _           x _         x _         x _         x _     x _      x _      x _       },
/*  0 11B */  { X _           x _         x _         x _         x _     x _      x _      x _       }, },
/*  1 mem */{ { X _           x _         x _         x _         x _     x _      x _      x _       },
/*  1 11B */  { X _           x _         x _         x _         x _     x _      x _      x _       }, },
/*  2 mem */{ { X _           x _         x _         x _         x _     x _      x _      x _       },
/*  2 11B */  { X _           x _         x _         x _         x _     x _      x _      x _       }, },
/*  3 mem */{ { X _           x _         x _         x _         x _     x _      x _      x _       },
/*  3 11B */  { X _           x _         x _         x _         x _     x _      x _      x _       }, },
/*  4 mem */{ { X _           x _         x _         x _         x _     x _      x _      x _       },
/*  4 11B */  { X _           x _         x _         x _         x _     x _      x _      x _       }, },
/*  5 mem */{ { X _           x _         x _         x _         x _     x _      x _      x _       },
/*  5 11B */  { X _           x _         x _         x _         x _     x _      x _      x _       }, },
/*  6 mem */{ { X _           x _         x _         x _         x _     x _      x _      x _       },
/*  6 11B */  { X _           x _         x _         x _         x _     x _      x _      x _       }, },
/*  7 mem */{ { X _           x _         x _         x _         x _     x _      x _      x _       },
/*  7 11B */  { X _           x _         x _         x _         x _     x _      x _      x _       }, },
/*  8 mem */{ { X _           x _         x _         x _         x _     x _      x _      x _       },
/*  8 11B */  { X _           x _         x _         x _         x _     x _      x _      x _       }, },
/*  9 mem */{ { X _           x _         x _         x _         x _     x _      x _      x _       },
/*  9 11B */  { X _           x _         x _         x _         x _     x _      x _      x _       }, },
/* 10 mem */{ { X _           x _         x _         x _         x _     x _      x _      x _       },
/* 10 11B */  { X _           x _         x _         x _         x _     x _      x _      x _       }, },
/* 11 mem */{ { X _           x _         x _         x _         x _     x _      x _      x _       },
/* 11 11B */  { X _           x _         x _         x _         x _     x _      x _      x _       }, },
/* 12 mem */{ { X _           x _         x _         x _         x _     x _      x _      x _       },
/* 12 11B */  { X _           x _         s PSRLW     x _         s PSRAW x _      s PSLLW  x _       }, },
/* 13 mem */{ { X _           x _         x _         x _         x _     x _      x _      x _       },
/* 13 11B */  { X _           x _         s PSRLD     x _         s PSRAD x _      s PSLLD  x _       }, },
/* 14 mem */{ { X _           x _         x _         x _         x _     x _      x _      x _       },
/* 14 11B */  { X _           x _         s PSRLQ     s PSRLDQ    x _     x _      s PSLLQ  s PSLLDQ  }, },
/* 15 mem */{ { X _           x _         x _         x _         x _     x _      x _      x _       },
/* 15 11B */  { X _           x _         x _         x _         x _     x _      x _      x _       }, },
/* 16 mem */{ { X _           x _         x _         x _         x _     x _      x _      x _       },
/* 16 11B */  { X _           x _         x _         x _         x _     x _      x _      x _       }, },
};
//------------------------------------------------------------------------------
// Escape Opcode Map
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_ia32::escMOD[8][8] =
{         // 0       1        2       3        4        5        6        7
/* D8 */ { X FADD  x FMUL   x FCOM  x FCOMP  x FSUB   x FSUBR  x FDIV   x FDIVR  },
/* D9 */ { X FLD   x _      x FST   x FSTP   x _      x FLDCW  x _      x FSTCW  },
/* DA */ { X FIADD x FIMUL  x FICOM x FICOMP x FISUB  x FISUBR x FIDIV  x FIDIVR },
/* DB */ { X FILD  s FISTTP x FIST  x FISTP  x _      x FLD    x _      x FSTP   },
/* DC */ { X FADD  x FMUL   x FCOM  x FCOMP  x FSUB   x FSUBR  x FDIV   x FDIVR  },
/* DD */ { X FLD   s FISTTP x FST   x FSTP   x _      x _      x _      x FSTSW  },
/* DE */ { X FIADD x FIMUL  x FICOM x FICOMP x FISUB  x FISUBR x FIDIV  x FIDIVR },
/* DF */ { X FILD  s FISTTP x FIST  x FISTP  x _      x FILD   x _      x FISTP  },
};
//------------------------------------------------------------------------------
#undef X
#undef x
#undef M
#undef m
#undef S
#undef s
//------------------------------------------------------------------------------
void x86_ia32::StepImplement(x86_i386& x86, Format& format)
{
    format.width = 32;
    format.length = 1;
    format.address = 32;
    format.operand_count = 2;
    format.prefix = 0;

    auto eip = EIP;
    for (;;) {
        x86.opcode = x86.memory_address + EIP;
        one[x86.opcode[0]](format, x86.opcode);
        EIP += format.length;
        if (format.operation)
            break;
        if (EIP - eip >= 15) {
            if (format.instruction[0] == 0)
                format.instruction = "UNKNOWN";
            break;
        }
    }
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x86_ia32::ESC(Format& format, const uint8_t* opcode)
{
    format.width = 64;
    format.length = 2;
    if ((opcode[1] & 0b11000000) == 0b11000000) {
        uint16_t index = 0;
        index |= (opcode[0] & 0b00000111) << 6;
        index |= (opcode[1] & 0b00111111);
        esc[index](format, opcode);
    }
    else {
        uint8_t l = (opcode[0] & 0b00000111);
        uint8_t r = (opcode[1] & 0b00111000) >> 3;
        escMOD[l][r](format, opcode);
    }
}
//------------------------------------------------------------------------------
void x86_ia32::TWO(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    if (format.prefix == 0x66 && two66[opcode[1]] != _) {
        two66[opcode[1]](format, opcode);
    }
    else if (format.prefix == 0xF2 && twoF2[opcode[1]] != _) {
        twoF2[opcode[1]](format, opcode);
    }
    else if (format.prefix == 0xF3 && twoF3[opcode[1]] != _) {
        twoF3[opcode[1]](format, opcode);
    }
    else {
        two[opcode[1]](format, opcode);
    }
}
//------------------------------------------------------------------------------
void x86_ia32::THREE38(Format& format, const uint8_t* opcode)
{
    format.length = 3;
    if (format.prefix == 0x66 && three6638[opcode[2]] != _) {
        three6638[opcode[2]](format, opcode);
    }
    else {
        three38[opcode[2]](format, opcode);
    }
}
//------------------------------------------------------------------------------
void x86_ia32::THREE3A(Format& format, const uint8_t* opcode)
{
    format.length = 3;
    if (format.prefix == 0x66 && three663A[opcode[2]] != _) {
        three663A[opcode[2]](format, opcode);
    }
    else {
        three3A[opcode[2]](format, opcode);
    }
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x86_ia32::grp1(Format& format, const uint8_t* opcode)
{
    int nnn = ((opcode[1] >> 3) & 0b111);
    int mod = ((opcode[1] >> 6) & 0b11) == 0b11 ? 1 : 0;
    if (format.prefix == 0x66 && group66[1][mod][nnn] != _) {
        group66[1][mod][nnn](format, opcode);
    }
    else {
        group[1][mod][nnn](format, opcode);
    }
}
//------------------------------------------------------------------------------
void x86_ia32::grp2(Format& format, const uint8_t* opcode)
{
    int nnn = ((opcode[1] >> 3) & 0b111);
    int mod = ((opcode[1] >> 6) & 0b11) == 0b11 ? 1 : 0;
    if (format.prefix == 0x66 && group66[1][mod][nnn] != _) {
        group66[2][mod][nnn](format, opcode);
    }
    else {
        group[2][mod][nnn](format, opcode);
    }
}
//------------------------------------------------------------------------------
void x86_ia32::grp3(Format& format, const uint8_t* opcode)
{
    int nnn = ((opcode[1] >> 3) & 0b111);
    int mod = ((opcode[1] >> 6) & 0b11) == 0b11 ? 1 : 0;
    if (format.prefix == 0x66 && group66[1][mod][nnn] != _) {
        group66[3][mod][nnn](format, opcode);
    }
    else {
        group[3][mod][nnn](format, opcode);
    }
}
//------------------------------------------------------------------------------
void x86_ia32::grp4(Format& format, const uint8_t* opcode)
{
    int nnn = ((opcode[1] >> 3) & 0b111);
    int mod = ((opcode[1] >> 6) & 0b11) == 0b11 ? 1 : 0;
    if (format.prefix == 0x66 && group66[1][mod][nnn] != _) {
        group66[4][mod][nnn](format, opcode);
    }
    else {
        group[4][mod][nnn](format, opcode);
    }
}
//------------------------------------------------------------------------------
void x86_ia32::grp5(Format& format, const uint8_t* opcode)
{
    int nnn = ((opcode[1] >> 3) & 0b111);
    int mod = ((opcode[1] >> 6) & 0b11) == 0b11 ? 1 : 0;
    if (format.prefix == 0x66 && group66[1][mod][nnn] != _) {
        group66[5][mod][nnn](format, opcode);
    }
    else {
        group[5][mod][nnn](format, opcode);
    }
}
//------------------------------------------------------------------------------
void x86_ia32::grp6(Format& format, const uint8_t* opcode)
{
    int nnn = ((opcode[1] >> 3) & 0b111);
    int mod = ((opcode[1] >> 6) & 0b11) == 0b11 ? 1 : 0;
    if (format.prefix == 0x66 && group66[1][mod][nnn] != _) {
        group66[6][mod][nnn](format, opcode);
    }
    else {
        group[6][mod][nnn](format, opcode);
    }
}
//------------------------------------------------------------------------------
void x86_ia32::grp7(Format& format, const uint8_t* opcode)
{
    int nnn = ((opcode[1] >> 3) & 0b111);
    int mod = ((opcode[1] >> 6) & 0b11) == 0b11 ? 1 : 0;
    if (format.prefix == 0x66 && group66[1][mod][nnn] != _) {
        group66[7][mod][nnn](format, opcode);
    }
    else {
        group[7][mod][nnn](format, opcode);
    }
}
//------------------------------------------------------------------------------
void x86_ia32::grp8(Format& format, const uint8_t* opcode)
{
    int nnn = ((opcode[1] >> 3) & 0b111);
    int mod = ((opcode[1] >> 6) & 0b11) == 0b11 ? 1 : 0;
    if (format.prefix == 0x66 && group66[1][mod][nnn] != _) {
        group66[8][mod][nnn](format, opcode);
    }
    else {
        group[8][mod][nnn](format, opcode);
    }
}
//------------------------------------------------------------------------------
void x86_ia32::grp9(Format& format, const uint8_t* opcode)
{
    int nnn = ((opcode[1] >> 3) & 0b111);
    int mod = ((opcode[1] >> 6) & 0b11) == 0b11 ? 1 : 0;
    if (format.prefix == 0x66 && group66[1][mod][nnn] != _) {
        group66[9][mod][nnn](format, opcode);
    }
    else {
        group[9][mod][nnn](format, opcode);
    }
}
//------------------------------------------------------------------------------
void x86_ia32::grp12(Format& format, const uint8_t* opcode)
{
    int nnn = ((opcode[1] >> 3) & 0b111);
    int mod = ((opcode[1] >> 6) & 0b11) == 0b11 ? 1 : 0;
    if (format.prefix == 0x66 && group66[1][mod][nnn] != _) {
        group66[12][mod][nnn](format, opcode);
    }
    else {
        group[12][mod][nnn](format, opcode);
    }
}
//------------------------------------------------------------------------------
void x86_ia32::grp13(Format& format, const uint8_t* opcode)
{
    int nnn = ((opcode[1] >> 3) & 0b111);
    int mod = ((opcode[1] >> 6) & 0b11) == 0b11 ? 1 : 0;
    if (format.prefix == 0x66 && group66[1][mod][nnn] != _) {
        group66[13][mod][nnn](format, opcode);
    }
    else {
        group[13][mod][nnn](format, opcode);
    }
}
//------------------------------------------------------------------------------
void x86_ia32::grp14(Format& format, const uint8_t* opcode)
{
    int nnn = ((opcode[1] >> 3) & 0b111);
    int mod = ((opcode[1] >> 6) & 0b11) == 0b11 ? 1 : 0;
    if (format.prefix == 0x66 && group66[1][mod][nnn] != _) {
        group66[14][mod][nnn](format, opcode);
    }
    else {
        group[14][mod][nnn](format, opcode);
    }
}
//------------------------------------------------------------------------------
void x86_ia32::grp15(Format& format, const uint8_t* opcode)
{
    int nnn = ((opcode[1] >> 3) & 0b111);
    int mod = ((opcode[1] >> 6) & 0b11) == 0b11 ? 1 : 0;
    if (format.prefix == 0x66 && group66[1][mod][nnn] != _) {
        group66[15][mod][nnn](format, opcode);
    }
    else {
        group[15][mod][nnn](format, opcode);
    }
}
//------------------------------------------------------------------------------
void x86_ia32::grp16(Format& format, const uint8_t* opcode)
{
    int nnn = ((opcode[1] >> 3) & 0b111);
    int mod = ((opcode[1] >> 6) & 0b11) == 0b11 ? 1 : 0;
    if (format.prefix == 0x66 && group66[1][mod][nnn] != _) {
        group66[16][mod][nnn](format, opcode);
    }
    else {
        group[16][mod][nnn](format, opcode);
    }
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x86_ia32::CPUID(Format& format, const uint8_t* opcode)
{
    format.instruction = "CPUID";

    OPERATION() {
        switch (EAX) {
        case 0x00:
            EAX = 1;
            EBX = __builtin_bswap32('Genu');
            EDX = __builtin_bswap32('ineI');
            ECX = __builtin_bswap32('ntel');
            break;
        case 0x01:
            EAX = 0;
            EBX = 0;
            EDX = 0;
            ECX = 0;
            EAX |= (1 <<  0);   // Stepping
            EAX |= (7 <<  4);   // Model
            EAX |= (6 <<  8);   // Family
            EDX |= (1 <<  0);   // FPU
            EDX |= (1 <<  4);   // TSC
            EDX |= (1 <<  8);   // CX8
            EDX |= (1 << 15);   // CMOV
            EDX |= (1 << 23);   // MMX
            EDX |= (1 << 25);   // SSE
            break;
        }
    };
}
//------------------------------------------------------------------------------
