#pragma once

#include "x86_format.h"
#include "mmx_register.h"

struct mmx_instruction : public x86_format
                       , public mmx_register
{
    mmx_instruction& mmx = (*this);

protected:
    static instruction EMMS;        // Empty MMX State
    static instruction MOVD;        // Move 32 Bits
    static instruction MOVQ;        // Move 64 Bits
    static instruction PACKSSWB;    // Pack with Signed Saturation
    static instruction PACKSSDW;    // Pack with Signed Saturation
    static instruction PACKUSWB;    // Pack with UnSigned Saturation
    static instruction PADDB;       // Packed Add
    static instruction PADDW;       // Packed Add
    static instruction PADDD;       // Packed Add
    static instruction PADDSB;      // Packed Add with Saturation
    static instruction PADDSW;      // Packed Add with Saturation
    static instruction PADDUSB;     // Packed Add Unsigned with Saturation
    static instruction PADDUSW;     // Packed Add Unsigned with Saturation
    static instruction PAND;        // Bitwise Logical And
    static instruction PANDN;       // Bitwise Logical And Not
    static instruction PCMPEQB;     // Packed Compare for Equal
    static instruction PCMPEQW;     // Packed Compare for Equal
    static instruction PCMPEQD;     // Packed Compare for Equal
    static instruction PCMPGTB;     // Packed Compare for Greater Than
    static instruction PCMPGTW;     // Packed Compare for Greater Than
    static instruction PCMPGTD;     // Packed Compare for Greater Than
    static instruction PMADDWD;     // Packed Multiply and Add
    static instruction PMULHW;      // Packed Multiply High
    static instruction PMULLW;      // Packed Multiply Low
    static instruction POR;         // Bitwise Logical Or
    static instruction PSLLW;       // Packed Shift Left Logical
    static instruction PSLLD;       // Packed Shift Left Logical
    static instruction PSLLQ;       // Packed Shift Left Logical
    static instruction PSRAW;       // Packed Shift Right Arithmetic
    static instruction PSRAD;       // Packed Shift Right Arithmetic
    static instruction PSRLW;       // Packed Shift Right Logical
    static instruction PSRLD;       // Packed Shift Right Logical
    static instruction PSRLQ;       // Packed Shift Right Logical
    static instruction PSUBB;       // Packed Subtract
    static instruction PSUBW;       // Packed Subtract
    static instruction PSUBD;       // Packed Subtract
    static instruction PSUBSB;      // Packed Subtract with Saturation
    static instruction PSUBSW;      // Packed Subtract with Saturation
    static instruction PSUBUSB;     // Packed Subtract Unsigned with Saturation
    static instruction PSUBUSW;     // Packed Subtract Unsigned with Saturation
    static instruction PUNPCKHBW;   // Unpack High Packed Data
    static instruction PUNPCKHWD;   // Unpack High Packed Data
    static instruction PUNPCKHDQ;   // Unpack High Packed Data
    static instruction PUNPCKLBW;   // Unpack Low Packed Data
    static instruction PUNPCKLWD;   // Unpack Low Packed Data
    static instruction PUNPCKLDQ;   // Unpack Low Packed Data
    static instruction PXOR;        // Bitwise Logical Exclusive OR
};
