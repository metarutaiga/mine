#include <stdint.h>
#include "xcoff32.h"

#define _L(x) (x)
#define _B(x) ((x >> 8) | (x << 8) & 0xFFFF)

const char* XCOFF32::GetMagic(uint16_t magic)
{
    switch (magic) {
    case _L(0514):  return "Intel 386";
    case _B(0630):  return "IBM ROMP (Writeable text segments)";
    case _B(0635):  return "IBM ROMP (Readonly sharable text segments)";
    case _B(0637):  return "IBM ROMP (Readonly text segments and TOC)";
    case _B(0730):  return "IBM POWER and PowerPC (Writeable text segments)";
    case _B(0735):  return "IBM POWER and PowerPC (Readonly sharable text segments)";
    case _B(0737):  return "IBM POWER and PowerPC (Readonly text segments and TOC)";
    }

    return "Unknown";
}
