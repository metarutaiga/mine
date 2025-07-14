#pragma once

#include <stdint.h>

struct x86_register
{
protected:
    union register_t
    {
        uint64_t q;
        uint32_t d;
        uint16_t w;
        struct
        {
            uint8_t l;
            uint8_t h;
        };
    };
    union flags_t
    {
        uint16_t w;
        struct
        {
            uint8_t l;
            uint8_t h;
        };
        struct
        {
            uint16_t _c:1;  // Carry Flag
            uint16_t _01:1; // 1
            uint16_t _p:1;  // Parity Flag
            uint16_t _03:1; // 0
            uint16_t _a:1;  // Auxiliary Carry
            uint16_t _05:1; // 0
            uint16_t _z:1;  // Zero Flag
            uint16_t _s:1;  // Sign Flag
            uint16_t _t:1;  // Trap Flag
            uint16_t _i:1;  // Interrupt Enable
            uint16_t _d:1;  // Direction Flag
            uint16_t _o:1;  // Overflow Flag
            uint16_t _l:2;  // I/O Privilege Level
            uint16_t _n:1;  // Nested Task Flag
            uint16_t _15:1; // 0
            uint16_t _r:1;  // Resume Flag
            uint16_t _v:1;  // Virtual 8086 Mode
        };
    };
    register_t empty = {};
    register_t regs[16] = {};
    register_t rip = {};
    uint16_t segments = {};
    flags_t eflags = { 0b10 };

    int operand_size = 0;
    bool repeat_string_operation = false;
};
