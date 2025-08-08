#pragma once

#include <bit>

//------------------------------------------------------------------------------
#define REG(reg)        [](){ x86_register x86; return (int)((&reg - &EAX) / (&ECX - &EAX)); }()
#define IMM8(m,i)       (*(int8_t*)(m+i))
#define IMM16(m,i)      (*(int16_t*)(m+i))
#define IMM32(m,i)      (*(int32_t*)(m+i))
#define Push16(reg)     (*(uint16_t*)(x86.stack = x86.memory + (x86.regs[4].q -= sizeof(uint16_t))                   ) = (uint16_t)reg)
#define Push32(reg)     (*(uint32_t*)(x86.stack = x86.memory + (x86.regs[4].q -= sizeof(uint32_t))                   ) = (uint32_t)reg)
#define Push64(reg)     (*(uint64_t*)(x86.stack = x86.memory + (x86.regs[4].q -= sizeof(uint64_t))                   ) = (uint64_t)reg)
#define Pop16()         (*(uint16_t*)(x86.stack = x86.memory + (x86.regs[4].q += sizeof(uint16_t)) - sizeof(uint16_t))                )
#define Pop32()         (*(uint32_t*)(x86.stack = x86.memory + (x86.regs[4].q += sizeof(uint32_t)) - sizeof(uint32_t))                )
#define Pop64()         (*(uint64_t*)(x86.stack = x86.memory + (x86.regs[4].q += sizeof(uint64_t)) - sizeof(uint64_t))                )
//------------------------------------------------------------------------------
template<typename T>
static auto specialize(auto lambda) {
    static const auto static_lambda = lambda;
    return [](x86_instruction& x86, x87_instruction& x87, const x86_format::Format& format, void* dest, const void* src1, const void* src2) {
        return static_lambda(x86, x87, format, *(T*)dest, *(T*)src1, *(T*)src2);
    };
}
//------------------------------------------------------------------------------
#define BEGIN_OPERATION() { \
        auto operation = [](x86_instruction& x86, x87_instruction& x87, const Format& format, auto& DEST, auto SRC1, auto SRC2) { \
            auto SRC = SRC1; (void)SRC;
#define END_OPERATION }; \
        switch (format.width) { \
        case 8:     format.operation = specialize<uint8_t>(operation);  break; \
        case 16:    format.operation = specialize<uint16_t>(operation); break; \
        case 32:    format.operation = specialize<uint32_t>(operation); break; \
        case 64:    format.operation = specialize<uint64_t>(operation); break; \
        } \
    }
//------------------------------------------------------------------------------
template<int O, int S, int Z, int A, int P, int C, typename L, typename R>
inline void x86_instruction::UpdateFlags(x86_instruction& x86, L& DEST, R TEMP)
{
    typename std::make_signed_t<L> dest = DEST;
    typename std::make_signed_t<R> temp = TEMP;
    uint64_t sign = (uint64_t)1 << (sizeof(L) * 8 - 1);
    if (C == -1)    CF = DEST > TEMP ? 1 : 0;
    if (C ==  1)    CF = DEST < TEMP ? 1 : 0;
    if (P ==  1)    PF = TEMP &    1 ? 1 : 0;
//  if (A ==  1)    AF = TEMP &    1 ? 1 : 0;
    if (Z ==  1)    ZF = TEMP ==   0 ? 1 : 0;
    if (S ==  1)    SF = TEMP & sign ? 1 : 0;
    if (O == -1)    OF = dest > temp ? 1 : 0;
    if (O ==  1)    OF = dest < temp ? 1 : 0;
    DEST = TEMP;
}
//------------------------------------------------------------------------------
template<int O, int S, int Z, int A, int P, int C, typename L, typename R, typename X, typename Y>
inline void x86_instruction::UpdateFlags(x86_instruction& x86, L& DEST, R TEMP, X SRC1, Y SRC2)
{
    uint64_t bc = ( TEMP & (~SRC1 | SRC2)) | (~SRC1 & SRC2);
    uint64_t cc = (~TEMP & ( SRC1 | SRC2)) | ( SRC1 & SRC2);
    uint64_t pp = std::popcount((uint8_t)TEMP) ^ 1;
    uint64_t bits = sizeof(L) * 8;
    uint64_t sign = (uint64_t)1 << (bits - 1);
    uint64_t sign2 = (uint64_t)1 << (bits - 2);
    if (C == -1)    CF = bc &                sign ? 1 : 0;
    if (C ==  1)    CF = cc &                sign ? 1 : 0;
    if (P ==  1)    PF = pp &                   1 ? 1 : 0;
    if (A == -1)    AF = bc &                   8 ? 1 : 0;
    if (A ==  1)    AF = cc &                   8 ? 1 : 0;
    if (Z ==  1)    ZF = TEMP ==                0 ? 1 : 0;
    if (S ==  1)    SF = TEMP &              sign ? 1 : 0;
    if (O == -1)    OF = (bc ^ (bc >> 1)) & sign2 ? 1 : 0;
    if (O ==  1)    OF = (cc ^ (cc >> 1)) & sign2 ? 1 : 0;
    DEST = TEMP;
}
//------------------------------------------------------------------------------
