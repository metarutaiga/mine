//==============================================================================
// miCPU : Debugger Source
//
// Copyright (c) 2025 TAiGA
// https://github.com/metarutaiga/miCPU
//==============================================================================
#include "EmulatorPCH.h"
#include <IconFontCppHeaders/IconsFontAwesome4.h>
#include "format/coff/pe.h"
#include "format/syscall/simple_allocator.h"
#include "format/syscall/syscall.h"
#include "x86/x86_i386.h"
#include "Debugger.h"

//------------------------------------------------------------------------------
static miCPU* cpu;
static std::string status;
static std::string disasm;
static std::string logs;
static int stackIndex;
static int symbolIndex;
static std::vector<std::pair<std::string, size_t>> symbols;
static bool refresh;
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
static size_t Exception(miCPU* cpu, size_t index)
{
    auto allocator = cpu->Allocator();
    auto memory = cpu->Memory();
    auto stack = memory + cpu->Stack();
    switch (index) {
    case (uint32_t)-1:  return syscall_calloc(stack, allocator);
    case (uint32_t)-2:  return syscall_malloc(stack, allocator);
    case (uint32_t)-3:  return syscall_realloc(stack, allocator);
    case (uint32_t)-4:  return syscall_free(stack, allocator);
    case (uint32_t)-5:  return syscall_expand(stack, allocator);
    case (uint32_t)-6:  return syscall_msize(stack, allocator);
    case (uint32_t)-7:  return syscall_recalloc(stack, allocator);
    case (uint32_t)-11: return syscall_printf(memory, stack, LoggerV);
    case (uint32_t)-12: return syscall_vprintf(memory, stack, LoggerV);
    case (uint32_t)-13: return syscall_sprintf(memory, stack);
    case (uint32_t)-14: return syscall_vsprintf(memory, stack);
    case (uint32_t)-15: return syscall_snprintf(memory, stack);
    case (uint32_t)-16: return syscall_vsnprintf(memory, stack);
    }
    return 0;
}
//------------------------------------------------------------------------------
static size_t Symbol(const char* file, const char* name, size_t address, void* sym_data)
{
    if (file) {
        switch (operator""_CC(name, strlen(name))) {
        case "calloc"_CC:       return (uint32_t)-1;
        case "malloc"_CC:       return (uint32_t)-2;
        case "realloc"_CC:      return (uint32_t)-3;
        case "free"_CC:         return (uint32_t)-4;
        case "_expand"_CC:      return (uint32_t)-5;
        case "_msize"_CC:       return (uint32_t)-6;
        case "_recalloc"_CC:    return (uint32_t)-7;
        case "printf"_CC:       return (uint32_t)-11;
        case "vprintf"_CC:      return (uint32_t)-12;
        case "sprintf"_CC:      return (uint32_t)-13;
        case "vsprintf"_CC:     return (uint32_t)-14;
        case "snprintf"_CC:     return (uint32_t)-15;
        case "vsnprintf"_CC:    return (uint32_t)-16;
        }
        Logger("Symbol : %s.%s is not found", file, name);
    }
    else {
        auto& symbols = *(std::vector<std::pair<std::string, size_t>>*)sym_data;
        symbols.emplace_back(name, address);
    }
    return address;
}
//------------------------------------------------------------------------------
void Debugger::Initialize()
{
}
//------------------------------------------------------------------------------
void Debugger::Shutdown()
{
    delete cpu;
    disasm = std::string();
    status = std::string();
    symbols = std::vector<std::pair<std::string, size_t>>();
    logs = std::string();
}
//------------------------------------------------------------------------------
bool Debugger::Update(const UpdateData& updateData, bool& show)
{
    if (show == false)
        return false;

    bool updated = false;
    ImGui::SetNextWindowSize(ImVec2(1366.0f, 768.0f), ImGuiCond_Appearing);
    if (ImGui::Begin("Debugger", &show, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking)) {
        ImVec2 windowSize = ImGui::GetContentRegionAvail();

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
            if (ImGui::Button(ICON_FA_ARROW_UP))    { refresh = true; if (cpu) cpu->Step(-1); }
            ImGui::Separator();
            static const char* const samples[] = {
                "malloc.exe",
                "printf.exe",
                "x86.dll",
                "x86_arithmetic.exe",
            };
            for (auto sample : samples) {
                float pos = ImGui::CalcTextSize(sample).x + ImGui::GetStyle().FramePadding.x * 2.0f;
                float avail = ImGui::GetContentRegionAvail().x;
                if (pos > avail)
                    ImGui::NewLine();
                if (ImGui::Button(sample)) {
                    file = xxGetExecutablePath();
                    file += "/../../../../../SDK/miCPU/format/sample/pe.x86/";
                    file += sample;
                }
                ImGui::SameLine();
            }

            ImGui::EndTable();
        }
        ImGui::InputTextMultiline("##100", logs, ImVec2(windowSize.x, 256.0f - 16.0f), ImGuiInputTextFlags_ReadOnly);

        if (file.empty() == false) {
            delete cpu;
            status.clear();
            disasm.clear();
            logs.clear();
            stackIndex = 0;
            symbolIndex = 0;
            symbols.clear();
            refresh = true;

            cpu = new x86_i386;
            cpu->Initialize(SimpleAllocator<16>::Initialize(16777216));
            cpu->Exception(Exception);

            PE pe;
            pe.Load(file.c_str(), [](size_t base, size_t size, void* userdata) {
                miCPU* cpu = (miCPU*)userdata;
                return cpu->Memory(base, size);
            }, cpu, Symbol, &symbols, Logger);

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
    }
    ImGui::End();

    return updated;
}
//------------------------------------------------------------------------------
