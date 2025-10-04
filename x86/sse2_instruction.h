#pragma once

#include "sse_instruction.h"

struct sse2_instruction : public sse_instruction
{
    static instruction ADDPD;       // Add Packed Double-Precision Floating-Point Values
    static instruction ADDSD;       // Add Scalar Double-Precision Floating-Point Values
    static instruction ANDNPD;      // Bitwise Logical AND NOT of Packed Double-Precision Floating-Point Values
    static instruction ANDPD;       // Bitwise Logical AND of Packed Double-Precision Floating-Point Values
    static instruction CLFLUSH;     // Flush Cache Line
    static instruction CMPPD;       // Compare Packed Double-Precision Floating-Point Values
    static instruction CMPSD;       // Compare Scalar Double-Precision Floating-Point Values
    static instruction COMISD;      // Compare Scalar Ordered Double-Precision Floating-Point Values and Set EFLAGS
    static instruction CVTDQ2PD;    // Convert Packed Doubleword Integers to Packed Single-Precision Floating-Point Values
    static instruction CVTDQ2PS;    // Convert Packed Doubleword Integers to Packed Double-Precision Floating-Point Values
    static instruction CVTPD2DQ;    // Convert Packed Double-Precision Floating-Point Values to Packed Doubleword Integers
    static instruction CVTPD2PI;    // Convert Packed Double-Precision Floating-Point Values to Packed Doubleword Integers
    static instruction CVTPD2PS;    // Covert Packed Double-Precision Floating-Point Values to Packed Single-Precision Floating-Point Values
    static instruction CVTPI2PD;    // Convert Packed Doubleword Integers to Packed Double-Precision Floating-Point Values
    static instruction CVTPS2DQ;    // Convert Packed Single-Precision Floating-Point Values to Packed Doubleword Integers
    static instruction CVTPS2PD;    // Covert Packed Single-Precision Floating-Point Values to Packed Double-Precision Floating-Point Values
    static instruction CVTSD2SI;    // Convert Scalar Double-Precision Floating-Point Value to Doubleword Integer
    static instruction CVTSD2SS;    // Covert Scalar Double-Precision Floating-Point Value to Scalar Single-Precision Floating-Point Value
    static instruction CVTSI2SD;    // Convert Doubleword Integer to Scalar Double-Precision Floating-Point Value
    static instruction CVTSS2SD;    // Covert Scalar Single-Precision Floating-Point Value to Scalar Double-Precision Floating-Point Value
    static instruction CVTTPD2DQ;   // Convert With Truncation Packed Double-Precision Floating-Point Values to Packed Doubleword Integers
    static instruction CVTTPD2PI;   // Convert with Truncation Packed Double-Precision Floating-Point Values to Packed Doubleword Integers
    static instruction CVTTPS2DQ;   // Convert With Truncation Packed Single-Precision Floating-Point Values to Packed Doubleword Integers
    static instruction CVTTSD2SI;   // Convert with Truncation Scalar Double-Precision Floating-Point Value to Doubleword Integer
    static instruction DIVPD;       // Divide Packed Double-Precision Floating-Point Values
    static instruction DIVSD;       // Divide Scalar Double-Precision Floating-Point Values
    static instruction LFENCE;      // Load Fence
    static instruction MASKMOVDQU;  // Store Selected Bytes of Double Quadword
    static instruction MAXPD;       // Return Maximum Packed Double-Precision Floating-Point Values
    static instruction MAXSD;       // Return Maximum Scalar Double-Precision Floating-Point Value
    static instruction MFENCE;      // Memory Fence
    static instruction MINPD;       // Return Minimum Packed Double-Precision Floating-Point Values
    static instruction MINSD;       // Return Minimum Scalar Double-Precision Floating-Point Value
    static instruction MOVAPD;      // Move Aligned Packed Double-Precision Floating-Point Values
    static instruction MOVD;        // Move Doubleword
    static instruction MOVDQ2Q;     // Move Quadword from XMM to MMX Register
    static instruction MOVDQA;      // Move Aligned Double Quadword
    static instruction MOVDQU;      // Move Unaligned Double Quadword
    static instruction MOVHPD;      // Move High Packed Double-Precision Floating-Point Values
    static instruction MOVLPD;      // Move Low Packed Double-Precision Floating-Point Values
    static instruction MOVMSKPD;    // Extract Packed Double-Precision Floating-Point Sign Mask
    static instruction MOVNTDQ;     // Store Packed Double-Precision Floating-Point Values Using Non-Temporal Hint
    static instruction MOVNTI;      // Store Doubleword Using Non-Temporal Hint
    static instruction MOVNTPD;     // Store Double Quadword Using Non-Temporal Hint
    static instruction MOVQ;        // Move Quadword
    static instruction MOVQ2DQ;     // Move Quadword from MMX to XMM Register
    static instruction MOVSD;       // Move Scalar Double-Precision Floating-Point Values
    static instruction MOVUPD;      // Move Unaligned Packed Double-Precision Floating-Point Values
    static instruction MULPD;       // Multiply Packed Double-Precision Floating-Point Values
    static instruction MULSD;       // Multiply Scalar Double-Precision Floating-Point Values
    static instruction ORPD;        // Bitwise Logical OR of Double-Precision Floating-Point Values
    static instruction PACKSSDW;    // Pack Dword To Word Data (signed with saturation)
    static instruction PACKSSWB;    // Pack Word To Byte Data (signed with saturation)
    static instruction PACKUSWB;    // Pack Word To Byte Data (unsigned with saturation)
    static instruction PADDB;       // Add With Wrap-around
    static instruction PADDW;       // Add With Wrap-around
    static instruction PADDD;       // Add With Wrap-around
    static instruction PADDQ;       // Add Packed Quadword Integers
    static instruction PADDSB;      // Add Signed With Saturation
    static instruction PADDSW;      // Add Signed With Saturation
    static instruction PADDUSB;     // Add Unsigned With Saturation
    static instruction PADDUSW;     // Add Unsigned With Saturation
    static instruction PAND;        // Bitwise And
    static instruction PANDN;       // Bitwise AndNot
    static instruction PAUSE;       // Spin Loop Hint
    static instruction PAVGB;       // Average Packed Integers
    static instruction PAVGW;       // Average Packed Integers
    static instruction PCMPEQB;     // Packed Compare For Equality
    static instruction PCMPEQW;     // Packed Compare For Equality
    static instruction PCMPEQD;     // Packed Compare For Equality
    static instruction PCMPGTB;     // Packed Compare Greater (signed)
    static instruction PCMPGTW;     // Packed Compare Greater (signed)
    static instruction PCMPGTD;     // Packed Compare Greater (signed)
    static instruction PEXTRW;      // Extract Word
    static instruction PINSRW;      // Insert Word
    static instruction PMADDWD;     // Packed Multiply Add
    static instruction PMAXSW;      // Maximum of Packed Signed Word Integers
    static instruction PMAXUB;      // Maximum of Packed Unsigned Byte Integers
    static instruction PMINSW;      // Minimum of Packed Signed Word Integers
    static instruction PMINUB;      // Minimum of Packed Unsigned Byte Integers
    static instruction PMOVMSKB;    // Move Byte Mask To Integer
    static instruction PMULHUW;     // Packed multiplication, store high word (unsigned)
    static instruction PMULHW;      // Packed Multiplication, store high word
    static instruction PMULLW;      // Packed Multiplication, store low word
    static instruction PMULUDQ;     // Multiply Packed Unsigned Doubleword Integers
    static instruction POR;         // Bitwise Or
    static instruction PSADBW;      // Compute Sum of Absolute Differences
    static instruction PSHUFD;      // Shuffle Packed Doublewords
    static instruction PSHUFHW;     // Shuffle Packed High Words
    static instruction PSHUFLW;     // Shuffle Packed Low Words
    static instruction PSLLW;       // Packed Shift Left Logical
    static instruction PSLLD;       // Packed Shift Left Logical
    static instruction PSLLQ;       // Packed Shift Left Logical
    static instruction PSLLDQ;      // Shift Double Quadword Left Logical
    static instruction PSRAW;       // Packed Shift Right Arithmetic
    static instruction PSRAD;       // Packed Shift Right Arithmetic
    static instruction PSRLW;       // Packed Shift Right Logical
    static instruction PSRLD;       // Packed Shift Right Logical
    static instruction PSRLQ;       // Packed Shift Right Logical
    static instruction PSRLDQ;      // Shift Double Quadword Right Logical
    static instruction PSUBB;       // Subtract With Wrap-around
    static instruction PSUBW;       // Subtract With Wrap-around
    static instruction PSUBD;       // Subtract With Wrap-around
    static instruction PSUBQ;       // Subtract Packed Quadword Integers
    static instruction PSUBSB;      // Subtract Signed With Saturation
    static instruction PSUBSW;      // Subtract Signed With Saturation
    static instruction PSUBUSB;     // Subtract Unsigned With Saturation
    static instruction PSUBUSW;     // Subtract Unsigned With Saturation
    static instruction PUNPCKHBW;   // Unpack High Data To Next Larger Type
    static instruction PUNPCKHWD;   // Unpack High Data To Next Larger Type
    static instruction PUNPCKHDQ;   // Unpack High Data To Next Larger Type
    static instruction PUNPCKHQDQ;  // Unpack High Data
    static instruction PUNPCKLBW;   // Unpack Low Data To Next Larger Type
    static instruction PUNPCKLWD;   // Unpack Low Data To Next Larger Type
    static instruction PUNPCKLDQ;   // Unpack Low Data To Next Larger Type
    static instruction PUNPCKLQDQ;  // Unpack Low Data
    static instruction PXOR;        // Bitwise Xor
    static instruction SHUFPD;      // Shuffle Packed Double-Precision Floating-Point Values
    static instruction SQRTPD;      // Compute Square Roots of Packed Double-Precision Floating-Point Values
    static instruction SQRTSD;      // Compute Square Root of Scalar Double-Precision Floating-Point Value
    static instruction SUBPD;       // Subtract Packed Double-Precision Floating-Point Values
    static instruction SUBSD;       // Subtract Scalar Double-Precision Floating-Point Values
    static instruction UCOMISD;     // Unordered Compare Scalar Ordered Double-Precision Floating-Point Values and Set EFLAGS
    static instruction UNPCKHPD;    // Unpack and Interleave High Packed Double-Precision Floating-Point Values
    static instruction UNPCKLPD;    // Unpack and Interleave Low Packed Double-Precision Floating-Point Values
    static instruction XORPD;       // Bitwise Logical OR of Double-Precision Floating-Point Values
};
