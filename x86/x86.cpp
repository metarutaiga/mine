//==============================================================================
// miCPU : x86 Source
//
// Copyright (c) 2025 TAiGA
// https://github.com/metarutaiga/miCPU
//==============================================================================
#include "x86.h"
#include <format>

//------------------------------------------------------------------------------
#define o &x86::
#define x , o
//------------------------------------------------------------------------------
// One-Byte Opcode Map
//------------------------------------------------------------------------------
const x86::instruction_pointer x86::one[256] =
{      // 0       1      2      3      4      5      6      7      8       9       A      B      C       D      E      F
/* 0 */ o ADD   x ADD  x ADD  x ADD  x ADD  x ADD  x PUSH x POP  x OR    x OR    x OR   x OR   x OR    x OR   x PUSH x two
/* 1 */ x ADC   x ADC  x ADC  x ADC  x ADC  x ADC  x PUSH x POP  x SBB   x SBB   x SBB  x SBB  x SBB   x SBB  x PUSH x POP
/* 2 */ x AND   x AND  x AND  x AND  x AND  x AND  x ___  x DAA  x SUB   x SUB   x SUB  x SUB  x SUB   x SUB  x ___  x DAS
/* 3 */ x XOR   x XOR  x XOR  x XOR  x XOR  x XOR  x ___  x AAA  x CMP   x CMP   x CMP  x CMP  x CMP   x CMP  x ___  x AAS
/* 4 */ x INC   x INC  x INC  x INC  x INC  x INC  x INC  x INC  x DEC   x DEC   x DEC  x DEC  x DEC   x DEC  x DEC  x DEC
/* 5 */ x PUSH  x PUSH x PUSH x PUSH x PUSH x PUSH x PUSH x PUSH x POP   x POP   x POP  x POP  x POP   x POP  x POP  x POP
/* 6 */ x PUSHA x POPA x ___  x ___  x ___  x ___  x OS   x ___  x PUSH  x IMUL  x PUSH x IMUL x INS   x INS  x OUTS x OUTS
/* 7 */ x Jcc   x Jcc  x Jcc  x Jcc  x Jcc  x Jcc  x Jcc  x Jcc  x Jcc   x Jcc   x Jcc  x Jcc  x Jcc   x Jcc  x Jcc  x Jcc
/* 8 */ x grp1  x grp1 x ___  x grp1 x TEST x TEST x XCHG x XCHG x MOV   x MOV   x MOV  x MOV  x MOV   x LEA  x MOV  x POP
/* 9 */ x XCHG  x XCHG x XCHG x XCHG x XCHG x XCHG x XCHG x XCHG x Cxx   x Cxx   x CALL x WAIT x PUSHF x POPF x SAHF x LAHF
/* A */ x MOV   x MOV  x MOV  x MOV  x MOVS x MOVS x CMPS x CMPS x TEST  x TEST  x STOS x STOS x LODS  x LODS x SCAS x SCAS
/* B */ x MOV   x MOV  x MOV  x MOV  x MOV  x MOV  x MOV  x MOV  x MOV   x MOV   x MOV  x MOV  x MOV   x MOV  x MOV  x MOV
/* C */ x grp2  x grp2 x RET  x RET  x ___  x ___  x MOV  x MOV  x ENTER x LEAVE x RET  x RET  x INT   x INT  x INT  x IRET
/* D */ x grp2  x grp2 x grp2 x grp2 x AAM  x AAD  x ___  x XLAT x esc   x esc   x esc  x esc  x esc   x esc  x esc  x esc
/* E */ x LOOP  x LOOP x LOOP x Jcc  x IN   x IN   x OUT  x OUT  x CALL  x Jcc   x Jcc  x Jcc  x IN    x IN   x OUT  x OUT
/* F */ x LOCK  x ___  x REP  x REP  x HLT  x CMC  x grp3 x grp3 x CLC   x STC   x CLI  x STI  x CLD   x STD  x grp4 x grp5
};
//------------------------------------------------------------------------------
const x86::instruction_pointer x86::group[16][8] =
{        // 0      1     2      3      4     5      6      7
/* 0 */ { o ___  x ___ x ___ x  ___  x ___ x ___  x ___  x ___  },
/* 1 */ { o ADD  x OR  x ADC  x SBB  x AND x SUB  x XOR  x CMP  },
/* 2 */ { o Rxx  x Rxx x Rxx  x Rxx  x Sxx x Sxx  x ___  x Sxx  },
/* 3 */ { o TEST x ___ x NOT  x NEG  x MUL x IMUL x DIV  x IDIV },
/* 4 */ { o INC  x DEC x ___  x ___  x ___ x ___  x ___  x ___  },
/* 5 */ { o INC  x DEC x CALL x CALL x JMP x JMP  x PUSH x ___  },
/* 6 */ { o ___  x ___ x ___ x  ___  x ___ x ___  x ___  x ___  },
/* 7 */ { o ___  x ___ x ___ x  ___  x ___ x ___  x ___  x ___  },
/* 8 */ { o ___  x ___ x ___ x  ___  x BT  x BTS  x BTR  x BTC  },
};
//------------------------------------------------------------------------------
#undef o
#undef x
//------------------------------------------------------------------------------
x86::~x86()
{
    free(memory);
}
//------------------------------------------------------------------------------
bool x86::Initialize(size_t space, const void* program, size_t size)
{
    if (space & (1024 - 1)) {
        printf("space %zd is not aligned 1024\n", space);
        return false;
    }
    if (space < (1024 + size + 65536)) {
        printf("space %zd is less than %zd\n", space, 1024 + size + 65536);
        return false;
    }
    memory = (uint8_t*)aligned_alloc(1024, space);
    if (memory == nullptr) {
        printf("memory allocation is failed\n");
        return false;
    }
    memset(memory, 0, space);
    stack = memory + space - 16;

    eip = 1024;
    memcpy(memory + eip, program, size);

    return true;
}
//------------------------------------------------------------------------------
bool x86::Step()
{
    for (;;) {
        opcode = memory + eip;
        (this->*one[opcode[0]])();

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
            continue;
            break;
        default:
            operand_size_override = false;
            break;
        }
        break;
    }

    return true;
}
//------------------------------------------------------------------------------
std::string x86::Disassemble(int count)
{
    std::string output;

    x86 backup;
    for (int i = 0; i < 8; ++i)
        backup.regs[i] = regs[i];
    backup.eflags = eflags;
    backup.eip = eip;

    disasm = &output;

    for (int i = 0; i < count; ++i) {
        uint32_t address = eip;
        output += std::format("{:08X}", address) + " : ";
        size_t insert = output.size();

        Step();

        for (uint32_t i = 0; i < 16; ++i) {
            if (address + i >= eip) {
                output.insert(insert, "  ");
                continue;;
            }
            std::string byte = std::format("{:02X}", memory[address + i]);
            output.insert(insert, byte);
            insert += 2;
        }
    }

    disasm = nullptr;

    for (int i = 0; i < 8; ++i)
        regs[i] = backup.regs[i];
    eflags = backup.eflags;
    eip = backup.eip;

    return output;
}
//------------------------------------------------------------------------------
#define AL              regs[0].l
#define AH              regs[0].h
#define AX              regs[0].w
#define EAX             regs[0].d
#define CF              eflags._c
#define PF              eflags._p
#define AF              eflags._a
#define ZF              eflags._z
#define SF              eflags._s
#define OF              eflags._o
#define EIP             eip
#define IMM8(m,i)       (*(int8_t*)(m+i))
#define IMM16(m,i)      (*(int16_t*)(m+i))
#define IMM32(m,i)      (*(int32_t*)(m+i))
//------------------------------------------------------------------------------
static const std::string REG8[8] = { "AL", "CL", "DL", "BL", "AH", "CH", "DH", "BH" };
static const std::string REG16[8] = { "AX", "CX", "DX", "BX", "SP", "BP", "SI", "DI" };
static const std::string REG32[8] = { "EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI" };
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
x86::Format x86::Decode(int offset, const char* instruction, int direction, int operand_size, int immediate_size)
{
    int OPREG = (direction == 0) ? 1 : 0;
    int MODRM = (direction == 0) ? 0 : 1;

    x86::Format format;
    format.size = (operand_size == 0) ? 8 : operand_size_override ? 16 : 32;
    format.length = offset + 1;
    format.instruction = instruction;
    if (offset) {
        int MOD = (opcode[offset] >> 6) & 0b11;
        int REG = (opcode[offset] >> 3) & 0b111;
        int RM = (opcode[offset] >> 0) & 0b111;

        int SS = (opcode[offset + 1] >> 6) & 0b11;
        int INDEX = (opcode[offset + 1] >> 3) & 0b111;
        int BASE = (opcode[offset + 1] >> 0) & 0b111;

        switch (opcode[offset] & 0b11111000) {
        case 0b00100000:
            format.length += 1;
            format.addressing[MODRM].scale = (INDEX == 0b100) ? 0 : (1 << SS);
            format.addressing[MODRM].index = (INDEX == 0b100) ? -1 : INDEX;
            format.addressing[MODRM].base = (BASE == 0b101) ? -1 : BASE;
            break;
        case 0b01100000:
            format.length += 2;
            format.addressing[MODRM].scale = (INDEX == 0b100) ? 0 : (1 << SS);
            format.addressing[MODRM].index = (INDEX == 0b100) ? -1 : INDEX;
            format.addressing[MODRM].base = BASE;
            format.addressing[MODRM].displacement = IMM8(opcode, format.length - 1);
            break;
        case 0b10100000:
            format.length += 5;
            format.addressing[MODRM].scale = (INDEX == 0b100) ? 0 : (1 << SS);
            format.addressing[MODRM].index = (INDEX == 0b100) ? -1 : INDEX;
            format.addressing[MODRM].base = BASE;
            format.addressing[MODRM].displacement = IMM32(opcode, format.length - 4);
            break;
        default:
            switch (MOD) {
            case 0b00:
                format.addressing[MODRM].base = RM;
                break;
            case 0b01:
                format.length += 1;
                format.addressing[MODRM].base = RM;
                format.addressing[MODRM].displacement = IMM8(opcode, format.length - 1);
                break;
            case 0b10:
                format.length += 4;
                format.addressing[MODRM].base = RM;
                format.addressing[MODRM].displacement = IMM32(opcode, format.length - 4);
                break;
            case 0b11:
                format.addressing[MODRM].reg = true;
                format.addressing[MODRM].base = RM;
                break;
            }
            break;
        }
        format.addressing[OPREG].reg = true;
        format.addressing[OPREG].base = REG;
    }
    else {
        format.addressing[MODRM].reg = true;
        format.addressing[MODRM].base = 0;
        format.addressing[OPREG].reg = true;
        format.addressing[OPREG].base = 0;
    }

    if (immediate_size) {
        if (immediate_size < 0)
            immediate_size = format.size;
        switch (immediate_size) {
        case 8:
            format.length += 1;
            format.addressing[OPREG].imm = true;
            format.addressing[OPREG].displacement = IMM8(opcode, format.length - 1);
            break;
        case 16:
            format.length += 2;
            format.addressing[OPREG].imm = true;
            format.addressing[OPREG].displacement = IMM16(opcode, format.length - 2);
            break;
        case 32:
            format.length += 4;
            format.addressing[OPREG].imm = true;
            format.addressing[OPREG].displacement = IMM32(opcode, format.length - 4);
            break;
        }
    }

    return format;
}
//------------------------------------------------------------------------------
void x86::Disasm(const Format& format)
{
    if (disasm) {
        auto hex = [](auto imm) {
            uint64_t value = std::abs((int64_t)imm);
            if (value > 0xFFFF) return std::format("{}{:08X}", imm < 0 ? "-" : "", value);
            if (value > 0xFF) return std::format("{}{:04X}", imm < 0 ? "-" : "", value);
            return std::format("{}{:02X}", imm < 0 ? "-" : "", value);
        };

        (*disasm) += format.instruction;
        for (int i = 0; i < 2; ++i) {
            if (i) (*disasm) += ',';
            (*disasm) += ' ';
            if (format.addressing[i].imm) {
                (*disasm) += hex(format.addressing[i].displacement);
            }
            else if (format.addressing[i].reg) {
                switch (format.size) {
                case 8:     (*disasm) += REG8[format.addressing[i].base];   break;
                case 16:    (*disasm) += REG16[format.addressing[i].base];  break;
                case 32:    (*disasm) += REG32[format.addressing[i].base];  break;
                }
            }
            else {
                switch (format.size) {
                case 8:     (*disasm) += "BYTE PTR"; break;
                case 16:    (*disasm) += "WORD PTR"; break;
                case 32:    (*disasm) += "DWORD PTR"; break;
                }
                (*disasm) += ' ';
                (*disasm) += '[';
                if (format.addressing[i].scale > 0) {
                    (*disasm) += REG32[format.addressing[i].index];
                    (*disasm) += '*';
                    (*disasm) += std::to_string(format.addressing[i].scale);
                }
                if (format.addressing[i].base >= 0) {
                    if ((*disasm).back() != '[')
                        (*disasm) += '+';
                    (*disasm) += REG32[format.addressing[i].base];
                }
                if (format.addressing[i].displacement) {
                    if ((*disasm).back() != '[')
                        (*disasm) += '+';
                    (*disasm) += hex(format.addressing[i].displacement);
                }
                if ((*disasm).back() == '[')
                    (*disasm) += '0';
                (*disasm) += ']';
            }
        }
    }
}
//------------------------------------------------------------------------------
void x86::Fixup(Format& format)
{
    for (int i = 0; i < 2; ++i) {
        if (format.addressing[i].imm) {
            format.addressing[i].memory = nullptr;
            format.addressing[i].memory += format.addressing[i].displacement;
        }
        else if (format.addressing[i].reg) {
            if (format.size != 8 || format.addressing[i].base < 4) {
                format.addressing[i].memory = &regs[format.addressing[i].base].l;
            }
            else {
                format.addressing[i].memory = &regs[format.addressing[i].base - 4].h;
            }
        }
        else {
            format.addressing[i].memory = memory;
            format.addressing[i].memory += regs[format.addressing[i].index].d * format.addressing[i].scale;
            format.addressing[i].memory += regs[format.addressing[i].base].d;
            format.addressing[i].memory += format.addressing[i].displacement;
        }
    }
}
//------------------------------------------------------------------------------
template<typename A, typename B>
void x86::UpdateEFlags(A& DEST, B TEMP)
{
    int bits = sizeof(A) * 8;
    CF = ((DEST ^ TEMP) &  (1 << (bits - 1))) ? 1 : 0;
    PF = ((       TEMP) &  (1              )) ? 1 : 0;
    AF = ((DEST ^ TEMP) &  (1              )) ? 1 : 0;
    ZF = ((       TEMP) == (0              )) ? 1 : 0;
    SF = ((       TEMP) &  (1 << (bits - 1))) ? 1 : 0;
    OF = ((DEST ^ TEMP) &  (1 << (bits - 2))) ? 1 : 0;
    DEST = TEMP;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x86::esc()
{
}
//------------------------------------------------------------------------------
void x86::grp1()
{
    int nnn = (opcode[1] >> 3) & 0b111;
    (this->*group[1][nnn])();
}
//------------------------------------------------------------------------------
void x86::grp2()
{
    int nnn = (opcode[1] >> 3) & 0b111;
    (this->*group[2][nnn])();
}
//------------------------------------------------------------------------------
void x86::grp3()
{
    int nnn = (opcode[1] >> 3) & 0b111;
    (this->*group[3][nnn])();
}
//------------------------------------------------------------------------------
void x86::grp4()
{
    int nnn = (opcode[1] >> 3) & 0b111;
    (this->*group[4][nnn])();
}
//------------------------------------------------------------------------------
void x86::grp5()
{
    int nnn = (opcode[1] >> 3) & 0b111;
    (this->*group[5][nnn])();
}
//------------------------------------------------------------------------------
void x86::two()
{
}
//------------------------------------------------------------------------------
void x86::___()
{
    EIP += 1;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x86::OS()
{
    EIP += 1;

    operand_size_override = true;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
#define BEGIN_OPERATION ; \
    auto operation = [this](auto& DEST, auto SRC)
#define END_OPERATION ; \
    switch (format.size) { \
    case 8:     operation(*(uint8_t*)format.addressing[0].memory, *(uint8_t*)format.addressing[1].memory);      break; \
    case 16:    operation(*(uint16_t*)format.addressing[0].memory, *(uint16_t*)format.addressing[1].memory);    break; \
    case 32:    operation(*(uint32_t*)format.addressing[0].memory, *(uint32_t*)format.addressing[1].memory);    break; \
    }
//------------------------------------------------------------------------------
void x86::AAA()
{
    EIP += 1;

    if (disasm) {
        (*disasm) += "AAA";
        (*disasm) += '\n';
        return;
    }

    if ((AL & 0x0F) > 9 || AF == 1) {
        AL = (AL + 6) & 0x0F;
        AH = AH + 1;
        AF = 1;
        CF = 1;
    }
    else {
        CF = 0;
        AF = 0;
    }
}
//------------------------------------------------------------------------------
void x86::AAD()
{
    EIP += 2;

    if (disasm) {
        (*disasm) += "AAD";
        (*disasm) += '\n';
        return;
    }

    AL = AH * 10 + AL;
    AH = 0;
}
//------------------------------------------------------------------------------
void x86::AAM()
{
    EIP += 2;

    if (disasm) {
        (*disasm) += "AAM";
        (*disasm) += '\n';
        return;
    }

    AH = AL / 10;
    AL = AL % 10;
}
//------------------------------------------------------------------------------
void x86::AAS()
{
    EIP += 1;

    if (disasm) {
        (*disasm) += "AAS";
        (*disasm) += '\n';
        return;
    }

    if ((AL & 0x0F) > 9 || AF == 1) {
        AL = AL - 6;
        AL = AL & 0x0F;
        AH = AH - 1;
        AF = 1;
        CF = 1;
    }
    else {
        CF = 0;
        AF = 0;
    }
}
//------------------------------------------------------------------------------
void x86::ADC()
{
    Format format;
    switch (opcode[0]) {
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:  format = Decode(1, "ADC", opcode[0] & 0b10, opcode[0] & 0b01, 0);   break;
    case 0x14:
    case 0x15:  format = Decode(0, "ADC",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x80:
    case 0x81:  format = Decode(1, "ADC",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x83:  format = Decode(1, "ADC",             0b00, opcode[0] & 0b01, 8);   break;
    }
    Fixup(format);

    EIP += format.length;

    if (disasm) {
        Disasm(format);
        (*disasm) += '\n';
        return;
    }

    BEGIN_OPERATION {
        UpdateEFlags(DEST, DEST + SRC + CF);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86::ADD()
{
    Format format;
    switch (opcode[0]) {
    case 0x00:
    case 0x01:
    case 0x02:
    case 0x03:  format = Decode(1, "ADD", opcode[0] & 0b10, opcode[0] & 0b01, 0);   break;
    case 0x04:
    case 0x05:  format = Decode(0, "ADD",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x80:
    case 0x81:  format = Decode(1, "ADD",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x83:  format = Decode(1, "ADD",             0b00, opcode[0] & 0b01, 8);   break;
    }
    Fixup(format);

    EIP += format.length;

    if (disasm) {
        Disasm(format);
        (*disasm) += '\n';
        return;
    }

    BEGIN_OPERATION {
        UpdateEFlags(DEST, DEST + SRC);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86::AND()
{
    Format format;
    switch (opcode[0]) {
    case 0x20:
    case 0x21:
    case 0x22:
    case 0x23:  format = Decode(1, "AND", opcode[0] & 0b10, opcode[0] & 0b01, 0);   break;
    case 0x24:
    case 0x25:  format = Decode(0, "AND",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x80:
    case 0x81:  format = Decode(1, "AND",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x83:  format = Decode(1, "AND",             0b00, opcode[0] & 0b01, 8);   break;
    }
    Fixup(format);

    EIP += format.length;

    if (disasm) {
        Disasm(format);
        (*disasm) += '\n';
        return;
    }

    BEGIN_OPERATION {
        UpdateEFlags(DEST, DEST & SRC);
        CF = 0;
        OF = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86::BSF()
{
    Format format = Decode(2, "BSF", 0, 1, 0);
    Fixup(format);

    EIP += format.length;

    if (disasm) {
        Disasm(format);
        (*disasm) += '\n';
        return;
    }

    BEGIN_OPERATION {
        ZF = 1;
        if (SRC) {
            ZF = 0;
            DEST = __builtin_ctz(SRC);
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86::BSR()
{
    Format format = Decode(2, "BSR", 0, 1, 0);
    Fixup(format);

    EIP += format.length;

    if (disasm) {
        Disasm(format);
        (*disasm) += '\n';
        return;
    }

    BEGIN_OPERATION {
        ZF = 1;
        if (SRC) {
            ZF = 0;
            DEST = __builtin_clz(SRC) ^ (sizeof(SRC) * 8 - 1);
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86::BT()
{
    Format format;
    switch (opcode[1]) {
    case 0xA3:  format = Decode(2, "BT", 0, 1, 0);  break;
    case 0xBA:  format = Decode(2, "BT", 0, 1, -1); break;
    }
    Fixup(format);

    EIP += format.length;

    if (disasm) {
        Disasm(format);
        (*disasm) += '\n';
        return;
    }

    BEGIN_OPERATION {
        CF = (DEST & (1 << SRC)) ? 1 : 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86::BTC()
{
    Format format;
    switch (opcode[1]) {
    case 0xBA:  format = Decode(2, "BTC", 0, 1, -1);    break;
    case 0xBB:  format = Decode(2, "BTC", 0, 1, 0);     break;
    }
    Fixup(format);

    EIP += format.length;

    if (disasm) {
        Disasm(format);
        (*disasm) += '\n';
        return;
    }

    BEGIN_OPERATION {
        CF = (DEST & (1 << SRC)) ? 1 : 0;
        DEST = (DEST ^ (1 << SRC));
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86::BTR()
{
    Format format;
    switch (opcode[1]) {
    case 0xB3:  format = Decode(2, "BTR", 0, 1, 0);     break;
    case 0xBA:  format = Decode(2, "BTR", 0, 1, -1);    break;
    }
    Fixup(format);

    EIP += format.length;

    if (disasm) {
        Disasm(format);
        (*disasm) += '\n';
        return;
    }

    BEGIN_OPERATION {
        CF = (DEST & (1 << SRC)) ? 1 : 0;
        DEST = (DEST & ~(1 << SRC));
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86::BTS()
{
    Format format;
    switch (opcode[1]) {
    case 0xAB:  format = Decode(2, "BTS", 0, 1, 0);     break;
    case 0xBA:  format = Decode(2, "BTS", 0, 1, -1);    break;
    }
    Fixup(format);

    EIP += format.length;

    if (disasm) {
        Disasm(format);
        (*disasm) += '\n';
        return;
    }

    BEGIN_OPERATION {
        CF = (DEST & (1 << SRC)) ? 1 : 0;
        DEST = (DEST | (1 << SRC));
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86::CALL()
{
}
//------------------------------------------------------------------------------
void x86::CLC()
{
}
//------------------------------------------------------------------------------
void x86::CLD()
{
}
//------------------------------------------------------------------------------
void x86::CLI()
{
}
//------------------------------------------------------------------------------
void x86::CMC()
{
}
//------------------------------------------------------------------------------
void x86::CMP()
{
}
//------------------------------------------------------------------------------
void x86::CMPS()
{
}
//------------------------------------------------------------------------------
void x86::Cxx()
{
}
//------------------------------------------------------------------------------
void x86::DAA()
{
}
//------------------------------------------------------------------------------
void x86::DAS()
{
}
//------------------------------------------------------------------------------
void x86::DEC()
{
}
//------------------------------------------------------------------------------
void x86::DIV()
{
}
//------------------------------------------------------------------------------
void x86::ENTER()
{
}
//------------------------------------------------------------------------------
void x86::HLT()
{
}
//------------------------------------------------------------------------------
void x86::IDIV()
{
}
//------------------------------------------------------------------------------
void x86::IMUL()
{
}
//------------------------------------------------------------------------------
void x86::IN()
{
}
//------------------------------------------------------------------------------
void x86::INC()
{
}
//------------------------------------------------------------------------------
void x86::INS()
{
}
//------------------------------------------------------------------------------
void x86::INT()
{
}
//------------------------------------------------------------------------------
void x86::IRET()
{
}
//------------------------------------------------------------------------------
void x86::Jcc()
{
}
//------------------------------------------------------------------------------
void x86::JMP()
{
}
//------------------------------------------------------------------------------
void x86::LAHF()
{
}
//------------------------------------------------------------------------------
void x86::LAR()
{
}
//------------------------------------------------------------------------------
void x86::LEA()
{
}
//------------------------------------------------------------------------------
void x86::LEAVE()
{
}
//------------------------------------------------------------------------------
void x86::LOCK()
{
}
//------------------------------------------------------------------------------
void x86::LODS()
{
}
//------------------------------------------------------------------------------
void x86::LOOP()
{
}
//------------------------------------------------------------------------------
void x86::MOV()
{
}
//------------------------------------------------------------------------------
void x86::MOVS()
{
}
//------------------------------------------------------------------------------
void x86::MOVSX()
{
}
//------------------------------------------------------------------------------
void x86::MOVZX()
{
}
//------------------------------------------------------------------------------
void x86::MUL()
{
}
//------------------------------------------------------------------------------
void x86::NEG()
{
}
//------------------------------------------------------------------------------
void x86::NOP()
{
}
//------------------------------------------------------------------------------
void x86::NOT()
{
}
//------------------------------------------------------------------------------
void x86::OR()
{
}
//------------------------------------------------------------------------------
void x86::OUT()
{
}
//------------------------------------------------------------------------------
void x86::OUTS()
{
}
//------------------------------------------------------------------------------
void x86::POP()
{
}
//------------------------------------------------------------------------------
void x86::POPA()
{
}
//------------------------------------------------------------------------------
void x86::POPF()
{
}
//------------------------------------------------------------------------------
void x86::PUSH()
{
}
//------------------------------------------------------------------------------
void x86::PUSHA()
{
}
//------------------------------------------------------------------------------
void x86::PUSHF()
{
}
//------------------------------------------------------------------------------
void x86::Rxx()
{
}
//------------------------------------------------------------------------------
void x86::REP()
{
}
//------------------------------------------------------------------------------
void x86::RET()
{
}
//------------------------------------------------------------------------------
void x86::SAHF()
{
}
//------------------------------------------------------------------------------
void x86::Sxx()
{
}
//------------------------------------------------------------------------------
void x86::SBB()
{
}
//------------------------------------------------------------------------------
void x86::SCAS()
{
}
//------------------------------------------------------------------------------
void x86::SETcc()
{
}
//------------------------------------------------------------------------------
void x86::SHLD()
{
}
//------------------------------------------------------------------------------
void x86::SHRD()
{
}
//------------------------------------------------------------------------------
void x86::STC()
{
}
//------------------------------------------------------------------------------
void x86::STD()
{
}
//------------------------------------------------------------------------------
void x86::STI()
{
}
//------------------------------------------------------------------------------
void x86::STOS()
{
}
//------------------------------------------------------------------------------
void x86::SUB()
{
}
//------------------------------------------------------------------------------
void x86::TEST()
{
}
//------------------------------------------------------------------------------
void x86::WAIT()
{
}
//------------------------------------------------------------------------------
void x86::XCHG()
{
}
//------------------------------------------------------------------------------
void x86::XLAT()
{
}
//------------------------------------------------------------------------------
void x86::XOR()
{
}
//------------------------------------------------------------------------------
