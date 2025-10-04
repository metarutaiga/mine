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
#include "sse3_instruction.h"

//------------------------------------------------------------------------------
#define o x86_ia32::
#define x , o
#define X , sse3_instruction::
//------------------------------------------------------------------------------
// One-Byte Opcode Map
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_ia32::one[256] =
{      // 0        1       2       3      4       5       6       7       8       9       A       B       C        D       E       F
/* 0 */ o ADD    x ADD   x ADD   x ADD  x ADD   x ADD   x _     x _     x OR    x OR    x OR    x OR    x OR     x OR    x _     x TWO
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
{      // 0           1           2           3          4          5          6           7            8           9           A           B           C            D            E         F
/* 0 */ o grp6      x grp7      x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 1 */ x MOVUPS    x MOVUPS    x MOVLPS    x MOVLPS   x UNPCKLPS x UNPCKHPS x MOVHPS    x MOVHPS     x grp16     x _         x _         x _         x _          x _          x _       x _
/* 2 */ x _         x _         x _         x _        x _        x _        x _         x _          x MOVAPS    x MOVAPS    x CVTPI2PS  x MOVNTPS   x CVTTPS2PI  x CVTPS2PI   x UCOMISS x COMISS
/* 3 */ x _         x RDTSC     x _         x RDPMC    x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 4 */ x CMOVcc    x CMOVcc    x CMOVcc    x CMOVcc   x CMOVcc   x CMOVcc   x CMOVcc    x CMOVcc     x CMOVcc    x CMOVcc    x CMOVcc    x CMOVcc    x CMOVcc     x CMOVcc     x CMOVcc  x CMOVcc
/* 5 */ x MOVMSKPS  x SQRTPS    x RSQRTPS   x RCPPS    x ANDPS    x ANDNPS   x ORPS      x XORPS      x ADDPS     x MULPS     x _         x _         x SUBPS      x MINPS      x DIVPS   x MAXPS
/* 6 */ x PUNPCKLBW x PUNPCKLWD x PUNPCKLDQ x PACKSSWB x PCMPGTB  x PCMPGTW  x PCMPGTD   x PACKUSWB   x PUNPCKHBW x PUNPCKHWD x PUNPCKHDQ x PACKSSDW  x _          x _          x MOVD    x MOVQ
/* 7 */ x PSHUFW    x grp12     x grp13     x grp14    x PCMPEQB  x PCMPEQW  x PCMPEQD   x EMMS       x _         x _         x _         x _         x _          x _          x MOVD    x MOVQ
/* 8 */ x Jcc       x Jcc       x Jcc       x Jcc      x Jcc      x Jcc      x Jcc       x Jcc        x Jcc       x Jcc       x Jcc       x Jcc       x Jcc        x Jcc        x Jcc     x Jcc
/* 9 */ x SETcc     x SETcc     x SETcc     x SETcc    x SETcc    x SETcc    x SETcc     x SETcc      x SETcc     x SETcc     x SETcc     x SETcc     x SETcc      x SETcc      x SETcc   x SETcc
/* A */ x _         x _         x CPUID     x BT       x SHxD     x SHxD     x _         x _          x _         x _         x _         x BTS       x SHxD       x SHxD       x grp15   x IMUL
/* B */ x CMPXCHG   x CMPXCHG   x _         x BTR      x _        x _        x MOVZX     x MOVZX      x _         x _         x grp8      x BTC       x BSF        x BSR        x MOVSX   x MOVSX
/* C */ x XADD      x XADD      x CMPPS     x _        x PINSRW   x PEXTRW   x SHUFPS    x grp9       x BSWAP     x BSWAP     x BSWAP     x BSWAP     x BSWAP      x BSWAP      x BSWAP   x BSWAP
/* D */ x _         x PSRLW     x PSRLD     x PSRLQ    x _        x PMULLW   x _         x PMOVMSKB   x PSUBUSB   x PSUBUSW   x PMINUB    x PAND      x PADDUSB    x PADDUSW    x PMAXUB  x PANDN
/* E */ x PAVGB     x PSRAW     x PSRAD     x PAVGW    x PMULHUW  x PMULHW   x _         x MOVNTQ     x PSUBSB    x PSUBSW    x PMINSW    x POR       x PADDSB     x PADDSW     x PMAXSW  x PXOR
/* F */ x _         x PSLLW     x PSLLD     x PSLLQ    x _        x PMADDWD  x PSADBW    x _          x PSUBB     x PSUBW     x PSUBD     x _         x PADDB      x PADDW      x PADDD   x _
};
//------------------------------------------------------------------------------
// Two-Byte Opcode Map - 66
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_ia32::two66[256] =
{      // 0           1           2           3          4          5          6           7            8           9           A           B           C            D            E         F
/* 0 */ o _         x _         x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 1 */ X MOVUPD    X MOVUPD    X MOVLPD    X MOVLPD   X UNPCKLPD X UNPCKHPD X MOVHPD    X MOVHPD     X MOVAPD    X MOVAPD    X CVTPI2PD  X MOVNTPD   X CVTTPD2PI  X CVTPD2PI   X UCOMISD X COMISD
/* 2 */ x _         x _         x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 3 */ x _         x _         x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 4 */ x _         x _         x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 5 */ X MOVMSKPD  X SQRTPD    x _         x _        X ANDPD    X ANDNPD   X ORPD      X XORPD      X ADDPD     X MULPD     X CVTPD2PS  X CVTPS2DQ  X SUBPD      X MINPD      X DIVPD   X MAXPD
/* 6 */ X PUNPCKLBW X PUNPCKLWD X PUNPCKLDQ X PACKSSWB X PCMPGTB  X PCMPGTW  X PCMPGTD   X PACKUSWB   X PUNPCKHBW X PUNPCKHWD X PUNPCKHDQ X PACKSSDW  X PUNPCKLQDQ X PUNPCKHQDQ X MOVD    X MOVDQA
/* 7 */ X PSHUFD    x _         x _         x _        X PCMPEQB  X PCMPEQW  X PCMPEQD   x _          x _         x _         x _         x _         X HADDPD     X HSUBPD     X MOVD    X MOVDQA
/* 8 */ x _         x _         x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 9 */ x _         x _         x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* A */ x _         x _         x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* B */ x _         x _         x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* C */ x _         x _         X CMPPD     x _        X PINSRW   X PEXTRW   X SHUFPD    x _          x _         x _         x _         x _         x _          x _          x _       x _
/* D */ X ADDSUBPD  X PSRLW     X PSRLD     X PSRLQ    X PADDQ    X PMULLW   X MOVQ      X PMOVMSKB   X PSUBUSB   X PSUBUSW   X PMINUB    X PAND      X PADDUSB    X PADDUSW    X PMAXUB  X PANDN
/* E */ X PAVGB     X PSRAW     X PSRAD     X PAVGW    X PMULHUW  X PMULHW   X CVTTPD2DQ X MOVNTDQ    X PSUBSB    X PSUBSW    X PMINSW    X POR       X PADDSB     X PADDSW     X PMAXSW  X PXOR
/* F */ x _         X PSLLW     X PSLLD     X PSLLQ    X PMULUDQ  X PMADDWD  X PSADBW    X MASKMOVDQU X PSUBB     X PSUBW     X PSUBD     X PSUBQ     X PADDB      X PADDW      X PADDD   x _
};
//------------------------------------------------------------------------------
// Two-Byte Opcode Map - F2
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_ia32::twoF2[256] =
{      // 0           1           2           3          4          5          6           7            8           9           A           B           C            D            E         F
/* 0 */ o _         x _         x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 1 */ X MOVSD     X MOVSD     X MOVDDUP   x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 2 */ x _         x _         x _         x _        x _        x _        x _         x _          x _         x _         X CVTSI2SD  x _         X CVTTSD2SI  X CVTSD2SI   x _       x _
/* 3 */ x _         x _         x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 4 */ x _         x _         x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 5 */ x _         X SQRTSD    x _         x _        x _        x _        x _         x _          X ADDSD     X MULSD     X CVTSD2SS  x _         X SUBSD      X MINSD      X DIVSD   X MAXSD
/* 6 */ x _         x _         x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 7 */ X PSHUFLW   x _         x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         X HADDPS     X HSUBPS     x _       x _
/* 8 */ x _         x _         x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 9 */ x _         x _         x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* A */ x _         x _         x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* B */ x _         x _         x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* C */ x _         x _         X CMPSD     x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* D */ x _         x _         x _         x _        x _        x _        X MOVDQ2Q   x _          x _         x _         x _         x _         x _          x _          x _       x _
/* E */ x _         x _         x _         x _        x _        x _        X CVTPD2DQ  x _          x _         x _         x _         x _         x _          x _          x _       x _
/* F */ X LDDQU     x _         x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
};
//------------------------------------------------------------------------------
// Two-Byte Opcode Map - F3
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_ia32::twoF3[256] =
{      // 0           1           2           3          4          5          6           7            8           9           A           B           C            D            E         F
/* 0 */ o _         x _         x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 1 */ x MOVSS     x MOVSS     X MOVSLDUP  x _        x _        x _        X MOVSHDUP  x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 2 */ x _         x _         x _         x _        x _        x _        x _         x _          x _         x _         x CVTSI2SS  x _         x CVTTSS2SI  x CVTSS2SI   x _       x _
/* 3 */ x _         x _         x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 4 */ x _         x _         x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 5 */ x _         x SQRTSS    x RSQRTSS   x RCPSS    x _        x _        x _         x _          x ADDSS     x MULSS     X CVTSS2SD  X CVTTPS2DQ x SUBSS      x MINSS      x DIVSS   x MAXSS
/* 6 */ x _         x _         x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       X MOVDQU
/* 7 */ X PSHUFHW   x _         x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          X MOVQ    X MOVDQU
/* 8 */ x _         x _         x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* 9 */ x _         x _         x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* A */ x _         x _         x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* B */ x _         x _         x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* C */ x _         x _         x CMPSS     x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
/* D */ x _         x _         x _         x _        x _        x _        X MOVQ2DQ   x _          x _         x _         x _         x _         x _          x _          x _       x _
/* E */ x _         x _         x _         x _        x _        x _        X CVTDQ2PD  x _          x _         x _         x _         x _         x _          x _          x _       x _
/* F */ x _         x _         x _         x _        x _        x _        x _         x _          x _         x _         x _         x _         x _          x _          x _       x _
};
//------------------------------------------------------------------------------
// Opcodes determined by bits 5,4,3 of modR/M byte
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_ia32::group[17][2][8] =
{              // 0             1           2           3           4       5        6        7
/*  0 mem */{ { o _           x _         x _         x _         x _     x _      x _      x _       },
/*  0 11B */  { o _           x _         x _         x _         x _     x _      x _      x _       }, },
/*  1 mem */{ { o ADD         x OR        x ADC       x SBB       x AND   x SUB    x XOR    x CMP     },
/*  1 11B */  { o ADD         x OR        x ADC       x SBB       x AND   x SUB    x XOR    x CMP     }, },
/*  2 mem */{ { o Rxx         x Rxx       x Rxx       x Rxx       x Sxx   x Sxx    x _      x Sxx     },
/*  2 11B */  { o Rxx         x Rxx       x Rxx       x Rxx       x Sxx   x Sxx    x _      x Sxx     }, },
/*  3 mem */{ { o TEST        x _         x NOT       x NEG       x MUL   x IMUL   x DIV    x IDIV    },
/*  3 11B */  { o TEST        x _         x NOT       x NEG       x MUL   x IMUL   x DIV    x IDIV    }, },
/*  4 mem */{ { o INC         x DEC       x _         x _         x _     x _      x _      x _       },
/*  4 11B */  { o INC         x DEC       x _         x _         x _     x _      x _      x _       }, },
/*  5 mem */{ { o INC         x DEC       x CALL      x _         x JMP   x _      x PUSH   x _       },
/*  5 11B */  { o INC         x DEC       x CALL      x _         x JMP   x _      x PUSH   x _       }, },
/*  6 mem */{ { o _           x _         x _         x _         x _     x _      x _      x _       },
/*  6 11B */  { o _           x _         x _         x _         x _     x _      x _      x _       }, },
/*  7 mem */{ { o _           x _         x _         x _         x _     x _      x _      x _       },
/*  7 11B */  { o _           X MONITOR   x _         x _         x _     x _      x _      x _       }, },
/*  8 mem */{ { o _           x _         x _         x _         x BT    x BTS    x BTR    x BTC     },
/*  8 11B */  { o _           x _         x _         x _         x BT    x BTS    x BTR    x BTC     }, },
/*  9 mem */{ { o _           x CMPXCHG8B x _         x _         x _     x _      x _      x _       },
/*  9 11B */  { o _           x _         x _         x _         x _     x _      x _      x _       }, },
/* 10 mem */{ { o _           x _         x _         x _         x _     x _      x _      x _       },
/* 10 11B */  { o _           x _         x _         x _         x _     x _      x _      x _       }, },
/* 11 mem */{ { o _           x _         x _         x _         x _     x _      x _      x _       },
/* 11 11B */  { o _           x _         x _         x _         x _     x _      x _      x _       }, },
/* 12 mem */{ { o _           x _         x _         x _         x _     x _      x _      x _       },
/* 12 11B */  { o _           x _         x PSRLW     x _         x PSRAW x _      x PSLLW  x _       }, },
/* 13 mem */{ { o _           x _         x _         x _         x _     x _      x _      x _       },
/* 13 11B */  { o _           x _         x PSRLD     x _         x PSRAD x _      x PSLLD  x _       }, },
/* 14 mem */{ { o _           x _         x _         x _         x _     x _      x _      x _       },
/* 14 11B */  { o _           x _         x PSRLQ     x _         x _     x _      x PSLLQ  x _       }, },
/* 15 mem */{ { o _           x _         x LDMXCSR   x STMXCSR   x _     x _      x _      X CLFLUSH },
/* 15 11B */  { o _           x _         x _         x _         x _     X LFENCE X MFENCE x SFENCE  }, },
/* 16 mem */{ { o PREFETCHNTA x PREFETCH0 x PREFETCH1 x PREFETCH2 x _     x _      x _      x _       },
/* 16 11B */  { o _           x _         x _         x _         x _     x _      x _      x _       }, },
};
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_ia32::group66[17][2][8] =
{              // 0             1           2           3           4       5        6        7
/*  0 mem */{ { o _           x _         x _         x _         x _     x _      x _      x _       },
/*  0 11B */  { o _           x _         x _         x _         x _     x _      x _      x _       }, },
/*  1 mem */{ { o _           x _         x _         x _         x _     x _      x _      x _       },
/*  1 11B */  { o _           x _         x _         x _         x _     x _      x _      x _       }, },
/*  2 mem */{ { o _           x _         x _         x _         x _     x _      x _      x _       },
/*  2 11B */  { o _           x _         x _         x _         x _     x _      x _      x _       }, },
/*  3 mem */{ { o _           x _         x _         x _         x _     x _      x _      x _       },
/*  3 11B */  { o _           x _         x _         x _         x _     x _      x _      x _       }, },
/*  4 mem */{ { o _           x _         x _         x _         x _     x _      x _      x _       },
/*  4 11B */  { o _           x _         x _         x _         x _     x _      x _      x _       }, },
/*  5 mem */{ { o _           x _         x _         x _         x _     x _      x _      x _       },
/*  5 11B */  { o _           x _         x _         x _         x _     x _      x _      x _       }, },
/*  6 mem */{ { o _           x _         x _         x _         x _     x _      x _      x _       },
/*  6 11B */  { o _           x _         x _         x _         x _     x _      x _      x _       }, },
/*  7 mem */{ { o _           x _         x _         x _         x _     x _      x _      x _       },
/*  7 11B */  { o _           x _         x _         x _         x _     x _      x _      x _       }, },
/*  8 mem */{ { o _           x _         x _         x _         x _     x _      x _      x _       },
/*  8 11B */  { o _           x _         x _         x _         x _     x _      x _      x _       }, },
/*  9 mem */{ { o _           x _         x _         x _         x _     x _      x _      x _       },
/*  9 11B */  { o _           x _         x _         x _         x _     x _      x _      x _       }, },
/* 10 mem */{ { o _           x _         x _         x _         x _     x _      x _      x _       },
/* 10 11B */  { o _           x _         x _         x _         x _     x _      x _      x _       }, },
/* 11 mem */{ { o _           x _         x _         x _         x _     x _      x _      x _       },
/* 11 11B */  { o _           x _         x _         x _         x _     x _      x _      x _       }, },
/* 12 mem */{ { o _           x _         x _         x _         x _     x _      x _      x _       },
/* 12 11B */  { o _           x _         X PSRLW     x _         X PSRAW x _      X PSLLW  x _       }, },
/* 13 mem */{ { o _           x _         x _         x _         x _     x _      x _      x _       },
/* 13 11B */  { o _           x _         X PSRLD     x _         X PSRAD x _      X PSLLD  x _       }, },
/* 14 mem */{ { o _           x _         x _         x _         x _     x _      x _      x _       },
/* 14 11B */  { o _           x _         X PSRLQ     X PSRLDQ    x _     x _      X PSLLQ  X PSLLDQ  }, },
/* 15 mem */{ { o _           x _         x _         x _         x _     x _      x _      x _       },
/* 15 11B */  { o _           x _         x _         x _         x _     x _      x _      x _       }, },
/* 16 mem */{ { o _           x _         x _         x _         x _     x _      x _      x _       },
/* 16 11B */  { o _           x _         x _         x _         x _     x _      x _      x _       }, },
};
//------------------------------------------------------------------------------
// Escape Opcode Map
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_ia32::escMOD[8][8] =
{         // 0       1        2       3        4        5        6        7
/* D8 */ { o FADD  x FMUL   x FCOM  x FCOMP  x FSUB   x FSUBR  x FDIV   x FDIVR  },
/* D9 */ { o FLD   x _      x FST   x FSTP   x _      x FLDCW  x _      x FSTCW  },
/* DA */ { o FIADD x FIMUL  x FICOM x FICOMP x FISUB  x FISUBR x FIDIV  x FIDIVR },
/* DB */ { o FILD  X FISTTP x FIST  x FISTP  x _      x FLD    x _      x FSTP   },
/* DC */ { o FADD  x FMUL   x FCOM  x FCOMP  x FSUB   x FSUBR  x FDIV   x FDIVR  },
/* DD */ { o FLD   X FISTTP x FST   x FSTP   x _      x _      x _      x FSTSW  },
/* DE */ { o FIADD x FIMUL  x FICOM x FICOMP x FISUB  x FISUBR x FIDIV  x FIDIVR },
/* DF */ { o FILD  X FISTTP x FIST  x FISTP  x _      x FILD   x _      x FISTP  },
};
//------------------------------------------------------------------------------
#undef o
#undef x
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
