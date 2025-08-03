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
    return [](x86_instruction& x86, const x86_format::Format& format, void* dest, const void* src1, const void* src2) {
        return static_lambda(x86, format, *(T*)dest, *(T*)src1, *(T*)src2);
    };
}
//------------------------------------------------------------------------------
#define BEGIN_OPERATION() { \
        auto operation = [](x86_instruction& x86, const Format& format, auto& DEST, auto SRC1, auto SRC2) { \
            auto SRC = SRC1; (void)SRC;
#define END_OPERATION }; \
        switch (format.width) { \
        case 8:     format.operation = specialize<uint8_t>(operation);  break; \
        case 16:    format.operation = specialize<uint16_t>(operation); break; \
        case 32:    format.operation = specialize<uint32_t>(operation); break; \
        } \
    }
//------------------------------------------------------------------------------
