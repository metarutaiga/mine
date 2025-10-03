#pragma once

#include <stdint.h>
#include <string>

#define HAVE_X64 0

struct x86_register;
struct x87_register;
struct mmx_register;
struct sse_register;

struct x86_format
{
    struct Format
    {
        struct Operand
        {
            enum Type { IMM, REL, ADR, REG, X87, MMX, SSE };
            Type type;

            enum Flag : int8_t { NONE = 0, BIT8 = 1, BIT16 = 2, BIT32 = 3, HIDE = 4 };
            Flag flags;

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

        const char* instruction = "";
        const char* segment = "";
        char width = 0;
        char length = 0;
        char address = 0;
        char operand_count = 0;
        Operand operand[3] = {};
        uint8_t prefix = 0;

        void (*operation)(x86_register&, x87_register&, mmx_register&, sse_register&, const Format&, void*, void*, void*) = nullptr;
    };

    enum
    {
        OPERAND_SIZE    = 0b0000001,
        DIRECTION       = 0b0000010,
        THREE_OPERAND   = 0b0000100,
        IMMEDIATE       = 0b0001000,
        INDIRECT        = 0b0010000,
        RELATIVE        = 0b0011000,
        X87_REGISTER    = 0b0100000,
        MMX_REGISTER    = 0b1000000,
        SSE_REGISTER    = 0b1100000,
    };

    static void         Decode(Format& format, const uint8_t* opcode, const char* instruction, int offset = 0, int immediate_size = 0, int flags = 0);
    static std::string  Disasm(const Format& format, x86_register& x86, x87_register& x87, mmx_register& mmx, sse_register& sse);
    static void         Fixup(Format& format, x86_register& x86, x87_register& x87, mmx_register& mmx, sse_register& sse);

    typedef void instruction(Format&, const uint8_t*);
    typedef void (*instruction_pointer)(Format&, const uint8_t*);

    static const char* const REG8HL[8];
    static const char* const REG8[8];
    static const char* const REG16[8];
    static const char* const REG32[8];
    static const char* const REG64[8];
};
