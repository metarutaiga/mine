#pragma once

#include <stdint.h>
#include <string>

#define HAVE_BCD 0
#define HAVE_FAR 0
#define HAVE_INOUT 0
#define HAVE_INTERRUPT 0
#define HAVE_SEGMENT 0
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
            enum Type { IMM, REL, ADR, REG, SEG, X87, MMX, SSE };
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
        uint8_t width = 0;
        uint8_t length = 0;
        uint8_t address = 0;
        uint8_t operand_count = 0;
        Operand operand[3] = {};
        uint8_t prefix = 0;
        bool string = false;

        void (*operation)(x86_register&, x87_register&, mmx_register&, sse_register&, const Format&, void*, void*, void*) = nullptr;
    };

    enum
    {
        OPERAND_SIZE    = 0b000000001,
        DIRECTION       = 0b000000010,
        THREE_OPERAND   = 0b000000100,
//      RESERVED_08     = 0b000001000,
        INDIRECT        = 0b000010000,
        RELATIVE        = 0b000011000,
        IMM_SIZE        = 0b000100000,
        IMM_8BIT        = 0b001000000,
//      RESERVED_60     = 0b001100000,
        X87_REGISTER    = 0b010000000,
        MMX_REGISTER    = 0b100000000,
        SSE_REGISTER    = 0b110000000,
    };

    static void         Decode(Format& format, const uint8_t* opcode, const char* instruction, int offset = 0, int flags = 0);
    static std::string  Disasm(const Format& format, x86_register& x86, x87_register& x87, mmx_register& mmx, sse_register& sse);
    static void         Fixup(Format& format, x86_register& x86, x87_register& x87, mmx_register& mmx, sse_register& sse);

    typedef void instruction(Format&, const uint8_t*);
    typedef void (*instruction_pointer)(Format&, const uint8_t*);

    static const char* const REG8HL[8];
#if HAVE_X64
    static const char* const REG8[8];
#endif
    static const char* const REG16[8];
    static const char* const REG32[8];
    static const char* const REG64[8];
    static const char* const SEG[8];
};
