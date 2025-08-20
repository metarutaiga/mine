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
static int syscall_GetProcAddress(uint8_t* memory, uint32_t* stack);
static int syscall_GetModuleHandleA(uint8_t* memory, uint32_t* stack);
static int syscall_LoadLibary(uint8_t* memory, uint32_t* stack);
//------------------------------------------------------------------------------
static size_t Exception(miCPU* data, size_t index)
{
    auto* cpu = (x86_i386*)data;
    auto& x86 = cpu->x86;
    auto* allocator = cpu->allocator;
    auto* memory = cpu->Memory();
    auto* stack = (uint32_t*)(memory + cpu->Stack());

    auto EAX = [&](long value) {
        x86.regs[0].d = uint32_t(value);
        return 0;
    };

    switch (index) {

    // base
    case (uint32_t)-1001:   return EAX(syscall_printf(memory, stack, LoggerV));
    case (uint32_t)-1002:   return EAX(syscall_vprintf(memory, stack, LoggerV));
    case (uint32_t)-1011:   return 0;
    case (uint32_t)-1012:   return EAX(1);
    case (uint32_t)-1013:   return 0;
    case (uint32_t)-1014:   return 0;
    case (uint32_t)-1015:   return 0;
    case (uint32_t)-1016:   return 0;
    case (uint32_t)-1017:   return 0;
    case (uint32_t)-1018:   return 0;
    case (uint32_t)-1021:   return 0;
    case (uint32_t)-1022:   return 0;
    case (uint32_t)-1023:   return 0;
    case (uint32_t)-1024:   return 0;
    case (uint32_t)-1025:   return 0;
    case (uint32_t)-1026:   return 0;
    case (uint32_t)-1031:   return EAX(syscall_malloc(stack, allocator));
    case (uint32_t)-1032:   return EAX(syscall_free(stack, allocator));

    // kernel32.dll
    case (uint32_t)-1101:   syscall_exit(stack);                            return sizeof(uint32_t) * 1;
    case (uint32_t)-1102:   EAX(0);                                         return sizeof(uint32_t) * 1;
    case (uint32_t)-1103:   EAX(0);                                         return sizeof(uint32_t) * 2;
    case (uint32_t)-1104:   EAX(0);                                         return sizeof(uint32_t) * 2;
    case (uint32_t)-1105:   EAX(0);                                         return 0;
    case (uint32_t)-1106:   EAX(0);                                         return 0;
    case (uint32_t)-1107:   EAX(0);                                         return 0;
    case (uint32_t)-1108:   EAX(syscall_GetModuleHandleA(memory, stack));   return sizeof(uint32_t) * 1;
    case (uint32_t)-1109:   EAX(syscall_GetProcAddress(memory, stack));     return sizeof(uint32_t) * 2;
    case (uint32_t)-1110:   EAX(0);                                         return sizeof(uint32_t) * 1;
    case (uint32_t)-1111:   EAX(0);                                         return 0;
    case (uint32_t)-1112:   EAX(syscall_LoadLibary(memory, stack));         return sizeof(uint32_t) * 1;
    case (uint32_t)-1113:   EAX(0);                                         return sizeof(uint32_t) * 1;

    // advapi32.dll
    case (uint32_t)-1201:   EAX(0);                                         return sizeof(uint32_t) * 1;
    case (uint32_t)-1202:   EAX(0);                                         return sizeof(uint32_t) * 5;
    case (uint32_t)-1203:   EAX(0);                                         return sizeof(uint32_t) * 6;
    }

    return syscall_i386(cpu, index);
}
//------------------------------------------------------------------------------
static size_t Symbol(const char* file, const char* name, size_t address, void* sym_data)
{
    if (file) {
        switch (operator""_CC(name, strlen(name))) {
        case "printf"_CC:                       address = (uint32_t)-1001;  break;
        case "vprintf"_CC:                      address = (uint32_t)-1002;  break;
        case "_amsg_exit"_CC:                   address = (uint32_t)-1011;  break;
        case "_callnewh"_CC:                    address = (uint32_t)-1012;  break;
        case "_cexit"_CC:                       address = (uint32_t)-1013;  break;
        case "_controlfp"_CC:                   address = (uint32_t)-1014;  break;
        case "_flushall"_CC:                    address = (uint32_t)-1015;  break;
        case "_initterm"_CC:                    address = (uint32_t)-1016;  break;
        case "_onexit"_CC:                      address = (uint32_t)-1017;  break;
        case "_XcptFilter"_CC:                  address = (uint32_t)-1018;  break;
        case "__getmainargs"_CC:                address = (uint32_t)-1021;  break;
        case "__p__commode"_CC:                 address = (uint32_t)-1022;  break;
        case "__p__fmode"_CC:                   address = (uint32_t)-1023;  break;
        case "__p___initenv"_CC:                address = (uint32_t)-1024;  break;
        case "__setusermatherr"_CC:             address = (uint32_t)-1025;  break;
        case "__set_app_type"_CC:               address = (uint32_t)-1026;  break;
        case "??2@YAPAXI@Z"_CC:                 address = (uint32_t)-1031;  break;
        case "??_U@YAPAXI@Z"_CC:                address = (uint32_t)-1031;  break;
        case "??3@YAXPAX@Z"_CC:                 address = (uint32_t)-1032;  break;
        case "??_V@YAXPAX@Z"_CC:                address = (uint32_t)-1032;  break;
        }
        if (strcasecmp(file, "kernel32.dll") == 0) {
            switch (operator""_CC(name, strlen(name))) {
            case "ExitProcess"_CC:              address = (uint32_t)-1101;  break;
            case "FindClose"_CC:                address = (uint32_t)-1102;  break;
            case "FindFirstFileA"_CC:           address = (uint32_t)-1103;  break;
            case "FindNextFileA"_CC:            address = (uint32_t)-1104;  break;
            case "GetCurrentProcessId"_CC:      address = (uint32_t)-1105;  break;
            case "GetCurrentThreadId"_CC:       address = (uint32_t)-1106;  break;
            case "GetLastError"_CC:             address = (uint32_t)-1107;  break;
            case "GetModuleHandleA"_CC:         address = (uint32_t)-1108;  break;
            case "GetProcAddress"_CC:           address = (uint32_t)-1109;  break;
            case "GetSystemTimeAsFileTime"_CC:  address = (uint32_t)-1110;  break;
            case "GetTickCount"_CC:             address = (uint32_t)-1111;  break;
            case "LoadLibraryA"_CC:             address = (uint32_t)-1112;  break;
            case "QueryPerformanceCounter"_CC:  address = (uint32_t)-1113;  break;
            }
        }
        if (strcasecmp(file, "advapi32.dll") == 0) {
            switch (operator""_CC(name, strlen(name))) {
            case "RegCloseKey"_CC:              address = (uint32_t)-1201;  break;
            case "RegOpenKeyExA"_CC:            address = (uint32_t)-1202;  break;
            case "RegQueryValueExA"_CC:         address = (uint32_t)-1203;  break;
            }
        }
        if (address == 0) {
            address = syscall_symbol_i386(file, name, address, sym_data);
        }
    }
    else {
        auto& symbols = *(std::vector<std::pair<std::string, size_t>>*)sym_data;
        symbols.emplace_back(name, address);
    }
    return address;
}
//------------------------------------------------------------------------------
static int syscall_GetProcAddress(uint8_t* memory, uint32_t* stack)
{
    if (stack[1] == 0 || stack[2] == 0)
        return 0;
    auto image = (void*)(memory + stack[1]);
    auto function = (char*)(memory + stack[2]);

    struct Data {
        const char* name;
        size_t address;
    } data = { function };

    PE::Exports(image, [](const char* name, size_t address, void* sym_data) {
        auto& data = *(Data*)sym_data;
        if (strcmp(data.name, name) == 0)
            data.address = address;
    }, &data);

    return (int)data.address;
}
//------------------------------------------------------------------------------
static int syscall_GetModuleHandleA(uint8_t* memory, uint32_t* stack)
{
    auto name = (char*)(memory + stack[1]);
    if (name) {
        for (auto& [dll, image] : dlls) {
            if (dll == name)
                return (int)((uint8_t*)image - memory);
        }
    }

    return 0;
}
//------------------------------------------------------------------------------
static int syscall_LoadLibary(uint8_t* memory, uint32_t* stack)
{
    auto name = (char*)(memory + stack[1]);
    for (auto& [dll, image] : dlls) {
        if (dll == name)
            return (int)((uint8_t*)image - memory);
    }

    std::string folder;
    folder += xxGetExecutablePath();
    folder += "/../../../../../SDK/miCPU/format/sample/pe.x86/";
    folder += name;
#if defined(__APPLE__) || defined(__linux__)
    for (char& c : folder) {
        if (c == '\\')
            c = '/';
    }
#endif
    void* image = PE::Load(folder.c_str(), [](size_t base, size_t size, void* userdata) {
        miCPU* cpu = (miCPU*)userdata;
        return cpu->Memory(base, size);
    }, cpu, Logger);
    PE::Imports(image, Symbol, &symbols, Logger);
    dlls.emplace_back(name, image);
    return image ? (int)((uint8_t*)image - memory) : 0;
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

            void* image = PE::Load(file.c_str(), [](size_t base, size_t size, void* userdata) {
                miCPU* cpu = (miCPU*)userdata;
                return cpu->Memory(base, size);
            }, cpu, Logger);
            symbols.emplace_back("Entry", PE::Entry(image));
            PE::Imports(image, Symbol, &symbols, Logger);
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
