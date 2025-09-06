#pragma once

#include "x86_format.h"
#include "x87_register.h"

struct x87_instruction : public x86_format
                       , public x87_register
{
    x87_instruction& x87 = (*this);

protected:
    static instruction F2XM1;       // Compute 2x-1
    static instruction FABS;        // Absolute value
    static instruction FADD;        // Floating point add
    static instruction FADDP;       // Floating point add and pop
    static instruction FBLD;        // Load BCD
    static instruction FBSTP;       // Store BCD and pop
    static instruction FCHS;        // Change sign
    static instruction FCLEX;       // Clear exceptions
//  static instruction FNCLEX;      // Clear exceptions, no wait
    static instruction FCOM;        // Floating point compare
    static instruction FCOMP;       // Floating point compare and pop
    static instruction FCOMPP;      // Floating point compare and pop twice
    static instruction FDECSTP;     // Decrement floating point stack pointer
//  static instruction FDISI;       // Disable interrupts (8087 only, others do fnop)
//  static instruction FNDISI;      // Disable interrupts, no wait (8087 only, others do fnop)
    static instruction FDIV;        // Floating divide
    static instruction FDIVP;       // Floating divide and pop
    static instruction FDIVR;       // Floating divide reversed
    static instruction FDIVRP;      // Floating divide reversed and pop
//  static instruction FENI;        // Enable interrupts (8087 only, others do fnop)
//  static instruction FNENI;       // Enable interrupts, nowait (8087 only, others do fnop)
//  static instruction FFREE;       // Free register
    static instruction FIADD;       // Integer add
    static instruction FICOM;       // Integer compare
    static instruction FICOMP;      // Integer compare and pop
    static instruction FIDIV;       // Integer divide
    static instruction FIDIVR;      // Integer divide reversed
    static instruction FILD;        // Load integer
    static instruction FIMUL;       // Integer multiply
    static instruction FINCSTP;     // Increment floating point stack pointer
//  static instruction FINIT;       // Initialize floating point processor
//  static instruction FNINIT;      // Initialize floating point processor, no wait
    static instruction FIST;        // Store integer
    static instruction FISTP;       // Store integer and pop
    static instruction FISUB;       // Integer subtract
    static instruction FISUBR;      // Integer subtract reversed
    static instruction FLD;         // Floating point load
    static instruction FLDZ;        // Load constant onto stack, 0.0
    static instruction FLD1;        // Load constant onto stack, 1.0
    static instruction FLDL2E;      // Load constant onto stack, logarithm base 2 (e)
    static instruction FLDL2T;      // Load constant onto stack, logarithm base 2 (10)
    static instruction FLDLG2;      // Load constant onto stack, logarithm base 10 (2)
    static instruction FLDLN2;      // Load constant onto stack, natural logarithm (2)
    static instruction FLDPI;       // Load constant onto stack, pi (3.14159...)
    static instruction FLDCW;       // Load control word
//  static instruction FLDENV;      // Load environment state
    static instruction FMUL;        // Floating point multiply
    static instruction FMULP;       // Floating point multiply and pop
//  static instruction FNOP;        // no operation
    static instruction FPATAN;      // Partial arctangent
    static instruction FPREM;       // Partial remainder
    static instruction FPTAN;       // Partial tangent
    static instruction FRNDINT;     // Round to integer
//  static instruction FRSTOR;      // Restore saved state
//  static instruction FSAVE;       // Save FPU state
//  static instruction FSAVEW;      // Save FPU state, 16-bit format (387+)
//  static instruction FSAVED;      // Save FPU state, 32-bit format (387+)
//  static instruction FNSAVE;      // Save FPU state, no wait
//  static instruction FNSAVEW;     // Save FPU state, no wait, 16-bit format (387+)
//  static instruction FNSAVED;     // Save FPU state, no wait, 32-bit format (387+)
    static instruction FSCALE;      // Scale by factor of 2
    static instruction FSQRT;       // Square root
    static instruction FST;         // Floating point store
    static instruction FSTP;        // Floating point store and pop
    static instruction FSTCW;       // Store control word
//  static instruction FNSTCW;      // Store control word, no wait
//  static instruction FSTENV;      // Store FPU environment
//  static instruction FSTENVW;     // Store FPU environment, 16-bit format (387+)
//  static instruction FSTENVD;     // Store FPU environment, 32-bit format (387+)
//  static instruction FNSTENV;     // Store FPU environment, no wait
//  static instruction FNSTENVW;    // Store FPU environment, no wait, 16-bit format (387+)
//  static instruction FNSTENVD;    // Store FPU environment, no wait, 32-bit format (387+)
    static instruction FSTSW;       // Store status word
//  static instruction FNSTSW;      // Store status word, no wait
    static instruction FSUB;        // Floating point subtract
    static instruction FSUBP;       // Floating point subtract and pop
    static instruction FSUBR;       // Floating point reverse subtract
    static instruction FSUBRP;      // Floating point reverse subtract and pop
    static instruction FTST;        // Floating point test for zero
//  static instruction FWAIT;       // Wait while FPU is executing
    static instruction FXAM;        // Examine condition flags
    static instruction FXCH;        // Exchange floating point registers
    static instruction FXTRACT;     // Extract exponent and significand
    static instruction FYL2X;       // Compute Y * log2(x)
    static instruction FYL2XP1;     // Compute Y * log2(x+1)

    // 80287 - 1982
//  static instruction FSETPM;      // Set protected mode (287 only, 387+ = fnop)

    // 80387 - 1985
    static instruction FCOS;        // Floating point cosine (387+)
    static instruction FPREM1;      // Partial remainder (IEEE compatible, 387+)
    static instruction FSIN;        // Sine (387+)
    static instruction FSINCOS;     // Sine and cosine (387+)
    static instruction FUCOM;       // Unordered floating point compare (387+)
    static instruction FUCOMP;      // Unordered floating point compare and pop (387+)
    static instruction FUCOMPP;     // Unordered floating point compare and pop twice (387+)

    // Pentium Pro - 1995
    static instruction FCMOVcc;     // Floating point conditional move (P6+)
    static instruction FCOMI;       // Compare real and set EFLAGS (P6+)
    static instruction FCOMIP;      // Compare real and set EFLAGS and pop (P6+)
    static instruction FUCOMI;      // Unordered compare real and set EFLAGS (P6+)
    static instruction FUCOMIP;     // Unordered compare real and set EFLAGS and pop (P6+)
};
