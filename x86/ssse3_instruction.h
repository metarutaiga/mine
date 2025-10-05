#pragma once

#include "sse3_instruction.h"

struct ssse3_instruction : public sse3_instruction
{
    static instruction PSIGNB;      // Packed Sign Bytes
    static instruction PSIGNW;      // Packed Sign Words
    static instruction PSIGND;      // Packed Sign Doublewords
    static instruction PABSB;       // Packed Absolute Value Bytes
    static instruction PABSW;       // Packed Absolute Value Words
    static instruction PABSD;       // Packed Absolute Value Doublewords
    static instruction PALIGNR;     // Packed Align Right
    static instruction PSHUFB;      // Packed Shuffle Bytes
    static instruction PMULHRSW;    // Packed Multiply High with Round and Scale
    static instruction PMADDUBSW;   // Multiply and Add Packed Signed and Unsigned Bytes
    static instruction PHSUBW;      // Packed Horizontal Subtract Words
    static instruction PHSUBD;      // Packed Horizontal Subtract Doublewords
    static instruction PHSUBSW;     // Packed Horizontal Subtract and Saturate Words
    static instruction PHADDW;      // Packed Horizontal Add Words
    static instruction PHADDD;      // Packed Horizontal Add Doublewords
    static instruction PHADDSW;     // Packed Horizontal Add and Saturate Words
};
