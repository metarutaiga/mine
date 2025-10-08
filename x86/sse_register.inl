#pragma once

#define MXCSR_RC            sse.mxcsr._RC
#define MXCSR               sse.mxcsr.d
#define XMM(i)              sse.regs[i & 0b111]
#define CastXMM(operand)    (operand.type == Format::Operand::ADR ? *(sse_register::register_t*)operand.memory : XMM(operand.base))
#define RoundNearest        0b00
#define RoundDown           0b01
#define RoundUp             0b10
#define RoundChop           0b11
