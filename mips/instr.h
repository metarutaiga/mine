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

struct INSTR
{
    // CPU Instruction Formats
    // A CPU instruction is a single 32 - bit aligned word.The major instruction formats are shown in Figure A - 10.
    //
    // I - Type (Immediate).
    // 31......26 25..21 20..16 15..........0
    // [ opcode ] [ rs ] [ rt ] [ immediate ]
    //
    // J - Type (Jump).
    // 31......26 25............0
    // [ opcode ] [ instr_index ]
    //
    // R-Type (Register).
    // 31......26 25..21 20..16 15..11 10...6 5..........0
    // [ opcode ] [ rs ] [ rt ] [ rd ] [ sa ] [ function ]
    //
    // Figure A-10 CPU Instruction Formats
    //
    // opcode       6-bit primary operation code
    // rd           5-bit destination register specifier
    // rs           5-bit source register specifier
    // rt           5-bit target (source/destination) register specifier or used to
    //              specify functions within the primary opcode value REGIMM
    // immediate    16-bit signed immediate used for: logical operands, arithmetic
    //              signed operands, load/store address byte offsets, PC-relative
    //              branch signed instruction displacement
    // instr_index  26-bit index shifted left two bits to supply the low-order 28 bits of
    //              the jump target address.
    // sa           5-bit shift amount
    // function     6-bit function field used to specify functions within the primary
    //              operation code value SPECIAL
    union
    {
        // CPU
        struct
        {
            unsigned int function : 6;
            unsigned int sa : 5;
            unsigned int rd : 5;
            unsigned int rt : 5;
            unsigned int rs : 5;
            unsigned int opcode : 6;
        };
        signed int immediate : 16;
        unsigned int immediate_unsigned : 16;
        unsigned int instr_index : 26;
        signed int offset_16 : 16;
        signed int offset_18 : 18;
        signed int offset_19 : 19;
        signed int offset_21 : 21;
        signed int offset_26 : 26;

        // FPU
        struct
        {
            unsigned int : 6;
            unsigned int fd : 5;
            unsigned int fs : 5;
            unsigned int ft : 5;
            unsigned int fr : 5;
        };
        struct
        {
            unsigned int : 6;
            unsigned int : 5;
            unsigned int : 5;
            unsigned int wt : 5;
        };
        struct
        {
            unsigned int : 6;
            unsigned int : 5;
            unsigned int : 5;
            unsigned int tf : 1;
            unsigned int nd : 1;
            unsigned int cc : 3;
        };

        // Encode
        unsigned int Encode;
    };
};
