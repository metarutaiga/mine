#pragma once

#include <stdint.h>

struct sse_register
{
    typedef int64_t i64x2 __attribute__((vector_size(16)));
    typedef int32_t i32x4 __attribute__((vector_size(16)));
    typedef int16_t i16x8 __attribute__((vector_size(16)));
    typedef int8_t i8x16 __attribute__((vector_size(16)));
    typedef uint64_t u64x2 __attribute__((vector_size(16)));
    typedef uint32_t u32x4 __attribute__((vector_size(16)));
    typedef uint16_t u16x8 __attribute__((vector_size(16)));
    typedef uint8_t u8x16 __attribute__((vector_size(16)));
    typedef double f64x2 __attribute__((vector_size(16)));
    typedef float f32x4 __attribute__((vector_size(16)));

    union register_t
    {
        i64x2 i64;
        i32x4 i32;
        i16x8 i16;
        i8x16 i8;
        u64x2 u64;
        u32x4 u32;
        u16x8 u16;
        u8x16 u8;
        f64x2 f64;
        f32x4 f32;
    };
    register_t regs[8] = {};
    uint32_t mxcsr = 0;
};
