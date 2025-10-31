#pragma once

//------------------------------------------------------------------------------
#define IMM8(m,i)           (*(int8_t*)(m+i))
#define IMM16(m,i)          (*(int16_t*)(m+i))
#define IMM32(m,i)          (*(int32_t*)(m+i))
#define Push16(reg)         (*(uint16_t*)(x86.stack_address = x86.memory_address + (x86.regs[4].q -= sizeof(uint16_t))                   ) = (uint16_t)reg)
#define Push32(reg)         (*(uint32_t*)(x86.stack_address = x86.memory_address + (x86.regs[4].q -= sizeof(uint32_t))                   ) = (uint32_t)reg)
#define Push64(reg)         (*(uint64_t*)(x86.stack_address = x86.memory_address + (x86.regs[4].q -= sizeof(uint64_t))                   ) = (uint64_t)reg)
#define Pop16()             (*(uint16_t*)(x86.stack_address = x86.memory_address + (x86.regs[4].q += sizeof(uint16_t)) - sizeof(uint16_t))                )
#define Pop32()             (*(uint32_t*)(x86.stack_address = x86.memory_address + (x86.regs[4].q += sizeof(uint32_t)) - sizeof(uint32_t))                )
#define Pop64()             (*(uint64_t*)(x86.stack_address = x86.memory_address + (x86.regs[4].q += sizeof(uint64_t)) - sizeof(uint64_t))                )
#define LSB(v)              (((v)              ) & 1)
#define MSB(v)              (((v) >> (SIZE - 1)) & 1)
#define SMSB(v)             (((v) >> (SIZE - 2)) & 1)
//------------------------------------------------------------------------------
#define Tuple8(h,l)         ((uint16_t(uint8_t(h))  <<  8) | uint8_t(l))
#define Tuple16(h,l)        ((uint32_t(uint16_t(h)) << 16) | uint16_t(l))
#define Tuple32(h,l)        ((uint64_t(uint32_t(h)) << 32) | uint32_t(l))
//------------------------------------------------------------------------------
#define Abs                 __builtin_elementwise_abs
#define CopySign(x, y)      (y < 0 ? -x : x)
#define Max                 __builtin_elementwise_max
#define Min                 __builtin_elementwise_min
#define Saturate(x, a, b)   __builtin_elementwise_min(__builtin_elementwise_max(x, a), b)
#define SaturateAdd         __builtin_elementwise_add_sat
#define SaturateSub         __builtin_elementwise_sub_sat
#define Sqrt                __builtin_elementwise_sqrt
//------------------------------------------------------------------------------
#define REGISTER_ARGS   x86_register& x86, x87_register& x87, mmx_register& mmx, sse_register& sse
//------------------------------------------------------------------------------
template<typename D, typename S>
static auto specialize(auto lambda) {
    static const auto static_lambda = lambda;
    return [](REGISTER_ARGS, const x86_format::Format& format, void* dest, void* src1, void* src2) {
        return static_lambda(x86, x87, mmx, sse, format, *(D*)dest, *(S*)src1, *(S*)src2);
    };
}
//------------------------------------------------------------------------------
#define OPERATION() \
        format.operation = [](REGISTER_ARGS, const Format& format, void* dest, void* src1, void* src2)
//------------------------------------------------------------------------------
#define BEGIN_OPERATION() { \
        auto operation = [](REGISTER_ARGS, const Format& format, auto& DEST, auto& SRC1, auto& SRC2) { \
            auto& DEST1 = DEST; (void)DEST1; \
            auto& DEST2 = (decltype(DEST)&)SRC1; (void)DEST2; \
            auto& SRC = SRC1; (void)SRC;
//------------------------------------------------------------------------------
#define END_OPERATION_RANGE(low, high) }; \
        if (format.width == 8 && format.width >= low && format.width <= high) \
            format.operation = specialize<uint8_t, uint8_t>(operation); \
        if (format.width == 16 && format.width >= low && format.width <= high) \
            format.operation = specialize<uint16_t, uint16_t>(operation); \
        if (format.width == 32 && format.width >= low && format.width <= high) \
            format.operation = specialize<uint32_t, uint32_t>(operation); \
        if (format.width == 64 && format.width >= low && format.width <= high) \
            format.operation = specialize<uint64_t, uint64_t>(operation); \
    }
//------------------------------------------------------------------------------
#define END_OPERATION_RANGE_SIGNED(low, high) }; \
        if (format.width == 8 && format.width >= low && format.width <= high) \
            format.operation = specialize<int8_t, int8_t>(operation); \
        if (format.width == 16 && format.width >= low && format.width <= high) \
            format.operation = specialize<int16_t, int16_t>(operation); \
        if (format.width == 32 && format.width >= low && format.width <= high) \
            format.operation = specialize<int32_t, int32_t>(operation); \
        if (format.width == 64 && format.width >= low && format.width <= high) \
            format.operation = specialize<int64_t, int64_t>(operation); \
    }
//------------------------------------------------------------------------------
#if HAVE_X64
#define END_OPERATION END_OPERATION_RANGE(8, 64)
#define END_OPERATION_SIGNED END_OPERATION_RANGE_SIGNED(8, 64)
#else
#define END_OPERATION END_OPERATION_RANGE(8, 32)
#define END_OPERATION_SIGNED END_OPERATION_RANGE_SIGNED(8, 32)
#endif
//------------------------------------------------------------------------------
#if HAVE_BCD == 0
#define AAA _
#define AAD _
#define AAM _
#define AAS _
#define DAA _
#define DAS _
#endif
//------------------------------------------------------------------------------
#if HAVE_FAR == 0
#define CALLF _
#define JMPF _
#define RETF _
#endif
//------------------------------------------------------------------------------
#if HAVE_INOUT == 0
#define IN _
#define INS _
#define OUT _
#define OUTS _
#endif
//------------------------------------------------------------------------------
#if HAVE_INTERRUPT == 0
#define CLI _
#define IRET _
#define STI _
#endif
//------------------------------------------------------------------------------
#if HAVE_SEGMENT == 0
#define LDS _
#define LES _
#define LSS _
#define LFS _
#define LGS _
#endif
//------------------------------------------------------------------------------
#define ARPL _
#define BOUND _
#define CLTS _
#define FXRSTOR _
#define FXSAVE _
#define HLT _
#define INVD _
#define INVLPG _
#define LAR _
#define LGDT _
#define LIDT _
#define LLDT _
#define LMSW _
#define LSL _
#define LTR _
#define MOVB _
#define RSM _
#define RDMSR _
#define SGDT _
#define SIDT _
#define SLDT _
#define SMSW _
#define STR _
#define SYSENTER _
#define SYSEXIT _
#define VERR _
#define VERW _
#define WBINVD _
#define WRMSR _
//------------------------------------------------------------------------------
