#include <stdint.h>
#include "ecoff64.h"

#define _L(x) (x)
#define _B(x) ((x >> 8) | (x << 8) & 0xFFFF)

const char* ECOFF64::GetMagic(uint16_t magic)
{
    switch (magic) {
    case _L(0603):  return "Alpha (Object file)";
    case _L(0610):  return "Alpha (Compressed object file)";
    case _L(0617):  return "Alpha ucode (Object file)";
    }

    return "Unknown";
}
