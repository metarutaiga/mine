#pragma once

#define MM(i)               mmx.regs[i & 0b111]
#define CastMM(operand)     (operand.type == Format::Operand::ADR ? *(mmx_register::register_t*)operand.memory : MM(operand.base))
