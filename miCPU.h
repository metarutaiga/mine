//==============================================================================
// miCPU : miCPU Header
//
// Copyright (c) 2025 TAiGA
// https://github.com/metarutaiga/miCPU
//==============================================================================
#pragma once

#include <string>

struct miCPU
{
    virtual ~miCPU() = default;
    virtual bool Initialize(size_t space, const void* program = nullptr, size_t size = 0) = 0;
    virtual bool Run() = 0;
    virtual bool Step(int type) = 0;
    virtual bool Jump(size_t address) = 0;
    virtual void Exception(void(*callback)(size_t, void*, void*)) = 0;
    virtual size_t Stack() = 0;
    virtual uint8_t* Memory(size_t base = 0, size_t size = 0) = 0;
    virtual std::string Status() = 0;
    virtual std::string Disassemble(int count) = 0;
};
