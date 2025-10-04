//==============================================================================
// Pentium(TM) Pro Family Developers Manual Volume 3
//
// 1996
//==============================================================================
#pragma once

#include "x86_i586.h"
#include "sse_register.h"

struct x86_i686 : public x86_i586
{
    sse_register sse;

public:
    x86_i686(void(*step)(x86_i386&, Format&) = StepImplement) : x86_i586(step) {}

protected:
    const void* Register(int type) const override;
    std::string Status() const override;

protected:
    static void StepImplement(x86_i386& x86, Format& format);

protected:
    static instruction ESC;
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
    static instruction grp13;
    static instruction grp14;
    static instruction grp15;
    static instruction grp16;
    static instruction grp17;

    static instruction CPUID;

    static const instruction_pointer one[256];
    static const instruction_pointer two[256];
    static const instruction_pointer twoF3[256];
    static const instruction_pointer group[18][8];

    static const instruction_pointer esc[512];
};
