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
    case 0b110: return HINT();
    case 0b111: return HINT();
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::LOAD_FP()
{
    
}
//------------------------------------------------------------------------------
void riscv_cpu::MISC_MEM()
{
    
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
    case 0b101: if (funct7 == 0)
                    return SRLI();
                else
                    return SRAI();
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
    case 0b101: if (funct7 == 0)
                    return SRLIW();
                else
                    return SRAIW();
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
    
}
//------------------------------------------------------------------------------
void riscv_cpu::AMO()
{
    
}
//------------------------------------------------------------------------------
void riscv_cpu::OP()
{
    switch (funct3)
    {
    case 0b000: if (funct7 == 0)
                    return ADD();
                else
                    return SUB();
    case 0b001: return SLL();
    case 0b010: return SLT();
    case 0b011: return SLTU();
    case 0b100: return XOR();
    case 0b101: if (funct7 == 0)
                    return SRL();
                else
                    return SRA();
    case 0b110: return OR();
    case 0b111: return AND();
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::OP_32()
{
    switch (funct3)
    {
    case 0b000: if (funct7 == 0)
                    return ADDW();
                else
                    return SUBW();
    case 0b001: return SLLW();
    case 0b010: return HINT();
    case 0b011: return HINT();
    case 0b100: return HINT();
    case 0b101: if (funct7 == 0)
                    return SRLW();
                else
                    return SRAW();
    case 0b110: return HINT();
    case 0b111: return HINT();
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::MADD()
{
    
}
//------------------------------------------------------------------------------
void riscv_cpu::MSUB()
{
    
}
//------------------------------------------------------------------------------
void riscv_cpu::NMSUB()
{
    
}
//------------------------------------------------------------------------------
void riscv_cpu::NMADD()
{
    
}
//------------------------------------------------------------------------------
void riscv_cpu::OP_FP()
{
    
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
    case 0b000: if (immI() == 0)
                    return ECALL();
                else
                    return EBREAK();
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
