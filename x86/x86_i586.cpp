//==============================================================================
// Pentium(TM) Processor User's Manual, Volume 3
// Architecture and Programming Manual
// (Order Number 241430)
//
// 1993
// (C) INTEL CORPORATION 1993
//==============================================================================
#include "x86_i586.h"
#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"
#include "mmx_instruction.h"

//------------------------------------------------------------------------------
#define X x86_i586::
#define x , X
#define M mmx_instruction::
#define m , M
//------------------------------------------------------------------------------
// One-Byte Opcode Map
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_i586::one[256] =
{      // 0        1       2       3      4       5       6       7       8       9       A       B       C        D       E        F
/* 0 */ X ADD    x ADD   x ADD   x ADD  x ADD   x ADD   x PUSH  x POP   x OR    x OR    x OR    x OR    x OR     x OR    x PUSH  x TWO
/* 1 */ x ADC    x ADC   x ADC   x ADC  x ADC   x ADC   x PUSH  x POP   x SBB   x SBB   x SBB   x SBB   x SBB    x SBB   x PUSH  x POP
/* 2 */ x AND    x AND   x AND   x AND  x AND   x AND   x ES    x DAA   x SUB   x SUB   x SUB   x SUB   x SUB    x SUB   x CS    x DAS
/* 3 */ x XOR    x XOR   x XOR   x XOR  x XOR   x XOR   x SS    x AAA   x CMP   x CMP   x CMP   x CMP   x CMP    x CMP   x DS    x AAS
/* 4 */ x INC    x INC   x INC   x INC  x INC   x INC   x INC   x INC   x DEC   x DEC   x DEC   x DEC   x DEC    x DEC   x DEC   x DEC
/* 5 */ x PUSH   x PUSH  x PUSH  x PUSH x PUSH  x PUSH  x PUSH  x PUSH  x POP   x POP   x POP   x POP   x POP    x POP   x POP   x POP
/* 6 */ x PUSHAD x POPAD x BOUND x ARPL x FS    x GS    x OSIZE x ASIZE x PUSH  x IMUL  x PUSH  x IMUL  x INS    x INS   x OUTS  x OUTS
/* 7 */ x Jcc    x Jcc   x Jcc   x Jcc  x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc    x Jcc   x Jcc   x Jcc
/* 8 */ x grp1   x grp1  x MOVB  x grp1 x TEST  x TEST  x XCHG  x XCHG  x MOV   x MOV   x MOV   x MOV   x MOV    x LEA   x MOV   x POP
/* 9 */ x NOP    x XCHG  x XCHG  x XCHG x XCHG  x XCHG  x XCHG  x XCHG  x CBW   x CWD   x CALLF x WAIT  x PUSHFD x POPFD x SAHF  x LAHF
/* A */ x MOV    x MOV   x MOV   x MOV  x MOVSx x MOVSx x CMPSx x CMPSx x TEST  x TEST  x STOSx x STOSx x LODSx  x LODSx x SCASx x SCASx
/* B */ x MOV    x MOV   x MOV   x MOV  x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV    x MOV   x MOV   x MOV
/* C */ x grp2   x grp2  x RET   x RET  x LES   x LDS   x MOV   x MOV   x ENTER x LEAVE x RETF  x RETF  x INT    x INT   x INT   x IRET
/* D */ x grp2   x grp2  x grp2  x grp2 x AAM   x AAD   x _     x XLAT  x ESC   x ESC   x ESC   x ESC   x ESC    x ESC   x ESC   x ESC
/* E */ x LOOP   x LOOP  x LOOP  x JCXZ x IN    x IN    x OUT   x OUT   x CALL  x JMP   x JMPF  x JMP   x IN     x IN    x OUT   x OUT
/* F */ x LOCK   x _     x REPNE x REPE x HLT   x CMC   x grp3  x grp3  x CLC   x STC   x CLI   x STI   x CLD    x STD   x grp4  x grp5
};
//------------------------------------------------------------------------------
// Two-Byte Opcode Map
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_i586::two[256] =
{      // 0           1           2           3          4         5         6         7          8           9           A           B          C         D         E       F
/* 0 */ X grp6      x grp7      x LAR       x LSL      x _       x _       x CLTS    x _        x INVD      x WBINVD    x _         x _        x _       x _       x _     x _
/* 1 */ x _         x _         x _         x _        x _       x _       x _       x _        x _         x _         x _         x _        x _       x _       x _     x _
/* 2 */ x MOV       x MOV       x MOV       x MOV      x _       x _       x _       x _        x _         x _         x _         x _        x _       x _       x _     x _
/* 3 */ x WRMSR     x RDTSC     x RDMSR     x _        x _       x _       x _       x _        x _         x _         x _         x _        x _       x _       x _     x _
/* 4 */ x _         x _         x _         x _        x _       x _       x _       x _        x _         x _         x _         x _        x _       x _       x _     x _
/* 5 */ x _         x _         x _         x _        x _       x _       x _       x _        x _         x _         x _         x _        x _       x _       x _     x _
/* 6 */ m PUNPCKLBW m PUNPCKLWD m PUNPCKLDQ m PACKSSWB m PCMPGTB m PCMPGTW m PCMPGTD m PACKUSWB m PUNPCKHBW m PUNPCKHWD m PUNPCKHDQ m PACKSSDW x _       x _       m MOVD  m MOVQ
/* 7 */ x _         x grpA1     x grpA2     x grpA3    m PCMPEQB m PCMPEQW m PCMPEQD m EMMS     x _         x _         x _         x _        x _       x _       m MOVD  m MOVQ
/* 8 */ x Jcc       x Jcc       x Jcc       x Jcc      x Jcc     x Jcc     x Jcc     x Jcc      x Jcc       x Jcc       x Jcc       x Jcc      x Jcc     x Jcc     x Jcc   x Jcc
/* 9 */ x SETcc     x SETcc     x SETcc     x SETcc    x SETcc   x SETcc   x SETcc   x SETcc    x SETcc     x SETcc     x SETcc     x SETcc    x SETcc   x SETcc   x SETcc x SETcc
/* A */ x PUSH      x POP       x CPUID     x BT       x SHLD    x SHLD    x _       x _        x PUSH      x POP       x RSM       x BTS      x SHRD    x SHRD    x _     x IMUL
/* B */ x CMPXCHG   x CMPXCHG   x LSS       x BTR      x LFS     x LGS     x MOVZX   x MOVZX    x _         x _         x grp8      x BTC      x BSF     x BSR     x MOVSX x MOVSX
/* C */ x XADD      x XADD      x _         x _        x _       x _       x _       x grp9     x BSWAP     x BSWAP     x BSWAP     x BSWAP    x BSWAP   x BSWAP   x BSWAP x BSWAP
/* D */ x _         m PSRLW     m PSRLD     m PSRLQ    x _       m PMULLW  x _       x _        m PSUBUSB   m PSUBUSW   x _         m PAND     m PADDUSB m PADDUSW x _     m PANDN
/* E */ x _         m PSRAW     m PSRAD     x _        x _       m PMULHW  x _       x _        m PSUBSB    m PSUBSW    x _         m POR      m PADDSB  m PADDSW  x _     m PXOR
/* F */ x _         m PSLLW     m PSLLD     m PSLLQ    x _       m PMADDWD x _       x _        m PSUBB     m PSUBW     m PSUBD     x _        m PADDB   m PADDW   m PADDD x _
};
//------------------------------------------------------------------------------
// Opcodes determined by bits 5,4,3 of modR/M byte
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_i586::group[10][8] =
{        // 0      1           2      3       4      5      6      7
/* 0 */ { X _    x _         x _    x _     x _    x _    x _    x _      },
/* 1 */ { X ADD  x OR        x ADC  x SBB   x AND  x SUB  x XOR  x CMP    },
/* 2 */ { X ROL  x ROR       x RCL  x RCR   x SHL  x SHR  x _    x SAR    },
/* 3 */ { X TEST x _         x NOT  x NEG   x MUL  x IMUL x DIV  x IDIV   },
/* 4 */ { X INC  x DEC       x _    x _     x _    x _    x _    x _      },
/* 5 */ { X INC  x DEC       x CALL x CALLF x JMP  x JMPF x PUSH x _      },
/* 6 */ { X SLDT x STR       x LLDT x LTR   x VERR x VERW x _    x _      },
/* 7 */ { X SGDT x SIDT      x LGDT x LIDT  x SMSW x _    x LMSW x INVLPG },
/* 8 */ { X _    x _         x _    x _     x BT   x BTS  x BTR  x BTC    },
/* 9 */ { X _    x CMPXCHG8B x _    x _     x _    x _    x _    x _      },
};
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_i586::groupA[4][8] =
{        // 0     1     2       3     4       5     6       7
/* 0 */ { X _   x _   x _     x _   x _     x _   x _     x _ },
/* 1 */ { X _   x _   m PSRLW x _   m PSRAW x _   m PSLLW x _ },
/* 2 */ { X _   x _   m PSRLD x _   m PSRAD x _   m PSLLD x _ },
/* 3 */ { X _   x _   m PSRLQ x _   x _     x _   m PSLLQ x _ },
};
//------------------------------------------------------------------------------
#undef X
#undef x
#undef M
#undef m
//------------------------------------------------------------------------------
const void* x86_i586::Register(int type) const
{
    switch (type) {
    case __builtin_bswap32('ix86'):
        return &x86;
    case __builtin_bswap32('ix87'):
        return &x87;
    case __builtin_bswap32('immx'):
        return &mmx;
    }
    return nullptr;
}
//------------------------------------------------------------------------------
void x86_i586::StepImplement(x86_i386& x86, Format& format)
{
    format.width = 32;
    format.length = 1;
    format.address = 32;
    format.operand_count = 2;

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
    format.length = 2;

    int nnn = (opcode[1] >> 3) & 0b111;
    group[1][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i586::grp2(Format& format, const uint8_t* opcode)
{
    format.length = 2;

    int nnn = (opcode[1] >> 3) & 0b111;
    group[2][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i586::grp3(Format& format, const uint8_t* opcode)
{
    format.length = 2;

    int nnn = (opcode[1] >> 3) & 0b111;
    group[3][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i586::grp4(Format& format, const uint8_t* opcode)
{
    format.length = 2;

    int nnn = (opcode[1] >> 3) & 0b111;
    group[4][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i586::grp5(Format& format, const uint8_t* opcode)
{
    format.length = 2;

    int nnn = (opcode[1] >> 3) & 0b111;
    group[5][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i586::grp6(Format& format, const uint8_t* opcode)
{
    format.length = 3;

    int nnn = (opcode[2] >> 3) & 0b111;
    group[6][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i586::grp7(Format& format, const uint8_t* opcode)
{
    format.length = 3;

    int nnn = (opcode[2] >> 3) & 0b111;
    group[7][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i586::grp8(Format& format, const uint8_t* opcode)
{
    format.length = 3;

    int nnn = (opcode[2] >> 3) & 0b111;
    group[8][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i586::grp9(Format& format, const uint8_t* opcode)
{
    format.length = 3;

    int nnn = (opcode[2] >> 3) & 0b111;
    group[9][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i586::grpA1(Format& format, const uint8_t* opcode)
{
    format.length = 3;

    int nnn = (opcode[2] >> 3) & 0b111;
    groupA[1][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i586::grpA2(Format& format, const uint8_t* opcode)
{
    format.length = 3;

    int nnn = (opcode[2] >> 3) & 0b111;
    groupA[2][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i586::grpA3(Format& format, const uint8_t* opcode)
{
    format.length = 3;

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
