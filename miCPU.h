//==============================================================================
// miCPU : miCPU Header
//
// Copyright (c) 2025 TAiGA
// https://github.com/metarutaiga/miCPU
//==============================================================================
#pragma once

#include <string>

struct allocator_t;
struct miCPU
{
    virtual ~miCPU() = default;
    virtual bool Initialize(allocator_t* allocator, size_t stack) = 0;
    virtual bool Run() = 0;
    virtual bool Step(int type) = 0;
    virtual bool Jump(size_t address) = 0;
    virtual void Exception(size_t(*callback)(miCPU*, size_t)) = 0;
    virtual allocator_t* Allocator() const = 0;
    virtual uint8_t* Memory(size_t base = 0, size_t size = 0) const = 0;
    virtual size_t Stack() const = 0;
    virtual std::string Status() const = 0;
    virtual std::string Disassemble(int count) const = 0;
};
