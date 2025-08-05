#pragma once

#include "x86_instruction.h"
#include "x87_register.h"

struct x87_instruction : public x86_instruction
                       , public x87_register
{
    x87_instruction& x87 = (*this);

protected:
    instruction F2XM1;      // Compute 2x-1
    instruction FABS;       // Absolute value
    instruction FADD;       // Floating point add
    instruction FADDP;      // Floating point add and pop
    instruction FBLD;       // Load BCD
    instruction FBSTP;      // Store BCD and pop
    instruction FCHS;       // Change sign
    instruction FCLEX;      // Clear exceptions
//  instruction FNCLEX;     // Clear exceptions, no wait
    instruction FCOM;       // Floating point compare
    instruction FCOMP;      // Floating point compare and pop
    instruction FCOMPP;     // Floating point compare and pop twice
    instruction FCOS;       // Floating point cosine (387+)
    instruction FDECSTP;    // Decrement floating point stack pointer
//  instruction FDISI;      // Disable interrupts (8087 only, others do fnop)
//  instruction FNDISI;     // Disable interrupts, no wait (8087 only, others do fnop)
    instruction FDIV;       // Floating divide
    instruction FDIVP;      // Floating divide and pop
    instruction FDIVR;      // Floating divide reversed
    instruction FDIVRP;     // Floating divide reversed and pop
//  instruction FENI;       // Enable interrupts (8087 only, others do fnop)
//  instruction FNENI;      // Enable interrupts, nowait (8087 only, others do fnop)
    instruction FFREE;      // Free register
    instruction FIADD;      // Integer add
    instruction FICOM;      // Integer compare
    instruction FICOMP;     // Integer compare and pop
    instruction FIDIV;      // Integer divide
    instruction FIDIVR;     // Integer divide reversed
    instruction FILD;       // Load integer
    instruction FIMUL;      // Integer multiply
    instruction FINCSTP;    // Increment floating point stack pointer
//  instruction FINIT;      // Initialize floating point processor
//  instruction FNINIT;     // Initialize floating point processor, no wait
    instruction FIST;       // Store integer
    instruction FISTP;      // Store integer and pop
    instruction FISUB;      // Integer subtract
    instruction FISUBR;     // Integer subtract reversed
    instruction FLD;        // Floating point load
    instruction FLDZ;       // Load constant onto stack, 0.0
    instruction FLD1;       // Load constant onto stack, 1.0
    instruction FLDL2E;     // Load constant onto stack, logarithm base 2 (e)
    instruction FLDL2T;     // Load constant onto stack, logarithm base 2 (10)
    instruction FLDLG2;     // Load constant onto stack, logarithm base 10 (2)
    instruction FLDLN2;     // Load constant onto stack, natural logarithm (2)
    instruction FLDPI;      // Load constant onto stack, pi (3.14159...)
    instruction FLDCW;      // Load control word
//  instruction FLDENV;     // Load environment state
    instruction FMUL;       // Floating point multiply
    instruction FMULP;      // Floating point multiply and pop
    instruction FNOP;       // no operation
    instruction FPATAN;     // Partial arctangent
    instruction FPREM;      // Partial remainder
    instruction FPREM1;     // Partial remainder (IEEE compatible, 387+)
    instruction FPTAN;      // Partial tangent
    instruction FRNDINT;    // Round to integer
//  instruction FRSTOR;     // Restore saved state
//  instruction FSAVE;      // Save FPU state
//  instruction FSAVEW;     // Save FPU state, 16-bit format (387+)
//  instruction FSAVED;     // Save FPU state, 32-bit format (387+)
//  instruction FNSAVE;     // Save FPU state, no wait
//  instruction FNSAVEW;    // Save FPU state, no wait, 16-bit format (387+)
//  instruction FNSAVED;    // Save FPU state, no wait, 32-bit format (387+)
    instruction FSCALE;     // Scale by factor of 2
//  instruction FSETPM;     // Set protected mode (287 only, 387+ = fnop)
    instruction FSIN;       // Sine (387+)
    instruction FSINCOS;    // Sine and cosine (387+)
    instruction FSQRT;      // Square root
    instruction FST;        // Floating point store
    instruction FSTP;       // Floating point store and pop
    instruction FSTCW;      // Store control word
//  instruction FNSTCW;     // Store control word, no wait
//  instruction FSTENV;     // Store FPU environment
//  instruction FSTENVW;    // Store FPU environment, 16-bit format (387+)
//  instruction FSTENVD;    // Store FPU environment, 32-bit format (387+)
//  instruction FNSTENV;    // Store FPU environment, no wait
//  instruction FNSTENVW;   // Store FPU environment, no wait, 16-bit format (387+)
//  instruction FNSTENVD;   // Store FPU environment, no wait, 32-bit format (387+)
    instruction FSTSW;      // Store status word
//  instruction FNSTSW;     // Store status word, no wait
    instruction FSUB;       // Floating point subtract
    instruction FSUBP;      // Floating point subtract and pop
    instruction FSUBR;      // Floating point reverse subtract
    instruction FSUBRP;     // Floating point reverse subtract and pop
    instruction FTST;       // Floating point test for zero
    instruction FUCOM;      // Unordered floating point compare (387+)
    instruction FUCOMP;     // Unordered floating point compare and pop (387+)
    instruction FUCOMPP;    // Unordered floating point compare and pop twice (387+)
//  instruction FWAIT;      // Wait while FPU is executing
    instruction FXAM;       // Examine condition flags
    instruction FXCH;       // Exchange floating point registers
    instruction FXTRACT;    // Extract exponent and significand
    instruction FYL2X;      // Compute Y * log2(x)
    instruction FYL2XP1;    // Compute Y * log2(x+1)
};
