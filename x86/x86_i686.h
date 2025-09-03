//==============================================================================
// Pentium(TM) Pro Family Developers Manual Volume 3
//
// 1996
//==============================================================================
#pragma once

#include "x86_i586.h"

struct x86_i686 : public x86_i586
{
public:
    x86_i686(void(*step)(x86_i386&, Format&) = StepImplement) : x86_i586(step) {}

protected:
    static void StepImplement(x86_i386& x86, Format& format);

protected:
    static instruction TWO;

    static instruction CPUID;

    static const instruction_pointer one[256];
    static const instruction_pointer two[256];
};
