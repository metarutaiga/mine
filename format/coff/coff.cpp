#include <stdint.h>
#include "coff.h"

#define _L(x) (x)
#define _B(x) ((x >> 8) | (x << 8) & 0xFFFF)

const char* COFF::GetMagic(uint16_t magic)
{
    switch (magic) {
    case _L(0401):  return "DEC PDP-11 (UNIX-rt ldp)";
    case _L(0405):  return "DEC PDP-11 (Overlay)";
    case _L(0407):  return "DEC PDP-11 (System V vax executable)";
    case _L(0410):  return "DEC PDP-11 (System V vax pure executable)";
    case _L(0411):  return "DEC PDP-11 (Seperate I&D)";
    case _L(0437):  return "DEC PDP-11 (Kernel overlay)";
    case _L(0502):  return "Basic-16, no TV";
    case _L(0503):  return "Basic-16 with TV";
    case _L(0504):  return "iAPX 16, no TV";
    case _L(0505):  return "iAPX 16 with TV";
    case _L(0506):  return "iAPX 20, no TV";
    case _L(0507):  return "iAPX 20 with TV";
    case _L(0510):  return "x86, no TV";
    case _L(0511):  return "x86 with TV";
    case _L(0512):  return "Intel 286 (Small memory model)";
    case _L(0522):  return "Intel 286 (Large memory model)";
    case _L(0514):  return "Intel 386";
    case _L(0515):  return "Intel i860";
    case _L(0516):  return "Intel (Reserved)";
    case _L(0517):  return "Intel (Reserved)";
    case _B(0210):
    case _B(0520):  return "Motorola 68000 (Writeable text segments)";
    case _B(0211):
    case _B(0521):  return "Motorola 68000 (Readonly shareable text segments)";
    case _B(0522):  return "Motorola 68000 (Demand paged text segments)";
    case _B(0526):  return "Motorola 68000 (Bull dpx/2)";
    case _B(0627):  return "Apollo 68000";
    case _L(0524):  return "National Semiconductor 32000 (Writeable text segments)";
//  case _L(0524):  return "National Semiconductor 32000 (Readonly shareable text segments)";
    case _L(0525):  return "National Semiconductor 32000 (Module table magic)";
    case _B(0530):  return "IBM 370 (Writeble text segments)";
    case _B(0532):  return "IBM 370 (Reserved)";
    case _B(0533):  return "IBM 370 (Reserved)";
    case _B(0535):  return "IBM 370 (Readonly sharable text segments)";
    case _L(0531):  return "Amdahl 470/580 (Writable text segments)";
    case _L(0534):  return "Amdahl 470/580 (Readonly sharable text segments)";
    case _L(0540):  return "XL";
    case _B(0540):  return "Motorola 88000 (88k BCS executable)";
    case _B(0541):  return "Motorola 88000 (DG/UX executable)";
    case _B(0555):  return "Motorola 88000 (Object file)";
    case _B(0544):  return "Zilog Z8000, no TV";
    case _B(0545):  return "Zilog Z8000 with TV";
    case _L(0550):  return "Western Electric 3B20, no TV";
    case _L(0551):  return "Western Electric 3B20 with TV";
    case _B(0554):  return "Motorola 68030 (UniSoft UNIX System V/68 3.2)";
    case _B(0x666): return "Motorola 68040 (AT&T UNIX System V/68 4.0)";
    case _L(0560):  return "Western Electric 32000, no TV";
    case _L(0561):  return "Western Electric 32000 with TV";
    case _L(0562):  return "Western Electric 32000 (Reserved)";
    case _L(0570):  return "DEC VAX-11 (Writable text segments)";
    case _L(0575):  return "DEC VAX-11 (Readonly sharable text segments)";
    case _B(0572):  return "AMD Am29000 (Byte 0 is MSB)";
    case _L(0573):  return "AMD Am29000 (Byte 0 is LSB)";
    }

    return "Unknown";
}
