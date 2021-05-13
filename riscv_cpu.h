//==============================================================================
// The RISC-V Instruction Set Manual
// Volume I: Unprivileged ISA
// Document Version 20191213
// December 13, 2019
//==============================================================================

#pragma once

#include <stddef.h>
#include "riscv_instruction.h"

struct riscv_cpu : public riscv_instruction
{
    riscv_cpu();
    ~riscv_cpu();

    void program(const void* code, size_t size);

    bool issue();
    bool run();
    bool runOnce();

public:
    uintptr_t* stack;
    uintptr_t reservation;
    register_t x[32];
    uintptr_t pc;

    uintptr_t begin;
    uintptr_t end;

    // Environment Call and Breakpoints
    void (*environmentCall)(riscv_cpu& cpu);
    void (*environmentBreakpoint)(riscv_cpu& cpu);

protected:
    typedef void instruction();
    typedef void (riscv_cpu::*instruction_pointer)();

    // RV32I Base Instruction Set
    instruction LUI;
    instruction AUIPC;
    instruction JAL;
    instruction JALR;
    instruction BEQ;
    instruction BNE;
    instruction BLT;
    instruction BGE;
    instruction BLTU;
    instruction BGEU;
    instruction LB;
    instruction LH;
    instruction LW;
    instruction LBU;
    instruction LHU;
    instruction SB;
    instruction SH;
    instruction SW;
    instruction ADDI;
    instruction SLTI;
    instruction SLTIU;
    instruction XORI;
    instruction ORI;
    instruction ANDI;
    instruction SLLI;
    instruction SRLI;
    instruction SRAI;
    instruction ADD;
    instruction SUB;
    instruction SLL;
    instruction SLT;
    instruction SLTU;
    instruction XOR;
    instruction SRL;
    instruction SRA;
    instruction OR;
    instruction AND;
    instruction FENCE;
    instruction ECALL;
    instruction EBREAK;

    // RV64I Base Instruction Set
    instruction LWU;
    instruction LD;
    instruction SD;
    instruction ADDIW;
    instruction SLLIW;
    instruction SRLIW;
    instruction SRAIW;
    instruction ADDW;
    instruction SUBW;
    instruction SLLW;
    instruction SRLW;
    instruction SRAW;

    // RV32/RV64 Zifencei Standard Extension
    instruction FENCE_I;

    // RV32/RV64 Zicsr Standard Extension
    instruction CSRRW;
    instruction CSRRS;
    instruction CSRRC;
    instruction CSRRWI;
    instruction CSRRSI;
    instruction CSRRCI;

    // RV32M Standard Extension
    instruction MUL;
    instruction MULH;
    instruction MULHSU;
    instruction MULHU;
    instruction DIV;
    instruction DIVU;
    instruction REM;
    instruction REMU;

    // RV64M Standard Extension
    instruction MULW;
    instruction DIVW;
    instruction DIVUW;
    instruction REMW;
    instruction REMUW;

    // RV32A Standard Extension
    instruction LR_W;
    instruction SC_W;
    instruction AMOSWAP_W;
    instruction AMOADD_W;
    instruction AMOXOR_W;
    instruction AMOAND_W;
    instruction AMOOR_W;
    instruction AMOMIN_W;
    instruction AMOMAX_W;
    instruction AMOMINU_W;
    instruction AMOMAXU_W;

    // RV64A Standard Extension
    instruction LR_D;
    instruction SC_D;
    instruction AMOSWAP_D;
    instruction AMOADD_D;
    instruction AMOXOR_D;
    instruction AMOAND_D;
    instruction AMOOR_D;
    instruction AMOMIN_D;
    instruction AMOMAX_D;
    instruction AMOMINU_D;
    instruction AMOMAXU_D;

    // RV32F Standard Extension
    instruction FLW;
    instruction FSW;
    instruction FMADD_S;
    instruction FMSUB_S;
    instruction FNMSUB_S;
    instruction FNMADD_S;
    instruction FADD_S;
    instruction FSUB_S;
    instruction FMUL_S;
    instruction FDIV_S;
    instruction FSQRT_S;
    instruction FSGNJ_S;
    instruction FSGNJN_S;
    instruction FSGNJX_S;
    instruction FMIN_S;
    instruction FMAX_S;
    instruction FCVT_W_S;
    instruction FCVT_WU_S;
    instruction FMV_X_W;
    instruction FEQ_S;
    instruction FLT_S;
    instruction FLE_S;
    instruction FCLASS_S;
    instruction FCVT_S_W;
    instruction FCVT_S_WU;
    instruction FMV_W_X;

    // RV64F Standard Extension
    instruction FCVT_L_S;
    instruction FCVT_LU_S;
    instruction FCVT_S_L;
    instruction FCVT_S_LU;

    // RV32D Standard Extension
    instruction FLD;
    instruction FSD;
    instruction FMADD_D;
    instruction FMSUB_D;
    instruction FNMSUB_D;
    instruction FNMADD_D;
    instruction FADD_D;
    instruction FSUB_D;
    instruction FMUL_D;
    instruction FDIV_D;
    instruction FSQRT_D;
    instruction FSGNJ_D;
    instruction FSGNJN_D;
    instruction FSGNJX_D;
    instruction FMIN_D;
    instruction FMAX_D;
    instruction FCVT_S_D;
    instruction FCVT_D_S;
    instruction FEQ_D;
    instruction FLT_D;
    instruction FLE_D;
    instruction FCLASS_D;
    instruction FCVT_W_D;
    instruction FCVT_WU_D;
    instruction FCVT_D_W;
    instruction FCVT_D_WU;

    // RV64D Standard Extension
    instruction FCVT_L_D;
    instruction FCVT_LU_D;
    instruction FMV_X_D;
    instruction FCVT_D_L;
    instruction FCVT_D_LU;
    instruction FMV_D_X;

    // Opcode
    instruction HINT;
    instruction LOAD;
    instruction LOAD_FP;
    instruction MISC_MEM;
    instruction OP_IMM;
    instruction OP_IMM_32;
    instruction STORE;
    instruction STORE_FP;
    instruction AMO;
    instruction OP;
    instruction OP_32;
    instruction MADD;
    instruction MSUB;
    instruction NMSUB;
    instruction NMADD;
    instruction OP_FP;
    instruction BRANCH;
    instruction SYSTEM;

    // Opcode map
    static const instruction_pointer map32[8 * 4];
};
