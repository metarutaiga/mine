#pragma once

#include <stdint.h>

struct sse_register
{
    typedef double f64x2 __attribute__((vector_size(16)));
    typedef float f32x4 __attribute__((vector_size(16)));
    typedef int64_t i64x2 __attribute__((vector_size(16)));
    typedef int32_t i32x4 __attribute__((vector_size(16)));
    typedef int16_t i16x8 __attribute__((vector_size(16)));
    typedef int8_t i8x16 __attribute__((vector_size(16)));
    typedef uint64_t u64x2 __attribute__((vector_size(16)));
    typedef uint32_t u32x4 __attribute__((vector_size(16)));
    typedef uint16_t u16x8 __attribute__((vector_size(16)));
    typedef uint8_t u8x16 __attribute__((vector_size(16)));

    union register_t
    {
        f64x2 f64;
        f32x4 f32;
        i64x2 i64;
        i32x4 i32;
        i16x8 i16;
        i8x16 i8;
        u64x2 u64;
        u32x4 u32;
        u16x8 u16;
        u8x16 u8;
    };
    union control_t
    {
        uint16_t w;
        uint32_t d;
        struct
        {
            uint32_t _IE:1;     // Invalid Operation Flag
            uint32_t _DE:1;     // Denormal Flag
            uint32_t _ZE:1;     // Divide-by-Zero Flag
            uint32_t _OE:1;     // Overflow Flag
            uint32_t _UE:1;     // Underflow Flag
            uint32_t _PE:1;     // Precision Flag
            uint32_t _DAZ:1;    // Denormals Are Zeros
            uint32_t _IM:1;     // Invalid Operation Mask
            uint32_t _DM:1;     // Denormal Operation Mask
            uint32_t _ZM:1;     // Divide-by-Zero Mask
            uint32_t _OM:1;     // Overflow Mask
            uint32_t _UM:1;     // Underflow Mask
            uint32_t _PM:1;     // Precision Mask
            uint32_t _RC:2;     // Rounding Control
            uint32_t _FTZ:1;    // Flush to Zero
        };
    };
    register_t regs[8] = {};
    control_t mxcsr = {};
    uint8_t version = 0;
};
