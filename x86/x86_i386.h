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
public:
    virtual ~x86_i386();
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
