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
    virtual bool Initialize(size_t space, const void* program, size_t size) = 0;
    virtual bool Step() = 0;
    virtual std::string Disassemble(int count) = 0;
};
