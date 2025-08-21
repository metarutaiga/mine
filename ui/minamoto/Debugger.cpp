//==============================================================================
// miCPU : Debugger Source
//
// Copyright (c) 2025 TAiGA
// https://github.com/metarutaiga/miCPU
//==============================================================================
#include "EmulatorPCH.h"
#include <IconFontCppHeaders/IconsFontAwesome4.h>
#include <imgui_club/imgui_memory_editor/imgui_memory_editor.h>
#include "format/coff/pe.h"
#include "syscall/simple_allocator.h"
#include "syscall/syscall.h"
#include "syscall/syscall_windows.h"
#include "x86/x86_i386.h"
#include "Debugger.h"

#if defined(_UCRT)
#define strcasecmp _stricmp
#endif

//------------------------------------------------------------------------------
static miCPU* cpu;
static std::string status;
static std::string disasm;
static std::string logs;
static int symbolIndex;
static std::vector<std::pair<std::string, size_t>> symbols;
static std::vector<std::pair<std::string, void*>> dlls;
//------------------------------------------------------------------------------
static std::vector<std::pair<std::string, std::string>> samples;
//------------------------------------------------------------------------------
static int LoggerV(const char* format, va_list va)
{
    int length = vsnprintf(nullptr, 0, format, va) + 1;
    size_t offset = logs.size();
    logs.resize(offset + length);
    vsnprintf(logs.data() + offset, length, format, va);
    logs.pop_back();
    if (logs.empty() == false && logs.back() != '\n')
        logs += '\n';
    return length;
}
//------------------------------------------------------------------------------
static int Logger(const char* format, ...)
{
    va_list va;
    va_start(va, format);
    int length = LoggerV(format, va);
    va_end(va);
    return length;
}
//------------------------------------------------------------------------------
static size_t Exception(miCPU* data, size_t index)
{
    size_t result = 0;
    if (result == 0) {
        result = syscall_windows_execute(data, index, Logger);
    }
    if (result == 0) {
        result = syscall_i386_execute(cpu, index, LoggerV);
    }
    return result;
}
//------------------------------------------------------------------------------
static size_t Symbol(const char* file, const char* name)
{
    size_t address = 0;
    if (address == 0) {
        address = syscall_windows_symbol(file, name);
    }
    if (address == 0) {
        address = syscall_i386_symbol(file, name);
    }
    return address;
}
//------------------------------------------------------------------------------
void Debugger::Initialize()
{
    std::string folder;
    folder += xxGetExecutablePath();
    folder += "/../../../../../SDK/miCPU/format/sample/pe.x86/";

    uint64_t handle = 0;
    while (char* filename = xxOpenDirectory(&handle, folder.c_str(), nullptr)) {
        if (strstr(filename, ".dll") || strstr(filename, ".exe")) {
            samples.emplace_back(filename, folder + filename);
        }
        xxFree(filename);
    }
    xxCloseDirectory(&handle);
}
//------------------------------------------------------------------------------
void Debugger::Shutdown()
{
    delete cpu;
    disasm = std::string();
    status = std::string();
    symbols = std::vector<std::pair<std::string, size_t>>();
    logs = std::string();
    samples = std::vector<std::pair<std::string, std::string>>();
    dlls = std::vector<std::pair<std::string, void*>>();
}
//------------------------------------------------------------------------------
bool Debugger::Update(const UpdateData& updateData, bool& show)
{
    if (show == false)
        return false;

    bool updated = false;
    ImGui::SetNextWindowSize(ImVec2(1366.0f, 768.0f), ImGuiCond_Appearing);
    if (ImGui::Begin("Debugger", &show, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoDocking)) {
        ImVec2 windowSize = ImGui::GetContentRegionAvail();

        static int stackIndex = 0;
        static bool refresh = false;
        static bool showMemoryEditor = false;
        std::string file;

        if (ImGui::BeginTable("Debugger", 3)) {
            float left = windowSize.x / 2.0f;
            float right = windowSize.x / 4.0f;
            float height = windowSize.y - 256.0f;
            float halfHeight = height / 2.0f;

            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, left);
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, right);
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, right);

            // 1st Column
            ImGui::TableNextColumn();
            ImGui::InputTextMultiline("##10", disasm, ImVec2(left, height), ImGuiInputTextFlags_ReadOnly);

            // 2nd Column
            ImGui::TableNextColumn();
            ImGui::InputTextMultiline("##20", status, ImVec2(right, halfHeight), ImGuiInputTextFlags_ReadOnly);

            int stackCount = halfHeight / ImGui::GetTextLineHeightWithSpacing();
            ImGui::SetNextWindowSize(ImVec2(right, halfHeight));
            ImGui::ListBox("##21", &stackIndex, [](void* user_data, int index) -> const char* {
                if (cpu == nullptr)
                    return "";

                auto stackCount = *(int*)user_data / 2;
                size_t stack = cpu->Stack() + (index - stackCount) * sizeof(uint32_t);
                auto* value = (uint32_t*)cpu->Memory(stack);
                if (value == nullptr)
                    return "";

                static char temp[32];
                snprintf(temp, 32, "%s%08zX %08X", stackCount == index ? ">" : " ", stack, *value);
                return temp;
            }, &stackCount, stackCount);

            // 3rd Column
            ImGui::TableNextColumn();
            ImGui::SetNextWindowSize(ImVec2(right, halfHeight));
            if (ImGui::ListBox("##30", &symbolIndex, [](void* user_data, int index) {
                auto& symbols = *(std::vector<std::pair<std::string, size_t>>*)user_data;
                return symbols[index].first.c_str();
            }, &symbols, (int)symbols.size())) {
                cpu->Jump(symbols[symbolIndex].second);
                refresh = true;
            }

            if (ImGui::Button(ICON_FA_PLAY))        { refresh = true; if (cpu) cpu->Run();    } ImGui::SameLine();
            if (ImGui::Button(ICON_FA_ARROW_RIGHT)) { refresh = true; if (cpu) cpu->Step(0);  } ImGui::SameLine();
            if (ImGui::Button(ICON_FA_ARROW_DOWN))  { refresh = true; if (cpu) cpu->Step(1);  } ImGui::SameLine();
            if (ImGui::Button(ICON_FA_ARROW_UP))    { refresh = true; if (cpu) cpu->Step(-1); } ImGui::SameLine();
            ImGui::Checkbox("Memory", &showMemoryEditor);
            ImGui::Separator();
            for (auto& [name, path] : samples) {
                float pos = ImGui::CalcTextSize(name.c_str()).x + ImGui::GetStyle().FramePadding.x * 2.0f;
                float avail = ImGui::GetContentRegionAvail().x;
                if (pos > avail)
                    ImGui::NewLine();
                if (ImGui::Button(name.c_str())) {
                    file = path;
                }
                ImGui::SameLine();
            }

            ImGui::EndTable();
        }
        ImGui::InputTextMultiline("##100", logs, ImVec2(windowSize.x, 256.0f - 16.0f), ImGuiInputTextFlags_ReadOnly);

        if (file.empty() == false) {
            syscall_windows_delete(cpu);
            delete cpu;

            status.clear();
            disasm.clear();
            logs.clear();
            stackIndex = 0;
            symbolIndex = 0;
            symbols.clear();
            dlls.clear();
            refresh = true;

            cpu = new x86_i386;
            cpu->Initialize(simple_allocator<16>::construct(16777216));
            cpu->Exception(Exception);
            syscall_windows_new(cpu, file.substr(0, file.rfind('/')).c_str());

            void* image = PE::Load(file.c_str(), [](size_t base, size_t size, void* userdata) {
                miCPU* cpu = (miCPU*)userdata;
                return cpu->Memory(base, size);
            }, cpu, Logger);
            symbols.emplace_back("Entry", PE::Entry(image));
            PE::Imports(image, Symbol, Logger);
            PE::Exports(image, [](const char* name, size_t address, void* sym_data) {
                auto& symbols = *(std::vector<std::pair<std::string, size_t>>*)sym_data;
                symbols.emplace_back(name, address);
            }, &symbols);

            if (symbols.empty() == false) {
                cpu->Jump(symbols.front().second);
            }
        }

        if (refresh) {
            refresh = false;
            ImGui::BeginChild("##100");
            if (ImGui::GetScrollY() == ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);
            ImGui::EndChild();
            if (cpu) {
                status = cpu->Status();
                disasm = cpu->Disassemble(64);
            }
        }

        if (showMemoryEditor) {
            static MemoryEditor memoryEditor;
            if (cpu) {
                auto allocator = cpu->Allocator();
                ImGui::Begin("Memory", &showMemoryEditor, ImGuiWindowFlags_NoScrollbar);
                ImGui::End();
                memoryEditor.DrawWindow("Memory", allocator->address(), allocator->max_size());
            }
        }
    }
    ImGui::End();

    return updated;
}
//------------------------------------------------------------------------------
