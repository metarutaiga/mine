//==============================================================================
// mine : Debugger Header
//
// Copyright (c) 2025 TAiGA
// https://github.com/metarutaiga/mine
//==============================================================================
#pragma once

struct Debugger
{
    static void Initialize();
    static void Shutdown();
    static bool Update(const UpdateData& updateData, bool& show);
};
