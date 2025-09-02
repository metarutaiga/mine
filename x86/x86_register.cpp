#include "x86_register.h"

const char* const x86_register::REG8HL[8] = { "AL", "CL", "DL", "BL", "AH", "CH", "DH", "BH" };
const char* const x86_register::REG8[8] = { "AL", "CL", "DL", "BL", "SPL", "BPL", "SIL", "DIL" };
const char* const x86_register::REG16[8] = { "AX", "CX", "DX", "BX", "SP", "BP", "SI", "DI" };
const char* const x86_register::REG32[8] = { "EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI" };
const char* const x86_register::REG64[8] = { "RAX", "RCX", "RDX", "RBX", "RSP", "RBP", "RSI", "RDI" };
