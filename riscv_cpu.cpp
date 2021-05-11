//==============================================================================
// The RISC-V Instruction Set Manual
// Volume I: Unprivileged ISA
// Document Version 20191213
// December 13, 2019
//==============================================================================

#include <setjmp.h>
#include <signal.h>
#include "riscv_cpu.h"

//------------------------------------------------------------------------------
#define o &riscv_cpu::
#define x , o
//------------------------------------------------------------------------------
// Table 24.1: RISC-V base opcode map, inst[1:0]=11
//------------------------------------------------------------------------------
const riscv_cpu::instruction_pointer riscv_cpu::map32[8 * 4] =
{
    o LOAD      x LOAD_FP   x ____  x MISC_MEM  x OP_IMM    x AUIPC x OP_IMM_32 x ____
    x STORE     x STORE_FP  x ____  x AMO       x OP        x LUI   x OP_32     x ____
    x MADD      x MSUB      x NMSUB x NMADD     x OP_FP     x ____  x ____      x ____
    x BRANCH    x JALR      x ____  x JAL       x SYSTEM    x ____  x ____      x ____
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
    format = 0;
    for (int i = 0; i < 32; ++i)
    {
        x[i] = 0;
    }
    pc = 0;
    begin = 0;
    end = 0;
    x[2] = (uintptr_t)&stack[8188];
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
    pc = (uintptr_t)code;
    begin = pc;
    end = pc + size;
    x[2] = (uintptr_t)&stack[8188];
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
            uintptr_t address = pc;
            format = *(uint32_t*)address;

            if ((opcode & 0b11) == 0b11)
            {
                instruction_pointer inst = map32[opcode >> 2];
                (this->*inst)();
            }
            x[0] = 0;

            if (pc == address)
                pc += 4;
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
        if (pc >= begin && pc < end)
        {
            uintptr_t address = pc;
            format = *(uint32_t*)address;

            if ((opcode & 0b11) == 0b11)
            {
                instruction_pointer inst = map32[opcode >> 2];
                (this->*inst)();
            }
            x[0] = 0;

            if (pc == address)
                pc += 4;
        }
        success = true;
    }
    unregister_handler();

    return success;
}
//------------------------------------------------------------------------------
void riscv_cpu::____()
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
    case 0b110: return ____();
    case 0b111: return ____();
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
    case 0b010: return ____();
    case 0b011: return ____();
    case 0b100: return ____();
    case 0b101: if (funct7 == 0)
                    return SRLIW();
                else
                    return SRAIW();
    case 0b110: return ____();
    case 0b111: return ____();
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
    case 0b100: return ____();
    case 0b101: return ____();
    case 0b110: return ____();
    case 0b111: return ____();
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
    case 0b010: return ____();
    case 0b011: return ____();
    case 0b100: return ____();
    case 0b101: if (funct7 == 0)
                    return SRLW();
                else
                    return SRAW();
    case 0b110: return ____();
    case 0b111: return ____();
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
    case 0b010: return ____();
    case 0b011: return ____();
    case 0b100: return BLT();
    case 0b101: return BGE();
    case 0b110: return BLTU();
    case 0b111: return BGEU();
    }
}
//------------------------------------------------------------------------------
void riscv_cpu::SYSTEM()
{
    switch (rs2)
    {
    case 0: return ECALL();
    case 1: return EBREAK();
    }
}
//------------------------------------------------------------------------------
