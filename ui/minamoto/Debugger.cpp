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
static int exportIndex;
static int stackIndex;
static std::vector<std::pair<std::string, size_t>> exports;
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
    exports = std::vector<std::pair<std::string, size_t>>();
}
//------------------------------------------------------------------------------
bool Debugger::Update(const UpdateData& updateData, bool& show)
{
    if (show == false)
        return false;

    bool updated = false;
    ImGui::SetNextWindowSize(ImVec2(1366.0f, 768.0f), ImGuiCond_Appearing);
    if (ImGui::Begin("Debugger", &show, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking)) {
        ImVec2 windowSize = ImGui::GetWindowSize();

        std::string file;

        if (ImGui::BeginTable("Debugger", 3)) {
            float left = windowSize.x / 2.0f;
            float right = windowSize.x / 4.0f;
            float height = windowSize.y - 128.0f;
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
            if (ImGui::ListBox("##30", &exportIndex, [](void* user_data, int index) {
                auto& exports = *(std::vector<std::pair<std::string, size_t>>*)user_data;
                return exports[index].first.c_str();
            }, &exports, (int)exports.size())) {
                cpu->Jump(exports[exportIndex].second);
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

            ImGui::EndTable();
        }

        if (file.empty() == false) {
            delete cpu;
            status.clear();
            disasm.clear();
            exportIndex = 0;
            stackIndex = 0;
            exports.clear();

            cpu = new x86_i386;
            cpu->Initialize(16777216);

            PE pe;
            pe.Load(file.c_str(), [](size_t base, size_t size, void* userdata) {
                miCPU* cpu = (miCPU*)userdata;
                return cpu->Memory(base, size);
            }, cpu, [](const char* name, size_t address, void* sym_data) {
                if (address) {
                    auto& exports = *(std::vector<std::pair<std::string, size_t>>*)sym_data;
                    exports.emplace_back(name, address);
                }
                return address;
            }, &exports);

            if (exports.empty() == false) {
                cpu->Jump(exports.front().second);
            }
            status = cpu->Status();
            disasm = cpu->Disassemble(64);
        }
    }
    ImGui::End();

    return updated;
}
//------------------------------------------------------------------------------
