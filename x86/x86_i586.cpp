//==============================================================================
// Pentium(TM) Processor User's Manual, Volume 3
//
// 1993
//==============================================================================
#include "x86_i586.h"
#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"

//------------------------------------------------------------------------------
#define o x86_i586::
#define x , o
//------------------------------------------------------------------------------
// One-Byte Opcode Map
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_i586::one[256] =
{      // 0       1      2      3      4       5       6       7       8       9       A       B       C       D       E       F
/* 0 */ o ADD   x ADD  x ADD  x ADD  x ADD   x ADD   x _     x _     x OR    x OR    x OR    x OR    x OR    x OR    x _     x TWO
/* 1 */ x ADC   x ADC  x ADC  x ADC  x ADC   x ADC   x _     x _     x SBB   x SBB   x SBB   x SBB   x SBB   x SBB   x _     x _
/* 2 */ x AND   x AND  x AND  x AND  x AND   x AND   x ES    x _     x SUB   x SUB   x SUB   x SUB   x SUB   x SUB   x CS    x _
/* 3 */ x XOR   x XOR  x XOR  x XOR  x XOR   x XOR   x SS    x _     x CMP   x CMP   x CMP   x CMP   x CMP   x CMP   x DS    x _
/* 4 */ x INC   x INC  x INC  x INC  x INC   x INC   x INC   x INC   x DEC   x DEC   x DEC   x DEC   x DEC   x DEC   x DEC   x DEC
/* 5 */ x PUSH  x PUSH x PUSH x PUSH x PUSH  x PUSH  x PUSH  x PUSH  x POP   x POP   x POP   x POP   x POP   x POP   x POP   x POP
/* 6 */ x PUSHA x POPA x _    x _    x FS    x GS    x OSIZE x ASIZE x PUSH  x IMUL  x PUSH  x IMUL  x _     x _     x _     x _
/* 7 */ x Jcc   x Jcc  x Jcc  x Jcc  x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc
/* 8 */ x grp1  x grp1 x _    x grp1 x TEST  x TEST  x XCHG  x XCHG  x MOV   x MOV   x MOV   x MOV   x MOV   x LEA   x MOV   x POP
/* 9 */ x NOP   x XCHG x XCHG x XCHG x XCHG  x XCHG  x XCHG  x XCHG  x CBW   x CWD   x _     x WAIT  x PUSHF x POPF  x SAHF  x LAHF
/* A */ x MOV   x MOV  x MOV  x MOV  x MOVSx x MOVSx x CMPSx x CMPSx x TEST  x TEST  x STOSx x STOSx x LODSx x LODSx x SCASx x SCASx
/* B */ x MOV   x MOV  x MOV  x MOV  x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV
/* C */ x grp2  x grp2 x RET  x RET  x _     x _     x MOV   x MOV   x ENTER x LEAVE x _     x _     x _     x _     x _     x _
/* D */ x grp2  x grp2 x grp2 x grp2 x _     x _     x _     x XLAT  x ESC   x ESC   x ESC   x ESC   x ESC   x ESC   x ESC   x ESC
/* E */ x LOOP  x LOOP x LOOP x Jcc  x _     x _     x _     x _     x CALL  x JMP   x _     x JMP   x _     x _     x _     x _
/* F */ x _     x _    x REP  x REP  x _     x CMC   x grp3  x grp3  x CLC   x STC   x _     x _     x CLD   x STD   x grp4  x grp5
};
//------------------------------------------------------------------------------
// Two-Byte Opcode Map
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_i586::two[256] =
{      // 0           1           2           3          4         5         6         7          8           9           A           B          C         D         E       F
/* 0 */ o grp6      x grp7      x _         x _        x _       x _       x _       x _        x _         x _         x _         x _        x _       x _       x _     x _
/* 1 */ x _         x _         x _         x _        x _       x _       x _       x _        x _         x _         x _         x _        x _       x _       x _     x _
/* 2 */ x _         x _         x _         x _        x _       x _       x _       x _        x _         x _         x _         x _        x _       x _       x _     x _
/* 3 */ x _         x RDTSC     x _         x _        x _       x _       x _       x _        x _         x _         x _         x _        x _       x _       x _     x _
/* 4 */ x _         x _         x _         x _        x _       x _       x _       x _        x _         x _         x _         x _        x _       x _       x _     x _
/* 5 */ x _         x _         x _         x _        x _       x _       x _       x _        x _         x _         x _         x _        x _       x _       x _     x _
/* 6 */ x PUNPCKLBW x PUNPCKLWD x PUNPCKLDQ x PACKSSWB x PCMPGTB x PCMPGTW x PCMPGTD x PACKUSWB x PUNPCKHBW x PUNPCKHWD x PUNPCKHDQ x PACKSSDW x _       x _       x MOVD  x MOVQ
/* 7 */ x _         x grpA1     x grpA2     x grpA3    x PCMPEQB x PCMPEQW x PCMPEQD x EMMS     x _         x _         x _         x _        x _       x _       x MOVD  x MOVQ
/* 8 */ x Jcc       x Jcc       x Jcc       x Jcc      x Jcc     x Jcc     x Jcc     x Jcc      x Jcc       x Jcc       x Jcc       x Jcc      x Jcc     x Jcc     x Jcc   x Jcc
/* 9 */ x SETcc     x SETcc     x SETcc     x SETcc    x SETcc   x SETcc   x SETcc   x SETcc    x SETcc     x SETcc     x SETcc     x SETcc    x SETcc   x SETcc   x SETcc x SETcc
/* A */ x _         x _         x CPUID     x BT       x SHxD    x SHxD    x _       x _        x _         x _         x _         x BTS      x SHxD    x SHxD    x _     x IMUL
/* B */ x CMPXCHG   x CMPXCHG   x _         x BTR      x _       x _       x MOVZX   x MOVZX    x _         x _         x grp8      x BTC      x BSF     x BSR     x MOVSX x MOVSX
/* C */ x XADD      x XADD      x _         x _        x _       x _       x _       x grp9     x BSWAP     x BSWAP     x BSWAP     x BSWAP    x BSWAP   x BSWAP   x BSWAP x BSWAP
/* D */ x _         x PSRLW     x PSRLD     x PSRLQ    x _       x PMULLW  x _       x _        x PSUBUSB   x PSUBUSW   x _         x PAND     x PADDUSB x PADDUSW x _     x PANDN
/* E */ x _         x PSRAW     x PSRAD     x _        x _       x PMULHW  x _       x _        x PSUBSB    x PSUBSW    x _         x POR      x PADDSB  x PADDSW  x _     x PXOR
/* F */ x _         x PSLLW     x PSLLD     x PSLLQ    x _       x PMADDWD x _       x _        x PSUBB     x PSUBW     x PSUBD     x _        x PADDB   x PADDW   x PADDD x _
};
//------------------------------------------------------------------------------
// Opcodes determined by bits 5,4,3 of modR/M byte
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_i586::group[10][8] =
{        // 0      1           2      3     4     5      6      7
/* 0 */ { o _    x _         x _    x _   x _   x _    x _    x _    },
/* 1 */ { o ADD  x OR        x ADC  x SBB x AND x SUB  x XOR  x CMP  },
/* 2 */ { o Rxx  x Rxx       x Rxx  x Rxx x Sxx x Sxx  x _    x Sxx  },
/* 3 */ { o TEST x _         x NOT  x NEG x MUL x IMUL x DIV  x IDIV },
/* 4 */ { o INC  x DEC       x _    x _   x _   x _    x _    x _    },
/* 5 */ { o INC  x DEC       x CALL x _   x JMP x _    x PUSH x _    },
/* 6 */ { o _    x _         x _    x _   x _   x _    x _    x _    },
/* 7 */ { o _    x _         x _    x _   x _   x _    x _    x _    },
/* 8 */ { o _    x _         x _    x _   x BT  x BTS  x BTR  x BTC  },
/* 9 */ { o _    x CMPXCHG8B x _    x _   x _   x _    x _    x _    },
};
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_i586::groupA[4][8] =
{        // 0     1     2       3     4       5     6       7
/* 0 */ { o _   x _   x _     x _   x _     x _   x _     x _ },
/* 1 */ { o _   x _   x PSRLW x _   x PSRAW x _   x PSLLW x _ },
/* 2 */ { o _   x _   x PSRLD x _   x PSRAD x _   x PSLLD x _ },
/* 3 */ { o _   x _   x PSRLQ x _   x _     x _   x PSLLQ x _ },
};
//------------------------------------------------------------------------------
#undef o
#undef x
//------------------------------------------------------------------------------
void* x86_i586::Register(int type) const
{
    switch (type) {
    case 'x86 ':
        return (x86_register*)this;
    case 'x87 ':
        return (x87_register*)this;
    case 'mmx ':
        return (mmx_register*)this;
    }
    return nullptr;
}
//------------------------------------------------------------------------------
void x86_i586::StepImplement(x86_i386& x86, Format& format)
{
    format.width = 32;
    format.length = 1;
    format.address = 32;
    format.repeat = false;

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
void x86_i586::TWO(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    two[opcode[1]](format, opcode);
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x86_i586::grp1(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    group[1][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i586::grp2(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    group[2][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i586::grp3(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    group[3][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i586::grp4(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    group[4][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i586::grp5(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    group[5][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i586::grp6(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[2] >> 3) & 0b111;
    group[6][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i586::grp7(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[2] >> 3) & 0b111;
    group[7][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i586::grp8(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[2] >> 3) & 0b111;
    group[8][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i586::grp9(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[2] >> 3) & 0b111;
    group[9][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i586::grpA1(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[2] >> 3) & 0b111;
    groupA[1][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i586::grpA2(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[2] >> 3) & 0b111;
    groupA[2][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i586::grpA3(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[2] >> 3) & 0b111;
    groupA[3][nnn](format, opcode);
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x86_i586::CPUID(Format& format, const uint8_t* opcode)
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
            EAX |= (4 <<  4);   // Model
            EAX |= (5 <<  8);   // Family
            EDX |= (1 <<  0);   // FPU
            EDX |= (1 <<  4);   // TSC
            EDX |= (1 <<  8);   // CX8
            EDX |= (1 << 23);   // MMX
            break;
        }
    };
}
//------------------------------------------------------------------------------
