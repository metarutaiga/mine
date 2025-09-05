//==============================================================================
// i486(TM) MICROPROCESSOR PROGRAMMER'S REFERENCE MANUAL
//
// 1990
//==============================================================================
#pragma once

#include "x86_i386.h"

struct x86_i486 : public x86_i386
{
protected:
    virtual void StepInternal(x86_i386& x86, Format& format) const;

protected:
    static instruction TWO;

    static const instruction_pointer one[256];
    static const instruction_pointer two[256];
};
