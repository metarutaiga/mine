#pragma once

#define XMM(i)              sse.regs[i & 0b111]
#define MXCSR               sse.mxcsr
#define CastXMM(operand)    (operand.type == Format::Operand::ADR ? *(sse_register::register_t*)operand.memory : XMM(operand.base))
