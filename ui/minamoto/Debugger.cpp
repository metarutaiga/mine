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

//------------------------------------------------------------------------------
static miCPU* cpu;
static std::string disasm;
static std::string status;
static std::string logs[2];
static int exportIndex;
static std::vector<std::pair<std::string, size_t>> exports;
//------------------------------------------------------------------------------
static std::vector<std::pair<std::string, std::string>> samples;
//------------------------------------------------------------------------------
template<int INDEX>
static int LoggerV(const char* format, va_list va)
{
    int length = vsnprintf(nullptr, 0, format, va) + 1;
    size_t offset = logs[INDEX].size();
    logs[INDEX].resize(offset + length);
    vsnprintf(logs[INDEX].data() + offset, length, format, va);
    logs[INDEX].pop_back();
    if (logs[INDEX].empty() == false && logs[INDEX].back() != '\n')
        logs[INDEX] += '\n';
    return length;
}
//------------------------------------------------------------------------------
template<int INDEX>
static int Logger(const char* format, ...)
{
    va_list va;
    va_start(va, format);
    int length = LoggerV<INDEX>(format, va);
    va_end(va);
    return length;
}
//------------------------------------------------------------------------------
static size_t Exception(miCPU* data, size_t index)
{
    size_t result = 0;
    if (result == 0) {
        result = syscall_windows_execute(data, index, Logger<0>, Logger<1>);
    }
    if (result == 0) {
        result = syscall_i386_execute(cpu, index, LoggerV<0>, LoggerV<1>);
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
    exports = std::vector<std::pair<std::string, size_t>>();
    logs[0] = std::string();
    logs[1] = std::string();
    samples = std::vector<std::pair<std::string, std::string>>();
}
//------------------------------------------------------------------------------
bool Debugger::Update(const UpdateData& updateData, bool& show)
{
    if (show == false)
        return false;

    bool updated = false;
    ImGui::SetNextWindowSize(ImVec2(1366.0f, 768.0f), ImGuiCond_Appearing);
    if (ImGui::Begin("Debugger", &show, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoDocking)) {

        static int stackIndex = 0;
        static int stackFocus = -1;
        static bool refresh = false;
        static bool running = false;
        static bool showMemoryEditor = false;
        static const int stackSize = 65536;
        std::string file;

        ImGuiID id = ImGui::GetID("Debugger");

        if (ImGui::DockBuilderGetNode(id) == nullptr) {
            id = ImGui::DockBuilderAddNode(id);

            ImGuiID bottomLeft = ImGui::DockBuilderSplitNode(id, ImGuiDir_Down, 1.0f / 4.0f, nullptr, &id);
            ImGuiID bottomRight = ImGui::DockBuilderSplitNode(bottomLeft, ImGuiDir_Right, 1.0f / 2.0f, nullptr, &bottomLeft);
            ImGuiID left = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 1.0f / 2.0f, nullptr, &id);
            ImGuiID rightTop = ImGui::DockBuilderSplitNode(id, ImGuiDir_Right, 1.0f / 2.0f, nullptr, &id);
            ImGuiID rightBottom = ImGui::DockBuilderSplitNode(rightTop, ImGuiDir_Down, 1.0f / 2.0f, nullptr, &rightTop);
            ImGuiID middleTop = ImGui::DockBuilderSplitNode(id, ImGuiDir_Down, 1.0f / 2.0f, nullptr, &id);
            ImGuiID middleBottom = ImGui::DockBuilderSplitNode(middleTop, ImGuiDir_Down, 1.0f / 2.0f, nullptr, &middleTop);
            ImGui::DockBuilderDockWindow("Disassembly", left);
            ImGui::DockBuilderDockWindow("Status", middleTop);
            ImGui::DockBuilderDockWindow("Stack", middleBottom);
            ImGui::DockBuilderDockWindow("Export", rightTop);
            ImGui::DockBuilderDockWindow("Menu", rightBottom);
            ImGui::DockBuilderDockWindow("Syslog", bottomLeft);
            ImGui::DockBuilderDockWindow("Console", bottomRight);
            ImGui::DockBuilderFinish(id);

            id = ImGui::GetID("Debugger");
        }

        ImGui::DockSpace(id);

        if (ImGui::Begin("Disassembly", nullptr, ImGuiWindowFlags_NoScrollbar)) {
            ImGui::InputTextMultiline("##10", disasm, ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_ReadOnly);
        }
        ImGui::End();
        if (ImGui::Begin("Status", nullptr, ImGuiWindowFlags_NoScrollbar)) {
            ImGui::InputTextMultiline("##20", status, ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_ReadOnly);
        }
        ImGui::End();
        if (ImGui::Begin("Stack", nullptr, ImGuiWindowFlags_NoScrollbar)) {
            char temp[32];
            int stackCount = stackSize / sizeof(uint32_t);
            ImGui::SetNextWindowSize(ImGui::GetContentRegionAvail());
            ImGui::ListBox("##30", &stackIndex, &stackFocus, [](void* user_data, int index) -> const char* {
                if (cpu == nullptr)
                    return "";

                auto allocator = cpu->Allocator();
                size_t stack = allocator->max_size() - stackSize + index * sizeof(uint32_t);
                auto* value = (uint32_t*)cpu->Memory(stack);
                if (value == nullptr)
                    return "";

                char* temp = (char*)user_data;
                snprintf(temp, 32, "%s%08zX %08X", stack == cpu->Stack() ? ">" : " ", stack, *value);
                return temp;
            }, temp, stackCount);
            stackFocus = -1;
        }
        ImGui::End();
        if (ImGui::Begin("Export", nullptr, ImGuiWindowFlags_NoScrollbar)) {
            ImGui::SetNextWindowSize(ImGui::GetContentRegionAvail());
            if (ImGui::ListBox("##40", &exportIndex, [](void* user_data, int index) {
                auto& exports = *(std::vector<std::pair<std::string, size_t>>*)user_data;
                return exports[index].first.c_str();
            }, &exports, (int)exports.size())) {
                cpu->Jump(exports[exportIndex].second);
                refresh = true;
            }
        }
        ImGui::End();
        if (ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoScrollbar)) {
            const char* icon_fa_play_stop = running ? ICON_FA_STOP : ICON_FA_PLAY;
            if (ImGui::Button(ICON_FA_FORWARD))     { refresh = true; if (cpu) cpu->Run();          } ImGui::SameLine();
            if (ImGui::Button(icon_fa_play_stop))   { running = !running;                           } ImGui::SameLine();
            if (ImGui::Button(ICON_FA_ARROW_RIGHT)) { refresh = true; if (cpu) cpu->Step('OVER');   } ImGui::SameLine();
            if (ImGui::Button(ICON_FA_ARROW_DOWN))  { refresh = true; if (cpu) cpu->Step('INTO');   } ImGui::SameLine();
            if (ImGui::Button(ICON_FA_ARROW_UP))    { refresh = true; if (cpu) cpu->Step('OUT ');   } ImGui::SameLine();
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
        }
        ImGui::End();
        if (ImGui::Begin("Syslog", nullptr, ImGuiWindowFlags_NoScrollbar)) {
            ImGui::InputTextMultiline("##100", logs[0], ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_ReadOnly);
            ImGui::BeginChild("##100");
            if (ImGui::GetScrollY() == ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);
            ImGui::EndChild();
        }
        ImGui::End();
        if (ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoScrollbar)) {
            ImGui::InputTextMultiline("##101", logs[1], ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_ReadOnly);
            ImGui::BeginChild("##101");
            if (ImGui::GetScrollY() == ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);
            ImGui::EndChild();
        }
        ImGui::End();

        if (file.empty() == false) {
            syscall_windows_delete(cpu);
            delete cpu;

            status.clear();
            disasm.clear();
            logs[0].clear();
            logs[1].clear();
            exportIndex = 0;
            exports.clear();
            stackIndex = 0;
            refresh = true;
            running = false;

            cpu = new x86_i386;
            cpu->Initialize(simple_allocator<16>::construct(16777216), stackSize);
            cpu->Exception(Exception);

            void* image = PE::Load(file.c_str(), [](size_t base, size_t size, void* userdata) {
                miCPU* cpu = (miCPU*)userdata;
                return cpu->Memory(base, size);
            }, cpu, Logger<0>);
            if (image) {
                syscall_windows_new(cpu, file.substr(0, file.rfind('/')).c_str(), image);

                exports.emplace_back("Entry", PE::Entry(image));
                PE::Imports(image, Symbol, Logger<0>);
                PE::Exports(image, [](const char* name, size_t address, void* sym_data) {
                    auto& exports = *(std::vector<std::pair<std::string, size_t>>*)sym_data;
                    exports.emplace_back(name, address);
                }, &exports);

                if (exports.empty() == false) {
                    cpu->Jump(exports.front().second);
                }
            }
        }

        if (running) {
            if (cpu && cpu->Step('INTO')) {
                refresh = true;
                updated = true;
            }
            else {
                running = false;
            }
        }

        if (refresh) {
            refresh = false;
            if (cpu) {
                status = cpu->Status();
                disasm = cpu->Disassemble(64);

                auto allocator = cpu->Allocator();
                stackFocus = (int)(cpu->Stack() - (allocator->max_size() - stackSize)) / sizeof(uint32_t);
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
