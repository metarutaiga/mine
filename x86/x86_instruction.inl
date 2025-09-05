#pragma once

//------------------------------------------------------------------------------
static x86_register x86;
//------------------------------------------------------------------------------
#define REG(reg)        ((int)((& :: reg - & :: EAX) / (& :: ECX - & :: EAX)))
#define IMM8(m,i)       (*(int8_t*)(m+i))
#define IMM16(m,i)      (*(int16_t*)(m+i))
#define IMM32(m,i)      (*(int32_t*)(m+i))
#define Push16(reg)     (*(uint16_t*)(x86.stack_address = x86.memory_address + (x86.regs[4].q -= sizeof(uint16_t))                   ) = (uint16_t)reg)
#define Push32(reg)     (*(uint32_t*)(x86.stack_address = x86.memory_address + (x86.regs[4].q -= sizeof(uint32_t))                   ) = (uint32_t)reg)
#define Push64(reg)     (*(uint64_t*)(x86.stack_address = x86.memory_address + (x86.regs[4].q -= sizeof(uint64_t))                   ) = (uint64_t)reg)
#define Pop16()         (*(uint16_t*)(x86.stack_address = x86.memory_address + (x86.regs[4].q += sizeof(uint16_t)) - sizeof(uint16_t))                )
#define Pop32()         (*(uint32_t*)(x86.stack_address = x86.memory_address + (x86.regs[4].q += sizeof(uint32_t)) - sizeof(uint32_t))                )
#define Pop64()         (*(uint64_t*)(x86.stack_address = x86.memory_address + (x86.regs[4].q += sizeof(uint64_t)) - sizeof(uint64_t))                )
//------------------------------------------------------------------------------
#define REGISTER_ARGS   x86_register& x86, x87_register& x87, mmx_register& mmx, sse_register& sse
//------------------------------------------------------------------------------
template<typename T>
static auto specialize(auto lambda) {
    static const auto static_lambda = lambda;
    return [](REGISTER_ARGS, const x86_format::Format& format, void* dest, const void* src1, const void* src2) {
        return static_lambda(x86, x87, mmx, sse, format, *(T*)dest, *(T*)src1, *(T*)src2);
    };
}
//------------------------------------------------------------------------------
#define OPERATION() \
        format.operation = [](REGISTER_ARGS, const Format& format, void* dest, const void* src1, const void* src2)
//------------------------------------------------------------------------------
#define BEGIN_OPERATION() { \
        auto operation = [](REGISTER_ARGS, const Format& format, auto& DEST, const auto& SRC1, const auto& SRC2) { \
            const auto& SRC = SRC1; (void)SRC;
//------------------------------------------------------------------------------
#define END_OPERATION_RANGE(low, high) }; \
        if (format.width == 8 && format.width >= low && format.width <= high) \
            format.operation = specialize<uint8_t>(operation); \
        if (format.width == 16 && format.width >= low && format.width <= high) \
            format.operation = specialize<uint16_t>(operation); \
        if (format.width == 32 && format.width >= low && format.width <= high) \
            format.operation = specialize<uint32_t>(operation); \
        if (format.width == 64 && format.width >= low && format.width <= high) \
            format.operation = specialize<uint64_t>(operation); \
    }
//------------------------------------------------------------------------------
#define END_OPERATION_RANGE_SIGNED(low, high) }; \
        if (format.width == 8 && format.width >= low && format.width <= high) \
            format.operation = specialize<int8_t>(operation); \
        if (format.width == 16 && format.width >= low && format.width <= high) \
            format.operation = specialize<int16_t>(operation); \
        if (format.width == 32 && format.width >= low && format.width <= high) \
            format.operation = specialize<int32_t>(operation); \
        if (format.width == 64 && format.width >= low && format.width <= high) \
            format.operation = specialize<int64_t>(operation); \
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
