#pragma once

#include <stdint.h>

struct x86_format
{
protected:
    struct Format
    {
        struct Operand
        {
            bool adr = false;
            bool imm = false;
            bool reg = false;

            int scale = 0;
            int index = 0;
            int base = -1;
            int32_t displacement = 0;

            uint64_t address = 0;
            uint8_t* memory = nullptr;
        };
        int size = 0;
        int length = 1;
        const char* instruction = "";
        Operand operand[3] = {};
    };
};
