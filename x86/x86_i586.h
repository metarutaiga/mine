//==============================================================================
// Pentium(TM) Processor User's Manual, Volume 3
//
// 1993
//==============================================================================
#pragma once

#include "x86_i486.h"
#include "mmx_register.h"

struct x86_i586 : public x86_i486
{
    mmx_register mmx;

public:
    x86_i586(void(*step)(x86_i386&, Format&) = StepImplement) : x86_i486(step) {}

protected:
    const void* Register(int type) const override;

protected:
    static void StepImplement(x86_i386& x86, Format& format);

protected:
    static instruction TWO;

    static instruction grp1;
    static instruction grp2;
    static instruction grp3;
    static instruction grp4;
    static instruction grp5;
    static instruction grp6;
    static instruction grp7;
    static instruction grp8;
    static instruction grp9;
    static instruction grpA1;
    static instruction grpA2;
    static instruction grpA3;

    static instruction CPUID;

    static const instruction_pointer one[256];
    static const instruction_pointer two[256];
    static const instruction_pointer group[10][8];
    static const instruction_pointer groupA[4][8];
};
