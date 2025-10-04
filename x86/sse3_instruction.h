#pragma once

#include "sse2_instruction.h"

struct sse3_instruction : public sse2_instruction
{
    static instruction ADDSUBPD;    // Add / Sub packed DP FP numbers from XMM2/Mem to XMM1
    static instruction ADDSUBPS;    // Add / Sub packed SP FP numbers from XMM2/Mem to XMM1
    static instruction FISTTP;      // Store ST in int64 (chop) and pop m64int
    static instruction HADDPD;      // Add horizontally packed DP FP numbers XMM2/Mem to XMM1
    static instruction HADDPS;      // Add horizontally packed SP FP numbers XMM2/Mem to XMM1
    static instruction HSUBPD;      // Sub horizontally packed DP FP numbers XMM2/Mem to XMM1
    static instruction HSUBPS;      // Sub horizontally packed SP FP numbers XMM2/Mem to XMM1
    static instruction LDDQU;       // Load unaligned integer 128-bit
    static instruction MONITOR;     // Set up a linear address range to be monitored by hardware
    static instruction MOVDDUP;     // Move 64 bits representing one DP data from XMM2/Mem to XMM1 and duplicate
    static instruction MOVSHDUP;    // Move 128 bits representing 4 SP data from XMM2/Mem to XMM1 and duplicate high
    static instruction MOVSLDUP;    // Move 128 bits representing 4 SP data from XMM2/Mem to XMM1 and duplicate low
    static instruction MWAIT;       // Wait until write-back store performed within the range specified by the instruction MONITOR
};
