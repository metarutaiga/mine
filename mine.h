//==============================================================================
// mine : mine Header
//
// Copyright (c) 2025 TAiGA
// https://github.com/metarutaiga/mine
//==============================================================================
#pragma once

#include <string>

struct allocator_t;
struct mine
{
    virtual ~mine() = default;
    virtual bool Initialize(allocator_t* allocator, size_t stack) = 0;
    virtual bool Step(int count) = 0;
    virtual bool Jump(size_t address) = 0;
    virtual size_t Call(int count, size_t address, size_t size, ...) = 0;
    virtual const void* Register(int type) const = 0;
    virtual uint8_t* Memory(size_t base = 0, size_t size = 0) const = 0;
    virtual size_t Stack() const = 0;
    virtual size_t Program() const = 0;
    virtual std::string Status() const = 0;
    virtual std::string Disassemble(int count) const = 0;

    allocator_t* Allocator = nullptr;
    size_t BreakpointDataAddress = 0;
    size_t BreakpointDataValue = 0;
    size_t BreakpointProgram = 0;
    size_t (*Exception)(mine*, size_t) = [](mine*, size_t) { return size_t(0); };
};
