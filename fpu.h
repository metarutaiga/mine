//==============================================================================
// MIPS(R) Architecture For Programmers Volume II-A: The MIPS64(R) Instruction Set Reference Manual
//
// Document Number: MD00087
// Revision 6.06
// December 15, 2016
//
// Copyright (C) Wave Computing, Inc. All rights reserved.
//==============================================================================

#pragma once

#include "cop.h"

class FPU : public COP
{
    union SIMD
    {
        signed char         s8;
        signed short        s16;
        signed int          s32;
        signed long long    s64;

        unsigned char       u8;
        unsigned short      u16;
        unsigned int        u32;
        unsigned long long  u64;

        float               f32;
        double              f64;

        signed char         vs8[8];
        signed short        vs16[4];
        signed int          vs32[2];
        signed long long    vs64[1];

        unsigned char       vu8[8];
        unsigned short      vu16[4];
        unsigned int        vu32[2];
        unsigned long long  vu64[1];

        float               vf32[2];
        double              vf64[1];
    };

public:
    SIMD FPR[32];

public:
    FPU(CPU& cpu);
    ~FPU();

    void Execute(unsigned int encode);

    void LDC(unsigned int encode);
    void LWC(unsigned int encode);
    void SDC(unsigned int encode);
    void SWC(unsigned int encode);

protected:
    friend class CPU;

    typedef void INSTRUCTION();
    typedef void (FPU::*FINSTRUCTION)();

    // Reserved
    inline void ____()
    {
    }

    // Field
    INSTRUCTION S;
    INSTRUCTION D;
    INSTRUCTION W;
    INSTRUCTION L;

    // Table 7.2 FPU Loads and Stores Using Register+Offset Address Mode
    // Mnemonic             Instruction                                                             Defined in MIPS ISA
    INSTRUCTION LDC1;       // Load Doubleword to Floating Point                                    MIPS32
    INSTRUCTION LWC1;       // Load Word to Floating Point                                          MIPS32
    INSTRUCTION SDC1;       // Store Doubleword to Floating Point                                   MIPS32
    INSTRUCTION SWC1;       // Store Word to Floating Point                                         MIPS32

    // Table 7.4 FPU Move To and From Instructions
    // Mnemonic             Instruction                                                             Defined in MIPS ISA
    INSTRUCTION CFC1;       // Move Control Word From Floating Point                                MIPS32
    INSTRUCTION CTC1;       // Move Control Word To Floating Point                                  MIPS32
    INSTRUCTION DMFC1;      // Doubleword Move From Floating Point                                  MIPS64
    INSTRUCTION DMTC1;      // Doubleword Move To Floating Point                                    MIPS64
    INSTRUCTION MFC1;       // Move Word From Floating Point                                        MIPS32
    INSTRUCTION MFHC1;      // Move Word from High Half of Floating Point Register                  MIPS32 Release 2
    INSTRUCTION MTC1;       // Move Word To Floating Point                                          MIPS32
    INSTRUCTION MTHC1;      // Move Word to High Half of Floating Point Register                    MIPS32 Release 2

    // Table 7.5 FPU IEEE Arithmetic Operations
    // Mnemonic             Instruction                                                             Defined in MIPS ISA
    INSTRUCTION ABS_S;      //
    INSTRUCTION ABS_D;      // Floating Point Absolute Value                                        MIPS32
    INSTRUCTION ADD_S;      //
    INSTRUCTION ADD_D;      // Floating Point Add                                                   MIPS32
    INSTRUCTION DIV_S;      //
    INSTRUCTION DIV_D;      // Floating Point Divide                                                MIPS32
    INSTRUCTION MUL_S;      //
    INSTRUCTION MUL_D;      // Floating Point Multiply                                              MIPS32
    INSTRUCTION NEG_S;      //
    INSTRUCTION NEG_D;      // Floating Point Negate                                                MIPS32
    INSTRUCTION SQRT_S;     //
    INSTRUCTION SQRT_D;     // Floating Point Square Root                                           MIPS32
    INSTRUCTION SUB_S;      //
    INSTRUCTION SUB_D;      // Floating Point Subtract                                              MIPS32

    // Table 7.6 FPU - Approximate Arithmetic Operations
    // Mnemonic             Instruction                                                             Defined in MIPS ISA
    INSTRUCTION RECIP_S;    //
    INSTRUCTION RECIP_D;    // Floating Point Reciprocal Approximation                              MIPS32 Release 2
    INSTRUCTION RSQRT_S;    //
    INSTRUCTION RSQRT_D;    // Floating Point Reciprocal Square Root Approximation                  MIPS32 Release 2

    // Table 7.8 FPU Fused Multiply - Accumulate Arithmetic Operations (Release 6)
    // Mnemonic             Instruction                                                             Defined in MIPS ISA
    INSTRUCTION MADDF_S;    //
    INSTRUCTION MADDF_D;    // Fused Floating Point Multiply Add                                    MIPS32 Release 6
    INSTRUCTION MSUBF_S;    //
    INSTRUCTION MSUBF_D;    // Fused Floating Point Multiply Subtract                               MIPS32 Release 6

    // Table 7.9 Floating Point Comparison Instructions
    // Mnemonic             Instruction                                                             Defined in MIPS ISA
    INSTRUCTION CLASS_S;    //
    INSTRUCTION CLASS_D;    // Scalar Floating - Point Class Mask                                   MIPS32 Release 6
    INSTRUCTION CMP_cond_S; //
    INSTRUCTION CMP_cond_D; // Floating Point Compare (setting FPR)                                 MIPS32 Release 6
    INSTRUCTION MAX_S;      //
    INSTRUCTION MAX_D;      // Floating Point Maximum                                               MIPS32 Release 6
    INSTRUCTION MAXA_S;     //
    INSTRUCTION MAXA_D;     // Floating Point Value with Maximum Absolute Value                     MIPS32 Release 6
    INSTRUCTION MIN_S;      //
    INSTRUCTION MIN_D;      // Floating Point Minimum                                               MIPS32 Release 6
    INSTRUCTION MINA_S;     //
    INSTRUCTION MINA_D;     // Floating Point Value with Minimum Absolute Value                     MIPS32 Release 6

    // Table 7.10 FPU Conversion Operations Using the FCSR Rounding Mode
    // Mnemonic             Instruction                                                             Defined in MIPS ISA
    INSTRUCTION CVT_D_W;    //
    INSTRUCTION CVT_D_L;    //
    INSTRUCTION CVT_D_S;    // Floating Point Convert to Double Floating Point                      MIPS32
    INSTRUCTION CVT_L_S;    //
    INSTRUCTION CVT_L_D;    // Floating Point Convert to Long Fixed Point                           MIPS32 Release 2
    INSTRUCTION CVT_S_W;    //
    INSTRUCTION CVT_S_L;    //
    INSTRUCTION CVT_S_D;    // Floating Point Convert to Single Floating Point                      MIPS32
    INSTRUCTION CVT_W_S;    //
    INSTRUCTION CVT_W_D;    //
    INSTRUCTION CVT_W_L;    // Floating Point Convert to Word Fixed Point                           MIPS32
    INSTRUCTION RINT_S;     //
    INSTRUCTION RINT_D;     // Scalar floating - point round to integer                             MIPS32 Release 6

    // Table 7.11 FPU Conversion Operations Using a Directed Rounding Mode
    // Mnemonic             Instruction                                                             Defined in MIPS ISA
    INSTRUCTION CEIL_L_S;   //
    INSTRUCTION CEIL_L_D;   // Floating Point Ceiling to Long Fixed Point                           MIPS32 Release 2
    INSTRUCTION CEIL_W_S;   //
    INSTRUCTION CEIL_W_D;   // Floating Point Ceiling to Word Fixed Point                           MIPS32
    INSTRUCTION FLOOR_L_S;  //
    INSTRUCTION FLOOR_L_D;  // Floating Point Floor to Long Fixed Point                             MIPS32 Release 2
    INSTRUCTION FLOOR_W_S;  //
    INSTRUCTION FLOOR_W_D;  // Floating Point Floor to Word Fixed Point                             MIPS32
    INSTRUCTION ROUND_L_S;  //
    INSTRUCTION ROUND_L_D;  // Floating Point Round to Long Fixed Point                             MIPS32 Release 2
    INSTRUCTION ROUND_W_S;  //
    INSTRUCTION ROUND_W_D;  // Floating Point Round to Word Fixed Point                             MIPS32
    INSTRUCTION TRUNC_L_S;  //
    INSTRUCTION TRUNC_L_D;  // Floating Point Truncate to Long Fixed Point                          MIPS32 Release 2
    INSTRUCTION TRUNC_W_S;  //
    INSTRUCTION TRUNC_W_D;  // Floating Point Truncate to Word Fixed Point                          MIPS32

    // Table 7.12 FPU Formatted Unconditional Operand Move Instructions
    // Mnemonic             Instruction                                                             Defined in MIPS ISA
    INSTRUCTION MOV_S;      //
    INSTRUCTION MOV_D;      // Floating Point Move                                                  MIPS32

    // Table 7.15 FPU Conditional Select Instructions (Release 6)
    // Mnemonic             Instruction                                                             Defined in MIPS ISA
    INSTRUCTION SEL_S;      //
    INSTRUCTION SEL_D;      // Floating Point Select                                                MIPS32 Release 6
    INSTRUCTION SELEQZ_S;   //
    INSTRUCTION SELEQZ_D;   // Floating Point Select if condition Equal to Zero, Else 0.0           MIPS32 Release 6
    INSTRUCTION SELNEZ_S;   //
    INSTRUCTION SELNEZ_D;   // Floating Point Select if condition is Not Equal to Zero, Else 0.0    MIPS32 Release 6

    // Table 7.18 FPU Conditional Branch Instructions (Release 6)
    // Mnemonic             Instruction                                                             Defined in MIPS ISA
    INSTRUCTION BC1EQZ;     // Branch on FP condition Equal to Zero                                 MIPS32 Release 6
    INSTRUCTION BC1NEZ;     // Branch on FP condition Not Equal to Zero                             MIPS32 Release 6

protected:
    static const FINSTRUCTION tableRS[4 * 8];
    static const FINSTRUCTION tableS[8 * 8];
    static const FINSTRUCTION tableD[8 * 8];
    static const FINSTRUCTION tableW[8 * 8];
    static const FINSTRUCTION tableL[8 * 8];
};
