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

#if defined(_MSC_VER)
#    include <crtdefs.h>
#else
#    include <stdint.h>
#endif
#include "instr.h"
#include "sysctl.h"
#include "fpu.h"

#ifndef MIPS_BITS
#    if defined(_M_AMD64) || defined(__amd64__)
#        define MIPS_BITS 64
#    elif defined(__LP64__)
#        define MIPS_BITS 64
#    else
#        define MIPS_BITS 32
#    endif
#endif

#if defined(_DEBUG)
#    if defined(__ANDROID__)
#        include <android/log.h>
#        undef MIPS_DEBUG
#        define MIPS_DEBUG(name, format, ...) __android_log_print(ANDROID_LOG_DEBUG, "MIPS", "%014zX:%08X %-10s " format, PC, Encode, name, __VA_ARGS__);
#    elif defined(__APPLE__)
#        include <stdio.h>
#        undef MIPS_DEBUG
#        define MIPS_DEBUG(name, format, ...) printf("MIPS\t" "%014zX:%08X %-10s " format "\n", PC, Encode, name, __VA_ARGS__);
#    else
#        include <stdio.h>
#        undef MIPS_DEBUG
#        define MIPS_DEBUG(name, ...) { if (name && name[0]) printf("%s\t", name); printf(__VA_ARGS__); }
#    endif
#else
#    define MIPS_DEBUG(...)
#endif

class CPU : protected INSTR
{
public:

    // REG
    template<typename A, typename B>
    union REG
    {
        A a;
        B b;
        operator A() const
        {
            return a;
        }
        REG& operator = (const A& other)
        {
            a = other;
            return *this;
        }
    };

    // CPU
    intptr_t PC;
    enum { GPRLEN = sizeof(intptr_t) * 8 };
    union
    {
#if (MIPS_BITS >= 64)
        REG <int, intptr_t> GPR32[32];
        REG <unsigned int, uintptr_t> uGPR32[32];
#else
        intptr_t GPR32[32];
        uintptr_t uGPR32[32];
#endif
        intptr_t GPR[32];
        uintptr_t uGPR[32];
    };

    // COP0
    SYSCTL cop0;

    // COP1 - FPU
    FPU cop1;

    // COP2
    COP cop2;

    // Stack
    intptr_t* Stack;

public:
    CPU();
    ~CPU();

public:
    void Execute(const void* code);
    void BranchDelaySlot();

public:
    typedef void (*SYSTEMCALLFUNCTION)(CPU& cpu, unsigned int code);
    static void SetSystemCall(SYSTEMCALLFUNCTION systemCall);

    typedef void (*NATIVEFUNCTION)(CPU& cpu);
    static void SetNativeFunction(NATIVEFUNCTION nativeFunction);

protected:
    static SYSTEMCALLFUNCTION SystemCallFunction;
    static NATIVEFUNCTION NativeFunction;

public:
    enum
    {
        AddressErrorLoad,   // Fetch address alignment error. User mode fetch reference to kernel address.
        AddressErrorStore,  // Store address alignment error. User mode store to kernel address.
        Breakpoint,         // Execution of BREAK instruction.
        DebugBreakpoint,    // EJTAG Breakpoint (execution of SDBBP instruction).
        IntegerOverflow,    // Execution of an arithmetic instruction that overflowed.
        InvalidOperation,   // Invalid Operation.
        ReservedInstruction,// Execution of a Reserved Instruction.
        SystemCall,         // Execution of SYSCALL instruction.
        Trap,               // Execution of a trap (when trap condition is true).
    };
    void SignalException(int exception, int argument = 0);

public:
    typedef void INSTRUCTION();
    typedef void (CPU::*FINSTRUCTION)();

    // Reserved
    inline void ____()
    {
    }

    // Field
    INSTRUCTION POP06;
    INSTRUCTION POP07;
    INSTRUCTION POP10;
    INSTRUCTION POP26;
    INSTRUCTION POP27;
    INSTRUCTION POP30;
    INSTRUCTION POP66;
    INSTRUCTION POP76;
    INSTRUCTION SPECIAL;
    INSTRUCTION SPECIAL3;
    INSTRUCTION REGIMM;
    INSTRUCTION COP0;
    INSTRUCTION COP1;
    INSTRUCTION COP2;
    INSTRUCTION PCREL;
    INSTRUCTION SOP3x;
    INSTRUCTION BSHFL;
    INSTRUCTION DBSHFL;

    // Table 5.2 Naturally Aligned CPU Load / Store Instructions
    // Mnemonic             Instruction                                                                 Defined in MIPS ISA
    INSTRUCTION LB;         // Load Byte                                                                MIPS32
    INSTRUCTION LBE;        // Load Byte EVA                                                            MIPS32 Release 5
    INSTRUCTION LBU;        // Load Byte Unsigned                                                       MIPS32
    INSTRUCTION LBUE;       // Load Byte Unsigned EVA                                                   MIPS32 Release 5
    INSTRUCTION LD;         // Load Doubleword                                                          MIPS64
    INSTRUCTION LH;         // Load Halfword                                                            MIPS32
    INSTRUCTION LHE;        // Load Halfword EVA                                                        MIPS32 Release 5
    INSTRUCTION LHU;        // Load Halfword Unsigned                                                   MIPS32
    INSTRUCTION LHUE;       // Load Halfword Unsigned EVA                                               MIPS32 Release 5
    INSTRUCTION LW;         // Load Word                                                                MIPS32
    INSTRUCTION LWE;        // Load Word EVA                                                            MIPS32 Release 5
    INSTRUCTION LWU;        // Load Word Unsigned                                                       MIPS64
    INSTRUCTION SB;         // Store Byte                                                               MIPS32
    INSTRUCTION SBE;        // Store Byte EVA                                                           MIPS32 Release 5
    INSTRUCTION SD;         // Store Doubleword                                                         MIPS64
    INSTRUCTION SH;         // Store Halfword                                                           MIPS32
    INSTRUCTION SHE;        // Store Halfword EVA                                                       MIPS32 Release 5
    INSTRUCTION SW;         // Store Word                                                               MIPS32
    INSTRUCTION SWE;        // Store Word EVA                                                           MIPS32 Release 5

    // Table 5.4 PC - relative Loads
    // Mnemonic             Instruction                                                                 Defined in MIPS ISA
    INSTRUCTION LWPC;       // Load Word, PC - relative                                                 MIPS32 Release 6
    INSTRUCTION LWUPC;      // Load Unsigned Word, PC - relative                                        MIPS64 Release 6
    INSTRUCTION LDPC;       // Load Doubleword, PC - relative                                           MIPS64 Release 6

    // Table 5.5 Atomic Update CPU Load and Store Instructions
    // Mnemonic             Instruction                                                                 Defined in MIPS ISA
    INSTRUCTION LL;         // Load Linked Word                                                         MIPS32
    INSTRUCTION LLD;        // Load Linked Doubleword                                                   MIPS64
    INSTRUCTION LLE;        // Load Linked Word - EVA                                                   MIPS32 Release 3.03
    INSTRUCTION SC;         // Store Conditional Word                                                   MIPS32
    INSTRUCTION SCD;        // Store Conditional Doubleword                                             MIPS64
    INSTRUCTION SCE;        // Store Conditional Word EVA                                               MIPS32 Release 3.03

    // Table 5.6 Coprocessor Load and Store Instructions
    // Mnemonic             Instruction                                                                 Defined in MIPS ISA
    INSTRUCTION LDC1;       // Load Doubleword to Coprocessor - 1                                       MIPS32
    INSTRUCTION LDC2;       // Load Doubleword to Coprocessor - 2                                       MIPS32
    INSTRUCTION LWC1;       // Load Word to Coprocessor - 1                                             MIPS32
    INSTRUCTION LWC2;       // Load Word to Coprocessor - 2                                             MIPS32
    INSTRUCTION SDC1;       // Store Doubleword from Coprocessor - 1                                    MIPS32
    INSTRUCTION SDC2;       // Store Doubleword from Coprocessor - 2                                    MIPS32
    INSTRUCTION SWC1;       // Store Word from Coprocessor - 1                                          MIPS32
    INSTRUCTION SWC2;       // Store Word from Coprocessor - 2                                          MIPS32

    // Table 5.8 ALU Instructions With a 16 - bit Immediate Operand
    // Mnemonic             Instruction                                                                 Defined in MIPS ISA
    INSTRUCTION ADDIU;      // Add Immediate Unsigned Word                                              MIPS32
    INSTRUCTION ANDI;       // And Immediate                                                            MIPS32
    INSTRUCTION DADDIU;     // Doubleword Add Immediate Unsigned                                        MIPS64
    INSTRUCTION LUI;        // Load Upper Immediate                                                     MIPS32
    INSTRUCTION ORI;        // Or Immediate                                                             MIPS32
    INSTRUCTION SLTI;       // Set on Less Than Immediate                                               MIPS32
    INSTRUCTION SLTIU;      // Set on Less Than Immediate Unsigned                                      MIPS32
    INSTRUCTION XORI;       // Exclusive Or Immediate                                                   MIPS32

    // Table 5.9 Three - Operand ALU Instructions
    // Mnemonic             Instruction                                                                 Defined in MIPS ISA
    INSTRUCTION ADD;        // Add Word                                                                 MIPS32
    INSTRUCTION ADDU;       // Add Unsigned Word                                                        MIPS32
    INSTRUCTION AND;        // And                                                                      MIPS32
    INSTRUCTION DADD;       // Doubleword Add                                                           MIPS64
    INSTRUCTION DADDU;      // Doubleword Add Unsigned                                                  MIPS64
    INSTRUCTION DSUB;       // Doubleword Subtract                                                      MIPS64
    INSTRUCTION DSUBU;      // Doubleword Subtract Unsigned                                             MIPS64
    INSTRUCTION NOR;        // Nor                                                                      MIPS32
    INSTRUCTION OR;         // Or                                                                       MIPS32
    INSTRUCTION SEB;        // Sign - Extend Byte                                                       MIPS32 Release 2
    INSTRUCTION SEH;        // Sign - Extend Halfword                                                   MIPS32 Release 2
    INSTRUCTION SLT;        // Set on Less Than                                                         MIPS32
    INSTRUCTION SLTU;       // Set on Less Than Unsigned                                                MIPS32
    INSTRUCTION SUB;        // Subtract Word                                                            MIPS32
    INSTRUCTION SUBU;       // Subtract Unsigned Word                                                   MIPS32
    INSTRUCTION XOR;        // Exclusive Or                                                             MIPS32

    // Table 5.10 Two - Operand ALU Instructions
    // Mnemonic             Instruction                                                                 Defined in MIPS ISA
    INSTRUCTION CLO;        // Count Leading Ones in Word                                               MIPS32
    INSTRUCTION CLZ;        // Count Leading Zeros in Word                                              MIPS32
    INSTRUCTION DCLO;       // Count Leading Ones in Doubleword                                         MIPS64
    INSTRUCTION DCLZ;       // Count Leading Zeros in Doubleword                                        MIPS64

    // Table 5.11 Shift Instructions
    // Mnemonic             Instruction                                                                 Defined in MIPS ISA
    INSTRUCTION ALIGN;      // Extract byte - aligned word from concatenation of two words              MIPS32 Release 6
    INSTRUCTION DALIGN;     // Extract byte aligned doubleword from concatenation of two doublewords    MIPS64 Release 6
    INSTRUCTION BITSWAP;    // Swap bits in every byte of word operand                                  MIPS32 Release 6
    INSTRUCTION DBITSWAP;   // Swap bits in every byte of doubleword operand                            MIPS64 Release 6
    INSTRUCTION DROTR;      // Doubleword Rotate Right                                                  MIPS64 Release 2
    INSTRUCTION DROTR32;    // Doubleword Rotate Right Plus 32                                          MIPS64 Release 2
    INSTRUCTION DROTRV;     // Doubleword Rotate Right Variable                                         MIPS64 Release 2
    INSTRUCTION DSLL;       // Doubleword Shift Left Logical                                            MIPS64
    INSTRUCTION DSLL32;     // Doubleword Shift Left Logical + 32                                       MIPS64
    INSTRUCTION DSLLV;      // Doubleword Shift Left Logical Variable                                   MIPS64
    INSTRUCTION DSRA;       // Doubleword Shift Right Arithmetic                                        MIPS64
    INSTRUCTION DSRA32;     // Doubleword Shift Right Arithmetic + 32                                   MIPS64
    INSTRUCTION DSRAV;      // Doubleword Shift Right Arithmetic Variable                               MIPS64
    INSTRUCTION DSRL;       // Doubleword Shift Right Logical                                           MIPS64
    INSTRUCTION DSRL32;     // Doubleword Shift Right Logical + 32                                      MIPS64
    INSTRUCTION DSRLV;      // Doubleword Shift Right Logical Variable                                  MIPS64
    INSTRUCTION ROTR;       // Rotate Word Right                                                        MIPS32 Release 2
    INSTRUCTION ROTRV;      // Rotate Word Right Variable                                               MIPS32 Release 2
    INSTRUCTION SLL;        // Shift Word Left Logical                                                  MIPS32
    INSTRUCTION SLLV;       // Shift Word Left Logical Variable                                         MIPS32
    INSTRUCTION SRA;        // Shift Word Right Arithmetic                                              MIPS32
    INSTRUCTION SRAV;       // Shift Word Right Arithmetic Variable                                     MIPS32
    INSTRUCTION SRL;        // Shift Word Right Logical                                                 MIPS32
    INSTRUCTION SRLV;       // Shift Word Right Logical Variable                                        MIPS32

    // Table 5.13 Same - width Multiply / Divide Instructions (Release 6)
    // Mnemonic             Instruction                                                                 Defined in MIPS ISA
    INSTRUCTION MUL;        // Multiply word, Low part, signed                                          MIPS32 Release 6
    INSTRUCTION MUH;        // Multiply word, High part, signed                                         MIPS32 Release 6
    INSTRUCTION MULU;       // Multiply word, How part, Unsigned                                        MIPS32 Release 6
    INSTRUCTION MUHU;       // Multiply word, High part, Unsigned                                       MIPS32 Release 6
    INSTRUCTION DMUL;       // Multiply doubleword, Low part, signed                                    MIPS64 Release 6
    INSTRUCTION DMUH;       // Multiply doubleword, High part, signed                                   MIPS64 Release 6
    INSTRUCTION DMULU;      // Multiply doubleword, How part, Unsigned                                  MIPS64 Release 6
    INSTRUCTION DMUHU;      // Multiply double word, High part, Unsigned                                MIPS64 Release 6
    INSTRUCTION DIV;        // Divide words, signed                                                     MIPS32 Release 6
    INSTRUCTION MOD;        // Modulus remainder word division, signed                                  MIPS32 Release 6
    INSTRUCTION DIVU;       // Divide words, Unsigned                                                   MIPS32 Release 6
    INSTRUCTION MODU;       // Modulus remainder word division, Unsigned                                MIPS32 Release 6
    INSTRUCTION DDIV;       // Divide doublewords, signed                                               MIPS64 Release 6
    INSTRUCTION DMOD;       // Modulus remainder doubleword division, signed                            MIPS64 Release 6
    INSTRUCTION DDIVU;      // Divide doublewords, Unsigned                                             MIPS64 Release 6
    INSTRUCTION DMODU;      // Modulus remainder doubleword division, Unsigned                          MIPS64 Release 6

    // Table 5.14 Release 6 Compact Branch and Jump Instructions (Release 6)
    // Mnemonic             Instruction                                                                 Defined in MIPS ISA
    INSTRUCTION BC;         // Compact Branch                                                           MIPS32 Release 6
    INSTRUCTION BALC;       // Compact Branch And Link                                                  MIPS32 Release 6
    INSTRUCTION JIC;        // Compact Jump Indexed                                                     MIPS32 Release 6
    INSTRUCTION JIALC;      // Compact Jump Indexed And Link                                            MIPS32 Release 6
    INSTRUCTION BEQZC;      // Compact Branch if Equal to Zero                                          MIPS32 Release 6
    INSTRUCTION BNEZC;      // Compact Branch if Not Equal to Zero                                      MIPS32 Release 6
    INSTRUCTION BLEZC;      // Compact Branch if Less Than or Equal to Zero                             MIPS32 Release 6
    INSTRUCTION BGEZC;      // Compact Branch if Greater Than or Equal to Zero                          MIPS32 Release 6
    INSTRUCTION BGTZC;      // Compact Branch if Greater Than Zero                                      MIPS32 Release 6
    INSTRUCTION BLTZC;      // Compact Branch if Less Than Zero                                         MIPS32 Release 6
    INSTRUCTION BEQZALC;    // Compact Branch if Equal to Zero, And Link                                MIPS32 Release 6
    INSTRUCTION BNEZALC;    // Compact Branch if Not Equal to Zero, And Link                            MIPS32 Release 6
    INSTRUCTION BLEZALC;    // Compact Branch if Less Than or Equal to Zero, And Link                   MIPS32 Release 6
    INSTRUCTION BGEZALC;    // Compact Branch if Greater Than or Equal to Zero, And Link                MIPS32 Release 6
    INSTRUCTION BGTZALC;    // Compact Branch if Greater Than Zero, And Link                            MIPS32 Release 6
    INSTRUCTION BLTZALC;    // Compact Branch if Less Than Zero, And Link                               MIPS32 Release 6
    INSTRUCTION BEQC;       // Compact Branch if Equal                                                  MIPS32 Release 6
    INSTRUCTION BNEC;       // Compact Branch if Not Equal                                              MIPS32 Release 6
    INSTRUCTION BGEC;       // Compact Branch if Greater than or Equal                                  MIPS32 Release 6
    INSTRUCTION BLTC;       // Compact Branch if Less Than                                              MIPS32 Release 6
    INSTRUCTION BGEUC;      // Compact Branch if Greater than or Equal, Unsigned                        MIPS32 Release 6
    INSTRUCTION BLTUC;      // Compact Branch if Less Than, Unsigned                                    MIPS32 Release 6
    INSTRUCTION BLEC;       // Compact Branch if Less Than or Equal                                     MIPS32 Release 6
    INSTRUCTION BGTC;       // Compact Branch if Greater Than                                           MIPS32 Release 6
    INSTRUCTION BLEUC;      // Compact Branch if Less than or Equal, Unsigned                           MIPS32 Release 6
    INSTRUCTION BGTUC;      // Compact Branch if Greater Than, Unsigned                                 MIPS32 Release 6
    INSTRUCTION BOVC;       // Compact Branch if Overflow (word)                                        MIPS32 Release 6
    INSTRUCTION BNVC;       // Compact Branch if No overflow, word                                      MIPS32 Release 6

    // Table 5.15 Unconditional Jump Within a 256 - Megabyte Region
    // Mnemonic             Instruction                                                                 Defined in MIPS ISA
    INSTRUCTION J;          // Jump                                                                     MIPS32
    INSTRUCTION JAL;        // Jump and Link                                                            MIPS32

    // Table 5.16 Unconditional Jump using Absolute Address
    // Mnemonic             Instruction                                                                 Defined in MIPS ISA
    INSTRUCTION JALR;       // Jump and Link Register                                                   MIPS32
    INSTRUCTION JALR_HB;    // Jump and Link Register with Hazard Barrier                               MIPS32 Release 2
    INSTRUCTION JR;         // Jump Register                                                            MIPS32 Release 6 (Assembly idiom)
    INSTRUCTION JR_HB;      // Jump Register with Hazard Barrier                                        MIPS32 Release 6 (Assembly idiom)
    INSTRUCTION NAL;        // No-op and Link                                                           MIPS32 Release 6 (Assembly idiom)
    INSTRUCTION BAL;        // Branch and Link                                                          MIPS32 Release 6 (Assembly idiom)

    // Table 5.17 PC - Relative Conditional Branch Instructions Comparing Two Registers
    // Mnemonic             Instruction                                                                 Defined in MIPS ISA
    INSTRUCTION BEQ;        // Branch on Equal                                                          MIPS32
    INSTRUCTION BNE;        // Branch on Not Equal                                                      MIPS32

    // Table 5.18 PC - Relative Conditional Branch Instructions Comparing With Zero
    // Mnemonic             Instruction                                                                 Defined in MIPS ISA
    INSTRUCTION BGEZ;       // Branch on Greater Than or Equal to Zero                                  MIPS32
    INSTRUCTION BGTZ;       // Branch on Greater Than Zero                                              MIPS32
    INSTRUCTION BLEZ;       // Branch on Less Than or Equal to Zero                                     MIPS32
    INSTRUCTION BLTZ;       // Branch on Less Than Zero                                                 MIPS32

    // Table 5.20 Address Computation and Large Constant Instructions
    // Mnemonic             Instruction                                                                 Defined in MIPS ISA
    INSTRUCTION LSA;        // Left Shift Add (Word)                                                    MIPS32 Release 6
    INSTRUCTION DLSA;       // Left Shift Add (Doubleword)                                              MIPS64 Release 6
    INSTRUCTION AUI;        // Add Upper Immediate (Word)                                               MIPS32 Release 6
    INSTRUCTION DAUI;       // Add Upper Immediate (Doubleword)                                         MIPS64 Release 6
    INSTRUCTION DAHI;       // Add High Immediate (Doubleword)                                          MIPS64 Release 6
    INSTRUCTION DATI;       // Add Top Immediate (Doubleword)                                           MIPS64 Release 6
    INSTRUCTION ADDIUPC;    // Add Immediate Unsigned to PC                                             MIPS32 Release 6
    INSTRUCTION AUIPC;      // Add Upper Immediate to PC                                                MIPS64 Release 6
    INSTRUCTION ALUIPC;     // Add Upper Immediate to Aligned PC                                        MIPS64 Release 6

    // Table 5.21 Serialization Instruction
    // Mnemonic             Instruction                                                                 Defined in MIPS ISA
    INSTRUCTION CACHE;      // Perform Cache Operation                                                  MIPS32
    INSTRUCTION CACHEE;     // Perform Cache Operation EVA                                              MIPS32 Release 3.03
    INSTRUCTION SYNC;       // Synchronize Shared Memory                                                MIPS32
    INSTRUCTION SYNCI;      // Synchronize Caches to Make Instruction Writes Effective                  MIPS32 Release 2

    // Table 5.22 System Call and Breakpoint Instructions
    // Mnemonic             Instruction                                                                 Defined in MIPS ISA
    INSTRUCTION BREAK;      // Breakpoint                                                               MIPS32
    INSTRUCTION RDHWR;      // Read Hardware Register                                                   MIPS32 Release 2
    INSTRUCTION SDBBP;      // Software Debug Breakpoint                                                ETJAG
    INSTRUCTION SIGRIE;     // Signal Reserved Instruction Exception                                    MIPS32 Release 6
    INSTRUCTION SYSCALL;    // System Call                                                              MIPS32

    // Table 5.23 Trap - on - Condition Instructions Comparing Two Registers
    // Mnemonic             Instruction                                                                 Defined in MIPS ISA
    INSTRUCTION TEQ;        // Trap if Equal                                                            MIPS32
    INSTRUCTION TGE;        // Trap if Greater Than or Equal                                            MIPS32
    INSTRUCTION TGEU;       // Trap if Greater Than or Equal Unsigned                                   MIPS32
    INSTRUCTION TLT;        // Trap if Less Than                                                        MIPS32
    INSTRUCTION TLTU;       // Trap if Less Than Unsigned                                               MIPS32
    INSTRUCTION TNE;        // Trap if Not Equal                                                        MIPS32

    // Table 5.26 CPU Conditional Select Instructions (Release 6)
    // Mnemonic             Instruction                                                                 Defined in MIPS ISA
    INSTRUCTION SELEQZ;     // Select GPR rs if GPR rt is Equal to Zero, else select 0                  MIPS32 Release 6
    INSTRUCTION SELNEZ;     // Select GPR rs if GPR rt is Not Equal to Zero, else select 0              MIPS32 Release 6

    // Table 5.27 Prefetch Instructions
    // Mnemonic             Instruction                                                                 Defined in MIPS ISA
    INSTRUCTION PREF;       // Prefetch                                                                 MIPS32
    INSTRUCTION PREFE;      // Prefetch - EVA                                                           MIPS32 Release 3.03

    // Table 3.6 CPU Insert / Extract Instructions
    // Mnemonic             Instruction                                                                 Defined in MIPS ISA
    INSTRUCTION DEXT;       // Doubleword Extract Bit Field                                             MIPS64 Release 2
    INSTRUCTION DEXTM;      // Doubleword Extract Bit Field Middle                                      MIPS64 Release 2
    INSTRUCTION DEXTU;      // Doubleword Extract Bit Field Upper                                       MIPS64 Release 2
    INSTRUCTION DINS;       // Doubleword Insert Bit Field                                              MIPS64 Release 2
    INSTRUCTION DINSM;      // Doubleword Insert Bit Field Middle                                       MIPS64 Release 2
    INSTRUCTION DINSU;      // Doubleword Insert Bit Field Upper                                        MIPS64 Release 2
    INSTRUCTION DSBH;       // Doubleword Swap Bytes Within Halfwords                                   MIPS64 Release 2
    INSTRUCTION DSHD;       // Doubleword Swap Halfwords Within Doublewords                             MIPS64 Release 2
    INSTRUCTION EXT;        // Extract Bit Field                                                        MIPS32 Release 2
    INSTRUCTION INS;        // Insert Bit Field                                                         MIPS32 Release 2
    INSTRUCTION WSBH;       // Word Swap Bytes Within Halfwords

protected:
    static const FINSTRUCTION tableOPCODE[8 * 8];
    static const FINSTRUCTION tableREGIMM[4 * 8];
    static const FINSTRUCTION tablePCREL[4 * 8];
    static const FINSTRUCTION tableSPECIAL[8 * 8];
    static const FINSTRUCTION tableSPECIAL3[8 * 8];
    static const FINSTRUCTION tableMULDIV[4 * 8];
    static const FINSTRUCTION tableBSHFL[4 * 8];
    static const FINSTRUCTION tableDBSHFL[4 * 8];
    static const FINSTRUCTION tableCOP0[4 * 8];
    static const FINSTRUCTION tableCOP0C0[8 * 8];
};
