//==============================================================================
// mine : Debugger Source
//
// Copyright (c) 2025 TAiGA
// https://github.com/metarutaiga/mine
//==============================================================================
#include "EmulatorPCH.h"
#include <map>
#include <IconFontCppHeaders/IconsFontAwesome4.h>
#include <imgui_club/imgui_memory_editor/imgui_memory_editor.h>
#include "format/coff/pe.h"
#include "syscall/simple_allocator.h"
#include "syscall/syscall.h"
#include "syscall/windows/syscall_windows.h"
#include "x86/x86_i386.h"
#include "Debugger.h"
#if defined(_WIN32)
#include <windows.h>
#endif

//------------------------------------------------------------------------------
static mine* cpu;
static std::string arguments;
static std::string status;
static std::vector<std::string> logs[3];
static bool logsUpdated[3];
static std::map<std::string, std::map<size_t, std::string>> disasmses;
static int exportIndex;
static std::vector<std::pair<std::string, size_t>> exports;
//------------------------------------------------------------------------------
static std::vector<std::pair<std::string, std::string>> samples;
//------------------------------------------------------------------------------
#define CONSOLE 0
#define SYSTEM  1
#define CALL    2
//------------------------------------------------------------------------------
template<int INDEX>
static int LoggerV(const char* format, va_list va)
{
    int index = INDEX;
    int length = vsnprintf(nullptr, 0, format, va) + 1;
    if (index == SYSTEM) {
        if (strncmp(format, "[CALL]", 6) == 0) {
            index = CALL;
        }
    }
    if (index == SYSTEM || index == CALL) {
        logs[index].push_back(std::string());
    }
    else if (logs[index].empty()) {
        logs[index].push_back(std::string());
    }
    std::string& log = logs[index].back();
    size_t offset = log.size();
    log.resize(offset + length);
    vsnprintf(log.data() + offset, length, format, va);
    log.pop_back();

    if (index == CONSOLE) {

        // Tab
        for (size_t i = offset, j = 0; i < log.size(); ++i, ++j) {
            char c = log[i];
            if (c == '\n')
                j = size_t(0) - 1;
            if (c != '\t')
                continue;
            size_t tab = 8 - (j % 8);
            log.replace(i, 1, tab, ' ');
            i += tab - 1;
            j += tab - 1;
        }
        
        // Breakline
        bool first = true;
        size_t len = log.size();
        std::vector<std::string> appends;
        appends.push_back(std::string());
        for (size_t i = offset; i < log.size(); ++i) {
            char c = log[i];
            if (c == '\n') {
                if (first) {
                    len = i;
                }
                first = false;
                appends.push_back(std::string());
                continue;
            }
            if (first) {
                continue;
            }
            appends.back().push_back(c);
        }
        log.resize(len);
        std::move(appends.begin() + 1, appends.end(), std::back_inserter(logs[index]));
    }

    // Call
    if (index == CALL && logs[index].size() >= 2) {
        auto back1 = logs[index][logs[index].size() - 2];
        auto back2 = logs[index][logs[index].size() - 1];
        if (back1.size() >= back2.size() && back1.compare(0, back2.size(), back2) == 0) {
            logs[index].pop_back();
            logs[index].back() += '.';
        }
    }

    logsUpdated[index] = true;
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
static const char* Name(size_t index)
{
    const char* name = nullptr;
    if (name == nullptr) {
        name = syscall_windows_name(index);
    }
    if (name == nullptr) {
        name = syscall_i386_name(index);
    }
    return name;
}
//------------------------------------------------------------------------------
static size_t Exception(mine* data, size_t index)
{
    size_t result = 0;
    if (result == 0) {
        result = syscall_windows_execute(data, index);
    }
    if (result == 0) {
        result = syscall_i386_execute(cpu, index);
    }
    return result;
}
//------------------------------------------------------------------------------
static size_t Symbol(const char* file, const char* name, void* symbol_data)
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
    arguments = "main";

    std::string folder;
    folder += xxGetExecutablePath();
    folder += "/../../../../../SDK/mine/format/sample/pe.x86/";

    uint64_t handle = 0;
    while (char* filename = xxOpenDirectory(&handle, folder.c_str(), nullptr)) {
        if (strstr(filename, ".dll") || strstr(filename, ".exe")) {
            samples.emplace_back(filename, folder + filename);
        }
        xxFree(filename);
    }
    xxCloseDirectory(&handle);

    disasmses["Disassembly"];
}
//------------------------------------------------------------------------------
void Debugger::Shutdown()
{
    delete cpu;
    arguments = std::string();
    status = std::string();
    exports = std::vector<std::pair<std::string, size_t>>();
    logs[0].clear();
    logs[1].clear();
    logs[2].clear();
    disasmses = std::map<std::string, std::map<size_t, std::string>>();
    samples = std::vector<std::pair<std::string, std::string>>();
}
//------------------------------------------------------------------------------
bool Debugger::Update(const UpdateData& updateData, bool& show)
{
    if (show == false)
        return false;

    bool updated = false;
    ImGuiViewport* viewport = ImGui::GetWindowViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos + ImVec2((viewport->WorkSize.x - 1600.0f) / 2.0f, (viewport->WorkSize.y - 900.0f) / 2.0f), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(1600.0f, 900.0f), ImGuiCond_Once);
    if (ImGui::Begin("Debugger", &show, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoDocking)) {

        static int updateCount = 0;
        static int breakpointData[2] = {};
        static int breakpointProgram = 0;
        static int logsIndex[3] = {};
        static int logsFocus[3] = { -1, -1, -1 };
        static int disasmIndex = 0;
        static int disasmFocus = -1;
        static std::string disasmFocusTitle;
        static int stackIndex = 0;
        static int stackFocus = -1;
        static bool refresh = false;
        static int running = 0;
        static bool showMemoryEditor = false;
        static const int allocatorSize = 16777216;
        static const int stackSize = 65536;
        static ImGuiID disassemblyDockID;
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
            ImGui::DockBuilderDockWindow("Console", bottomRight);
            ImGui::DockBuilderDockWindow("System", bottomLeft);
            ImGui::DockBuilderDockWindow("Call", bottomLeft);
            ImGui::DockBuilderFinish(id);

            disassemblyDockID = left;

            id = ImGui::GetID("Debugger");
        }

        ImGui::DockSpace(id);

        for (auto& [title, disasms] : disasmses) {
            ImGui::DockBuilderDockWindow(title.c_str(), disassemblyDockID);
            if (disasmFocusTitle == title) {
                disasmFocusTitle.clear();
                ImGuiWindow* window = ImGui::FindWindowByName(title.c_str());
                if (window && window->DockNode && window->DockNode->TabBar)
                    window->DockNode->TabBar->NextSelectedTabId = window->TabId;
            }
            if (ImGui::Begin(title.c_str(), nullptr, ImGuiWindowFlags_NoScrollbar)) {
                struct Local {
                    std::map<size_t, std::string>& disasms;
                    std::map<size_t, std::string>::iterator temp_it;
                    int temp_index;
                    char temp[128];
                } local = {
                    .disasms = disasms,
                    .temp_it = disasms.begin(),
                };

                int disasmCount = (int)disasms.size();
                ImGui::SetNextWindowSize(ImGui::GetContentRegionAvail());
                ImGui::ListBox("##10", &disasmIndex, &disasmFocus, [](void* user_data, int index) -> const char* {
                    Local& local = *(Local*)user_data;
                    if (local.temp_index != index) {
                        local.temp_index = index;
                        local.temp_it = local.disasms.begin();
                        std::advance(local.temp_it, index);
                    }
                    auto& [address, disasm] = (*local.temp_it++);
                    local.temp_index++;

                    const char* comment = nullptr;
                    auto comma = disasm.rfind(" ");
                    if (comma != std::string::npos) {
                        size_t address = strtoll(disasm.c_str() + comma + 2, nullptr, 16);
                        if (address) {
                            auto it = local.disasms.lower_bound(address);
                            if (it == local.disasms.end() || std::abs(int64_t(address - (*it).first)) > 16)
                                comment = (char*)cpu->Memory(address);
                            if (comment && (address + 4) <= allocatorSize) {
                                uint32_t index = *(uint32_t*)comment;
                                const char* name = Name(index);
                                if (name) {
                                    comment = name;
                                }
                            }
                        }
                    }
                    if (comment == nullptr || (comment[0] < 0x20 || comment[0] > 0x7E)) {
                        comment = "";
                    }

                    int column = 8 + 3 + 32 + 40;
                    snprintf(local.temp, 128, "%*s", column, "");
                    auto pos = ImGui::GetCursorPos();
                    ImGui::Selectable(local.temp);
                    if (ImGui::IsItemHovered()) {
                        if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle)) {
                            cpu->BreakpointProgram = breakpointProgram = (int)address;
                        }
                        if (comment[0]) {
                            ImGui::SetTooltip("%s", comment);
                        }
                    }
                    ImGui::SetCursorPos(pos);
                    snprintf(local.temp, 128, "%-*s%s", column, disasm.c_str(), comment);
                    for (int i = column; i < column + 16; ++i) {
                        if (local.temp[i] == '\r' || local.temp[i] == '\n')
                            local.temp[i] = ' ';
                    }
                    local.temp[column + 16] = '.';
                    local.temp[column + 17] = '.';
                    local.temp[column + 18] = '.';
                    local.temp[column + 19] = 0;

                    return local.temp;
                }, &local, disasmCount);
                disasmFocus = -1;
            }
            ImGui::End();
        }
        if (ImGui::Begin("Status", nullptr, ImGuiWindowFlags_NoScrollbar)) {
            ImGui::InputTextMultiline("##20", status, ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_ReadOnly);
        }
        ImGui::End();
        if (ImGui::Begin("Stack", nullptr, ImGuiWindowFlags_NoScrollbar)) {
            char temp[128];
            int stackCount = stackSize / sizeof(uint32_t);
            ImGui::SetNextWindowSize(ImGui::GetContentRegionAvail());
            ImGui::ListBox("##30", &stackIndex, &stackFocus, [](void* user_data, int index) -> const char* {
                if (cpu == nullptr)
                    return "";

                auto allocator = cpu->Allocator;
                size_t memory_size = allocator->max_size();
                size_t stack = memory_size - stackSize + index * sizeof(uint32_t);
                auto* value = (uint32_t*)cpu->Memory(stack);
                if (value == nullptr)
                    return "";
                const char* text = nullptr;
                if (*value) {
                    text = (char*)cpu->Memory(*value);
                    if (text) {
                        uint32_t index = *(uint32_t*)text;
                        const char* name = Name(index);
                        if (name) {
                            text = name;
                        }
                    }
                }
                if (text == nullptr || (text[0] < 0x20 || text[0] > 0x7E)) {
                    text = "";
                }

                char* temp = (char*)user_data;
                snprintf(temp, 128, "%*s", 8 + 1 + 8 + 28, "");
                auto pos = ImGui::GetCursorPos();
                ImGui::Selectable(temp);
                if (ImGui::IsItemHovered() && text[0]) {
                    ImGui::SetTooltip("%s", text);
                }
                ImGui::SetCursorPos(pos);

                snprintf(temp, 128, "%08zX %08X %.28s", stack, *value, text);
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
            const char* icon_fa_forward_stop = running ? ICON_FA_STOP "##200" : ICON_FA_FORWARD;
            const char* icon_fa_play_stop = running ? ICON_FA_STOP "##201" : ICON_FA_PLAY;
//          if (ImGui::Button(ICON_FA_FORWARD))     { refresh = true; if (cpu) cpu->Run();          } ImGui::SameLine();
            if (ImGui::Button(icon_fa_forward_stop)){ running = running ? false : 2;                } ImGui::SameLine();
            if (ImGui::Button(icon_fa_play_stop))   { running = running ? false : 1;                } ImGui::SameLine();
            if (ImGui::Button(ICON_FA_ARROW_RIGHT)) { refresh = true; if (cpu) cpu->Step('OVER');   } ImGui::SameLine();
            if (ImGui::Button(ICON_FA_ARROW_DOWN))  { refresh = true; if (cpu) cpu->Step('INTO');   } ImGui::SameLine();
            if (ImGui::Button(ICON_FA_ARROW_UP))    { refresh = true; if (cpu) cpu->Step('OUT ');   } ImGui::SameLine();
            ImGui::Checkbox("Memory", &showMemoryEditor);
            ImGui::InputTextEx("Argument", nullptr, arguments);
            ImGui::Separator();
            if (ImGui::InputScalarN("Data", ImGuiDataType_U32, breakpointData, 2, nullptr, nullptr, "%08X")) {
                if (cpu) cpu->BreakpointDataAddress = breakpointData[0];
                if (cpu) cpu->BreakpointDataValue = breakpointData[1];
            }
            if (ImGui::InputInt("Program", &breakpointProgram, 0, 0, ImGuiInputTextFlags_CharsHexadecimal)) {
                if (cpu) cpu->BreakpointProgram = breakpointProgram;
            }
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
        if (ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoScrollbar)) {
            ImGui::SetNextWindowSize(ImGui::GetContentRegionAvail());
            ImGui::ListBox("##100", &logsIndex[0], &logsFocus[0], [](void* user_data, int index) {
                auto* logs = (std::string*)user_data;
                return logs[index].c_str();
            }, logs[0].data(), (int)logs[0].size());
            logsFocus[0] = -1;
        }
        ImGui::End();
        if (ImGui::Begin("System", nullptr, ImGuiWindowFlags_NoScrollbar)) {
            ImGui::SetNextWindowSize(ImGui::GetContentRegionAvail());
            ImGui::ListBox("##101", &logsIndex[1], &logsFocus[1], [](void* user_data, int index) {
                auto* logs = (std::string*)user_data;
                return logs[index].c_str();
            }, logs[1].data(), (int)logs[1].size());
            logsFocus[1] = -1;
        }
        ImGui::End();
        if (ImGui::Begin("Call", nullptr, ImGuiWindowFlags_NoScrollbar)) {
            ImGui::SetNextWindowSize(ImGui::GetContentRegionAvail());
            ImGui::ListBox("##102", &logsIndex[2], &logsFocus[2], [](void* user_data, int index) {
                auto* logs = (std::string*)user_data;
                return logs[index].c_str();
            }, logs[2].data(), (int)logs[2].size());
            logsFocus[2] = -1;
        }
        ImGui::End();

        if (file.empty() == false) {
            syscall_windows_delete(cpu);
            delete cpu;

            status.clear();
            logs[0].clear();
            logs[1].clear();
            logs[2].clear();
            disasmses.clear();
            disasmses["Disassembly"];
            exportIndex = 0;
            exports.clear();
            logsIndex[0] = 0;
            logsIndex[1] = 0;
            logsIndex[2] = 0;
            logsFocus[0] = -1;
            logsFocus[1] = -1;
            logsFocus[2] = -1;
            disasmIndex = 0;
            disasmFocus = -1;
            stackIndex = 0;
            stackFocus = -1;
            refresh = true;
            running = false;

            cpu = new x86_i386;
            cpu->Initialize(simple_allocator<16>::construct(allocatorSize), stackSize);
            cpu->BreakpointDataAddress = breakpointData[0];
            cpu->BreakpointDataValue = breakpointData[1];
            cpu->BreakpointProgram = breakpointProgram;
            cpu->Exception = Exception;

            void* image = PE::Load(file.c_str(), [](size_t base, size_t size, void* userdata) {
                mine* cpu = (mine*)userdata;
                return cpu->Memory(base, size);
            }, cpu, Logger<SYSTEM>);
            if (image) {
                auto disassembly = [](const char* file, void* image) {
                    auto& disasms = disasmses[file];
                    size_t base = 0;
                    size_t address = 0;
                    size_t size = 0;
                    PE::SectionCode(image, &base, &address, &size);
                    size_t begin = base + address;
                    size_t end = begin + size;
                    size_t program = cpu->Program();
                    while (begin < end) {
                        cpu->Jump(begin);
                        std::string disasm = cpu->Disassemble(1);
                        if (disasm.empty())
                            break;
                        if (disasm.back() == '\n')
                            disasm.pop_back();
                        size_t space = disasm.find("  ");
                        if (space == std::string::npos)
                            space = 15;
                        size_t address = strtoll(disasm.c_str(), nullptr, 16);
                        disasms[address] = disasm;
                        begin += (space - 8 - 3) / 2;
                    }
//                  auto it = disasms.lower_bound(end);
//                  if (it != disasms.begin())
//                      it--;
//                  while (it != disasms.begin()) {
//                      auto rem = it--;
//                      if ((*rem).second.find("0000  ") == std::string::npos)
//                          break;
//                      disasms.erase(rem);
//                  }
                    cpu->Jump(program);
                };

                std::vector<std::string> parsed;
                std::vector<const char*> args;
                for (size_t pos = 0, next = 0; next != std::string::npos; pos = arguments.find_first_not_of(' ', next)) {
                    next = arguments.find(' ', pos);
                    parsed.push_back(arguments.substr(pos, next - pos));
                }
                for (auto& arg : parsed) {
                    args.push_back(arg.data());
                }

                std::string basename = file.substr(0, file.rfind('/'));
                Syscall syscall = {
                    .path = basename.c_str(),
                    .argc = (int)args.size(),
                    .argv = args.data(),
                    .printf = Logger<CONSOLE>,
                    .vprintf = LoggerV<CONSOLE>,
                    .debugPrintf = Logger<SYSTEM>,
                    .debugVprintf = LoggerV<SYSTEM>,
                };
                syscall_i386_new(cpu, &syscall);

                SyscallWindows syscallWindows = {
                    .stack_base = allocatorSize,
                    .stack_limit = allocatorSize - stackSize,
                    .symbol = Symbol,
                    .debugModule = disassembly,
                    .argc = (int)args.size(),
                    .argv = args.data(),
                };
                syscall_windows_new(cpu, &syscallWindows);
                syscall_windows_import(cpu, "Disassembly", image, true);

                exports.emplace_back("Entry", PE::Entry(image));
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
            if (cpu) {
                uint32_t count = 0;
                uint32_t begin = 0;
                for (;;) {
                    if (cpu->Step('INTO') == false) {
                        running = false;
                        break;
                    }
                    if (running == 1)
                        break;
                    if (count == 0) {
                        count = 1000;
#if defined(_WIN32)
                        uint32_t now = GetTickCount();
#else
                        struct timespec ts = {};
                        clock_gettime(CLOCK_REALTIME, &ts);
                        uint32_t now = uint32_t(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#endif
                        if (begin == 0)
                            begin = now;
                        if (begin < now - 16)
                            break;
                    }
                    count--;
                }
                refresh = true;
            }
            else {
                running = false;
            }
        }

        if (refresh) {
            refresh = false;
            if (cpu) {
                status = cpu->Status();

                for (auto& [title, disasms] : disasmses) {
                    auto it = disasms.find(cpu->Program());
                    if (it != disasms.end()) {
                        disasmIndex = disasmFocus = (int)std::distance(disasms.begin(), it);
                        disasmFocusTitle = title;
                        break;
                    }
                }

                auto allocator = cpu->Allocator;
                size_t memory_size = allocator->max_size();
                stackIndex = stackFocus = (int)((cpu->Stack() - (memory_size - stackSize)) / sizeof(uint32_t));

                for (int i = 0; i < 3; ++i) {
                    if (logsUpdated[i]) {
                        logsUpdated[i] = false;
                        logsFocus[i] = (int)logs[i].size() - 1;
                    }
                }
            }
            updateCount = 3;
        }

        if (showMemoryEditor) {
            static MemoryEditor memoryEditor;
            if (cpu) {
                auto allocator = cpu->Allocator;
                ImGui::Begin("Memory", &showMemoryEditor, ImGuiWindowFlags_NoScrollbar);
                ImGui::End();
                memoryEditor.DrawWindow("Memory", allocator->address(), allocator->max_size());
            }
        }

        if (updateCount) {
            updateCount--;
            updated = true;
        }
    }
    ImGui::End();

    return updated;
}
//------------------------------------------------------------------------------
