#include <stdint.h>
#include "ecoff.h"

#define _L(x) (x)
#define _B(x) ((x >> 8) | (x << 8) & 0xFFFF)

const char* ECOFF::GetMagic(uint16_t magic)
{
    switch (magic) {
    case _L(0x160): return "MIPS I (Objects for big-endian machines)";
    case _L(0x162): return "MIPS I (Objects for little-endian machines)";
    case _L(0x163): return "MIPS II (Objects for big-endian machines)";
    case _L(0x166): return "MIPS II (Objects for little-endian machines)";
    case _L(0x140): return "MIPS III (Objects for big-endian machines)";
    case _L(0x142): return "MIPS III (Objects for little-endian machines)";
    case _L(0x180): return "MIPS ucode (Objects for big-endian machines)";
    case _L(0x182): return "MIPS ucode (Objects for little-endian machines)";
    }

    return "Unknown";
}
