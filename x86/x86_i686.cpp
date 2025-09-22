//==============================================================================
// Pentium(TM) Pro Family Developers Manual Volume 3
//
// 1996
//==============================================================================
#include "x86_i686.h"
#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"

//------------------------------------------------------------------------------
#define o x86_i686::
#define x , o
//------------------------------------------------------------------------------
// One-Byte Opcode Map
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_i686::one[256] =
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
const x86_instruction::instruction_pointer x86_i686::two[256] =
{      // 0           1           2           3          4          5          6         7          8           9           A           B          C           D          E         F
/* 0 */ o grp6      x grp7      x _         x _        x _        x _        x _       x _        x _         x _         x _         x _        x _         x _        x _       x _
/* 1 */ x MOVUPS    x MOVUPS    x MOVLPS    x MOVLPS   x UNPCKLPS x UNPCKHPS x MOVHPS  x MOVHPS   x grp17     x _         x _         x _        x _         x _        x _       x _
/* 2 */ x _         x _         x _         x _        x _        x _        x _       x _        x MOVAPS    x MOVAPS    x CVTPI2PS  x MOVNTPS  x CVTTPS2PI x CVTPS2PI x UCOMISS x COMISS
/* 3 */ x _         x RDTSC     x _         x RDPMC    x _        x _        x _       x _        x _         x _         x _         x _        x _         x _        x _       x _
/* 4 */ x CMOVcc    x CMOVcc    x CMOVcc    x CMOVcc   x CMOVcc   x CMOVcc   x CMOVcc  x CMOVcc   x CMOVcc    x CMOVcc    x CMOVcc    x CMOVcc   x CMOVcc    x CMOVcc   x CMOVcc  x CMOVcc
/* 5 */ x MOVMSKPS  x SQRTPS    x RSQRTPS   x RCPPS    x ANDPS    x ANDNPS   x ORPS    x XORPS    x ADDPS     x MULPS     x _         x _        x SUBPS     x MINPS    x DIVPS   x MAXPS
/* 6 */ x PUNPCKLBW x PUNPCKLWD x PUNPCKLDQ x PACKSSWB x PCMPGTB  x PCMPGTW  x PCMPGTD x PACKUSWB x PUNPCKHBW x PUNPCKHWD x PUNPCKHDQ x PACKSSDW x _         x _        x MOVD    x MOVQ
/* 7 */ x PSHUFW    x grp13     x grp14     x grp15    x PCMPEQB  x PCMPEQW  x PCMPEQD x EMMS     x _         x _         x _         x _        x _         x _        x MOVD    x MOVQ
/* 8 */ x Jcc       x Jcc       x Jcc       x Jcc      x Jcc      x Jcc      x Jcc     x Jcc      x Jcc       x Jcc       x Jcc       x Jcc      x Jcc       x Jcc      x Jcc     x Jcc
/* 9 */ x SETcc     x SETcc     x SETcc     x SETcc    x SETcc    x SETcc    x SETcc   x SETcc    x SETcc     x SETcc     x SETcc     x SETcc    x SETcc     x SETcc    x SETcc   x SETcc
/* A */ x _         x _         x CPUID     x BT       x SHxD     x SHxD     x _       x _        x _         x _         x _         x BTS      x SHxD      x SHxD     x grp16   x IMUL
/* B */ x CMPXCHG   x CMPXCHG   x _         x BTR      x _        x _        x MOVZX   x MOVZX    x _         x _         x grp8      x BTC      x BSF       x BSR      x MOVSX   x MOVSX
/* C */ x XADD      x XADD      x CMPPS     x _        x PINSRW   x PEXTRW   x SHUFPS  x grp9     x BSWAP     x BSWAP     x BSWAP     x BSWAP    x BSWAP     x BSWAP    x BSWAP   x BSWAP
/* D */ x _         x PSRLW     x PSRLD     x PSRLQ    x _        x PMULLW   x _       x PMOVMSKB x PSUBUSB   x PSUBUSW   x PMINUB    x PAND     x PADDUSB   x PADDUSW  x PMAXUB  x PANDN
/* E */ x PAVGB     x PSRAW     x PSRAD     x PAVGW    x PMULHUW  x PMULHW   x _       x MOVNTQ   x PSUBSB    x PSUBSW    x PMINSW    x POR      x PADDSB    x PADDSW   x PMAXSW  x PXOR
/* F */ x _         x PSLLW     x PSLLD     x PSLLQ    x _        x PMADDWD  x PSADBW  x _        x PSUBB     x PSUBW     x PSUBD     x _        x PADDB     x PADDW    x PADDD   x _
};
//------------------------------------------------------------------------------
// Opcodes determined by bits 5,4,3 of modR/M byte
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_i686::group[18][8] =
{        // 0             1           2           3           4       5      6       7
/*  0 */{ o _           x _         x _         x _         x _     x _    x _     x _      },
/*  1 */{ o ADD         x OR        x ADC       x SBB       x AND   x SUB  x XOR   x CMP    },
/*  2 */{ o Rxx         x Rxx       x Rxx       x Rxx       x Sxx   x Sxx  x _     x Sxx    },
/*  3 */{ o TEST        x _         x NOT       x NEG       x MUL   x IMUL x DIV   x IDIV   },
/*  4 */{ o INC         x DEC       x _         x _         x _     x _    x _     x _      },
/*  5 */{ o INC         x DEC       x CALL      x _         x JMP   x _    x PUSH  x _      },
/*  6 */{ o _           x _         x _         x _         x _     x _    x _     x _      },
/*  7 */{ o _           x _         x _         x _         x _     x _    x _     x _      },
/*  8 */{ o _           x _         x _         x _         x BT    x BTS  x BTR   x BTC    },
/*  9 */{ o _           x CMPXCHG8B x _         x _         x _     x _    x _     x _      },
/* 10 */{ o _           x _         x _         x _         x _     x _    x _     x _      },
/* 11 */{ o _           x _         x _         x _         x _     x _    x _     x _      },
/* 12 */{ o _           x _         x _         x _         x _     x _    x _     x _      },
/* 13 */{ o _           x _         x PSRLW     x _         x PSRAW x _    x PSLLW x _      },
/* 14 */{ o _           x _         x PSRLD     x _         x PSRAD x _    x PSLLD x _      },
/* 15 */{ o _           x _         x PSRLQ     x _         x _     x _    x PSLLQ x _      },
/* 16 */{ o _           x _         x LDMXCSR   x STMXCSR   x _     x _    x _     x SFENCE },
/* 17 */{ o PREFETCHNTA x PREFETCH0 x PREFETCH1 x PREFETCH2 x _     x _    x _     x _      },
};
//------------------------------------------------------------------------------
// Escape Opcode Map
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_i686::esc[512] =
{            // 0         1         2         3         4         5         6         7         8         9         A         B         C         D         E         F
/* D8 048C */ o FADD    x FADD    x FADD    x FADD    x FADD    x FADD    x FADD    x FADD    x FMUL    x FMUL    x FMUL    x FMUL    x FMUL    x FMUL    x FMUL    x FMUL
/* D8 159D */ x FCOM    x FCOM    x FCOM    x FCOM    x FCOM    x FCOM    x FCOM    x FCOM    x FCOMP   x FCOMP   x FCOMP   x FCOMP   x FCOMP   x FCOMP   x FCOMP   x FCOMP
/* D8 26AE */ x FSUB    x FSUB    x FSUB    x FSUB    x FSUB    x FSUB    x FSUB    x FSUB    x FSUBR   x FSUBR   x FSUBR   x FSUBR   x FSUBR   x FSUBR   x FSUBR   x FSUBR
/* D8 37BF */ x FDIV    x FDIV    x FDIV    x FDIV    x FDIV    x FDIV    x FDIV    x FDIV    x FDIVR   x FDIVR   x FDIVR   x FDIVR   x FDIVR   x FDIVR   x FDIVR   x FDIVR
/* D9 048C */ x FLD     x FLD     x FLD     x FLD     x FLD     x FLD     x FLD     x FLD     x FXCH    x FXCH    x FXCH    x FXCH    x FXCH    x FXCH    x FXCH    x FXCH
/* D9 159D */ x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _
/* D9 26AE */ x FCHS    x FABS    x _       x _       x FTST    x FXAM    x _       x _       x FLD1    x FLDL2T  x FLDL2E  x FLDPI   x FLDLG2  x FLDLN2  x FLDZ    x _
/* D9 37BF */ x F2XM1   x FYL2X   x FPTAN   x FPATAN  x FXTRACT x FPREM1  x FDECSTP x FINCSTP x FPREM   x FYL2XP1 x FSQRT   x FSINCOS x FRNDINT x FSCALE  x FSIN    x FCOS
/* DA 048C */ x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc
/* DA 159D */ x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc
/* DA 26AE */ x _       x _       x _       x _       x _       x _       x _       x _       x _       x FUCOMPP x _       x _       x _       x _       x _       x _
/* DA 37BF */ x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _
/* DB 048C */ x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc
/* DB 159D */ x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc x FCMOVcc
/* DB 26AE */ x _       x _       x FCLEX   x _       x _       x _       x _       x _       x FUCOMI  x FUCOMI  x FUCOMI  x FUCOMI  x FUCOMI  x FUCOMI  x FUCOMI  x FUCOMI
/* DB 37BF */ x FCOMI   x FCOMI   x FCOMI   x FCOMI   x FCOMI   x FCOMI   x FCOMI   x FCOMI   x _       x _       x _       x _       x _       x _       x _       x _
/* DC 048C */ x FADD    x FADD    x FADD    x FADD    x FADD    x FADD    x FADD    x FADD    x FMUL    x FMUL    x FMUL    x FMUL    x FMUL    x FMUL    x FMUL    x FMUL
/* DC 159D */ x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _
/* DC 26AE */ x FSUBR   x FSUBR   x FSUBR   x FSUBR   x FSUBR   x FSUBR   x FSUBR   x FSUBR   x FSUB    x FSUB    x FSUB    x FSUB    x FSUB    x FSUB    x FSUB    x FSUB
/* DC 37BF */ x FDIVR   x FDIVR   x FDIVR   x FDIVR   x FDIVR   x FDIVR   x FDIVR   x FDIVR   x FDIV    x FDIV    x FDIV    x FDIV    x FDIV    x FDIV    x FDIV    x FDIV
/* DD 048C */ x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _
/* DD 159D */ x FST     x FST     x FST     x FST     x FST     x FST     x FST     x FST     x FSTP    x FSTP    x FSTP    x FSTP    x FSTP    x FSTP    x FSTP    x FSTP
/* DD 26AE */ x FUCOM   x FUCOM   x FUCOM   x FUCOM   x FUCOM   x FUCOM   x FUCOM   x FUCOM   x FUCOMP  x FUCOMP  x FUCOMP  x FUCOMP  x FUCOMP  x FUCOMP  x FUCOMP  x FUCOMP
/* DD 37BF */ x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _
/* DE 048C */ x FADDP   x FADDP   x FADDP   x FADDP   x FADDP   x FADDP   x FADDP   x FADDP   x FMULP   x FMULP   x FMULP   x FMULP   x FMULP   x FMULP   x FMULP   x FMULP
/* DE 159D */ x _       x _       x _       x _       x _       x _       x _       x _       x _       x FCOMPP  x _       x _       x _       x _       x _       x _
/* DE 26AE */ x FSUBRP  x FSUBRP  x FSUBRP  x FSUBRP  x FSUBRP  x FSUBRP  x FSUBRP  x FSUBRP  x FSUBP   x FSUBP   x FSUBP   x FSUBP   x FSUBP   x FSUBP   x FSUBP   x FSUBP
/* DE 37BF */ x FDIVRP  x FDIVRP  x FDIVRP  x FDIVRP  x FDIVRP  x FDIVRP  x FDIVRP  x FDIVRP  x FDIVP   x FDIVP   x FDIVP   x FDIVP   x FDIVP   x FDIVP   x FDIVP   x FDIVP
/* DF 048C */ x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _
/* DF 159D */ x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _       x _
/* DF 26AE */ x FSTSW   x _       x _       x _       x _       x _       x _       x _       x FUCOMIP x FUCOMIP x FUCOMIP x FUCOMIP x FUCOMIP x FUCOMIP x FUCOMIP x FUCOMIP
/* DF 37BF */ x FCOMIP  x FCOMIP  x FCOMIP  x FCOMIP  x FCOMIP  x FCOMIP  x FCOMIP  x FCOMIP  x _       x _       x _       x _       x _       x _       x _       x _
};
//------------------------------------------------------------------------------
#undef o
#undef x
//------------------------------------------------------------------------------
void* x86_i686::Register(int type) const
{
    switch (type) {
    case 'x86 ':
        return (x86_register*)this;
    case 'x87 ':
        return (x87_register*)this;
    case 'mmx ':
        return (mmx_register*)this;
    case 'sse ':
        return (sse_register*)this;
    }
    return nullptr;
}
//------------------------------------------------------------------------------
std::string x86_i686::Status() const
{
    std::string output = x86_i486::Status();

    // SSE
    for (int i = 0; i < 8; ++i) {
        char temp[64];
        snprintf(temp, 64, "XMM%d    %016llX%016llX", i, sse.sse_register::regs[i].u64[1], sse.sse_register::regs[i].u64[0]);
        output += '\n';
        output += temp;
    }

    return output;
}
//------------------------------------------------------------------------------
void x86_i686::StepImplement(x86_i386& x86, Format& format)
{
    format.width = 32;
    format.length = 1;
    format.address = 32;
    format.repeatF2 = false;
    format.repeatF3 = false;

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
void x86_i686::ESC(Format& format, const uint8_t* opcode)
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
void x86_i686::TWO(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    two[opcode[1]](format, opcode);
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x86_i686::grp1(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    group[1][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i686::grp2(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    group[2][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i686::grp3(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    group[3][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i686::grp4(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    group[4][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i686::grp5(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    group[5][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i686::grp6(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[2] >> 3) & 0b111;
    group[6][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i686::grp7(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[2] >> 3) & 0b111;
    group[7][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i686::grp8(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[2] >> 3) & 0b111;
    group[8][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i686::grp9(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[2] >> 3) & 0b111;
    group[9][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i686::grp13(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[2] >> 3) & 0b111;
    group[13][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i686::grp14(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[2] >> 3) & 0b111;
    group[14][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i686::grp15(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[2] >> 3) & 0b111;
    group[15][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i686::grp16(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[2] >> 3) & 0b111;
    group[16][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i686::grp17(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[2] >> 3) & 0b111;
    group[17][nnn](format, opcode);
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x86_i686::CPUID(Format& format, const uint8_t* opcode)
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
