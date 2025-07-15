//==============================================================================
// 80386 Programmer's Reference Manual
// 1986
//
// INTEL CORPORATION 1987
//==============================================================================
#pragma once

#include "miCPU.h"

#include "x86_instruction.h"

struct x86_i386 : public miCPU
                , public x86_instruction
{
public:
    virtual ~x86_i386();
    bool Initialize(size_t space, const void* program, size_t size) override;
    bool Step() override;
    std::string Disassemble(int count) override;

protected:
    Format StepInternal();

protected:
    instruction ESC;
    instruction TWO;

    instruction grp1;
    instruction grp2;
    instruction grp3;
    instruction grp4;
    instruction grp5;
    instruction grp6;
    instruction grp7;
    instruction grp8;

    instruction OSIZE;  // Operand-size override
//  instruction ASIZE;  // Address-size override

    static const instruction_pointer one[256];
    static const instruction_pointer two[256];
    static const instruction_pointer group[16][8];
};
