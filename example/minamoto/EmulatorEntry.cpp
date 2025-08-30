//==============================================================================
// miCPU : EmulatorEntry Source
//
// Copyright (c) 2025 TAiGA
// https://github.com/metarutaiga/miCPU
//==============================================================================
#include "EmulatorPCH.h"
#include "Debugger.h"

#define PLUGIN_NAME     "Emulator"

//------------------------------------------------------------------------------
moduleAPI const char* Create(const CreateData& createData)
{
    Debugger::Initialize();
    return PLUGIN_NAME;
}
//------------------------------------------------------------------------------
moduleAPI void Shutdown(const ShutdownData& shutdownData)
{
    Debugger::Shutdown();
}
//------------------------------------------------------------------------------
moduleAPI void Message(const MessageData& messageData)
{
    if (messageData.length == 1)
    {
        switch (xxHash(messageData.data[0]))
        {
        case xxHash("INIT"):
            break;
        case xxHash("SHUTDOWN"):
            break;
        default:
            break;
        }
    }
}
//------------------------------------------------------------------------------
moduleAPI bool Update(const UpdateData& updateData)
{
    static bool showDebugger = false;

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Experimental"))
        {
            if (ImGui::GetCursorPos().y > ImGui::GetTextLineHeightWithSpacing())
                ImGui::Separator();
            ImGui::MenuItem("Debugger", nullptr, &showDebugger);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    bool updated = false;
    updated |= Debugger::Update(updateData, showDebugger);

    return updated;
}
//------------------------------------------------------------------------------
moduleAPI void Render(const RenderData& renderData)
{

}
//------------------------------------------------------------------------------
