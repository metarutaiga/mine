//==============================================================================
// The RISC-V Instruction Set Manual
// Volume I: Unprivileged ISA
// Document Version 20191213
// December 13, 2019
//==============================================================================

#pragma once

struct riscv_instruction
{
    // Chapter 24
    // RV32/64G Instruction Set Listings
    union
    {
        // Format
        unsigned int format;
        // R - Type
        // 31......25 24...20 19...15 14......12 11...7 6........0
        // [ funct7 ] [ rs2 ] [ rs1 ] [ funct3 ] [ rd ] [ opcode ]
        struct
        {
            unsigned int opcode : 7;
            unsigned int rd : 5;
            unsigned int funct3 : 3;
            unsigned int rs1 : 5;
            unsigned int rs2 : 5;
            unsigned int funct7 : 7;
        };
        // I - Type
        // 31.........20 19...15 14......12 11...7 6........0
        // [ imm[11:0] ] [ rs1 ] [ funct3 ] [ rd ] [ opcode ]
        struct
        {
            unsigned int _i_opcode : 7;
            unsigned int _i_rd : 5;
            unsigned int _i_funct3 : 3;
            unsigned int _i_rs1 : 5;
            unsigned int _i : 12;
        };
        // S - Type
        // 31.........25 24...20 19...15 14......12 11.........7 6........0
        // [ imm[11:5] ] [ rs2 ] [ rs1 ] [ funct3 ] [ imm[4:0] ] [ opcode ]
        struct
        {
            unsigned int _s_opcode : 7;
            unsigned int _s : 5;
            unsigned int _s_funct3 : 3;
            unsigned int _s_rs1 : 5;
            unsigned int _s_rs2 : 5;
            unsigned int _ss : 7;
        };
        // B - Type
        // 31............25 24...20 19...15 14......12 11............7 6........0
        // [ imm[12:10:5] ] [ rs2 ] [ rs1 ] [ funct3 ] [ imm[4:1|11] ] [ opcode ]
        struct
        {
            unsigned int _b_opcode : 7;
            unsigned int _b : 1;
            unsigned int _bb : 4;
            unsigned int _b_funct3 : 3;
            unsigned int _b_rs1 : 5;
            unsigned int _b_rs2 : 5;
            unsigned int _bbb : 6;
            unsigned int _bbbb : 1;
        };
        // U - Type
        // 31..........12 11...7 6........0
        // [ imm[31:12] ] [ rd ] [ opcode ]
        struct
        {
            unsigned int _u_opcode : 7;
            unsigned int _u_rd : 5;
            unsigned int _u : 20;
        };
        // J - Type
        // 31.....................12 11...7 6........0
        // [ imm[20|10:1|11|19:12] ] [ rd ] [ opcode ]
        struct
        {
            unsigned int _j_opcode : 7;
            unsigned int _j_rd : 5;
            unsigned int _j : 8;
            unsigned int _jj : 1;
            unsigned int _jjj : 10;
            unsigned int _jjjj : 1;
        };
    };

    unsigned int immI() const
    {
        return _i;
    }

    unsigned int immS() const
    {
        return _s | _ss << 5;
    }

    unsigned int immB() const
    {
        return _b << 11 | _bb << 1 | _bbb << 5 | _bbbb << 12;
    }

    unsigned int immU() const
    {
        return _u << 12;
    }

    unsigned int immJ() const
    {
        return _j << 12 | _jj << 11 | _jjj << 1 | _jjjj << 20;
    }
};
