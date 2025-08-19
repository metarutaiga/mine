//==============================================================================
// miCPU : Debugger Source
//
// Copyright (c) 2025 TAiGA
// https://github.com/metarutaiga/miCPU
//==============================================================================
#include "EmulatorPCH.h"
#include <IconFontCppHeaders/IconsFontAwesome4.h>
#include "format/coff/pe.h"
#include "syscall/simple_allocator.h"
#include "syscall/syscall.h"
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
    auto* cpu = (x86_i386*)data;
    auto& x86 = cpu->x86;
    auto* memory = cpu->Memory();
    auto* stack = memory + cpu->Stack();

    auto EAX = [&](long value) {
        x86.regs[0].d = uint32_t(value);
        return 0;
    };

    switch (index) {

    // base
    case (uint32_t)-164:    return EAX(syscall_printf(memory, stack, LoggerV));
    case (uint32_t)-182:    return EAX(syscall_vprintf(memory, stack, LoggerV));

    // kernel32.dll
    case (uint32_t)-1001:   syscall_exit(stack);    return sizeof(uint32_t) * 1;
    case (uint32_t)-1002:   EAX(0);                 return sizeof(uint32_t) * 1;
    case (uint32_t)-1003:   EAX(0);                 return sizeof(uint32_t) * 2;
    case (uint32_t)-1004:   EAX(0);                 return sizeof(uint32_t) * 2;
    case (uint32_t)-1005:   EAX(0);                 return 0;
    case (uint32_t)-1006:   EAX(0);                 return 0;
    case (uint32_t)-1007:   EAX(0);                 return 0;
    case (uint32_t)-1008:   EAX(0);                 return sizeof(uint32_t) * 1;
    case (uint32_t)-1009:   EAX(0);                 return sizeof(uint32_t) * 2;
    case (uint32_t)-1010:   EAX(0);                 return sizeof(uint32_t) * 1;
    case (uint32_t)-1011:   EAX(0);                 return 0;
    case (uint32_t)-1012:   EAX(0);                 return sizeof(uint32_t) * 1;
    case (uint32_t)-1013:   EAX(0);                 return sizeof(uint32_t) * 1;

    // advapi32.dll
    case (uint32_t)-1101:   EAX(0);                 return sizeof(uint32_t) * 1;
    case (uint32_t)-1102:   EAX(0);                 return sizeof(uint32_t) * 5;
    case (uint32_t)-1103:   EAX(0);                 return sizeof(uint32_t) * 6;
    }

    return syscall_i386(cpu, index);
}
//------------------------------------------------------------------------------
static size_t Symbol(const char* file, const char* name, size_t address, void* sym_data)
{
    if (file) {
        if (strcasecmp(file, "kernel32.dll") == 0) {
            switch (operator""_CC(name, strlen(name))) {
            case "ExitProcess"_CC:              address = (uint32_t)-1001;  break;
            case "FindClose"_CC:                address = (uint32_t)-1002;  break;
            case "FindFirstFileA"_CC:           address = (uint32_t)-1003;  break;
            case "FindNextFileA"_CC:            address = (uint32_t)-1004;  break;
            case "GetCurrentProcessId"_CC:      address = (uint32_t)-1005;  break;
            case "GetCurrentThreadId"_CC:       address = (uint32_t)-1006;  break;
            case "GetLastError"_CC:             address = (uint32_t)-1007;  break;
            case "GetModuleHandleA"_CC:         address = (uint32_t)-1008;  break;
            case "GetProcAddress"_CC:           address = (uint32_t)-1009;  break;
            case "GetSystemTimeAsFileTime"_CC:  address = (uint32_t)-1010;  break;
            case "GetTickCount"_CC:             address = (uint32_t)-1011;  break;
            case "LoadLibraryA"_CC:             address = (uint32_t)-1012;  break;
            case "QueryPerformanceCounter"_CC:  address = (uint32_t)-1013;  break;
            }
        }
        if (strcasecmp(file, "advapi32.dll") == 0) {
            switch (operator""_CC(name, strlen(name))) {
            case "RegCloseKey"_CC:              address = (uint32_t)-1101;  break;
            case "RegOpenKeyExA"_CC:            address = (uint32_t)-1102;  break;
            case "RegQueryValueExA"_CC:         address = (uint32_t)-1103;  break;
            }
        }
        if (address == 0) {
            address = syscall_symbol_i386(file, name, address, sym_data);
        }
        if (address == 0) {
            Logger("Symbol : %s.%s is not found", file, name);
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
            delete cpu;
            status.clear();
            disasm.clear();
            logs.clear();
            stackIndex = 0;
            symbolIndex = 0;
            symbols.clear();
            refresh = true;

            cpu = new x86_i386;
            cpu->Initialize(simple_allocator<16>::construct(16777216));
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
