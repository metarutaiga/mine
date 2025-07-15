//==============================================================================
// The 8086 Family Users Manual
// October 1979
//
// Intel Corporation 1978, 1979
//==============================================================================
#include "x86_i86.h"
#include "x86_register.h"
#include "x86_register.inl"

//------------------------------------------------------------------------------
#define o (x86_instruction::instruction_pointer)&x86_i86::
#define x , o
//------------------------------------------------------------------------------
// One-Byte Opcode Map
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_i86::one[256] =
{      // 0        1       2      3      4       5       6       7       8       9       A       B       C        D       E       F
/* 0 */ o ADD    x ADD   x ADD  x ADD  x ADD   x ADD   x PUSH  x POP   x OR    x OR    x OR    x OR    x OR     x OR    x PUSH  x ___
/* 1 */ x ADC    x ADC   x ADC  x ADC  x ADC   x ADC   x PUSH  x POP   x SBB   x SBB   x SBB   x SBB   x SBB    x SBB   x PUSH  x POP
/* 2 */ x AND    x AND   x AND  x AND  x AND   x AND   x ES    x ___   x SUB   x SUB   x SUB   x SUB   x SUB    x SUB   x CS    x ___
/* 3 */ x XOR    x XOR   x XOR  x XOR  x XOR   x XOR   x SS    x ___   x CMP   x CMP   x CMP   x CMP   x CMP    x CMP   x DS    x ___
/* 4 */ x INC    x INC   x INC  x INC  x INC   x INC   x INC   x INC   x DEC   x DEC   x DEC   x DEC   x DEC    x DEC   x DEC   x DEC
/* 5 */ x PUSH   x PUSH  x PUSH x PUSH x PUSH  x PUSH  x PUSH  x PUSH  x POP   x POP   x POP   x POP   x POP    x POP   x POP   x POP
/* 6 */ x ___    x ___   x ___  x ___  x ___   x ___   x ___   x ___   x ___   x ___   x ___   x ___   x ___    x ___   x ___   x ___
/* 7 */ x Jcc    x Jcc   x Jcc  x Jcc  x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc    x Jcc   x Jcc   x Jcc
/* 8 */ x grp1   x grp1  x ___  x grp1 x TEST  x TEST  x XCHG  x XCHG  x MOV   x MOV   x MOV   x MOV   x MOV    x LEA   x MOV   x POP
/* 9 */ x XCHG   x XCHG  x XCHG x XCHG x XCHG  x XCHG  x XCHG  x XCHG  x CWDE  x CDQ   x ___   x ___   x PUSHFD x POPFD x SAHF  x LAHF
/* A */ x ___    x ___   x ___  x ___  x MOVSx x MOVSx x CMPSx x CMPSx x TEST  x TEST  x STOSx x STOSx x LODSx  x LODSx x SCASx x SCASx
/* B */ x MOV    x MOV   x MOV  x MOV  x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV    x MOV   x MOV   x MOV
/* C */ x ___    x ___   x RET  x RET  x ___   x ___   x MOV   x MOV   x ___   x ___   x ___   x ___   x ___    x ___   x ___   x ___
/* D */ x grp2   x grp2  x grp2 x grp2 x ___   x ___   x ___   x XLAT  x ESC   x ESC   x ESC   x ESC   x ESC    x ESC   x ESC   x ESC
/* E */ x LOOP   x LOOP  x LOOP x Jcc  x ___   x ___   x ___   x ___   x CALL  x Jcc   x ___   x Jcc   x ___    x ___   x ___   x ___
/* F */ x ___    x ___   x REP  x REP  x ___   x CMC   x grp3  x grp3  x CLC   x STC   x ___   x ___   x CLD    x STD   x grp4  x grp5
};
//------------------------------------------------------------------------------
// Opcodes determined by bits 5,4,3 of modR/M byte
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_i86::group[16][8] =
{        // 0      1     2      3     4     5      6      7
/* 0 */ { o ___  x ___ x ___  x ___ x ___ x ___  x ___  x ___  },
/* 1 */ { o ADD  x OR  x ADC  x SBB x AND x SUB  x XOR  x CMP  },
/* 2 */ { o Rxx  x Rxx x Rxx  x Rxx x Sxx x Sxx  x ___  x Sxx  },
/* 3 */ { o TEST x ___ x NOT  x NEG x MUL x IMUL x DIV  x IDIV },
/* 4 */ { o INC  x DEC x ___  x ___ x ___ x ___  x ___  x ___  },
/* 5 */ { o INC  x DEC x CALL x ___ x JMP x ___  x PUSH x ___  },
};
//------------------------------------------------------------------------------
#undef o
#undef x
//------------------------------------------------------------------------------
x86_i86::~x86_i86()
{
#if defined(_UCRT)
    _aligned_free(memory);
#else
    free(memory);
#endif
}
//------------------------------------------------------------------------------
bool x86_i86::Initialize(size_t space, const void* program, size_t size)
{
    if (space & (1024 - 1)) {
        printf("space %zd is not aligned 1024\n", space);
        return false;
    }
    if (space < (1024 + size + 65536)) {
        printf("space %zd is less than %zd\n", space, 1024 + size + 65536);
        return false;
    }
#if defined(_UCRT)
    memory = (uint8_t*)_aligned_malloc(1024, space);
#else
    memory = (uint8_t*)aligned_alloc(1024, space);
#endif
    if (memory == nullptr) {
        printf("memory allocation is failed\n");
        return false;
    }
    memset(memory, 0, space);
    stack = memory + space - 16;

    IP = 1024;
    memcpy(memory + IP, program, size);

    return true;
}
//------------------------------------------------------------------------------
bool x86_i86::Step()
{
    Format format = StepInternal();
    format.operation(format, format.operand[0].memory, format.operand[1].memory);
    return true;
}
//------------------------------------------------------------------------------
std::string x86_i86::Disassemble(int count)
{
    std::string output;

    x86_i86 backup;
    for (int i = 0; i < 8; ++i)
        backup.regs[i] = regs[i];
    backup.flags = flags;
    backup.IP = IP;

    disasm = &output;

    for (int i = 0; i < count; ++i) {
        char temp[64];

        uint32_t address = IP;
        snprintf(temp, 64, "%08X", address);
        output += temp;
        output += ' ';
        output += ':';
        output += ' ';
        size_t insert = output.size();

        Format format = StepInternal();
        output += format.disassembly;
        output += '\n';

        for (uint32_t i = 0; i < 16; ++i) {
            if (address + i >= IP) {
                output.insert(insert, 2, ' ');
                continue;;
            }
            snprintf(temp, 64, "%02X", memory[address + i]);
            output.insert(insert, temp);
            insert += 2;
        }
    }

    disasm = nullptr;

    for (int i = 0; i < 8; ++i)
        regs[i] = backup.regs[i];
    flags = backup.flags;
    IP = backup.IP;

    return output;
}
//------------------------------------------------------------------------------
x86_format::Format x86_i86::StepInternal()
{
    Format format;

    operand_size = 16;
    repeat_string_operation = false;

    for (;;) {
        opcode = memory + IP;
        format = (this->*one[opcode[0]])();

        switch (opcode[0]) {
        case 0x26:
        case 0x2E:
        case 0x36:
        case 0x3E:
        case 0xF0:
        case 0xF2:
        case 0xF3:
            continue;
            break;
        default:
            operand_size = 16;
            repeat_string_operation = false;
            break;
        }
        break;
    }

    return format;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
x86_format::Format x86_i86::ESC()
{
    return Format();
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
x86_format::Format x86_i86::grp1()
{
    int nnn = (opcode[1] >> 3) & 0b111;
    return (this->*group[1][nnn])();
}
//------------------------------------------------------------------------------
x86_format::Format x86_i86::grp2()
{
    int nnn = (opcode[1] >> 3) & 0b111;
    return (this->*group[2][nnn])();
}
//------------------------------------------------------------------------------
x86_format::Format x86_i86::grp3()
{
    int nnn = (opcode[1] >> 3) & 0b111;
    return (this->*group[3][nnn])();
}
//------------------------------------------------------------------------------
x86_format::Format x86_i86::grp4()
{
    int nnn = (opcode[1] >> 3) & 0b111;
    return (this->*group[4][nnn])();
}
//------------------------------------------------------------------------------
x86_format::Format x86_i86::grp5()
{
    int nnn = (opcode[1] >> 3) & 0b111;
    return (this->*group[5][nnn])();
}
//------------------------------------------------------------------------------
