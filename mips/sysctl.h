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

class SYSCTL : public COP
{
public:
    SYSCTL(CPU& cpu);
    ~SYSCTL();

    void Execute(unsigned int encode);

    void LDC(unsigned int encode);
    void LWC(unsigned int encode);
    void SDC(unsigned int encode);
    void SWC(unsigned int encode);

protected:
    typedef void INSTRUCTION();
    typedef void (SYSCTL::*FINSTRUCTION)();
    
    // Reserved
    inline void ____()
    {
    }

    // Field
    INSTRUCTION C0;

    // Table 3.4 CPU Load, Store, and Memory Control Instructions
    INSTRUCTION LL;         // Load Linked Word
    INSTRUCTION SC;         // Store Conditional Word

    // Table 3.23 Privileged Instructions
    INSTRUCTION DMFC0;      // Doubleword Move from Coprocessor 0
    INSTRUCTION DMTC0;      // Doubleword Move to Coprocessor 0
    INSTRUCTION ERET;       // Exception Return
    INSTRUCTION MFC0;       // Move from Coprocessor 0
    INSTRUCTION MFMC0;      // Move (Release 2 & subsequent)
    INSTRUCTION MTC0;       // Move to Coprocessor 0
    INSTRUCTION RDPGPR;     // Read GPR from Previous Shadow Set (Release 2 & subsequent)
    INSTRUCTION TLBP;       // Probe TLB for Matching Entry
    INSTRUCTION TLBR;       // Read Indexed TLB Entry
    INSTRUCTION TLBINV;     // Invalidate TLB
    INSTRUCTION TLBINVF;    // Invalidate TLB Flush
    INSTRUCTION TLBWI;      // Write Indexed TLB Entry
    INSTRUCTION TLBWR;      // Write Random TLB Entry
    INSTRUCTION WAIT;       // Enter Standby Mode
    INSTRUCTION WRPGPR;     // Write GPR to Previous Shadow Set (Release 2 & subsequent)

    // Table 3.24 EJTAG Instructions
    INSTRUCTION DERET;      // Debug Exception Return
    
protected:
    static const FINSTRUCTION tableRS[4 * 8];
    static const FINSTRUCTION tableC0[8 * 8];
};
