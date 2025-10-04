//==============================================================================
// IA-32 Intel (R) Architecture Software Developer’s Manual
//
// 2004
//==============================================================================
#pragma once

#include "x86_i686.h"

struct x86_ia32 : public x86_i686
{
public:
    x86_ia32(void(*step)(x86_i386&, Format&) = StepImplement) : x86_i686(step) {}

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
    static instruction grp12;
    static instruction grp13;
    static instruction grp14;
    static instruction grp15;
    static instruction grp16;

    static instruction CPUID;

    static const instruction_pointer one[256];
    static const instruction_pointer two[256];
    static const instruction_pointer two66[256];
    static const instruction_pointer twoF2[256];
    static const instruction_pointer twoF3[256];
    static const instruction_pointer group[17][2][8];
    static const instruction_pointer group66[17][2][8];

    static const instruction_pointer escMOD[8][8];
};
