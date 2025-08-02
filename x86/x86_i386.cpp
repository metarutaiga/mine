//==============================================================================
// 80386 Programmer's Reference Manual
// 1986
//
// INTEL CORPORATION 1987
//==============================================================================
#include "x86_i386.h"
#include "x86_register.h"
#include "x86_register.inl"

//------------------------------------------------------------------------------
#define o (x86_instruction::instruction_pointer)&x86_i386::
#define x , o
//------------------------------------------------------------------------------
// One-Byte Opcode Map
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_i386::one[256] =
{      // 0        1       2      3      4       5       6       7       8       9       A       B       C        D       E       F
/* 0 */ o ADD    x ADD   x ADD  x ADD  x ADD   x ADD   x _     x _     x OR    x OR    x OR    x OR    x OR     x OR    x _     x TWO
/* 1 */ x ADC    x ADC   x ADC  x ADC  x ADC   x ADC   x _     x _     x SBB   x SBB   x SBB   x SBB   x SBB    x SBB   x _     x _
/* 2 */ x AND    x AND   x AND  x AND  x AND   x AND   x _     x _     x SUB   x SUB   x SUB   x SUB   x SUB    x SUB   x _     x _
/* 3 */ x XOR    x XOR   x XOR  x XOR  x XOR   x XOR   x _     x _     x CMP   x CMP   x CMP   x CMP   x CMP    x CMP   x _     x _
/* 4 */ x INC    x INC   x INC  x INC  x INC   x INC   x INC   x INC   x DEC   x DEC   x DEC   x DEC   x DEC    x DEC   x DEC   x DEC
/* 5 */ x PUSH   x PUSH  x PUSH x PUSH x PUSH  x PUSH  x PUSH  x PUSH  x POP   x POP   x POP   x POP   x POP    x POP   x POP   x POP
/* 6 */ x PUSHAD x POPAD x _    x _    x _     x _     x OSIZE x _     x PUSH  x IMUL  x PUSH  x IMUL  x _      x _     x _     x _
/* 7 */ x Jcc    x Jcc   x Jcc  x Jcc  x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc    x Jcc   x Jcc   x Jcc
/* 8 */ x grp1   x grp1  x _    x grp1 x TEST  x TEST  x XCHG  x XCHG  x MOV   x MOV   x MOV   x MOV   x MOV    x LEA   x MOV   x POP
/* 9 */ x NOP    x XCHG  x XCHG x XCHG x XCHG  x XCHG  x XCHG  x XCHG  x CWDE  x CDQ   x _     x _     x PUSHFD x POPFD x SAHF  x LAHF
/* A */ x _      x _     x _    x _    x MOVSx x MOVSx x CMPSx x CMPSx x TEST  x TEST  x STOSx x STOSx x LODSx  x LODSx x SCASx x SCASx
/* B */ x MOV    x MOV   x MOV  x MOV  x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV    x MOV   x MOV   x MOV
/* C */ x grp2   x grp2  x RET  x RET  x _     x _     x MOV   x MOV   x ENTER x LEAVE x _     x _     x _      x _     x _     x _
/* D */ x grp2   x grp2  x grp2 x grp2 x _     x _     x _     x XLAT  x ESC   x ESC   x ESC   x ESC   x ESC    x ESC   x ESC   x ESC
/* E */ x LOOP   x LOOP  x LOOP x Jcc  x _     x _     x _     x _     x CALL  x Jcc   x _     x Jcc   x _      x _     x _     x _
/* F */ x _      x _     x REP  x REP  x _     x CMC   x grp3  x grp3  x CLC   x STC   x _     x _     x CLD    x STD   x grp4  x grp5
};
//------------------------------------------------------------------------------
// Two-Byte Opcode Map
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_i386::two[256] =
{      // 0       1       2       3       4       5       6       7       8       9       A       B       C       D       E       F
/* 0 */ o grp6  x grp7  x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* 1 */ x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* 2 */ x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* 3 */ x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* 4 */ x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* 5 */ x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* 6 */ x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* 7 */ x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* 8 */ x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc
/* 9 */ x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc x SETcc
/* A */ x _     x _     x _     x BT    x SHxD  x SHxD  x _     x _     x _     x _     x _     x BTS   x SHxD  x SHxD  x _     x IMUL
/* B */ x _     x _     x _     x BTR   x _     x _     x MOVZX x MOVZX x _     x _     x grp8  x BTC   x BSF   x BSR   x MOVSX x MOVSX
/* C */ x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* D */ x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* E */ x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* F */ x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
};
//------------------------------------------------------------------------------
// Opcodes determined by bits 5,4,3 of modR/M byte
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_i386::group[16][8] =
{        // 0      1     2      3     4     5      6      7
/* 0 */ { o _    x _   x _    x _   x _   x _    x _    x _    },
/* 1 */ { o ADD  x OR  x ADC  x SBB x AND x SUB  x XOR  x CMP  },
/* 2 */ { o Rxx  x Rxx x Rxx  x Rxx x Sxx x Sxx  x _    x Sxx  },
/* 3 */ { o TEST x _   x NOT  x NEG x MUL x IMUL x DIV  x IDIV },
/* 4 */ { o INC  x DEC x _    x _   x _   x _    x _    x _    },
/* 5 */ { o INC  x DEC x CALL x _   x JMP x _    x PUSH x _    },
/* 6 */ { o _    x _   x _    x _   x _   x _    x _    x _    },
/* 7 */ { o _    x _   x _    x _   x _   x _    x _    x _    },
/* 8 */ { o _    x _   x _    x _   x BT  x BTS  x BTR  x BTC  },
};
//------------------------------------------------------------------------------
#undef o
#undef x
//------------------------------------------------------------------------------
x86_i386::~x86_i386()
{
#if defined(_UCRT)
    _aligned_free(memory);
#else
    free(memory);
#endif
}
//------------------------------------------------------------------------------
bool x86_i386::Initialize(size_t space, const void* program, size_t size)
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

    EIP = 1024;
    memcpy(memory + EIP, program, size);

    return true;
}
//------------------------------------------------------------------------------
bool x86_i386::Step()
{
    Format format;
    StepInternal(format);
    Fixup(format);
    format.operation(*this, format, format.operand[0].memory, format.operand[1].memory);
    return true;
}
//------------------------------------------------------------------------------
bool x86_i386::Jump(size_t address)
{
    if (address >= memory_size)
        return false;
    EIP = (uint32_t)address;
    return true;
}
//------------------------------------------------------------------------------
uint8_t* x86_i386::Memory(size_t base, size_t size)
{
    if (base + size >= memory_size)
        return nullptr;
    return memory + base;
}
//------------------------------------------------------------------------------
std::string x86_i386::Disassemble(int count)
{
    std::string output;

    x86_i386 backup;
    for (int i = 0; i < 8; ++i)
        backup.regs[i] = regs[i];
    backup.flags = flags;
    backup.ip = ip;

    disasm = &output;

    for (int i = 0; i < count; ++i) {
        char temp[64];

        uint32_t address = EIP;
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
            if (address + i >= EIP) {
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
void x86_i386::StepInternal(Format& format)
{
    operand_size = 32;
    repeat_string_operation = false;

    for (;;) {
        opcode = memory + EIP;
        (this->*one[opcode[0]])(format);
        EIP += format.length;

        switch (opcode[0]) {
        case 0x26:
        case 0x2E:
        case 0x36:
        case 0x3E:
        case 0x64:
        case 0x65:
        case 0x66:
        case 0x67:
        case 0xF0:
        case 0xF2:
        case 0xF3:
            format = Format();
            continue;
            break;
        default:
            operand_size = 32;
            repeat_string_operation = false;
            break;
        }
        break;
    }
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x86_i386::ESC(Format& format)
{
}
//------------------------------------------------------------------------------
void x86_i386::TWO(Format& format)
{
    (this->*two[opcode[1]])(format);
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x86_i386::grp1(Format& format)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    (this->*group[1][nnn])(format);
}
//------------------------------------------------------------------------------
void x86_i386::grp2(Format& format)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    (this->*group[2][nnn])(format);
}
//------------------------------------------------------------------------------
void x86_i386::grp3(Format& format)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    (this->*group[3][nnn])(format);
}
//------------------------------------------------------------------------------
void x86_i386::grp4(Format& format)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    (this->*group[4][nnn])(format);
}
//------------------------------------------------------------------------------
void x86_i386::grp5(Format& format)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    (this->*group[5][nnn])(format);
}
//------------------------------------------------------------------------------
void x86_i386::grp6(Format& format)
{
    int nnn = (opcode[2] >> 3) & 0b111;
    (this->*group[6][nnn])(format);
}
//------------------------------------------------------------------------------
void x86_i386::grp7(Format& format)
{
    int nnn = (opcode[2] >> 3) & 0b111;
    (this->*group[7][nnn])(format);
}
//------------------------------------------------------------------------------
void x86_i386::grp8(Format& format)
{
    int nnn = (opcode[2] >> 3) & 0b111;
    (this->*group[8][nnn])(format);
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x86_i386::OSIZE(Format& format)
{
    operand_size = 16;
}
//------------------------------------------------------------------------------
