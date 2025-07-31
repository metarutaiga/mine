#include <stdint.h>
#include "pe.h"

#define _L(x) (x)
#define _B(x) ((x >> 8) | (x << 8) & 0xFFFF)

const char* PE::GetMagic(uint16_t magic)
{
    switch (magic) {
    case _L(0x0):       return "The content of this field is assumed to be applicable to any machine type";
    case _L(0x1):       return "Interacts with the host and not a WOW64 guest";
    case _L(0x14C):     return "Intel 386 or later processors and compatible processors";
    case _L(0x160):     return "MIPS I compatible 32-bit big endian";
    case _L(0x162):     return "MIPS I compatible 32-bit little endian";
    case _L(0x166):     return "MIPS III compatible 64-bit little endian";
    case _L(0x168):     return "MIPS IV compatible 64-bit little endian";
    case _L(0x169):     return "MIPS little-endian WCE v2";
    case _L(0x266):     return "MIPS16";
    case _L(0x366):     return "MIPS with FPU";
    case _L(0x466):     return "MIPS16 with FPU";
    case _L(0x184):     return "Alpha AXP, 32-bit address space";
    case _L(0x284):     return "Alpha 64, 64-bit address space";
    case _L(0x1A2):     return "Hitachi SH3";
    case _L(0x1A3):     return "Hitachi SH3 DSP";
    case _L(0x1A4):     return "Hitachi SH3E";
    case _L(0x1A6):     return "Hitachi SH4";
    case _L(0x1A8):     return "Hitachi SH5";
    case _L(0x1C0):     return "ARM little endian";
    case _L(0x1C2):     return "ARM Thumb little endian";
    case _L(0x1C4):     return "ARM Thumb-2 little endian";
    case _L(0x1D3):     return "Matsushita AM33";
    case _L(0x1F0):     return "Power PC little endian";
    case _L(0x1F1):     return "Power PC with floating point support";
    case _L(0x1F2):     return "Power PC big endian";
    case _L(0x200):     return "Intel Itanium processor family";
    case _L(0x520):     return "Infineon TriCore";
    case _L(0xCEF):     return "CEF";
    case _L(0xEBC):     return "EFI byte code";
    case _L(0x5032):    return "RISC-V 32-bit address space";
    case _L(0x5064):    return "RISC-V 64-bit address space";
    case _L(0x5128):    return "RISC-V 128-bit address space";
    case _L(0x6232):    return "LoongArch 32-bit processor family";
    case _L(0x6264):    return "LoongArch 64-bit processor family";
    case _L(0x8664):    return "x64";
    case _L(0x9041):    return "Mitsubishi M32R little endian";
    case _L(0xAA64):    return "ARM64 little endian";
    case _L(0xA641):    return "ABI that enables interoperability between native ARM64 and emulated x64 code";
    case _L(0xA64E):    return "Binary format that allows both native ARM64 and ARM64EC code to coexist in the same file";
    case _L(0xC0EE):    return "Common Execution Environment";
    }

    return "Unknown";
}
