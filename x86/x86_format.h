#pragma once

#include <stdint.h>
#include <functional>
#include <string>

#define HAVE_X64 0

struct x86_instruction;
struct x87_instruction;

struct x86_format
{
    struct Format
    {
        struct Operand
        {
            enum Type : int8_t { NOP, ADR, IMM, REG, REL };
            Type type = NOP;

            int8_t scale;
            int8_t index;
            int8_t base;
#if HAVE_X64
            int64_t displacement;
            uint64_t address;
#else
            int32_t displacement;
            uint32_t address;
#endif

            uint8_t* memory;
        };
        char width = 0;
        char length = 0;
        char address = 0;
        bool floating = false;
        bool repeat = false;
        const char* instruction = "";
        const char* segment = "";
        Operand operand[3] = {};

        void (*operation)(x86_instruction&, x87_instruction&, const Format&, void*, const void*, const void*) = nullptr;
    };
};
