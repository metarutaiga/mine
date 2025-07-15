#pragma once

#include <stdint.h>
#include <functional>
#include <string>

struct x86_format
{
protected:
    struct Format
    {
        struct Operand
        {
            bool adr;
            bool imm;
            bool reg;

            int scale;
            int index;
            int base;
            int32_t displacement;

            uint64_t address;
            uint8_t* memory;
        };
        int size = 0;
        int length = 1;
        const char* instruction = "";
        Operand operand[3] = {};

        std::string disassembly;
        std::function<void(const Format&, void*, const void*)> operation;
    };
};
