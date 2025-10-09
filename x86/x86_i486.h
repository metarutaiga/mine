//==============================================================================
// i486(TM) MICROPROCESSOR PROGRAMMER'S REFERENCE MANUAL
// (Order Number 240486)
//
// 1990
// (C) INTEL CORPORATION 1989
//==============================================================================
#pragma once

#include "x86_i386.h"

struct x86_i486 : public x86_i386
{
public:
    x86_i486(void(*step)(x86_i386&, Format&) = StepImplement) : x86_i386(step) {}

protected:
    static void StepImplement(x86_i386& x86, Format& format);

protected:
    static instruction TWO;

    static const instruction_pointer one[256];
    static const instruction_pointer two[256];
};
