//==============================================================================
// The RISC-V Instruction Set Manual
// Volume I: Unprivileged ISA
// Document Version 20191213
// December 13, 2019
//==============================================================================

#pragma once

#include <stdint.h>

struct riscv_instruction
{
    // Chapter 24
    // RV32/64G Instruction Set Listings
    union
    {
        // Format
        uint32_t format;
        // R - Type
        // 31......25 24...20 19...15 14......12 11...7 6........0
        // [ funct7 ] [ rs2 ] [ rs1 ] [ funct3 ] [ rd ] [ opcode ]
        struct
        {
            uint32_t opcode : 7;
            uint32_t rd : 5;
            uint32_t funct3 : 3;
            uint32_t rs1 : 5;
            uint32_t rs2 : 5;
            uint32_t funct7 : 7;
        };
        // I - Type
        // 31.........20 19...15 14......12 11...7 6........0
        // [ imm[11:0] ] [ rs1 ] [ funct3 ] [ rd ] [ opcode ]
        struct
        {
            uint32_t _i_opcode : 7;
            uint32_t _i_rd : 5;
            uint32_t _i_funct3 : 3;
            uint32_t _i_rs1 : 5;
            uint32_t _i : 12;
        };
        // S - Type
        // 31.........25 24...20 19...15 14......12 11.........7 6........0
        // [ imm[11:5] ] [ rs2 ] [ rs1 ] [ funct3 ] [ imm[4:0] ] [ opcode ]
        struct
        {
            uint32_t _s_opcode : 7;
            uint32_t _s : 5;
            uint32_t _s_funct3 : 3;
            uint32_t _s_rs1 : 5;
            uint32_t _s_rs2 : 5;
            uint32_t _ss : 7;
        };
        // B - Type
        // 31............25 24...20 19...15 14......12 11............7 6........0
        // [ imm[12:10:5] ] [ rs2 ] [ rs1 ] [ funct3 ] [ imm[4:1|11] ] [ opcode ]
        struct
        {
            uint32_t _b_opcode : 7;
            uint32_t _b : 1;
            uint32_t _bb : 4;
            uint32_t _b_funct3 : 3;
            uint32_t _b_rs1 : 5;
            uint32_t _b_rs2 : 5;
            uint32_t _bbb : 6;
            uint32_t _bbbb : 1;
        };
        // U - Type
        // 31..........12 11...7 6........0
        // [ imm[31:12] ] [ rd ] [ opcode ]
        struct
        {
            uint32_t _u_opcode : 7;
            uint32_t _u_rd : 5;
            uint32_t _u : 20;
        };
        // J - Type
        // 31.....................12 11...7 6........0
        // [ imm[20|10:1|11|19:12] ] [ rd ] [ opcode ]
        struct
        {
            uint32_t _j_opcode : 7;
            uint32_t _j_rd : 5;
            uint32_t _j : 8;
            uint32_t _jj : 1;
            uint32_t _jjj : 10;
            uint32_t _jjjj : 1;
        };
    };

    uint32_t immI() const
    {
        return _i;
    }

    uint32_t immS() const
    {
        return _s | _ss << 5;
    }

    uint32_t immB() const
    {
        return _b << 11 | _bb << 1 | _bbb << 5 | _bbbb << 12;
    }

    uint32_t immU() const
    {
        return _u << 12;
    }

    uint32_t immJ() const
    {
        return _j << 12 | _jj << 11 | _jjj << 1 | _jjjj << 20;
    }

    int32_t simmI() const
    {
        return (int32_t)immI() << 20 >> 20;
    }

    int32_t simmS() const
    {
        return (int32_t)immS() << 20 >> 20;
    }

    int32_t simmB() const
    {
        return (int32_t)immB() << 19 >> 19;
    }

    int32_t simmU() const
    {
        return (int32_t)immU();
    }

    int32_t simmJ() const
    {
        return (int32_t)immJ() << 11 >> 11;
    }

    union register_t
    {
        uintptr_t value;
        intptr_t s;
        uintptr_t u;
        int64_t s64;
        uint64_t u64;
        int32_t s32;
        uint32_t u32;
        int16_t s16;
        uint16_t u16;
        int8_t s8;
        uint8_t u8;
        operator uintptr_t() const
        {
            return value;
        }
        register_t& operator = (const uintptr_t& other)
        {
            value = other;
            return *this;
        }
    };
};
