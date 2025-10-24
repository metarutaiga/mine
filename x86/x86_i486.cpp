//==============================================================================
// i486(TM) MICROPROCESSOR PROGRAMMER'S REFERENCE MANUAL
// (Order Number 240486)
//
// 1990
// (C) INTEL CORPORATION 1989
//==============================================================================
#include "x86_i486.h"
#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"

//------------------------------------------------------------------------------
#define X x86_i486::
#define x , X
//------------------------------------------------------------------------------
// One-Byte Opcode Map
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_i486::one[256] =
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
/* C */ x grp2   x grp2  x RET   x RET  x LES   x LDS   x MOV   x MOV   x ENTER x LEAVE x RETF  x RETF  x INT3   x INT   x INTO  x IRET
/* D */ x grp2   x grp2  x grp2  x grp2 x AAM   x AAD   x _     x XLAT  x ESC   x ESC   x ESC   x ESC   x ESC    x ESC   x ESC   x ESC
/* E */ x LOOP   x LOOP  x LOOP  x JCXZ x IN    x IN    x OUT   x OUT   x CALL  x JMP   x JMP   x JMP   x IN     x IN    x OUT   x OUT
/* F */ x LOCK   x _     x REPNE x REPE x HLT   x CMC   x grp3  x grp3  x CLC   x STC   x CLI   x STI   x CLD    x STD   x grp4  x grp5
};
//------------------------------------------------------------------------------
// Two-Byte Opcode Map
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_i486::two[256] =
{      // 0         1         2       3       4       5       6       7       8       9       A       B       C       D       E       F
/* 0 */ X grp6    x grp7    x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* 1 */ x _       x _       x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* 2 */ x _       x _       x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* 3 */ x _       x _       x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* 4 */ x _       x _       x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* 5 */ x _       x _       x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* 6 */ x _       x _       x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* 7 */ x _       x _       x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* 8 */ x Jcc     x Jcc     x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc
/* 9 */ x SETcc   x SETcc   x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc
/* A */ x _       x _       x _     x BT    x SHLD  x SHLD  x _     x _     x _     x _     x _     x BTS   x SHRD  x SHRD  x _     x IMUL
/* B */ x CMPXCHG x CMPXCHG x _     x BTR   x _     x _     x MOVZX x MOVZX x _     x _     x grp8  x BTC   x BSF   x BSR   x MOVSX x MOVSX
/* C */ x XADD    x XADD    x _     x _     x _     x _     x _     x _     x BSWAP x BSWAP x BSWAP x BSWAP x BSWAP x BSWAP x BSWAP x BSWAP
/* D */ x _       x _       x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* E */ x _       x _       x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* F */ x _       x _       x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
};
//------------------------------------------------------------------------------
#undef X
#undef x
//------------------------------------------------------------------------------
void x86_i486::StepImplement(x86_i386& x86, Format& format)
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
void x86_i486::TWO(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    two[opcode[1]](format, opcode);
}
//------------------------------------------------------------------------------
