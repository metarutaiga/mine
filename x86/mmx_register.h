#pragma once

#include <stdint.h>

struct mmx_register
{
    typedef int64_t i64x1 __attribute__((vector_size(8)));
    typedef int32_t i32x2 __attribute__((vector_size(8)));
    typedef int16_t i16x4 __attribute__((vector_size(8)));
    typedef int8_t i8x8 __attribute__((vector_size(8)));
    typedef uint64_t u64x1 __attribute__((vector_size(8)));
    typedef uint32_t u32x2 __attribute__((vector_size(8)));
    typedef uint16_t u16x4 __attribute__((vector_size(8)));
    typedef uint8_t u8x8 __attribute__((vector_size(8)));

    union register_t
    {
        i64x1 i64;
        i32x2 i32;
        i16x4 i16;
        i8x8 i8;
        u64x1 u64;
        u32x2 u32;
        u16x4 u16;
        u8x8 u8;
    };
    register_t regs[8] = {};
};
