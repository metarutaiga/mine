//==============================================================================
// miCPU : Debugger Source
//
// Copyright (c) 2025 TAiGA
// https://github.com/metarutaiga/miCPU
//==============================================================================
#include "EmulatorPCH.h"
#include "format/coff/pe.h"
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
static miCPU* cpu;
static std::string status;
static std::string disasm;
static std::string logs;
static int stackIndex;
static int symbolIndex;
static std::vector<std::pair<std::string, size_t>> symbols;
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
static void Exception(size_t index, void* memory, void* stack)
{
    auto memory8 = (uint8_t*)memory;
    auto stack32 = (uint32_t*)stack;
    switch (index) {
    case (uint32_t)-1: {
        size_t stackIndex = 2;
        std::vector<size_t> stack64;
        auto format = (const char*)memory8 + stack32[1];
        for (size_t i = 0; format[i]; ++i) {
            char c = format[i];
            if (c != '%')
                continue;
            int l = 0;
            for (size_t j = i + 1; format[j]; ++j) {
                char c = format[j];
                switch (c) {
                case 'l':
                    l++;
                    continue;
                case 'c':
                case 'd':
                case 'i':
                case 'o':
                case 'p':
                case 'u':
                case 'x':
                case 'X':
                    stack64.push_back(stack32[stackIndex++]);
                    if (l >= 2) {
                        stack64.back() |= (size_t)stack32[stackIndex++] << 32;
                    }
                    break;
                case 'a':
                case 'A':
                case 'e':
                case 'E':
                case 'f':
                case 'F':
                case 'g':
                case 'G':
                    stack64.push_back((size_t)stack32[stackIndex++]);
                    stack64.back() |= (size_t)stack32[stackIndex++] << 32;
                    break;
                case 's':
                    stack64.push_back((size_t)memory + stack32[stackIndex++]);
                    break;
                default:
                    continue;
                }
                i = j - 1;
                break;
            }
        }
        LoggerV(format, (va_list)stack64.data());
        break;
    }
    default:
        break;
    }
}
//------------------------------------------------------------------------------
static size_t Symbol(const char* file, const char* name, size_t address, void* sym_data)
{
    if (file) {
        switch (operator""_CC(name, strlen(name))) {
        case "printf"_CC:   address = (size_t)-1;   break;
        }
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
                auto* value = (uint32_t*)cpu->Memory(stack, sizeof(uint32_t));
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
                status = cpu->Status();
                disasm = cpu->Disassemble(64);
            }

            if (ImGui::Button("Step")) {
                if (cpu) {
                    cpu->Step();
                    status = cpu->Status();
                    disasm = cpu->Disassemble(64);
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Run")) {
                if (cpu) {
                    cpu->Run();
                    status = cpu->Status();
                    disasm = cpu->Disassemble(64);
                }
            }
            ImGui::Separator();
            if (ImGui::Button("Sample")) {
                disasm.clear();

                x86_i386 x86;
                x86.Initialize(1048576, sampleX86, sizeof(sampleX86));
                disasm = x86.Disassemble(countX86);
            }
            ImGui::SameLine();
            if (ImGui::Button("printf")) {
                file = xxGetExecutablePath();
                file += "/../../../../../SDK/miCPU/format/sample/pe.x86/printf.exe";
            }
            ImGui::SameLine();
            if (ImGui::Button("x86")) {
                file = xxGetExecutablePath();
                file += "/../../../../../SDK/miCPU/format/sample/pe.x86/x86.dll";
            }
            ImGui::SameLine();
            if (ImGui::Button("x86_arithmetic")) {
                file = xxGetExecutablePath();
                file += "/../../../../../SDK/miCPU/format/sample/pe.x86/x86_arithmetic.exe";
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

            cpu = new x86_i386;
            cpu->Initialize(16777216);
            cpu->Exception(Exception);

            PE pe;
            pe.Load(file.c_str(), [](size_t base, size_t size, void* userdata) {
                miCPU* cpu = (miCPU*)userdata;
                return cpu->Memory(base, size);
            }, cpu, Symbol, &symbols, Logger);

            if (symbols.empty() == false) {
                cpu->Jump(symbols.front().second);
            }
            status = cpu->Status();
            disasm = cpu->Disassemble(64);
        }
    }
    ImGui::End();

    return updated;
}
//------------------------------------------------------------------------------
