//==============================================================================
// The RISC-V Instruction Set Manual
// Volume I: Unprivileged ISA
// Document Version 20191213
// December 13, 2019
//==============================================================================

#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include "riscv_cpu.h"

#define HINT HINT

//------------------------------------------------------------------------------
#define o &riscv_cpu::
#define x , o
//------------------------------------------------------------------------------
// Table 24.1: RISC-V base opcode map, inst[1:0]=11
//------------------------------------------------------------------------------
const riscv_cpu::instruction_pointer riscv_cpu::map32[8 * 4] =
{
    o LOAD      x LOAD_FP   x HINT  x MISC_MEM  x OP_IMM    x AUIPC x OP_IMM_32 x HINT
    x STORE     x STORE_FP  x HINT  x AMO       x OP        x LUI   x OP_32     x HINT
    x MADD      x MSUB      x NMSUB x NMADD     x OP_FP     x HINT  x HINT      x HINT
    x BRANCH    x JALR      x HINT  x JAL       x SYSTEM    x HINT  x HINT      x HINT
};
//------------------------------------------------------------------------------
#undef o
#undef x
//------------------------------------------------------------------------------
static jmp_buf buf;
static sig_t sigsegv;
//------------------------------------------------------------------------------
static void register_handler()
{
    sigsegv = signal(SIGSEGV, [](int){ longjmp(buf, 1); });
}
//------------------------------------------------------------------------------
static int check_handler()
{
    return setjmp(buf);
}
//------------------------------------------------------------------------------
static void unregister_handler()
{
    signal(SIGSEGV, sigsegv);
}
//------------------------------------------------------------------------------
riscv_cpu::riscv_cpu()
{
    stack = new uintptr_t[8192];

    environmentCall = [](riscv_cpu&cpu) {};
    environmentBreakpoint = [](riscv_cpu&cpu) {};

    program(nullptr, 0);
}
//------------------------------------------------------------------------------
riscv_cpu::~riscv_cpu()
{
    delete[] stack;
}
//------------------------------------------------------------------------------
void riscv_cpu::program(const void* code, size_t size)
{
    format = 0;

    reservation = 0;
    for (int i = 0; i < 32; ++i)
    {
        x[i] = 0;
    }
    x[2] = (uintptr_t)&stack[8188];
    pc = (uintptr_t)code;

    begin = pc;
    end = pc + size;
}
//------------------------------------------------------------------------------
bool riscv_cpu::issue()
{
    uintptr_t address = pc;
    format = *(uint32_t*)address;

    switch (__builtin_ctz(~opcode))
    {
    case 0:
    case 1:
    {
        if (pc == address)
            pc += 2;
        break;
    }
    case 2:
    case 3:
    case 4:
    {
        instruction_pointer inst = map32[opcode >> 2];
        (this->*inst)();

        if (pc == address)
            pc += 4;
        break;
    }
    case 5:
    {
        if (pc == address)
            pc += 6;
        break;
    }
    case 6:
    {
        if (pc == address)
            pc += 8;
        break;
    }
    default:
        return false;
    }
    x[0] = 0;

    return true;
}
//------------------------------------------------------------------------------
bool riscv_cpu::run()
{
    bool success = false;
    register_handler();
    if (check_handler() == 0)
    {
        while (pc >= begin && pc < end)
        {
            if (issue() == false)
                break;
        }
        success = true;
    }
    unregister_handler();

    return success;
}
//------------------------------------------------------------------------------
bool riscv_cpu::runOnce()
{
    bool success = false;
    register_handler();
    if (check_handler() == 0)
    {
        while (pc >= begin && pc < end)
        {
            if (issue() == false)
                break;
            break;
        }
        success = true;
    }
    unregister_handler();

    return success;
}
//------------------------------------------------------------------------------
void riscv_cpu::HINT()
{
    
}
//------------------------------------------------------------------------------
void riscv_cpu::LOAD()
{
    switch (funct3)
    {
    case 0b000: return LB();
    case 0b001: return LH();
    case 0b010: return LW();
    case 0b011: return LD();
    case 0b100: return LBU();
    case 0b101: return LHU();
    case 0b110: return LWU();
    case 0b111: return HINT();
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::LOAD_FP()
{
    return HINT();
}
//------------------------------------------------------------------------------
void riscv_cpu::MISC_MEM()
{
    switch (funct3)
    {
    case 0b000: return FENCE();
    case 0b001: return FENCE_I();
    case 0b010: return HINT();
    case 0b011: return HINT();
    case 0b100: return HINT();
    case 0b101: return HINT();
    case 0b110: return HINT();
    case 0b111: return HINT();
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::OP_IMM()
{
    switch (funct3)
    {
    case 0b000: return ADDI();
    case 0b001: return SLLI();
    case 0b010: return SLTI();
    case 0b011: return SLTIU();
    case 0b100: return XORI();
    case 0b101: switch (funct7)
                {
                case 0b0000000: return SRLI();
                case 0b0100000: return SRAI();
                default:        return HINT();
                }
    case 0b110: return ORI();
    case 0b111: return ANDI();
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::OP_IMM_32()
{
    switch (funct3)
    {
    case 0b000: return ADDIW();
    case 0b001: return SLLIW();
    case 0b010: return HINT();
    case 0b011: return HINT();
    case 0b100: return HINT();
    case 0b101: switch (funct7)
                {
                case 0b0000000: return SRLIW();
                case 0b0100000: return SRAIW();
                default:        return HINT();
                }
    case 0b110: return HINT();
    case 0b111: return HINT();
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::STORE()
{
    switch (funct3)
    {
    case 0b000: return SB();
    case 0b001: return SH();
    case 0b010: return SW();
    case 0b011: return SD();
    case 0b100: return HINT();
    case 0b101: return HINT();
    case 0b110: return HINT();
    case 0b111: return HINT();
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::STORE_FP()
{
    return HINT();
}
//------------------------------------------------------------------------------
void riscv_cpu::AMO()
{
    switch (funct3)
    {
    case 0b000: return HINT();
    case 0b001: return HINT();
    case 0b010: switch (funct7 >> 2)
                {
                case 0b00000: return AMOADD_W();
                case 0b00001: return AMOSWAP_W();
                case 0b00010: return LR_W();
                case 0b00011: return SC_W();
                case 0b00100: return AMOXOR_W();
                case 0b01000: return AMOOR_W();
                case 0b01100: return AMOAND_W();
                case 0b10000: return AMOMIN_W();
                case 0b10100: return AMOMAX_W();
                case 0b11000: return AMOMINU_W();
                case 0b11100: return AMOMAXU_W();
                default:      return HINT();
                }
    case 0b011: switch (funct7 >> 2)
                {
                case 0b00000: return AMOADD_D();
                case 0b00001: return AMOSWAP_D();
                case 0b00010: return LR_D();
                case 0b00011: return SC_D();
                case 0b00100: return AMOXOR_D();
                case 0b01000: return AMOOR_D();
                case 0b01100: return AMOAND_D();
                case 0b10000: return AMOMIN_D();
                case 0b10100: return AMOMAX_D();
                case 0b11000: return AMOMINU_D();
                case 0b11100: return AMOMAXU_D();
                default:      return HINT();
                }
    case 0b100: return HINT();
    case 0b101: return HINT();
    case 0b110: return HINT();
    case 0b111: return HINT();
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::OP()
{
    switch (funct7)
    {
    case 0b0000000:
        switch (funct3)
        {
        case 0b000: return ADD();
        case 0b001: return SLL();
        case 0b010: return SLT();
        case 0b011: return SLTU();
        case 0b100: return XOR();
        case 0b101: return SRL();
        case 0b110: return OR();
        case 0b111: return AND();
        }
        break;
    case 0b0000001:
        switch (funct3)
        {
        case 0b000: return MUL();
        case 0b001: return MULH();
        case 0b010: return MULHSU();
        case 0b011: return MULHU();
        case 0b100: return DIV();
        case 0b101: return DIVU();
        case 0b110: return REM();
        case 0b111: return REMU();
        }
        break;
    case 0b0100000:
        switch (funct3)
        {
        case 0b000: return SUB();
        case 0b001: return HINT();
        case 0b010: return HINT();
        case 0b011: return HINT();
        case 0b100: return HINT();
        case 0b101: return SRA();
        case 0b110: return HINT();
        case 0b111: return HINT();
        }
        break;
    default:
        return HINT();
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::OP_32()
{
    switch (funct7)
    {
    case 0b0000000:
        switch (funct3)
        {
        case 0b000: return ADDW();
        case 0b001: return SLLW();
        case 0b010: return HINT();
        case 0b011: return HINT();
        case 0b100: return HINT();
        case 0b101: return SRLW();
        case 0b110: return HINT();
        case 0b111: return HINT();
        }
        break;
    case 0b0000001:
        switch (funct3)
        {
        case 0b000: return MULW();
        case 0b001: return HINT();
        case 0b010: return HINT();
        case 0b011: return HINT();
        case 0b100: return DIVW();
        case 0b101: return DIVUW();
        case 0b110: return REMW();
        case 0b111: return REMUW();
        }
        break;
    case 0b0100000:
        switch (funct3)
        {
        case 0b000: return SUBW();
        case 0b001: return HINT();
        case 0b010: return HINT();
        case 0b011: return HINT();
        case 0b100: return HINT();
        case 0b101: return SRAW();
        case 0b110: return HINT();
        case 0b111: return HINT();
        }
        break;
    default:
        return HINT();
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::MADD()
{
    return HINT();
}
//------------------------------------------------------------------------------
void riscv_cpu::MSUB()
{
    return HINT();
}
//------------------------------------------------------------------------------
void riscv_cpu::NMSUB()
{
    return HINT();
}
//------------------------------------------------------------------------------
void riscv_cpu::NMADD()
{
    return HINT();
}
//------------------------------------------------------------------------------
void riscv_cpu::OP_FP()
{
    return HINT();
}
//------------------------------------------------------------------------------
void riscv_cpu::BRANCH()
{
    switch (funct3)
    {
    case 0b000: return BEQ();
    case 0b001: return BNE();
    case 0b010: return HINT();
    case 0b011: return HINT();
    case 0b100: return BLT();
    case 0b101: return BGE();
    case 0b110: return BLTU();
    case 0b111: return BGEU();
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::SYSTEM()
{
    switch (funct3)
    {
    case 0b000: switch (immI())
                {
                case 0b000000000000: return ECALL();
                case 0b000000000001: return EBREAK();
                default:             return HINT();
                }
    case 0b001: return CSRRW();
    case 0b010: return CSRRS();
    case 0b011: return CSRRC();
    case 0b100: return HINT();
    case 0b101: return CSRRWI();
    case 0b110: return CSRRSI();
    case 0b111: return CSRRCI();
    }
}
//------------------------------------------------------------------------------
