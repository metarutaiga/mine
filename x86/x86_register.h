#pragma once

#include <stddef.h>
#include <stdint.h>

struct x86_register
{
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
            uint32_t _CF:1;     // S Carry Flag (CF)
            uint32_t _01:1;     // X 1
            uint32_t _PF:1;     // S Parity Flag (PF)
            uint32_t _03:1;     // X 0
            uint32_t _AF:1;     // S Auxiliary Carry Flag (AF)
            uint32_t _05:1;     // X 0
            uint32_t _ZF:1;     // S Zero Flag (ZF)
            uint32_t _SF:1;     // S Sign Flag (SF)
            uint32_t _TF:1;     // X Trap Flag (TF)
            uint32_t _IF:1;     // X Interrupt Enable Flag (IF)
            uint32_t _DF:1;     // C Direction Flag (DF)
            uint32_t _OF:1;     // S Overflow Flag (OF)
            uint32_t _IOPL:2;   // X I/O Privilege Level (IOPL)
            uint32_t _NT:1;     // X Nested Task (NT)
            uint32_t _15:1;     // X 0
            uint32_t _RF:1;     // X Resume Flag (RF)
            uint32_t _VM:1;     // X Virtual-8086 Mode (VM)
            uint32_t _AC:1;     // X Alignment Check / Access Control (AC)
            uint32_t _VIF:1;    // X Virtual Interrupt Flag (VIF)
            uint32_t _VIP:1;    // X Virtual Interrupt Pending (VIP)
            uint32_t _ID:1;     // X ID Flag (ID)
        };
    };
    register_t regs[8] = {};
    register_t ip = {};
    flags_t flags = {};

public:
    size_t memory_size = 0;
    size_t stack_size = 0;
    uint8_t* memory_address = nullptr;
    uint8_t* stack_address = nullptr;
    uint8_t* opcode = nullptr;

public:
    size_t timestamp_counter = 0;

protected:
    template<int F, bool B, typename L, typename R, typename X = R, typename Y = R>
    static void UpdateFlags(x86_register& x86, L& DEST, R TEMP, X SRC1 = X(), Y SRC2 = Y());
};
