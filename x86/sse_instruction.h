#pragma once

#include "mmx_instruction.h"
#include "sse_register.h"

struct sse_instruction : public mmx_instruction
                       , public sse_register
{
    sse_instruction& sse = (*this);

protected:
    static instruction ADDPS;       // Packed Single-FP Add
    static instruction ADDSS;       // Scalar Single-FP Add
    static instruction ANDNPS;      // Bit-wise Logical And Not For Single-FP
    static instruction ANDPS;       // Bit-wise Logical And For Single FP
    static instruction CMPPS;       // Packed Single-FP Compare
    static instruction CMPSS;       // Scalar Single-FP Compare
    static instruction COMISS;      // Scalar Ordered Single-FP Compare and Set EFLAGS
    static instruction CVTPI2PS;    // Packed Signed INT32 to Packed Single-FP Conversion
    static instruction CVTSI2SS;    // Scalar Signed INT32 to Single-FP Conversion
    static instruction CVTPS2PI;    // Packed Single-FP to Packed INT32 Conversion
    static instruction CVTSS2SI;    // Scalar Single-FP to Signed INT32 Conversion
    static instruction CVTTPS2PI;   // Packed Single-FP to Packed INT32 Conversion (Truncate)
    static instruction CVTTSS2SI;   // Scalar Single-FP to Signed INT32 Conversion (Truncate)
    static instruction DIVPS;       // Packed Single-FP Divide
    static instruction DIVSS;       // Scalar Single-FP Divide
    static instruction LDMXCSR;     // Load Streaming SIMD Extension Control/Status
    static instruction MAXPS;       // Packed Single-FP Maximum
    static instruction MAXSS;       // Scalar Single-FP Maximum
    static instruction MINPS;       // Packed Single-FP Minimum
    static instruction MINSS;       // Scalar Single-FP Minimum
    static instruction MOVAPS;      // Move Aligned Four Packed Single-FP
    static instruction MOVHPS;      // Move High Packed Single-FP
    static instruction MOVLPS;      // Move Low Packed Single-FP
    static instruction MOVMSKPS;    // Move Mask To Integer
    static instruction MOVNTPS;     // Move Aligned Four Packed Single-FP Non Temporal
    static instruction MOVSS;       // Move Scalar Single-FP
    static instruction MOVUPS;      // Move Unaligned Four Packed Single-FP
    static instruction MULPS;       // Packed Single-FP Multiply
    static instruction MULSS;       // Scalar Single-FP Multiply
    static instruction ORPS;        // Bit-wise Logical OR for Single-FP Data
    static instruction PREFETCH0;   // Prefetch
    static instruction PREFETCH1;   // Prefetch
    static instruction PREFETCH2;   // Prefetch
    static instruction PREFETCHNTA; // Prefetch
    static instruction RCPPS;       // Packed Single-FP Reciprocal
    static instruction RCPSS;       // Scalar Single-FP Reciprocal
    static instruction RSQRTPS;     // Packed Single-FP Square Root Reciprocal
    static instruction RSQRTSS;     // Scalar Single-FP Square Root Reciprocal
    static instruction SFENCE;      // Store Fence
    static instruction SHUFPS;      // Shuffle Single-FP
    static instruction SQRTPS;      // Packed Single-FP Square Root
    static instruction SQRTSS;      // Scalar Single-FP Square Root
    static instruction STMXCSR;     // Store Streaming SIMD Extension Control/Status
    static instruction SUBPS;       // Packed Single-FP Subtract
    static instruction SUBSS;       // Scalar Single-FP Subtract
    static instruction UCOMISS;     // Unordered Scalar Single-FP compare and set EFLAGS
    static instruction UNPCKHPS;    // Unpack High Packed Single-FP Data
    static instruction UNPCKLPS;    // Unpack Low Packed Single-FP Data
    static instruction XORPS;       // Bit-wise Logical Xor for Single-FP Data
};
