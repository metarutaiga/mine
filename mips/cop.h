//==============================================================================
// MIPS(R) Architecture For Programmers Volume II-A: The MIPS64(R) Instruction Set Reference Manual
//
// Document Number: MD00087
// Revision 6.06
// December 15, 2016
//
// Copyright (C) Wave Computing, Inc. All rights reserved.
//==============================================================================

#pragma once

#include "instr.h"

class CPU;

class COP : protected INSTR
{
protected:
    CPU& cpu;

public:
    COP(CPU& cpu);
    ~COP();

    void Execute(unsigned int encode);

    void LDC(unsigned int encode);
    void LWC(unsigned int encode);
    void SDC(unsigned int encode);
    void SWC(unsigned int encode);
};
