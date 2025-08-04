#pragma once

//------------------------------------------------------------------------------
#define REG(reg)        ((int)(((uint8_t*)&reg - (uint8_t*)&EAX) / ((uint8_t*)&ECX - (uint8_t*)&EAX)))
#define IMM8(m,i)       (*(int8_t*)(m+i))
#define IMM16(m,i)      (*(int16_t*)(m+i))
#define IMM32(m,i)      (*(int32_t*)(m+i))
#define Push(reg)       (*(uint32_t*)(x86.stack = x86.memory + (x86.regs[4].q -= sizeof(uint32_t))                   ) = reg)
#define Pop()           (*(uint32_t*)(x86.stack = x86.memory + (x86.regs[4].q += sizeof(uint32_t)) - sizeof(uint32_t))      )
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
    int bits = sizeof(L) * 8;
    if (C)  CF = ((DEST ^ TEMP) &  (1 << (bits - 1))) ? 1 : 0;
    if (P)  PF = ((       TEMP) &  (1              )) ? 1 : 0;
    if (A)  AF = ((DEST ^ TEMP) &  (1              )) ? 1 : 0;
    if (Z)  ZF = ((       TEMP) == (0              )) ? 1 : 0;
    if (S)  SF = ((       TEMP) &  (1 << (bits - 1))) ? 1 : 0;
    if (O)  OF = ((DEST ^ TEMP) &  (1 << (bits - 2))) ? 1 : 0;
    DEST = TEMP;
}
//------------------------------------------------------------------------------
