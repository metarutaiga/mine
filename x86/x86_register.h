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
        uint64_t q;
        uint32_t d;
        uint16_t w;
        struct
        {
            uint8_t l;
            uint8_t h;
        };
        struct
        {
            uint32_t CF:1;  // S Carry Flag (CF)
            uint32_t _01:1; // X 1
            uint32_t PF:1;  // S Parity Flag (PF)
            uint32_t _03:1; // X 0
            uint32_t AF:1;  // S Auxiliary Carry Flag (AF)
            uint32_t _05:1; // X 0
            uint32_t ZF:1;  // S Zero Flag (ZF)
            uint32_t SF:1;  // S Sign Flag (SF)
            uint32_t TF:1;  // X Trap Flag (TF)
            uint32_t IF:1;  // X Interrupt Enable Flag (IF)
            uint32_t DF:1;  // C Direction Flag (DF)
            uint32_t OF:1;  // S Overflow Flag (OF)
            uint32_t IOPL:2;// X I/O Privilege Level (IOPL)
            uint32_t NT:1;  // X Nested Task (NT)
            uint32_t _15:1; // X 0
            uint32_t RF:1;  // X Resume Flag (RF)
            uint32_t VM:1;  // X Virtual-8086 Mode (VM)
            uint32_t AC:1;  // X Alignment Check / Access Control (AC)
            uint32_t VIF:1; // X Virtual Interrupt Flag (VIF)
            uint32_t VIP:1; // X Virtual Interrupt Pending (VIP)
            uint32_t ID:1;  // X ID Flag (ID)
        };
    };
    register_t empty = {};
    register_t regs[16] = {};
    register_t ip = {};
    flags_t flags = { 0b10 };
    uint16_t segments = {};

    int operand_size = 0;
    bool repeat_string_operation = false;
};
