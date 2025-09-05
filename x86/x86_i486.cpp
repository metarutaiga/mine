//==============================================================================
// i486(TM) MICROPROCESSOR PROGRAMMER'S REFERENCE MANUAL
//
// 1990
//==============================================================================
#include "x86_i486.h"
#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"

//------------------------------------------------------------------------------
#define o x86_i486::
#define x , o
//------------------------------------------------------------------------------
// One-Byte Opcode Map
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_i486::one[256] =
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
const x86_instruction::instruction_pointer x86_i486::two[256] =
{      // 0         1         2       3       4       5       6       7       8       9       A       B       C       D       E       F
/* 0 */ o grp6    x grp7    x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* 1 */ x _       x _       x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* 2 */ x _       x _       x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* 3 */ x _       x _       x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* 4 */ x _       x _       x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* 5 */ x _       x _       x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* 6 */ x _       x _       x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* 7 */ x _       x _       x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* 8 */ x Jcc     x Jcc     x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc
/* 9 */ x SETcc   x SETcc   x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc
/* A */ x _       x _       x _     x BT    x SHxD  x SHxD  x _     x _     x _     x _     x _     x BTS   x SHxD  x SHxD  x _     x IMUL
/* B */ x CMPXCHG x CMPXCHG x _     x BTR   x _     x _     x MOVZX x MOVZX x _     x _     x grp8  x BTC   x BSF   x BSR   x MOVSX x MOVSX
/* C */ x XADD    x XADD    x _     x _     x _     x _     x _     x _     x BSWAP x BSWAP x BSWAP x BSWAP x BSWAP x BSWAP x BSWAP x BSWAP
/* D */ x _       x _       x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* E */ x _       x _       x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* F */ x _       x _       x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
};
//------------------------------------------------------------------------------
#undef o
#undef x
//------------------------------------------------------------------------------
void x86_i486::StepInternal(x86_i386& x86, Format& format) const
{
    format.type = Format::X86;
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
void x86_i486::TWO(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    two[opcode[1]](format, opcode);
}
//------------------------------------------------------------------------------
