#pragma once

#include <stdint.h>
#include <functional>
#include <string>

struct x86_instruction;
struct x87_instruction;

struct x86_format
{
    struct Format
    {
        struct Operand
        {
            enum Type { NOP, ADR, IMM, REG, REL };
            Type type = NOP;

            int scale;
            int index;
            int base;
            int64_t displacement;

            uint64_t address;
            uint8_t* memory;
        };
        int width = 0;
        int length = 0;
        int address = 0;
        bool repeat = false;
        const char* instruction = "";
        Operand operand[3] = {};

        void (*operation)(x86_instruction&, x87_instruction&, const Format&, void*, const void*, const void*) = nullptr;
    };
};
