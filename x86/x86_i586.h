//==============================================================================
// Pentium(TM) Processor User's Manual, Volume 3
//
// 1993
//==============================================================================
#pragma once

#include "x86_i486.h"

struct x86_i586 : public x86_i486
{
public:
    x86_i586(void(*step)(x86_i386&, Format&) = StepImplement) : x86_i486(step) {}

protected:
    static void StepImplement(x86_i386& x86, Format& format);

protected:
    static instruction TWO;

    static instruction CPUID;

    static const instruction_pointer one[256];
    static const instruction_pointer two[256];
};
