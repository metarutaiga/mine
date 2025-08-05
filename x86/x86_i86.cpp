//==============================================================================
// The 8086 Family Users Manual
// October 1979
//
// Intel Corporation 1978, 1979
//==============================================================================
#include "x86_i86.h"
#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"

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
/* E */ x LOOP   x LOOP  x LOOP x Jcc  x _     x _     x _     x _     x CALL  x JMP   x _     x JMP   x _      x _     x _     x _
/* F */ x _      x _     x REP  x REP  x _     x CMC   x grp3  x grp3  x CLC   x STC   x _     x _     x CLD    x STD   x grp4  x grp5
};
//------------------------------------------------------------------------------
// Opcodes determined by bits 5,4,3 of modR/M byte
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_i86::group[6][8] =
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
    memory_size = space;

    IP = 1024;
    SP = (uint16_t)space - 16;
    memcpy(memory + IP, program, size);
    stack = memory + SP;

    return true;
}
//------------------------------------------------------------------------------
bool x86_i86::Step()
{
    Format format;
    StepInternal(format);
    Fixup(format, *this);
    format.operation(*this, *this, format, format.operand[0].memory, format.operand[1].memory, format.operand[2].memory);
    if (IP >= memory_size) {
        exception(IP, memory, stack);
        IP = Pop();
    }
    return true;
}
//------------------------------------------------------------------------------
bool x86_i86::Run()
{
    while (IP) {
        if (Step() == false)
            return false;
    }
    return true;
}
//------------------------------------------------------------------------------
bool x86_i86::Jump(size_t address)
{
    if (address > memory_size)
        return false;
    IP = (uint16_t)address;
    return true;
}
//------------------------------------------------------------------------------
void x86_i86::Exception(void(*callback)(size_t, void*, void*))
{
    exception = callback;
}
//------------------------------------------------------------------------------
size_t x86_i86::Stack()
{
    return SP;
}
//------------------------------------------------------------------------------
uint8_t* x86_i86::Memory(size_t base, size_t size)
{
    if (base + size > memory_size)
        return nullptr;
    return memory + base;
}
//------------------------------------------------------------------------------
std::string x86_i86::Status()
{
    bool i87 = true;
    std::string output;

    char temp[32];
    for (int i = 0; i < 8; ++i) {
        snprintf(temp, 32, "%-8s%08X", REG16[i], regs[i].w);
        output += temp;

        if (i87) {
            output.insert(output.end(), 4, ' ');
            snprintf(temp, 32, "ST(%d)   %016llX", i, (uint64_t&)sts[(status._TOP + i) % 8].d);
            output += temp;
        }

        output += '\n';
    }

    output += '\n';
    snprintf(temp, 32, "%-8s%08X", "IP", ip.w);
    output += temp;

    if (i87) {
        output.insert(output.end(), 4, ' ');
        snprintf(temp, 32, "%-8s%04X", "CONTROL", control.w);
        output += temp;
    }

    output += '\n';
    snprintf(temp, 32, "%-8s%08X", "FLAGS", flags.w);
    output += temp;

    if (i87) {
        output.insert(output.end(), 4, ' ');
        snprintf(temp, 32, "%-8s%04X", "STATUS", status.w);
        output += temp;
    }

    return output;
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
        output += Disasm(format, *this);
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

    for (int i = 0; i < 8; ++i)
        regs[i] = backup.regs[i];
    flags = backup.flags;
    ip = backup.ip;

    return output;
}
//------------------------------------------------------------------------------
void x86_i86::StepInternal(Format& format)
{
    format.width = 16;
    format.repeat = false;

    for (;;) {
        opcode = memory + IP;
        one[opcode[0]](format, opcode);
        IP += format.length;
        if (format.operation)
            break;
    }
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x86_i86::ESC(Format& format, const uint8_t* opcode)
{
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x86_i86::grp1(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    group[1][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i86::grp2(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    group[2][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i86::grp3(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    group[3][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i86::grp4(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    group[4][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i86::grp5(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    group[5][nnn](format, opcode);
}
//------------------------------------------------------------------------------
