#include <string>
#include <vector>
#include "allocator.h"
#include "syscall.h"
#include "syscall_internal.h"
#include "syscall_windows.h"
#include "format/coff/pe.h"
#include "x86/x86_i386.h"
#include "x86/x86_register.inl"
#include "x86/x86_instruction.inl"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#else
#include <fnmatch.h>
#include <sys/dir.h>
#pragma pack(push, 1)
struct WIN32_FIND_DATAA {
    uint32_t    dwFileAttributes;
    uint64_t    ftCreationTime;
    uint64_t    ftLastAccessTime;
    uint64_t    ftLastWriteTime;
    uint32_t    nFileSizeHigh;
    uint32_t    nFileSizeLow;
    uint32_t    dwReserved0;
    uint32_t    dwReserved1;
    char        cFileName[260];
    char        cAlternateFileName[14];
};
#pragma pack(pop)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define SYMBOL_INDEX    1001
#define TIB_MSVCRT      0x600
#define TIB_WINDOWS     0x700

struct MSVCRT {
    uint32_t iob[3][8];
    uint32_t adjust_fdiv;
    uint32_t commode;
    uint32_t fmode;
    uint32_t initenv;
};

struct Windows {
    uint32_t image;
    std::vector<std::pair<std::string, void*>> modules;
    void (*loadLibraryCallback)(void*);
    char currentDirectory[260];
};

int syscall_FindClose(uint8_t* memory, uint32_t* stack, allocator_t* allocator)
{
#if defined(_WIN32)
    auto handle = physical(HANDLE*, stack[1]);
    auto result = FindClose(*handle);
    allocator->deallocate(handle);
    return result;
#else
    auto dir = physical(DIR**, stack[1]);
    auto result = closedir(*dir);
    allocator->deallocate(dir);
    return result ? false : true;
#endif
}

int syscall_FindNextFileA(uint8_t* memory, uint32_t* stack)
{
#if defined(_WIN32)
    auto handle = physical(HANDLE*, stack[1]);
    auto findFileData = physical(WIN32_FIND_DATAA*, stack[2]);
    return FindNextFileA(*handle, findFileData);
#else
    auto dir = physical(DIR**, stack[1]);
    auto findFileData = physical(WIN32_FIND_DATAA*, stack[2]);
    auto pattern = (char*)(dir + 1);

    dirent* dirent = nullptr;
    while ((dirent = readdir(*dir)) != nullptr) {
        if (pattern[0] && fnmatch(pattern, dirent->d_name, FNM_CASEFOLD) == 0)
            break;
    }
    if (dirent == nullptr)
        return false;

    switch (dirent->d_type) {
    case DT_DIR:    findFileData->dwFileAttributes = 0x010; break;
    case DT_REG:    findFileData->dwFileAttributes = 0x080; break;
    case DT_LNK:    findFileData->dwFileAttributes = 0x400; break;
    }
    findFileData->ftCreationTime = 0;
    findFileData->ftLastAccessTime = 0;
    findFileData->ftLastWriteTime = 0;
    findFileData->nFileSizeHigh = 0;
    findFileData->nFileSizeLow = 0;
    strncpy(findFileData->cFileName, dirent->d_name, 260);
    strncpy(findFileData->cAlternateFileName, dirent->d_name, 14);

    return true;
#endif
}

int syscall_FindFirstFileA(uint8_t* memory, uint32_t* stack, allocator_t* allocator)
{
#if defined(_WIN32)
    auto handle = (HANDLE*)allocator->allocate(sizeof(HANDLE));
    if (handle == nullptr)
        return 0xFFFFFFFF;
    auto name = physical(char*, stack[1]);
    auto findFileData = physical(WIN32_FIND_DATAA*, stack[2]);
    (*handle) = FindFirstFileA(name, findFileData);
    if ((*handle) == INVALID_HANDLE_VALUE) {
        allocator->deallocate(handle);
        return 0xFFFFFFFF;
    }
    return virtual(int, handle);
#else
    auto dir = (DIR**)allocator->allocate(sizeof(DIR*) + sizeof(char) * 120);
    if (dir == nullptr)
        return 0xFFFFFFFF;
    auto* windows = physical(Windows*, TIB_WINDOWS);
    auto name = physical(char*, stack[1]);

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
    auto slash = path.find_last_of("/\\");

    (*dir) = opendir(path.substr(0, slash).c_str());
    if ((*dir) == nullptr) {
        allocator->deallocate(dir);
        return 0xFFFFFFFF;
    }

    auto pattern = (char*)(dir + 1);
    strncpy(pattern, path.substr(slash + 1).c_str(), 120);

    stack[1] = virtual(int, dir);
    if (syscall_FindNextFileA(memory, stack) == false) {
        allocator->deallocate(dir);
        return 0xFFFFFFFF;
    }

    return virtual(int, dir);
#endif
}

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
    auto* windows = physical(Windows*, TIB_WINDOWS);
    auto& modules = windows->modules;
    auto name = physical(char*, stack[1]);

    if (name == nullptr)
        return windows->image;

    std::string path = name;
    auto slash = path.find_last_of("/\\");

    path = path.substr(slash + 1);
    for (auto& [name, image] : modules) {
        if (strcasecmp(name.c_str(), path.c_str()) == 0)
            return virtual(int, image);
    }

    return 0;
}

int syscall_LoadLibraryA(uint8_t* memory, uint32_t* stack, x86_i386* cpu, int(*log)(const char*, ...))
{
    int module = syscall_GetModuleHandleA(memory, stack);
    if (module)
        return module;

    auto* windows = physical(Windows*, TIB_WINDOWS);
    auto& modules = windows->modules;
    auto name = physical(char*, stack[1]);

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
    auto slash = path.find_last_of("/\\");

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
    modules.emplace_back(path.substr(slash + 1).c_str(), image);
    if (windows->loadLibraryCallback) {
        windows->loadLibraryCallback(image);
    }

    // _DllMainCRTStartup
    size_t entry = PE::Entry(image);
    if (entry) {
        auto& x86 = cpu->x86;

        //                  _DllMainCRTStartup
        //                  _DllMainCRTStartup
        //                  _DllMainCRTStartup
        //                  hDllHandle
        //                  dwReason
        //                  lpreserved
        //                  $$$$$$$$
        //                  hDllHandle
        //                  dwReason
        //                  lpreserved
        // LoadLibraryA     LoadLibraryA
        // lpLibFileName    lpLibFileName

        // 58 POP EAX
        // 58 POP EAX
        // C3 RET
        // C3 RET
        uint32_t ret = Pop32();
        Pop32();
        Push32(ret);
        Push32(virtual(int, image));
        Push32(0xC3C35858);
        uint32_t eax = ESP;
        Push32(0);
        Push32(2);
        Push32(0);
        Push32(eax);
        Push32(0);
        Push32(1);
        Push32(0);
        Push32(entry);
        Push32(entry);
        Push32(entry);
    }

    return virtual(int, image);
}

int syscall_FreeLibrary(uint8_t* memory, uint32_t* stack)
{
    return 0;
}

int syscall_SetCurrentDirectoryA(uint8_t* memory, uint32_t* stack)
{
    auto* windows = physical(Windows*, TIB_WINDOWS);
    char* pathName = physical(char*, stack[1]);
    strncpy(windows->currentDirectory, pathName, 260);
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

int syscall_expand(const uint32_t* stack, struct allocator_t* allocator)
{
    auto pointer = stack[1];
    auto new_size = stack[2];
    auto memory = (char*)allocator->address();
    auto old_pointer = physical(char*, pointer);
    auto old_size = allocator->size(old_pointer);
    if (old_size < new_size)
        return 0;
    return pointer;
}

int syscall_msize(const uint32_t* stack, struct allocator_t* allocator)
{
    auto pointer = stack[1];
    auto memory = (char*)allocator->address();
    return (uint32_t)allocator->size(physical(char*, pointer));
}

int syscall_recalloc(const uint32_t* stack, struct allocator_t* allocator)
{
    auto pointer = stack[1];
    auto new_size = stack[2] * stack[3];
    auto memory = (char*)allocator->address();
    auto old_pointer = physical(char*, pointer);
    auto old_size = allocator->size(old_pointer);
    if (old_size == new_size)
        return pointer;
    auto new_pointer = (uint8_t*)allocator->allocate(new_size);
    if (new_pointer == nullptr)
        return 0;
    if (pointer) {
        memcpy(new_pointer, old_pointer, new_size < old_size ? new_size : old_size);
        allocator->deallocate(old_pointer);
    }
    if (new_size > old_size) {
        memset(memory + old_size, 0, new_size - old_size);
    }
    return virtual(int, new_pointer);
}

int syscall_stricmp(uint8_t* memory, const uint32_t* stack)
{
    auto str1 = physical(char*, stack[1]);
    auto str2 = physical(char*, stack[2]);
    return strcasecmp(str1, str2);
}

int syscall_strnicmp(uint8_t* memory, const uint32_t* stack)
{
    auto str1 = physical(char*, stack[1]);
    auto str2 = physical(char*, stack[2]);
    auto num = stack[3];
    return strncasecmp(str1, str2, num);
}

int syscall__initterm(uint8_t* memory, const uint32_t* stack, x86_i386* cpu)
{
    auto begin = physical(uint32_t*, stack[1]);
    auto end = physical(uint32_t*, stack[2]);

    auto& x86 = cpu->x86;
    for (auto function = end - 1; function >= begin; --function) {
        if (*function) {
            Push32(*function);
        }
    }

    return 0;
}

int syscall___getmainargs(uint8_t* memory, const uint32_t* stack, struct allocator_t* allocator)
{
    auto argc = physical(int*, stack[1]);
    auto argv = physical(int*, stack[2]);
    auto envp = physical(int*, stack[3]);
//  auto expand = physical(int, stack[4]);
//  auto mode = physical(int*, stack[5]);

    auto args = (uint32_t*)allocator->allocate(sizeof(uint32_t) * 2);
    auto main = (char*)allocator->allocate(sizeof("main"));
    strcpy(main, "main");
    args[0] = virtual(int, main);
    args[1] = 0;

    auto environ = (uint32_t*)allocator->allocate(sizeof(uint32_t) * 2);
    auto path = (char*)allocator->allocate(sizeof("PATH=."));
    strcpy(path, "PATH=.");
    environ[0] = virtual(int, path);
    environ[1] = 0;

    *argc = 1;
    *argv = virtual(int, args);
    *envp = virtual(int, environ);

    return 0;
}

int syscall___p__commode(uint8_t* memory)
{
    auto* msvcrt = physical(MSVCRT*, TIB_MSVCRT);
    return virtual(int, &msvcrt->commode);
}

int syscall___p__fmode(uint8_t* memory)
{
    auto* msvcrt = physical(MSVCRT*, TIB_MSVCRT);
    return virtual(int, &msvcrt->fmode);
}

int syscall___p___initenv(uint8_t* memory)
{
    auto* msvcrt = physical(MSVCRT*, TIB_MSVCRT);
    return virtual(int, &msvcrt->initenv);
}

struct syscall_basic_string_char {
    uint32_t allocator;
    union {
        char buf[16];
        uint32_t ptr;
    };
    uint32_t size;
    uint32_t res;
};

int syscall_basic_string_char_copy_constructor(uint32_t thiz, uint8_t* memory, const uint32_t* stack, allocator_t* allocator) {
    auto& local = *physical(syscall_basic_string_char*, thiz);
    auto& other = *physical(syscall_basic_string_char*, stack[1]);
    local.allocator = other.allocator;
    local.size = other.size;
    local.res = other.res;
    if (local.res >= 16) {
        auto ptr = allocator->allocate(local.res + 1);
        memcpy(ptr, physical(char*, other.ptr), local.res + 1);
        local.ptr = virtual(int, ptr);
    }
    else {
        memcpy(local.buf, other.buf, 16);
    }
    return thiz;
}

int syscall_basic_string_char_cstr_constructor(uint32_t thiz, uint8_t* memory, const uint32_t* stack, allocator_t* allocator) {
    auto& local = *physical(syscall_basic_string_char*, thiz);
    auto* other = physical(char*, stack[1]);
    local.size = local.res = (uint32_t)strlen(other);
    if (local.size >= 16) {
        auto ptr = allocator->allocate(local.size + 1);
        memcpy(ptr, other, local.size + 1);
        local.ptr = virtual(int, ptr);
    }
    else {
        memcpy(local.buf, other, 16);
    }
    return thiz;
}

int syscall_basic_string_char_deconstructor(uint32_t thiz, uint8_t* memory, allocator_t* allocator) {
    auto& local = *physical(syscall_basic_string_char*, thiz);
    if (local.size >= 16) {
        allocator->deallocate(physical(char*, local.ptr));
    }
    return 0;
}

#define CALLBACK_ARGUMENT \
    x86_i386* cpu,          \
    x86_instruction& x86,   \
    x87_instruction& x87,   \
    uint8_t* memory,        \
    uint32_t* stack,        \
    allocator_t* allocator, \
    int(*syslog)(const char*, ...), \
    int(*log)(const char*, ...)

#define INT32(count, value) \
    [](CALLBACK_ARGUMENT) -> size_t {   \
        auto temp = value;              \
        x86.regs[0].d = uint32_t(temp); \
        return count; \
    }

static const struct {
    const char* name;
    size_t (*syscall)(CALLBACK_ARGUMENT);
} syscall_table[] = {

    // kernel32
    { "CloseHandle",                INT32(1, 0)                                                 },
    { "CreateFileA",                INT32(7, 0)                                                 },
    { "CreateFileMappingA",         INT32(6, 0)                                                 },
    { "CreateProcessA",             INT32(10, false)                                            },
    { "DeleteFileA",                INT32(1, true)                                              },
    { "FormatMessageA",             INT32(7, 0)                                                 },
    { "FreeLibrary",                INT32(1, syscall_FreeLibrary(memory, stack))                },
    { "GetProcAddress",             INT32(2, syscall_GetProcAddress(memory, stack))             },
    { "GetModuleHandleA",           INT32(1, syscall_GetModuleHandleA(memory, stack))           },
    { "ExitProcess",                INT32(1, syscall_exit(stack))                               },
    { "FindClose",                  INT32(1, syscall_FindClose(memory, stack, allocator))       },
    { "FindFirstFileA",             INT32(2, syscall_FindFirstFileA(memory, stack, allocator))  },
    { "FindNextFileA",              INT32(2, syscall_FindNextFileA(memory, stack))              },
    { "GetCurrentProcessId",        INT32(0, 0)                                                 },
    { "GetCurrentThreadId",         INT32(0, 0)                                                 },
    { "GetFileSize",                INT32(2, 0)                                                 },
    { "GetLastError",               INT32(0, 0)                                                 },
    { "GetSystemTimeAsFileTime",    INT32(1, 0)                                                 },
    { "GetTickCount",               INT32(0, 0)                                                 },
    { "InterlockedExchange",        INT32(2, 0)                                                 },
    { "LoadLibraryA",               INT32(1, syscall_LoadLibraryA(memory, stack, cpu, syslog))  },
    { "LocalAlloc",                 INT32(2, syscall_malloc(stack + 1, allocator))              },
    { "MapViewOfFile",              INT32(5, 0)                                                 },
    { "QueryPerformanceCounter",    INT32(0, 0)                                                 },
    { "RaiseException",             INT32(4, 0)                                                 },
    { "SetCurrentDirectoryA",       INT32(1, syscall_SetCurrentDirectoryA(memory, stack))       },
    { "UnmapViewOfFile",            INT32(1, true)                                              },
    { "WaitForSingleObject",        INT32(2, 0xFFFFFFFF)                                        },

    // advapi32
    { "RegCloseKey",                INT32(1, 0)                                                 },
    { "RegOpenKeyExA",              INT32(5, 2)                                                 },
    { "RegQueryValueExA",           INT32(6, 2)                                                 },

    // msvcrt
    { "recalloc",                   INT32(0, syscall_recalloc(stack, allocator))                },
    { "_expand",                    INT32(0, syscall_expand(stack, allocator))                  },
    { "_exit",                      INT32(0, syscall_exit(stack))                               },
    { "_amsg_exit",                 INT32(0, 0)                                                 },
    { "_callnewh",                  INT32(0, 1)                                                 },
    { "_cexit",                     INT32(0, syscall_exit(stack))                               },
    { "_controlfp",                 INT32(0, 0)                                                 },
    { "_CxxThrowException",         INT32(0, 0)                                                 },
    { "_c_exit",                    INT32(0, syscall_exit(stack))                               },
    { "_except_handler3",           INT32(0, 0)                                                 },
    { "_finite",                    INT32(0, syscall_isfinite(stack))                           },
    { "_flushall",                  INT32(0, 0)                                                 },
    { "_initterm",                  INT32(0, syscall__initterm(memory, stack, cpu))             },
    { "_isnan",                     INT32(0, syscall_isnan(stack))                              },
    { "_msize",                     INT32(0, syscall_msize(stack, allocator))                   },
    { "_onexit",                    INT32(0, 0)                                                 },
    { "_purecall",                  INT32(0, 0)                                                 },
    { "_snprintf",                  INT32(0, syscall_snprintf(memory, stack))                   },
    { "_splitpath",                 INT32(0, 0)                                                 },
    { "_stricmp",                   INT32(0, syscall_stricmp(memory, stack))                    },
    { "_strnicmp",                  INT32(0, syscall_strnicmp(memory, stack))                   },
    { "_XcptFilter",                INT32(0, 0)                                                 },
    { "__CppXcptFilter",            INT32(0, 0)                                                 },
    { "__CxxFrameHandler",          INT32(0, 0)                                                 },
    { "__dllonexit",                INT32(0, 0)                                                 },
    { "__getmainargs",              INT32(0, syscall___getmainargs(memory, stack, allocator))   },
    { "__p__commode",               INT32(0, syscall___p__commode(memory))                      },
    { "__p__fmode",                 INT32(0, syscall___p__fmode(memory))                        },
    { "__p___initenv",              INT32(0, syscall___p___initenv(memory))                     },
    { "__security_error_handler",   INT32(0, 0)                                                 },
    { "__setusermatherr",           INT32(0, 0)                                                 },
    { "__set_app_type",             INT32(0, 0)                                                 },
    { "?terminate@@YAXXZ",          INT32(0, 0)                                                 },
    { "?_Nomemory@std@@YAXXZ",      INT32(0, 0)                                                 },
    { "??2@YAPAXI@Z",               INT32(0, syscall_malloc(stack, allocator))                  },
    { "??3@YAXPAX@Z",               INT32(0, syscall_free(stack, allocator))                    },
    { "??0exception@@QAE@XZ",       INT32(0, 0)                                                 },
    { "??1exception@@UAE@XZ",       INT32(0, 0)                                                 },
    { "??0exception@@QAE@ABV0@@Z",  INT32(0, 0)                                                 },
    { "??1type_info@@UAE@XZ",       INT32(0, 0)                                                 },
    { "??_U@YAPAXI@Z",              INT32(0, syscall_malloc(stack, allocator))                  },
    { "??_V@YAXPAX@Z",              INT32(0, syscall_free(stack, allocator))                    },

    // std::string
    { "??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@ABV01@@Z",   INT32(0, syscall_basic_string_char_copy_constructor(ECX, memory, stack, allocator)) },
    { "??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@PBD@Z",      INT32(0, syscall_basic_string_char_cstr_constructor(ECX, memory, stack, allocator)) },
    { "??1?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@XZ",         INT32(0, syscall_basic_string_char_deconstructor(ECX, memory, allocator))           },
};

size_t syscall_windows_new(void* data, const char* path, void* image)
{
    if (data == nullptr)
        return 0;

    auto* cpu = (x86_i386*)data;
    auto* memory = cpu->Memory();
    auto* msvcrt = physical(MSVCRT*, TIB_MSVCRT);
    msvcrt->iob[0][0] = 1;
    msvcrt->iob[1][0] = 2;
    msvcrt->iob[2][0] = 3;

    auto* windows = physical(Windows*, TIB_WINDOWS);
    if (windows->image == 0) {
        windows->image = virtual(uint32_t, image);
        new (windows) Windows;
    }
#if defined(_WIN32)
    strncpy(windows->currentDirectory, path, 260);
#else
    realpath(path, windows->currentDirectory);
#endif
    return 0;
}

size_t syscall_windows_debug(void* data, void(*loadLibraryCallback)(void*))
{
    if (data == nullptr)
        return 0;

    auto* cpu = (x86_i386*)data;
    auto* memory = cpu->Memory();
    auto* windows = physical(Windows*, TIB_WINDOWS);
    windows->loadLibraryCallback = loadLibraryCallback;

    return 0;
}

size_t syscall_windows_delete(void* data)
{
    if (data == nullptr)
        return 0;

    auto* cpu = (x86_i386*)data;
    auto* memory = cpu->Memory();
    auto* windows = physical(Windows*, TIB_WINDOWS);
    if (windows->image) {
        windows->image = 0;
        windows->~Windows();
    }
    return 0;
}

size_t syscall_windows_execute(void* data, size_t index, int(*syslog)(const char*, ...), int(*log)(const char*, ...))
{
    index = uint32_t(-index - SYMBOL_INDEX);

    size_t count = sizeof(syscall_table) / sizeof(syscall_table[0]);
    if (index < count) {
        syslog("%s\n", syscall_table[index].name);

        auto* cpu = (x86_i386*)data;
        auto& x86 = cpu->x86;
        auto& x87 = cpu->x87;
        auto* memory = cpu->Memory();
        auto* stack = (uint32_t*)(memory + cpu->Stack());
        auto* allocator = cpu->Allocator();
        auto* syscall = syscall_table[index].syscall;
        return syscall(cpu, x86, x87, memory, stack, allocator, syslog, log) * sizeof(uint32_t);
    }

    return 0;
}

size_t syscall_windows_symbol(const char* file, const char* name)
{
    if (file == nullptr)
        return 0;

    if (strcmp(name, "_iob") == 0)
        return TIB_MSVCRT + offsetof(MSVCRT, iob);
    if (strcmp(name, "_adjust_fdiv") == 0)
        return TIB_MSVCRT + offsetof(MSVCRT, adjust_fdiv);

    size_t count = sizeof(syscall_table) / sizeof(syscall_table[0]);
    for (size_t index = 0; index < count; ++index) {
        if (strcmp(syscall_table[index].name, name) == 0)
            return (uint32_t)(-index - SYMBOL_INDEX);
    }

    return 0;
}

const char* syscall_windows_name(size_t index)
{
    if (index == TIB_MSVCRT + offsetof(MSVCRT, iob))
        return "_iob";
    if (index == TIB_MSVCRT + offsetof(MSVCRT, adjust_fdiv))
        return "_adjust_fdiv";

    index = uint32_t(-index - SYMBOL_INDEX);

    size_t count = sizeof(syscall_table) / sizeof(syscall_table[0]);
    if (index < count) {
        return syscall_table[index].name;
    }

    return nullptr;
}

#ifdef __cplusplus
}
#endif
