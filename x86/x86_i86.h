//==============================================================================
// The 8086 Family Users Manual
// October 1979
//
// Intel Corporation 1978, 1979
//==============================================================================
#pragma once

#include "miCPU.h"

#include "x86_instruction.h"

struct x86_i86 : public miCPU
               , public x86_instruction
{
public:
    virtual ~x86_i86();
    bool Initialize(size_t space, const void* program = nullptr, size_t size = 0) override;
    bool Step() override;
    bool Jump(size_t address) override;
    uint8_t* Memory(size_t base, size_t size) override;
    std::string Disassemble(int count) override;

protected:
    void StepInternal(Format& format);

protected:
    instruction ESC;

    instruction grp1;
    instruction grp2;
    instruction grp3;
    instruction grp4;
    instruction grp5;

    static const instruction_pointer one[256];
    static const instruction_pointer group[16][8];
};
