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
    allocator_t* allocator = nullptr;

public:
    virtual ~x86_i86();
    bool Initialize(allocator_t* allocator, size_t stack) override;
    bool Run() override;
    bool Step(int type) override;
    bool Jump(size_t address) override;
    void Breakpoint(size_t address) override;
    void Exception(size_t(*callback)(miCPU*, size_t)) override;
    allocator_t* Allocator() const override;
    uint8_t* Memory(size_t base = 0, size_t size = 0) const override;
    size_t Stack() const override;
    size_t Program() const override;
    std::string Status() const override;
    std::string Disassemble(int count) const override;

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
