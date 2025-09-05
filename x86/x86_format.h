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
            enum Type : int8_t { NOP, ADR, IMM, REG, REL };
            Type type;

            enum Flag : int8_t { NONE = 0, BIT8 = 1, BIT16 = 2 };
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
        enum Type : int8_t { X86, X87, MMX, SSE };
        Type type = X86;

        char width = 0;
        char length = 0;
        char address = 0;
        bool repeat = false;
        bool string = false;
        const char* instruction = "";
        const char* segment = "";
        Operand operand[3] = {};

        void (*operation)(x86_register&, x87_register&, mmx_register&, sse_register&, const Format&, void*, const void*, const void*) = nullptr;
    };

    enum
    {
        OPERAND_SIZE    = 0b000001,
        DIRECTION       = 0b000010,
        IMMEDIATE       = 0b000100,
        INDIRECT        = 0b001000,
        RELATIVE        = 0b010000,
        THREE_OPERAND   = 0b100000,
    };

    static void         Decode(Format& format, const uint8_t* opcode, const char* instruction, int offset = 0, int immediate_size = 0, int flags = 0);
    static std::string  Disasm(const Format& format, x86_register& x86);
    static void         Fixup(Format& format, x86_register& x86);

    typedef void instruction(Format&, const uint8_t*);
    typedef void (*instruction_pointer)(Format&, const uint8_t*);

    static const char* const REG8HL[8];
    static const char* const REG8[8];
    static const char* const REG16[8];
    static const char* const REG32[8];
    static const char* const REG64[8];
};
