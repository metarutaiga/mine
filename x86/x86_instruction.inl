#pragma once

#include <bit>

//------------------------------------------------------------------------------
extern x86_register& x86;
//------------------------------------------------------------------------------
#define REG(reg)        ((int)((& :: reg - & :: EAX) / (& :: ECX - & :: EAX)))
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
namespace std {
template<class T> struct make_widened { using type = T; };
template<class T> using make_widened_t = make_widened<T>::type;
template<> struct make_widened<int8_t> { using type = int16_t; };
template<> struct make_widened<int16_t> { using type = int32_t; };
template<> struct make_widened<int32_t> { using type = int64_t; };
template<> struct make_widened<int64_t> { using type = __int128_t; };
template<> struct make_widened<uint8_t> { using type = uint16_t; };
template<> struct make_widened<uint16_t> { using type = uint32_t; };
template<> struct make_widened<uint32_t> { using type = uint64_t; };
template<> struct make_widened<uint64_t> { using type = __uint128_t; };
};
//------------------------------------------------------------------------------
#define _____C  0b000001
#define ____P_  0b000010
#define ___A__  0b000100
#define __Z___  0b001000
#define _S____  0b010000
#define O_____  0b100000
#define _SZ_P_  0b011010
#define OSZAP_  0b111110
#define OSZAPC  0b111111
#define CARRY   false
#define BORROW  true
//------------------------------------------------------------------------------
template<int F, bool B = false, typename L, typename R, typename X, typename Y>
inline void x86_instruction::UpdateFlags(x86_instruction& x86, L& DEST, R TEMP, X SRC1, Y SRC2)
{
    uint64_t bc = ( TEMP & (~SRC1 | SRC2)) | (~SRC1 & SRC2);
    uint64_t cc = (~TEMP & ( SRC1 | SRC2)) | ( SRC1 & SRC2);
    uint64_t pp = std::popcount((uint8_t)TEMP) ^ 1;
    uint64_t bits = sizeof(L) * 8;
    uint64_t sign = (uint64_t)1 << (bits - 1);
    uint64_t sign2 = (uint64_t)1 << (bits - 2);
    uint64_t c = B ? bc : cc;
    if (F & _____C) CF = c &               sign ? 1 : 0;
    if (F & ____P_) PF = pp &                 1 ? 1 : 0;
    if (F & ___A__) AF = c &                  8 ? 1 : 0;
    if (F & __Z___) ZF = TEMP ==              0 ? 1 : 0;
    if (F & _S____) SF = TEMP &            sign ? 1 : 0;
    if (F & O_____) OF = (c ^ (c >> 1)) & sign2 ? 1 : 0;
    DEST = TEMP;
}
//------------------------------------------------------------------------------
