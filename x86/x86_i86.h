//==============================================================================
// The 8086 Family Users Manual
// October 1979
//
// Intel Corporation 1978, 1979
//==============================================================================
#pragma once

#include "miCPU.h"

#include "x87_instruction.h"

struct x86_i86 : public miCPU
               , public x87_instruction
{
public:
    virtual ~x86_i86();
    bool Initialize(size_t space, const void* program = nullptr, size_t size = 0) override;
    bool Run() override;
    bool Step() override;
    bool Jump(size_t address) override;
    void Exception(void(*callback)(size_t, void*, void*)) override;
    size_t Stack() override;
    uint8_t* Memory(size_t base = 0, size_t size = 0) override;
    std::string Status() override;
    std::string Disassemble(int count) override;

protected:
    void StepInternal(Format& format);

protected:
    static instruction ESC;

    static instruction grp1;
    static instruction grp2;
    static instruction grp3;
    static instruction grp4;
    static instruction grp5;

    static const instruction_pointer one[256];
    static const instruction_pointer group[6][8];
};
