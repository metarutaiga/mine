//==============================================================================
// 80386 Programmer's Reference Manual
// 1986
//
// INTEL CORPORATION 1987
//==============================================================================
#pragma once

#include "miCPU.h"

#include "x86_instruction.h"
#include "x87_instruction.h"

struct x86_i386 : public miCPU
                , public x86_instruction
                , public x87_instruction
{
public:
    x86_i386(void(*step)(x86_i386&, Format&) = StepImplement);
    virtual ~x86_i386();
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
    static void StepImplement(x86_i386& x86, Format& format);

    allocator_t* allocator = nullptr;
    void (*StepInternal)(x86_i386& x86, Format& format) = nullptr;

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

    static instruction OSIZE;   // Operand-size override
    static instruction ASIZE;   // Address-size override

    static const instruction_pointer one[256];
    static const instruction_pointer two[256];
    static const instruction_pointer group[9][8];

    static const instruction_pointer esc[512];
    static const instruction_pointer escMOD[8][8];
};
