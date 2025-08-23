//==============================================================================
// 80386 Programmer's Reference Manual
// 1986
//
// INTEL CORPORATION 1987
//==============================================================================
#include "x86_i386.h"
#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"
#include "syscall/allocator.h"

//------------------------------------------------------------------------------
#define o x86_i386::
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
/* 6 */ x PUSHAD x POPAD x _    x _    x _     x _     x OSIZE x ASIZE x PUSH  x IMUL  x PUSH  x IMUL  x _      x _     x _     x _
/* 7 */ x Jcc    x Jcc   x Jcc  x Jcc  x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc   x Jcc    x Jcc   x Jcc   x Jcc
/* 8 */ x grp1   x grp1  x _    x grp1 x TEST  x TEST  x XCHG  x XCHG  x MOV   x MOV   x MOV   x MOV   x MOV    x LEA   x MOV   x POP
/* 9 */ x NOP    x XCHG  x XCHG x XCHG x XCHG  x XCHG  x XCHG  x XCHG  x CWDE  x CDQ   x _     x _     x PUSHFD x POPFD x SAHF  x LAHF
/* A */ x MOV    x MOV   x MOV  x MOV  x MOVSx x MOVSx x CMPSx x CMPSx x TEST  x TEST  x STOSx x STOSx x LODSx  x LODSx x SCASx x SCASx
/* B */ x MOV    x MOV   x MOV  x MOV  x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV   x MOV    x MOV   x MOV   x MOV
/* C */ x grp2   x grp2  x RET  x RET  x _     x _     x MOV   x MOV   x ENTER x LEAVE x _     x _     x _      x _     x _     x _
/* D */ x grp2   x grp2  x grp2 x grp2 x _     x _     x _     x XLAT  x ESC   x ESC   x ESC   x ESC   x ESC    x ESC   x ESC   x ESC
/* E */ x LOOP   x LOOP  x LOOP x Jcc  x _     x _     x _     x _     x CALL  x JMP   x _     x JMP   x _      x _     x _     x _
/* F */ x _      x _     x REP  x REP  x _     x CMC   x grp3  x grp3  x CLC   x STC   x _     x _     x CLD    x STD   x grp4  x grp5
};
//------------------------------------------------------------------------------
// Two-Byte Opcode Map
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_i386::two[256] =
{      // 0       1       2       3       4       5       6       7       8       9       A       B       C       D       E       F
/* 0 */ o grp6  x grp7  x _     x _     x _     x _     x _     x _     x _     x _     x _     x UD    x _     x _     x _     x _
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
/* B */ x _     x _     x _     x BTR   x _     x _     x MOVZX x MOVZX x _     x UD    x grp8  x BTC   x BSF   x BSR   x MOVSX x MOVSX
/* C */ x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* D */ x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* E */ x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _
/* F */ x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x _     x UD
};
//------------------------------------------------------------------------------
// Opcodes determined by bits 5,4,3 of modR/M byte
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_i386::group[9][8] =
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
// Escape Opcode Map
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_i386::esc[512] =
{            // 0        1        2        3        4         5        6         7         8        9         A        B         C         D        E        F
/* D8 048C */ o FADD   x FADD   x FADD   x FADD   x FADD    x FADD   x FADD    x FADD    x FMUL   x FMUL    x FMUL   x FMUL    x FMUL    x FMUL   x FMUL   x FMUL
/* D8 159D */ x FCOM   x FCOM   x FCOM   x FCOM   x FCOM    x FCOM   x FCOM    x FCOM    x FCOMP  x FCOMP   x FCOMP  x FCOMP   x FCOMP   x FCOMP  x FCOMP  x FCOMP
/* D8 26AE */ x FSUB   x FSUB   x FSUB   x FSUB   x FSUB    x FSUB   x FSUB    x FSUB    x FSUBR  x FSUBR   x FSUBR  x FSUBR   x FSUBR   x FSUBR  x FSUBR  x FSUBR
/* D8 37BF */ x FDIV   x FDIV   x FDIV   x FDIV   x FDIV    x FDIV   x FDIV    x FDIV    x FDIVR  x FDIVR   x FDIVR  x FDIVR   x FDIVR   x FDIVR  x FDIVR  x FDIVR
/* D9 048C */ x FLD    x FLD    x FLD    x FLD    x FLD     x FLD    x FLD     x FLD     x FXCH   x FXCH    x FXCH   x FXCH    x FXCH    x FXCH   x FXCH   x FXCH
/* D9 159D */ x FNOP   x _      x _      x _      x _       x _      x _       x _       x _      x _       x _      x _       x _       x _      x _      x _
/* D9 26AE */ x FCHS   x FABS   x _      x _      x FTST    x FXAM   x _       x _       x FLD1   x FLDL2T  x FLDL2E x FLDPI   x FLDLG2  x FLDLN2 x FLDZ   x _
/* D9 37BF */ x F2XM1  x FYL2X  x FPTAN  x FPATAN x FXTRACT x FPREM1 x FDECSTP x FINCSTP x FPREM  x FYL2XP1 x FSQRT  x FSINCOS x FRNDINT x FSCALE x FSIN   x FCOS
/* DA 048C */ x _      x _      x _      x _      x _       x _      x _       x _       x _      x _       x _      x _       x _       x _      x _      x _
/* DA 159D */ x _      x _      x _      x _      x _       x _      x _       x _       x _      x _       x _      x _       x _       x _      x _      x _
/* DA 26AE */ x _      x _      x _      x _      x _       x _      x _       x _       x _      x FUCOMPP x _      x _       x _       x _      x _      x _
/* DA 37BF */ x _      x _      x _      x _      x _       x _      x _       x _       x _      x _       x _      x _       x _       x _      x _      x _
/* DB 048C */ x _      x _      x _      x _      x _       x _      x _       x _       x _      x _       x _      x _       x _       x _      x _      x _
/* DB 159D */ x _      x _      x _      x _      x _       x _      x _       x _       x _      x _       x _      x _       x _       x _      x _      x _
/* DB 26AE */ x _      x _      x FCLEX  x _      x _       x _      x _       x _       x _      x _       x _      x _       x _       x _      x _      x _
/* DB 37BF */ x _      x _      x _      x _      x _       x _      x _       x _       x _      x _       x _      x _       x _       x _      x _      x _
/* DC 048C */ x FADD   x FADD   x FADD   x FADD   x FADD    x FADD   x FADD    x FADD    x FMUL   x FMUL    x FMUL   x FMUL    x FMUL    x FMUL   x FMUL   x FMUL
/* DC 159D */ x _      x _      x _      x _      x _       x _      x _       x _       x _      x _       x _      x _       x _       x _      x _      x _
/* DC 26AE */ x FSUBR  x FSUBR  x FSUBR  x FSUBR  x FSUBR   x FSUBR  x FSUBR   x FSUBR   x FSUB   x FSUB    x FSUB   x FSUB    x FSUB    x FSUB   x FSUB   x FSUB
/* DC 37BF */ x FDIVR  x FDIVR  x FDIVR  x FDIVR  x FDIVR   x FDIVR  x FDIVR   x FDIVR   x FDIV   x FDIV    x FDIV   x FDIV    x FDIV    x FDIV   x FDIV   x FDIV
/* DD 048C */ x FFREE  x FFREE  x FFREE  x FFREE  x FFREE   x FFREE  x FFREE   x FFREE   x _      x _       x _      x _       x _       x _      x _      x _
/* DD 159D */ x FST    x FST    x FST    x FST    x FST     x FST    x FST     x FST     x FSTP   x FSTP    x FSTP   x FSTP    x FSTP    x FSTP   x FSTP   x FSTP
/* DD 26AE */ x FUCOM  x FUCOM  x FUCOM  x FUCOM  x FUCOM   x FUCOM  x FUCOM   x FUCOM   x FUCOMP x FUCOMP  x FUCOMP x FUCOMP  x FUCOMP  x FUCOMP x FUCOMP x FUCOMP
/* DD 37BF */ x _      x _      x _      x _      x _       x _      x _       x _       x _      x _       x _      x _       x _       x _      x _      x _
/* DE 048C */ x FADDP  x FADDP  x FADDP  x FADDP  x FADDP   x FADDP  x FADDP   x FADDP   x FMULP  x FMULP   x FMULP  x FMULP   x FMULP   x FMULP  x FMULP  x FMULP
/* DE 159D */ x _      x _      x _      x _      x _       x _      x _       x _       x _      x FCOMPP  x _      x _       x _       x _      x _      x _
/* DE 26AE */ x FSUBRP x FSUBRP x FSUBRP x FSUBRP x FSUBRP  x FSUBRP x FSUBRP  x FSUBRP  x FSUBP  x FSUBP   x FSUBP  x FSUBP   x FSUBP   x FSUBP  x FSUBP  x FSUBP
/* DE 37BF */ x FDIVRP x FDIVRP x FDIVRP x FDIVRP x FDIVRP  x FDIVRP x FDIVRP  x FDIVRP  x FDIVP  x FDIVP   x FDIVP  x FDIVP   x FDIVP   x FDIVP  x FDIVP  x FDIVP
/* DF 048C */ x _      x _      x _      x _      x _       x _      x _       x _       x _      x _       x _      x _       x _       x _      x _      x _
/* DF 159D */ x _      x _      x _      x _      x _       x _      x _       x _       x _      x _       x _      x _       x _       x _      x _      x _
/* DF 26AE */ x FSTSW  x _      x _      x _      x _       x _      x _       x _       x _      x _       x _      x _       x _       x _      x _      x _
/* DF 37BF */ x _      x _      x _      x _      x _       x _      x _       x _       x _      x _       x _      x _       x _       x _      x _      x _
};
//------------------------------------------------------------------------------
const x86_instruction::instruction_pointer x86_i386::escMOD[8][8] =
{         // 0       1       2       3        4        5        6        7
/* D8 */ { o FADD  x FMUL  x FCOM  x FCOMP  x FSUB   x FSUBR  x FDIV   x FDIVR  },
/* D9 */ { o FLD   x _     x FST   x FSTP   x _      x FLDCW  x _      x FSTCW  },
/* DA */ { o FIADD x FIMUL x FICOM x FICOMP x FISUB  x FISUBR x FIDIV  x FIDIVR },
/* DB */ { o FILD  x _     x FIST  x FISTP  x _      x FLD    x _      x FSTP   },
/* DC */ { o FADD  x FMUL  x FCOM  x FCOMP  x FSUB   x FSUBR  x FDIV   x FDIVR  },
/* DD */ { o FLD   x _     x FST   x FSTP   x _      x _      x _      x FSTSW  },
/* DE */ { o FIADD x FIMUL x FICOM x FICOMP x FISUB  x FISUBR x FIDIV  x FIDIVR },
/* DF */ { o FILD  x _     x FIST  x FISTP  x FBLD   x FILD   x FBSTP  x FISTP  },
};
//------------------------------------------------------------------------------
#undef o
#undef x
//------------------------------------------------------------------------------
x86_i386::~x86_i386()
{
    delete allocator;
}
//------------------------------------------------------------------------------
bool x86_i386::Initialize(allocator_t* allocator)
{
    if (allocator == nullptr)
        return false;
    this->allocator = allocator;

    memory = (uint8_t*)allocator->address();
    memory_size = allocator->max_size();

    allocator->allocate(4096);
    allocator->allocate(65536, memory_size - 65536);

    EIP = 4096;
    ESP = (uint32_t)memory_size - 16;
    EFLAGS = 0b0000001000000010;

    stack = memory + ESP;

    return true;
}
//------------------------------------------------------------------------------
bool x86_i386::Run()
{
    while (EIP) {
        if (Step('INTO') == false)
            return false;
    }
    return true;
}
//------------------------------------------------------------------------------
bool x86_i386::Step(int type)
{
    auto eip = EIP;
    auto esp = ESP;
    while (EIP) {
        Format format;
        StepInternal(format);
        Fixup(format, *this);
        format.operation(*this, *this, format, format.operand[0].memory, format.operand[1].memory, format.operand[2].memory);
        if (EIP == 0) {
            EIP = eip;
            return false;
        }
        if (EIP >= memory_size) {
            auto count = exception(this, EIP);
            EIP = Pop32();
            ESP += count;
        }
        switch (type) {
        case 'INTO':
            return true;
        case 'OVER':
            if (ESP >= esp || EIP - eip < 16)
                return true;
            continue;
        case 'OUT ':
            if (strcmp(format.instruction, "RET") == 0)
                return true;
            break;
        }
        eip = EIP;
    }
    return true;
}
//------------------------------------------------------------------------------
bool x86_i386::Jump(size_t address)
{
    if (address > memory_size)
        return false;
    EIP = (uint32_t)address;
    return true;
}
//------------------------------------------------------------------------------
void x86_i386::Exception(size_t(*callback)(miCPU*, size_t))
{
    exception = callback;
}
//------------------------------------------------------------------------------
allocator_t* x86_i386::Allocator() const
{
    return allocator;
}
//------------------------------------------------------------------------------
uint8_t* x86_i386::Memory(size_t base, size_t size) const
{
    if (size == 0) {
        if (base + size >= memory_size)
            return nullptr;
        return memory + base;
    }
    return (uint8_t*)allocator->allocate(size, base);
}
//------------------------------------------------------------------------------
size_t x86_i386::Stack() const
{
    return ESP;
}
//------------------------------------------------------------------------------
std::string x86_i386::Status() const
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

    size_t now = 0;
    auto push_second_line = [&](const char* format, ...) {
        va_list va;
        va_start(va, format);
        vsnprintf(temp, 32, format, va);
        va_end(va);
        if (now != output.size()) {
            now = output.find('\n', now);
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
        now = now + 4 + len + 1;
    };

    // CPU
    for (int i = 0; i < 8; ++i) {
        push_first_line("%-8s%08X", REG32[i], regs[i].d);
    }
    push_first_line("");
    push_first_line("%-8s%08X", "EIP", ip.d);
    push_first_line("%-8s%08X", "EFLAGS", flags.d);

    for (int i = 0; i < 16; ++i) {
        static const char* name = "1N11ODITSZ1A1P1C";
        temp[i] = (flags.w & (1 << (15 - i))) ? name[i] : '.';
    }
    push_first_line("%.16s", temp);

    // FPU
    for (int i = 0; i < 8; ++i) {
        push_second_line("ST(%d)   %016llX", i, (uint64_t&)sts[(status._TOP + i) % 8].d);
    }
    push_second_line("");
    push_second_line("%-8s%04X", "CONTROL", control.w);
    push_second_line("%-8s%04X", "STATUS", status.w);

    return output;
}
//------------------------------------------------------------------------------
std::string x86_i386::Disassemble(int count) const
{
    std::string output;

    x86_i386 x86;
    for (int i = 0; i < 8; ++i)
        x86.regs[i] = regs[i];
    x86.flags = flags;
    x86.ip = ip;
    x86.memory = memory;

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
        x86.StepInternal(format);
        output += Disasm(format, x86);
        output += '\n';

        for (uint32_t i = 0; i < 16; ++i) {
            if (address + i >= EIP) {
                output.insert(insert, 2, ' ');
                continue;
            }
            snprintf(temp, 64, "%02X", memory[address + i]);
            output.insert(insert, temp);
            insert += 2;
        }
    }

    return output;
}
//------------------------------------------------------------------------------
void x86_i386::StepInternal(Format& format)
{
    format.length = 1;
    format.width = 32;
    format.repeat = false;

    for (;;) {
        opcode = memory + EIP;
        one[opcode[0]](format, opcode);
        EIP += format.length;
        if (format.operation)
            break;
    }
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x86_i386::ESC(Format& format, const uint8_t* opcode)
{
    if ((opcode[1] & 0b11000000) == 0b11000000) {
        uint16_t index = 0;
        index |= (opcode[0] & 0b00000111) << 6;
        index |= (opcode[1] & 0b00111111);
        esc[index](format, opcode);
    }
    else {
        uint8_t l = (opcode[0] & 0b00000111);
        uint8_t r = (opcode[1] & 0b00111000) >> 3;
        escMOD[l][r](format, opcode);
    }
}
//------------------------------------------------------------------------------
void x86_i386::TWO(Format& format, const uint8_t* opcode)
{
    format.length = 2;
    two[opcode[1]](format, opcode);
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x86_i386::grp1(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    group[1][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i386::grp2(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    group[2][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i386::grp3(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    group[3][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i386::grp4(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    group[4][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i386::grp5(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[1] >> 3) & 0b111;
    group[5][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i386::grp6(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[2] >> 3) & 0b111;
    group[6][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i386::grp7(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[2] >> 3) & 0b111;
    group[7][nnn](format, opcode);
}
//------------------------------------------------------------------------------
void x86_i386::grp8(Format& format, const uint8_t* opcode)
{
    int nnn = (opcode[2] >> 3) & 0b111;
    group[8][nnn](format, opcode);
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x86_i386::OSIZE(Format& format, const uint8_t* opcode)
{
    format.width = 16;
}
//------------------------------------------------------------------------------
void x86_i386::ASIZE(Format& format, const uint8_t* opcode)
{
    format.address = 16;
}
//------------------------------------------------------------------------------
