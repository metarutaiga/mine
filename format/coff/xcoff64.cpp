#include <stdint.h>
#include "xcoff64.h"

#define _L(x) (x)
#define _B(x) ((x >> 8) | (x << 8) & 0xFFFF)

const char* XCOFF64::GetMagic(uint16_t magic)
{
    switch (magic) {
    case _B(0747):  return "IBM POWER and PowerPC (Obsolete XCOFF64 - not used)";
    case _B(0757):  return "IBM POWER and PowerPC (AIX 4.3 64-bit XCOFF)";
    case _B(0767):  return "IBM POWER and PowerPC (AIX 5+ 64-bit XCOFF)";
    }

    return "Unknown";
}
