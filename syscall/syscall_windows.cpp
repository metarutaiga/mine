#include <string>
#include <vector>
#include "syscall.h"
#include "syscall_internal.h"
#include "syscall_windows.h"
#include "format/coff/pe.h"
#include "x86/x86_i386.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SYMBOL_INDEX 1001

struct Windows {
    bool initialized;
    std::vector<std::pair<std::string, void*>> modules;
    char currentDirectory[280];
};

int syscall_GetProcAddress(uint8_t* memory, uint32_t* stack)
{
    if (stack[1] == 0 || stack[2] == 0)
        return 0;
    auto image = physical(void*, stack[1]);
    auto function = physical(char*, stack[2]);

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

int syscall_GetModuleHandleA(uint8_t* memory, uint32_t* stack)
{
    auto* windows = physical(Windows*, 0x700);
    auto& modules = windows->modules;
    auto name = (char*)(memory + stack[1]);
    if (name) {
        std::string file = name;
        file = file.substr(file.find_last_of("/\\") + 1);
        for (auto& [name, image] : modules) {
            if (strcasecmp(name.c_str(), file.c_str()) == 0)
                return virtual(int, image);
        }
    }

    return 0;
}

int syscall_LoadLibraryA(uint8_t* memory, uint32_t* stack, miCPU* cpu, int(*log)(const char*, ...))
{
    int module = syscall_GetModuleHandleA(memory, stack);
    if (module)
        return module;

    auto* windows = physical(Windows*, 0x700);
    auto& modules = windows->modules;
    auto name = (char*)(memory + stack[1]);

    std::string path = windows->currentDirectory;
    path += '\\';
    path += name;
#if defined(_WIN32)
#else
    for (char& c : path) {
        if (c == '\\')
            c = '/';
    }
#endif
    void* image = PE::Load(path.c_str(), [](size_t base, size_t size, void* userdata) {
        miCPU* cpu = (miCPU*)userdata;
        return cpu->Memory(base, size);
    }, cpu, log);
    PE::Imports(image, [](const char* file, const char* name) {
        size_t address = 0;
        if (address == 0)
            address = syscall_windows_symbol(file, name);
        if (address == 0)
            address = syscall_i386_symbol(file, name);
        return address;
    }, log);
    modules.emplace_back(path.substr(path.find_last_of("/\\") + 1).c_str(), image);
    return virtual(int, image);
}

int syscall_FreeLibrary(uint8_t* memory, uint32_t* stack)
{
    return 0;
}

int syscall_SetCurrentDirectoryA(uint8_t* memory, uint32_t* stack)
{
    auto* windows = physical(Windows*, 0x700);
    char* pathName = physical(char*, stack[1]);
    strcpy(windows->currentDirectory, pathName);
#if defined(_WIN32)
    return true;
#else
    for (char& c : windows->currentDirectory) {
        if (c == '\\')
            c = '/';
    }
    return true;
#endif
}

#define EAX(count, value) \
    [](x86_i386* cpu, uint8_t* memory, uint32_t* stack, allocator_t* allocator, int(*log)(const char*, ...)) -> size_t { \
        x86_instruction& x86 = cpu->x86; \
        auto temp = value; \
        x86.regs[0].d = uint32_t(temp); \
        return count; \
    }

static const struct {
    const char* name;
    size_t (*syscall)(x86_i386* cpu, uint8_t* memory, uint32_t* stack, allocator_t* allocator, int(*log)(const char*, ...));
} syscall_tables[] = {

    // kernel32
    { "GetProcAddress",         EAX(2, syscall_GetProcAddress(memory, stack))           },
    { "GetModuleHandleA",       EAX(1, syscall_GetModuleHandleA(memory, stack))         },
    { "LoadLibraryA",           EAX(1, syscall_LoadLibraryA(memory, stack, cpu, log))   },
    { "FreeLibrary",            EAX(1, syscall_FreeLibrary(memory, stack))              },
    { "SetCurrentDirectoryA",   EAX(1, syscall_SetCurrentDirectoryA(memory, stack))     },
    { "ExitProcess",            EAX(1, syscall_exit(stack))                             },
    { "FindClose",              EAX(1, 0)                                               },
    { "FindFirstFileA",         EAX(2, 0)                                               },
    { "FindNextFileA",          EAX(2, 0)                                               },
    { "GetCurrentProcessId",    EAX(0, 0)                                               },
    { "GetCurrentThreadId",     EAX(0, 0)                                               },
    { "GetLastError",           EAX(0, 0)                                               },
    { "GetSystemTimeAsFileTime",EAX(1, 0)                                               },
    { "GetTickCount",           EAX(0, 0)                                               },
    { "QueryPerformanceCounter",EAX(0, 0)                                               },

    // advapi32
    { "RegCloseKey",            EAX(1, 0)                                               },
    { "RegOpenKeyExA",          EAX(5, 0)                                               },
    { "RegQueryValueExA",       EAX(6, 0)                                               },

    // msvcrt
    { "_amsg_exit",             EAX(0, 0)                                               },
    { "_callnewh",              EAX(0, 1)                                               },
    { "_cexit",                 EAX(0, 0)                                               },
    { "_controlfp",             EAX(0, 0)                                               },
    { "_flushall",              EAX(0, 0)                                               },
    { "_initterm",              EAX(0, 0)                                               },
    { "_onexit",                EAX(0, 0)                                               },
    { "_XcptFilter",            EAX(0, 0)                                               },
    { "__getmainargs",          EAX(0, 0)                                               },
    { "__p__commode",           EAX(0, 0)                                               },
    { "__p__fmode",             EAX(0, 0)                                               },
    { "__p___initenv",          EAX(0, 0)                                               },
    { "__setusermatherr",       EAX(0, 0)                                               },
    { "__set_app_type",         EAX(0, 0)                                               },
    { "??2@YAPAXI@Z",           EAX(0, syscall_malloc(stack, allocator))                },
    { "??_U@YAPAXI@Z",          EAX(0, syscall_malloc(stack, allocator))                },
    { "??3@YAXPAX@Z",           EAX(0, syscall_free(stack, allocator))                  },
    { "??_V@YAXPAX@Z",          EAX(0, syscall_free(stack, allocator))                  },
};

size_t syscall_windows_new(void* data, const char* path)
{
    if (data == nullptr)
        return 0;

    auto* cpu = (x86_i386*)data;
    auto* memory = cpu->Memory();
    auto* windows = physical(Windows*, 0x700);
    if (windows->initialized == false) {
        windows->initialized = true;
        new (windows) Windows;
    }
    realpath(path, windows->currentDirectory);
    return 0;
}

size_t syscall_windows_delete(void* data)
{
    if (data == nullptr)
        return 0;

    auto* cpu = (x86_i386*)data;
    auto* memory = cpu->Memory();
    auto* windows = physical(Windows*, 0x700);
    if (windows->initialized) {
        windows->initialized = false;
        windows->~Windows();
    }
    return 0;
}

size_t syscall_windows_execute(void* data, size_t index, int(*log)(const char*, ...))
{
    index = uint32_t(-index - SYMBOL_INDEX);

    size_t count = sizeof(syscall_tables) / sizeof(syscall_tables[0]);
    if (index < count) {
        auto* cpu = (x86_i386*)data;
        auto* memory = cpu->Memory();
        auto* stack = (uint32_t*)(memory + cpu->Stack());
        auto* allocator = cpu->Allocator();
        auto syscall = syscall_tables[index].syscall;
        return syscall(cpu, memory, stack, allocator, log) * sizeof(uint32_t);
    }

    return 0;
}

size_t syscall_windows_symbol(const char* file, const char* name)
{
    if (file == nullptr)
        return 0;

    size_t count = sizeof(syscall_tables) / sizeof(syscall_tables[0]);
    for (size_t index = 0; index < count; ++index) {
        if (strcmp(syscall_tables[index].name, name) == 0)
            return (uint32_t)(-index - SYMBOL_INDEX);
    }

    return 0;
}

#ifdef __cplusplus
}
#endif
