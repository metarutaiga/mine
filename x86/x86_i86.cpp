//==============================================================================
// The 8086 Family Users Manual
// (Order Number 9800722)
//
// October 1979
// (C) Intel Corporation 1978, 1979
//==============================================================================
#include <stdarg.h>
#include "x86_i86.h"
#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"
#include "../syscall/allocator.h"

//------------------------------------------------------------------------------
#define o x86_i86::
#define x , o
//------------------------------------------------------------------------------
// One-Byte Opcode Map
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_i86::one[256] =
{      // 0       1      2       3      4       5       6       7       8       9       A       B       C       D       E       F
/* 0 */ o ADD   x ADD  x ADD   x ADD  x ADD   x ADD   x PUSH  x POP   x OR    x OR    x OR    x OR    x OR    x OR    x PUSH  x _
/* 1 */ x ADC   x ADC  x ADC   x ADC  x ADC   x ADC   x PUSH  x POP   x SBB   x SBB   x SBB   x SBB   x SBB   x SBB   x PUSH  x POP
/* 2 */ x AND   x AND  x AND   x AND  x AND   x AND   x ES    x _     x SUB   x SUB   x SUB   x SUB   x SUB   x SUB   x CS    x _
/* 3 */ x XOR   x XOR  x XOR   x XOR  x XOR   x XOR   x SS    x _     x CMP   x CMP   x CMP   x CMP   x CMP   x CMP   x DS    x _
/* 4 */ x INC   x INC  x INC   x INC  x INC   x INC   x INC   x INC   x DEC   x DEC   x DEC   x DEC   x DEC   x DEC   x DEC   x DEC
/* 5 */ x PUSH  x PUSH x PUSH  x PUSH x PUSH  x PUSH  x PUSH  x PUSH  x POP   x POP   x POP   x POP   x POP   x POP   x POP   x POP
/* 6 */ x _     x _    x _     x _    x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* 7 */ x Jcc   x Jcc  x Jcc   x Jcc  x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc
/* 8 */ x grp1  x grp1 x _     x grp1 x TEST  x TEST  x XCHG  x XCHG  x MOV   x MOV   x MOV   x MOV   x MOV   x LEA   x _     x POP
/* 9 */ x XCHG  x XCHG x XCHG  x XCHG x XCHG  x XCHG  x XCHG  x XCHG  x CBW   x CWD   x _     x _     x PUSHF x POPF  x SAHF  x LAHF
/* A */ x MOV   x MOV  x MOV   x MOV  x MOVSx x MOVSx x CMPSx x CMPSx x TEST  x TEST  x STOSx x STOSx x LODSx x LODSx x SCASx x SCASx
/* B */ x MOV   x MOV  x MOV   x MOV  x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV
/* C */ x _     x _    x RET   x RET  x _     x _     x MOV   x MOV   x _     x _     x _     x _     x _     x _     x _     x _
/* D */ x grp2  x grp2 x grp2  x grp2 x _     x _     x _     x XLAT  x ESC   x ESC   x ESC   x ESC   x ESC   x ESC   x ESC   x ESC
/* E */ x LOOP  x LOOP x LOOP  x Jcc  x _     x _     x _     x _     x CALL  x JMP   x _     x JMP   x _     x _     x _     x _
/* F */ x _     x _    x REPNE x REPE x _     x CMC   x grp3  x grp3  x CLC   x STC   x _     x _     x CLD   x STD   x grp4  x grp5
};
//------------------------------------------------------------------------------
// Opcodes determined by bits 5,4,3 of modR/M byte
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_i86::group[6][8] =
{        // 0      1     2      3     4     5      6      7
/* 0 */ { o _    x _   x _    x _   x _   x _    x _    x _    },
/* 1 */ { o ADD  x OR  x ADC  x SBB x AND x SUB  x XOR  x CMP  },
/* 2 */ { o ROL  x ROR x RCL  x RCR x SHL x SHR  x _    x SAR  },
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
    delete allocator;
}
//------------------------------------------------------------------------------
bool x86_i86::Initialize(allocator_t* allocator, size_t stack)
{
    if (allocator == nullptr)
        return false;
    this->allocator = allocator;

    memory_size = allocator->max_size();
    stack_size = stack;
    memory_address = (uint8_t*)allocator->allocate(256, 0);
    stack_address = (uint8_t*)allocator->allocate(stack, memory_size - stack);

    IP = 0;
    SP = (uint32_t)memory_size - 16;
    FLAGS = 0b0000001000000010;

    return true;
}
//------------------------------------------------------------------------------
bool x86_i86::Step(int count)
{
    auto& mmx = *(mmx_register*)Register('mmx ');
    auto& sse = *(sse_register*)Register('sse ');

    auto ip_over = IP;
    auto ip = IP;
    auto sp = SP;
    while (IP) {
        timestamp_counter++;

        Format format;
        StepInternal(format);
        Fixup(format, x86, x87, mmx, sse);
        if (format.operation == nullptr)
            return false;
        format.operation(x86, x87, mmx, sse, format, format.operand[0].memory, format.operand[1].memory, format.operand[2].memory);
        if (IP >= memory_size) {
            auto count = (uint16_t)Exception(this, IP);
            IP = Pop16();
            SP += count;
        }
        if (IP == 0) {
            IP = ip;
            return false;
        }
        if (BreakpointDataAddress || BreakpointProgram) {
            if (BreakpointDataAddress && BreakpointDataAddress < memory_size) {
                if (memcmp(memory_address + BreakpointDataAddress, &BreakpointDataValue, sizeof(uint16_t)) == 0)
                    return false;
            }
            if (BreakpointProgram == IP)
                return false;
        }
        switch (count) {
        case 'INTO':
            return true;
        case 'OVER':
            if (SP >= sp || IP - ip_over < 16)
                return true;
            break;
        case 'OUT ':
            if (strcmp(format.instruction, "RET") == 0)
                return true;
            break;
        default:
            if (count == 0)
                return true;
            count--;
            break;
        }
        ip = IP;
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
const void* x86_i86::Register(int type) const
{
    switch (type) {
    case 'x86 ':
        return &x86;
    case 'x87 ':
        return &x87;
    }
    return nullptr;
}
//------------------------------------------------------------------------------
uint8_t* x86_i86::Memory(size_t base, size_t size) const
{
    if (size == 0) {
        if (base + size >= memory_size)
            return nullptr;
        return memory_address + base;
    }
    return (uint8_t*)allocator->allocate(size, base);
}
//------------------------------------------------------------------------------
size_t x86_i86::Stack() const
{
    return SP;
}
//------------------------------------------------------------------------------
size_t x86_i86::Program() const
{
    return IP;
}
//------------------------------------------------------------------------------
std::string x86_i86::Status() const
{
    std::string output;

    char temp[32];
    auto push_first_line = [&](const char* format, ...) {
        va_list va;
        va_start(va, format);
        vsnprintf(temp, 32, format, va);
        va_end(va);
        output += temp;
        output += '\n';
    };

    size_t current = 0;
    auto push_second_line = [&](const char* format, ...) {
        va_list va;
        va_start(va, format);
        vsnprintf(temp, 32, format, va);
        va_end(va);
        size_t now = output.size();
        if (current != std::string::npos) {
            now = output.find('\n', current);
            if (now == std::string::npos)
                now = output.size();
        }
        size_t len = strlen(temp);
        if (len == 0) {
            now = now + 1;
            return;
        }
        output.insert(now, 4, ' ');
        output.insert(now + 4, temp);
        current = now + 4 + len + 1;
    };

    // CPU
    for (int i = 0; i < 8; ++i) {
        push_first_line("%-8s%04X", REG16[i], regs[i].w);
    }
    push_first_line("");
    push_first_line("%-8s%04X", "IP", ip.w);
    push_first_line("%-8s%04X", "FLAGS", flags.w);

    for (int i = 0; i < 16; ++i) {
        static const char* name = "1N11ODITSZ1A1P1C";
        temp[i] = (flags.w & (1 << (15 - i))) ? name[i] : '.';
    }
    push_first_line("%.16s", temp);

    // FPU
    for (int i = 0; i < 8; ++i) {
        push_second_line("ST(%d)   %016llX", i, (uint64_t&)x87.sts[(x87.status._TOP + i) % 8].d);
    }
    push_second_line("");
    push_second_line("%-8s%04X", "CONTROL", x87.control.w);
    push_second_line("%-8s%04X", "STATUS", x87.status.w);

    return output;
}
//------------------------------------------------------------------------------
std::string x86_i86::Disassemble(int count) const
{
    std::string output;

    x86_i86 x86;
    for (int i = 0; i < 8; ++i)
        x86.regs[i] = regs[i];
    x86.flags = flags;
    x86.ip = ip;
    x86.memory_size = memory_size;
    x86.memory_address = memory_address;

    auto& x87 = x86.x87;
    auto& mmx = *(mmx_register*)x86.Register('mmx ');
    auto& sse = *(sse_register*)x86.Register('sse ');

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
        x86.StepInternal(format);
        output += Disasm(format, x86, x87, mmx, sse);
        output += '\n';

        for (uint32_t i = 0; i < 16; ++i) {
            if (address + i >= IP) {
                output.insert(insert, 2, ' ');
                continue;
            }
            snprintf(temp, 64, "%02X", memory_address[address + i]);
            output.insert(insert, temp);
            insert += 2;
        }
    }

    return output;
}
//------------------------------------------------------------------------------
void x86_i86::StepInternal(Format& format)
{
    format.width = 16;
    format.length = 1;
    format.address = 16;
    format.operand_count = 2;

    auto ip = IP;
    for (;;) {
        opcode = memory_address + IP;
        one[opcode[0]](format, opcode);
        IP += format.length;
        if (format.operation)
            break;
        if (IP - ip >= 15) {
            if (format.instruction[0] == 0)
                format.instruction = "UNKNOWN";
            break;
        }
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
    format.length = 2;

    int nnn = (opcode[1] >> 3) & 0b111;
    group[1][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i86::grp2(Format& format, const uint8_t* opcode)
{
    format.length = 2;

    int nnn = (opcode[1] >> 3) & 0b111;
    group[2][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i86::grp3(Format& format, const uint8_t* opcode)
{
    format.length = 2;

    int nnn = (opcode[1] >> 3) & 0b111;
    group[3][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i86::grp4(Format& format, const uint8_t* opcode)
{
    format.length = 2;

    int nnn = (opcode[1] >> 3) & 0b111;
    group[4][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i86::grp5(Format& format, const uint8_t* opcode)
{
    format.length = 2;

    int nnn = (opcode[1] >> 3) & 0b111;
    group[5][nnn](format, opcode);
}
//------------------------------------------------------------------------------
