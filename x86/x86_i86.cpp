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
/* 0 */ o ADD    x ADD   x ADD  x ADD  x ADD   x ADD   x PUSH  x POP   x OR    x OR    x OR    x OR    x OR     x OR    x PUSH  x _
/* 1 */ x ADC    x ADC   x ADC  x ADC  x ADC   x ADC   x PUSH  x POP   x SBB   x SBB   x SBB   x SBB   x SBB    x SBB   x PUSH  x POP
/* 2 */ x AND    x AND   x AND  x AND  x AND   x AND   x ES    x _     x SUB   x SUB   x SUB   x SUB   x SUB    x SUB   x CS    x _
/* 3 */ x XOR    x XOR   x XOR  x XOR  x XOR   x XOR   x SS    x _     x CMP   x CMP   x CMP   x CMP   x CMP    x CMP   x DS    x _
/* 4 */ x INC    x INC   x INC  x INC  x INC   x INC   x INC   x INC   x DEC   x DEC   x DEC   x DEC   x DEC    x DEC   x DEC   x DEC
/* 5 */ x PUSH   x PUSH  x PUSH x PUSH x PUSH  x PUSH  x PUSH  x PUSH  x POP   x POP   x POP   x POP   x POP    x POP   x POP   x POP
/* 6 */ x _      x _     x _    x _    x _     x _     x _     x _     x _     x _     x _     x _     x _      x _     x _     x _
/* 7 */ x Jcc    x Jcc   x Jcc  x Jcc  x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc    x Jcc   x Jcc   x Jcc
/* 8 */ x grp1   x grp1  x _    x grp1 x TEST  x TEST  x XCHG  x XCHG  x MOV   x MOV   x MOV   x MOV   x MOV    x LEA   x MOV   x POP
/* 9 */ x XCHG   x XCHG  x XCHG x XCHG x XCHG  x XCHG  x XCHG  x XCHG  x CWDE  x CDQ   x _     x _     x PUSHFD x POPFD x SAHF  x LAHF
/* A */ x _      x _     x _    x _    x MOVSx x MOVSx x CMPSx x CMPSx x TEST  x TEST  x STOSx x STOSx x LODSx  x LODSx x SCASx x SCASx
/* B */ x MOV    x MOV   x MOV  x MOV  x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV    x MOV   x MOV   x MOV
/* C */ x _      x _     x RET  x RET  x _     x _     x MOV   x MOV   x _     x _     x _     x _     x _      x _     x _     x _
/* D */ x grp2   x grp2  x grp2 x grp2 x _     x _     x _     x XLAT  x ESC   x ESC   x ESC   x ESC   x ESC    x ESC   x ESC   x ESC
/* E */ x LOOP   x LOOP  x LOOP x Jcc  x _     x _     x _     x _     x CALL  x Jcc   x _     x Jcc   x _      x _     x _     x _
/* F */ x _      x _     x REP  x REP  x _     x CMC   x grp3  x grp3  x CLC   x STC   x _     x _     x CLD    x STD   x grp4  x grp5
};
//------------------------------------------------------------------------------
// Opcodes determined by bits 5,4,3 of modR/M byte
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_i86::group[16][8] =
{        // 0      1     2      3     4     5      6      7
/* 0 */ { o _    x _   x _    x _   x _   x _    x _    x _    },
/* 1 */ { o ADD  x OR  x ADC  x SBB x AND x SUB  x XOR  x CMP  },
/* 2 */ { o Rxx  x Rxx x Rxx  x Rxx x Sxx x Sxx  x _    x Sxx  },
/* 3 */ { o TEST x _   x NOT  x NEG x MUL x IMUL x DIV  x IDIV },
/* 4 */ { o INC  x DEC x _    x _   x _   x _    x _    x _    },
/* 5 */ { o INC  x DEC x CALL x _   x JMP x _    x PUSH x _    },
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
    memory_size = space;

    IP = 1024;
    memcpy(memory + IP, program, size);

    return true;
}
//------------------------------------------------------------------------------
bool x86_i86::Step()
{
    Format format;
    StepInternal(format);
    Fixup(format);
    format.operation(*this, format, format.operand[0].memory, format.operand[1].memory);
    return true;
}
//------------------------------------------------------------------------------
bool x86_i86::Jump(size_t address)
{
    if (address >= memory_size)
        return false;
    IP = (uint16_t)address;
    return true;
}
//------------------------------------------------------------------------------
uint8_t* x86_i86::Memory(size_t base, size_t size)
{
    if (base + size >= memory_size)
        return nullptr;
    return memory + base;
}
//------------------------------------------------------------------------------
std::string x86_i86::Disassemble(int count)
{
    std::string output;

    x86_i86 backup;
    for (int i = 0; i < 8; ++i)
        backup.regs[i] = regs[i];
    backup.flags = flags;
    backup.ip = ip;

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

        Format format;
        StepInternal(format);
        output += Disasm(format);
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
    ip = backup.ip;

    return output;
}
//------------------------------------------------------------------------------
void x86_i86::StepInternal(Format& format)
{
    operand_size = 16;
    repeat_string_operation = false;

    for (;;) {
        opcode = memory + IP;
        (this->*one[opcode[0]])(format);
        IP += format.length;

        switch (opcode[0]) {
        case 0x26:
        case 0x2E:
        case 0x36:
        case 0x3E:
        case 0xF0:
        case 0xF2:
        case 0xF3:
            format = Format();
            continue;
            break;
        default:
            operand_size = 16;
            repeat_string_operation = false;
            break;
        }
        break;
    }
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x86_i86::ESC(Format& format)
{
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x86_i86::grp1(Format& format)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    (this->*group[1][nnn])(format);
}
//------------------------------------------------------------------------------
void x86_i86::grp2(Format& format)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    (this->*group[2][nnn])(format);
}
//------------------------------------------------------------------------------
void x86_i86::grp3(Format& format)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    (this->*group[3][nnn])(format);
}
//------------------------------------------------------------------------------
void x86_i86::grp4(Format& format)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    (this->*group[4][nnn])(format);
}
//------------------------------------------------------------------------------
void x86_i86::grp5(Format& format)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    (this->*group[5][nnn])(format);
}
//------------------------------------------------------------------------------
