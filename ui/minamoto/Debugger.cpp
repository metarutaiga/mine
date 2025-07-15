//==============================================================================
// miCPU : Debugger Source
//
// Copyright (c) 2025 TAiGA
// https://github.com/metarutaiga/miCPU
//==============================================================================
#include "EmulatorPCH.h"
#include "x86/x86_i386.h"
#include "Debugger.h"

//------------------------------------------------------------------------------
static const uint8_t sampleX86[] =
{
    0x37,
    0x3F,
    0x10, 0x00,
    0x11, 0x00,
    0x12, 0x00,
    0x13, 0x00,
    0x14, 0x11,
    0x66, 0x15, 0x34, 0x12,
    0x15, 0x78, 0x56, 0x34, 0x12,
    0x80, 0x10, 0x22,
    0x66, 0x81, 0x10, 0x10, 0x32,
    0x81, 0x10, 0x10, 0x32, 0x54, 0x76,
    0x66, 0x83, 0x10, 0x33,
    0x83, 0x10, 0x44,
};
static const int countX86 = 14;
//------------------------------------------------------------------------------
static std::string disasm;
//------------------------------------------------------------------------------
void Debugger::Initialize()
{
}
//------------------------------------------------------------------------------
void Debugger::Shutdown()
{
}
//------------------------------------------------------------------------------
bool Debugger::Update(const UpdateData& updateData, bool& show)
{
    if (show == false)
        return false;

    bool updated = false;
    ImGui::SetNextWindowSize(ImVec2(1280.0f, 768.0f), ImGuiCond_Appearing);
    if (ImGui::Begin("Debugger", &show, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking))
    {
        if (ImGui::Button("Disasm"))
        {
            disasm.clear();

            x86_i386 x86;
            x86.Initialize(1048576, sampleX86, sizeof(sampleX86));
            disasm = x86.Disassemble(countX86);
        }
        ImGui::InputTextMultiline("", disasm, ImVec2(960.0f, 640.0f), ImGuiInputTextFlags_ReadOnly);
    }
    ImGui::End();

    return updated;
}
//------------------------------------------------------------------------------
