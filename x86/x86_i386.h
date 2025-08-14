//==============================================================================
// 80386 Programmer's Reference Manual
// 1986
//
// INTEL CORPORATION 1987
//==============================================================================
#pragma once

#include "miCPU.h"

#include "x87_instruction.h"

struct x86_i386 : public miCPU
                , public x87_instruction
{
    allocator_t* allocator = nullptr;

public:
    virtual ~x86_i386();
    bool Initialize(allocator_t* allocator) override;
    bool Run() override;
    bool Step(int type) override;
    bool Jump(size_t address) override;
    void Exception(size_t(*callback)(miCPU*, size_t)) override;
    allocator_t* Allocator() const override;
    uint8_t* Memory(size_t base = 0, size_t size = 0) const override;
    size_t Stack() const override;
    std::string Status() const override;
    std::string Disassemble(int count) const override;

protected:
    void StepInternal(Format& format);

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

    static instruction OSIZE;  // Operand-size override
    static instruction ASIZE;  // Address-size override

    static const instruction_pointer one[256];
    static const instruction_pointer two[256];
    static const instruction_pointer group[9][8];

    static const instruction_pointer esc[512];
    static const instruction_pointer escMOD[8][8];
};
