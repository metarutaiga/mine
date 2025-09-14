//==============================================================================
// miCPU : Debugger Source
//
// Copyright (c) 2025 TAiGA
// https://github.com/metarutaiga/miCPU
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

//------------------------------------------------------------------------------
static miCPU* cpu;
static std::string arguments;
static std::string status;
static std::string logs[3];
static std::map<size_t, std::string> disasms;
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
    size_t offset = logs[index].size();
    logs[index].resize(offset + length);
    vsnprintf(logs[index].data() + offset, length, format, va);
    logs[index].pop_back();
    if (index == SYSTEM || index == CALL) {
        if (logs[index].empty() == false && logs[index].back() != '\n') {
            logs[index] += '\n';
        }
    }
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
static size_t Exception(miCPU* data, size_t index)
{
    size_t result = 0;
    if (result == 0) {
        result = syscall_windows_execute(data, index, LoggerV<SYSTEM>, LoggerV<CONSOLE>);
    }
    if (result == 0) {
        result = syscall_i386_execute(cpu, index, LoggerV<SYSTEM>, LoggerV<CONSOLE>);
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
    arguments = "main";

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
    arguments = std::string();
    status = std::string();
    exports = std::vector<std::pair<std::string, size_t>>();
    logs[0] = std::string();
    logs[1] = std::string();
    logs[2] = std::string();
    disasms = std::map<size_t, std::string>();
    samples = std::vector<std::pair<std::string, std::string>>();
}
//------------------------------------------------------------------------------
bool Debugger::Update(const UpdateData& updateData, bool& show)
{
    if (show == false)
        return false;

    bool updated = false;
    ImGui::SetNextWindowSize(ImVec2(1536.0f, 900.0f), ImGuiCond_Appearing);
    if (ImGui::Begin("Debugger", &show, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoDocking)) {

        static int updateCount = 0;
        static int breakpointData[2] = {};
        static int breakpointProgram = 0;
        static int disasmIndex = 0;
        static int disasmFocus = -1;
        static int stackIndex = 0;
        static int stackFocus = -1;
        static bool refresh = false;
        static bool running = false;
        static bool showMemoryEditor = false;
        static const int allocatorSize = 16777216;
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
            ImGui::DockBuilderDockWindow("Console", bottomRight);
            ImGui::DockBuilderDockWindow("System", bottomLeft);
            ImGui::DockBuilderDockWindow("Call", bottomLeft);
            ImGui::DockBuilderFinish(id);

            id = ImGui::GetID("Debugger");
        }

        ImGui::DockSpace(id);

        if (ImGui::Begin("Disassembly", nullptr, ImGuiWindowFlags_NoScrollbar)) {
            struct Local {
                std::map<size_t, std::string>::iterator temp_it = disasms.begin();
                int temp_index = 0;
                char temp[128];
            } local;

            int disasmCount = (int)disasms.size();
            ImGui::SetNextWindowSize(ImGui::GetContentRegionAvail());
            ImGui::ListBox("##10", &disasmIndex, &disasmFocus, [](void* user_data, int index) -> const char* {
                Local& local = *(Local*)user_data;
                if (local.temp_index != index) {
                    local.temp_index = index;
                    local.temp_it = disasms.begin();
                    std::advance(local.temp_it, index);
                }
                auto& [address, disasm] = (*local.temp_it++);
                local.temp_index++;

                const char* comment = nullptr;
                auto comma = disasm.rfind(" ");
                if (comma != std::string::npos) {
                    size_t address = strtoll(disasm.c_str() + comma + 2, nullptr, 16);
                    if (address) {
                        auto it = disasms.lower_bound(address);
                        if (it == disasms.end() || std::abs(int64_t(address - (*it).first)) > 16)
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
            const char* icon_fa_play_stop = running ? ICON_FA_STOP : ICON_FA_PLAY;
            if (ImGui::Button(ICON_FA_FORWARD))     { refresh = true; if (cpu) cpu->Run();          } ImGui::SameLine();
            if (ImGui::Button(icon_fa_play_stop))   { running = !running;                           } ImGui::SameLine();
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
            ImGui::InputTextMultiline("##100", logs[0], ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_ReadOnly);
            ImGui::BeginChild("##100");
            if (ImGui::GetScrollY() == ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);
            ImGui::EndChild();
        }
        ImGui::End();
        if (ImGui::Begin("System", nullptr, ImGuiWindowFlags_NoScrollbar)) {
            ImGui::InputTextMultiline("##101", logs[1], ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_ReadOnly);
            ImGui::BeginChild("##101");
            if (ImGui::GetScrollY() == ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);
            ImGui::EndChild();
        }
        ImGui::End();
        if (ImGui::Begin("Call", nullptr, ImGuiWindowFlags_NoScrollbar)) {
            ImGui::InputTextMultiline("##102", logs[2], ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_ReadOnly);
            ImGui::BeginChild("##102");
            if (ImGui::GetScrollY() == ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);
            ImGui::EndChild();
        }
        ImGui::End();

        if (file.empty() == false) {
            syscall_windows_delete(cpu);
            delete cpu;

            status.clear();
            logs[0].clear();
            logs[1].clear();
            logs[2].clear();
            disasms.clear();
            exportIndex = 0;
            exports.clear();
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
                miCPU* cpu = (miCPU*)userdata;
                return cpu->Memory(base, size);
            }, cpu, Logger<SYSTEM>);
            if (image) {
                auto disassembly = [](void* image) {
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

                size_t stack_base = allocatorSize;
                size_t stack_limit = allocatorSize - stackSize;
                syscall_windows_new(cpu, stack_base, stack_limit, image, (int)args.size(), args.data(), 0, nullptr);
                syscall_windows_debug(cpu, disassembly);

                syscall_i386_new(cpu, file.substr(0, file.rfind('/')).c_str(), (int)args.size(), args.data(), 0, nullptr);

                exports.emplace_back("Entry", PE::Entry(image));
                PE::Imports(image, Symbol, Logger<SYSTEM>);
                PE::Exports(image, [](const char* name, size_t address, void* sym_data) {
                    auto& exports = *(std::vector<std::pair<std::string, size_t>>*)sym_data;
                    exports.emplace_back(name, address);
                }, &exports);

                disassembly(image);

                if (exports.empty() == false) {
                    cpu->Jump(exports.front().second);
                }
            }
        }

        if (running) {
            if (cpu && cpu->Step('INTO')) {
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

                auto it = disasms.find(cpu->Program());
                if (it != disasms.end()) {
                    disasmIndex = disasmFocus = (int)std::distance(disasms.begin(), it);
                }

                auto allocator = cpu->Allocator;
                size_t memory_size = allocator->max_size();
                stackIndex = stackFocus = (int)((cpu->Stack() - (memory_size - stackSize)) / sizeof(uint32_t));
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
