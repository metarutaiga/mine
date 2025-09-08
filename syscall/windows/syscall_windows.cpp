#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <vector>
#include "syscall/allocator.h"
#include "syscall/syscall.h"
#include "syscall/syscall_internal.h"
#include "syscall_windows.h"
#include "x86/x86_i386.h"
#include "x86/x86_register.inl"
#include "x86/x86_instruction.inl"
#include "msvcrt.h"
#include "windows.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SYMBOL_INDEX 1000

#define CALLBACK_ARGUMENT \
    x86_i386* cpu,          \
    x86_instruction& x86,   \
    x87_instruction& x87,   \
    uint8_t* memory,        \
    uint32_t* stack,        \
    allocator_t* allocator, \
    int(*syslog)(const char*, va_list), \
    int(*log)(const char*, va_list)

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

    // kernel32 - atomic
    { "InterlockedCompareExchange", INT32(3, syscall_InterlockedCompareExchange(memory, stack))     },
    { "InterlockedExchange",        INT32(2, syscall_InterlockedExchange(memory, stack))            },

    // kernel32 - critical section
    { "DeleteCriticalSection",      INT32(1, syscall_DeleteCriticalSection(memory, stack, allocator))       },
    { "EnterCriticalSection",       INT32(1, syscall_EnterCriticalSection(memory, stack))                   },
    { "InitializeCriticalSection",  INT32(1, syscall_InitializeCriticalSection(memory, stack, allocator))   },
    { "LeaveCriticalSection",       INT32(1, syscall_LeaveCriticalSection(memory, stack))                   },
    { "TryEnterCriticalSection",    INT32(1, syscall_TryEnterCriticalSection(memory, stack))                },

    // kernel32 - directory
    { "SetCurrentDirectoryA",       INT32(1, syscall_SetCurrentDirectoryA(memory, stack))           },

    // kernel32 - exception
    { "IsDebuggerPresent",          INT32(0, false)                                                 },
    { "SetUnhandledExceptionFilter",INT32(1, 0)                                                     },
    { "UnhandledExceptionFilter",   INT32(1, 0)                                                     },

    // kernel32 - file
    { "CloseHandle",                INT32(1, syscall_CloseHandle(memory, stack, allocator))         },
    { "CreateFileA",                INT32(7, syscall_CreateFileA(memory, stack, allocator))         },
    { "CreateFileMappingA",         INT32(6, syscall_CreateFileMappingA(memory, stack, allocator))  },
    { "GetFileSize",                INT32(2, syscall_GetFileSize(memory, stack))                    },
    { "GetFullPathNameA",           INT32(4, syscall_GetFullPathNameA(memory, stack))               },
    { "MapViewOfFile",              INT32(5, syscall_MapViewOfFile(memory, stack, allocator))       },
    { "UnmapViewOfFile",            INT32(1, syscall_UnmapViewOfFile(memory, stack, allocator))     },

    // kernel32 - find
    { "FindClose",                  INT32(1, syscall_FindClose(memory, stack, allocator))           },
    { "FindFirstFileA",             INT32(2, syscall_FindFirstFileA(memory, stack, allocator))      },
    { "FindNextFileA",              INT32(2, syscall_FindNextFileA(memory, stack))                  },

    // kernel32 - library
    { "DisableThreadLibraryCalls",  INT32(1, true)                                                  },
    { "FreeLibrary",                INT32(1, syscall_FreeLibrary(memory, stack))                    },
    { "GetModuleBaseNameA",         INT32(4, syscall_GetModuleBaseNameA(memory, stack))             },
    { "GetModuleFileNameA",         INT32(3, syscall_GetModuleFileNameA(memory, stack))             },
    { "GetModuleHandleA",           INT32(1, syscall_GetModuleHandleA(memory, stack))               },
    { "GetProcAddress",             INT32(2, syscall_GetProcAddress(memory, stack, syslog))         },
    { "LoadLibraryA",               INT32(1, syscall_LoadLibraryA(memory, stack, cpu, syslog))      },

    // kernel32 - memory
    { "LocalAlloc",                 INT32(2, syscall_LocalAlloc(memory, stack, allocator))          },
    { "LocalFree",                  INT32(1, syscall_LocalFree(memory, stack, allocator))           },
    { "VirtualAlloc",               INT32(4, syscall_VirtualAlloc(memory, stack, allocator))        },
    { "VirtualFree",                INT32(3, syscall_VirtualFree(memory, stack, allocator))         },
    { "VirtualProtect",             INT32(4, true)                                                  },
    { "WriteProcessMemory",         INT32(5, syscall_memcpy(memory, stack + 1))                     },

    // kernel32 - system
    { "ExitProcess",                INT32(1, syscall_exit(stack))                                   },
    { "GetCommandLineA",            INT32(0, syscall_GetCommandLineA(memory))                       },
    { "GetCurrentDirectoryA",       INT32(2, syscall_GetCurrentDirectoryA(memory, stack))           },
    { "GetCurrentProcess",          INT32(0, syscall_GetCurrentProcessId())                         },
    { "GetCurrentProcessId",        INT32(0, syscall_GetCurrentProcessId())                         },
    { "GetCurrentThreadId",         INT32(0, syscall_GetCurrentThreadId())                          },
    { "GetLastError",               INT32(0, 0)                                                     },
    { "GetSystemInfo",              INT32(1, 0)                                                     },
    { "OutputDebugStringA",         INT32(1, syscall_OutputDebugStringA(memory, stack, syslog))     },
    { "SetLastError",               INT32(1, 0)                                                     },
    { "TerminateProcess",           INT32(2, 0)                                                     },

    // kernel32 - time
    { "GetSystemTimeAsFileTime",    INT32(1, syscall_GetSystemTimeAsFileTime(memory, stack))        },
    { "GetTickCount",               INT32(0, syscall_GetTickCount())                                },
    { "QueryPerformanceCounter",    INT32(1, syscall_QueryPerformanceCounter(memory, stack))        },
    { "QueryPerformanceFrequency",  INT32(1, syscall_QueryPerformanceFrequency(memory, stack))      },

    // kernel32 - unimplemented
//  { "CreateProcessA",             INT32(10, false)                                                },
//  { "DeleteFileA",                INT32(1, true)                                                  },
//  { "FormatMessageA",             INT32(7, 0)                                                     },
//  { "RaiseException",             INT32(4, 0)                                                     },
//  { "WaitForSingleObject",        INT32(2, 0xFFFFFFFF)                                            },

    // advapi32
    { "RegCloseKey",                INT32(1, 0)                                                     },
    { "RegOpenKeyExA",              INT32(5, 2)                                                     },
    { "RegQueryValueExA",           INT32(6, 2)                                                     },

    // msvcrt
    { "fopen_s",                    INT32(0, syscall_fopen_s(memory, stack, allocator))             },
    { "memmove_s",                  INT32(0, syscall_memmove_s(memory, stack))                      },
    { "recalloc",                   INT32(0, syscall_recalloc(stack, allocator))                    },
    { "strcat_s",                   INT32(0, syscall_strcat_s(memory, stack))                       },
    { "_callnewh",                  INT32(0, 1)                                                     },
    { "_cexit",                     INT32(0, syscall_exit(stack))                                   },
    { "_CIatan",                    INT32(0, syscall__CIatan(cpu))                                  },
    { "_CIsqrt",                    INT32(0, syscall__CIsqrt(cpu))                                  },
    { "_controlfp",                 INT32(0, syscall__controlfp(stack, cpu))                        },
    { "_controlfp_s",               INT32(0, syscall__controlfp_s(memory, stack, cpu))              },
    { "_crt_debugger_hook",         INT32(0, 0)                                                     },
    { "_c_exit",                    INT32(0, syscall_exit(stack))                                   },
    { "_decode_pointer",            INT32(0, syscall__decode_pointer(stack))                        },
    { "_encode_pointer",            INT32(0, syscall__encode_pointer(stack))                        },
    { "_expand",                    INT32(0, syscall_expand(stack, allocator))                      },
    { "_finite",                    INT32(0, syscall_isfinite(stack))                               },
    { "_flushall",                  INT32(0, 0)                                                     },
    { "_initterm",                  INT32(0, syscall__initterm(memory, stack, cpu))                 },
    { "_initterm_e",                INT32(0, syscall__initterm(memory, stack, cpu))                 },
    { "_lock",                      INT32(0, 0)                                                     },
    { "_malloc_crt",                INT32(0, syscall_malloc(stack, allocator))                      },
    { "_msize",                     INT32(0, syscall_msize(stack, allocator))                       },
    { "_onexit",                    INT32(0, 0)                                                     },
    { "_setjmp3",                   INT32(0, syscall__setjmp3(memory, stack, cpu))                  },
    { "_splitpath",                 INT32(0, syscall_splitpath(memory, stack))                      },
    { "_strdup",                    INT32(0, syscall_strdup(memory, stack, allocator))              },
    { "_stricmp",                   INT32(0, syscall_stricmp(memory, stack))                        },
    { "_strnicmp",                  INT32(0, syscall_strnicmp(memory, stack))                       },
    { "_unlock",                    INT32(0, 0)                                                     },
    { "__acrt_iob_func",            INT32(0, syscall___acrt_iob_func(memory, stack))                },
    { "__dllonexit",                INT32(0, 0)                                                     },
    { "__getmainargs",              INT32(0, syscall___getmainargs(memory, stack, allocator))       },
    { "__iob_func",                 INT32(0, syscall___iob_func(memory))                            },
    { "__p__commode",               INT32(0, syscall___p__commode(memory))                          },
    { "__p__fmode",                 INT32(0, syscall___p__fmode(memory))                            },
    { "__p___initenv",              INT32(0, syscall___p___initenv(memory))                         },
    { "__set_app_type",             INT32(0, 0)                                                     },
    { "??2@YAPAXI@Z",               INT32(0, syscall_malloc(stack, allocator))                      },
    { "??3@YAXPAX@Z",               INT32(0, syscall_free(stack, allocator))                        },
    { "??_U@YAPAXI@Z",              INT32(0, syscall_malloc(stack, allocator))                      },
    { "??_V@YAXPAX@Z",              INT32(0, syscall_free(stack, allocator))                        },

    // msvcrt - duplicated
//  { "_exit",                      INT32(0, syscall_exit(stack))                                   },
//  { "_isnan",                     INT32(0, syscall_isnan(stack))                                  },
//  { "_snprintf",                  INT32(0, syscall_snprintf(memory, stack))                       },
//  { "_vsnprintf",                 INT32(0, syscall_vsnprintf(memory, stack))                      },

    // msvcrt - unimplemented
//  { "_amsg_exit",                 INT32(0, 0)                                                     },
//  { "_CIfmod",                    INT32(0, 0)                                                     },
//  { "_CIpow",                     INT32(0, 0)                                                     },
//  { "_CxxThrowException",         INT32(0, 0)                                                     },
//  { "_except_handler3",           INT32(0, 0)                                                     },
//  { "_fileno",                    INT32(0, 0)                                                     },
//  { "_fpclass",                   INT32(0, 0)                                                     },
//  { "_get_osfhandle",             INT32(0, 0)                                                     },
//  { "_purecall",                  INT32(0, 0)                                                     },
//  { "_XcptFilter",                INT32(0, 0)                                                     },
//  { "__CppXcptFilter",            INT32(0, 0)                                                     },
//  { "__CxxFrameHandler",          INT32(0, 0)                                                     },
//  { "__security_error_handler",   INT32(0, 0)                                                     },
//  { "__setusermatherr",           INT32(0, 0)                                                     },
//  { "?terminate@@YAXXZ",          INT32(0, 0)                                                     },
//  { "?_Nomemory@std@@YAXXZ",      INT32(0, 0)                                                     },
//  { "??0exception@@QAE@XZ",       INT32(0, 0)                                                     },
//  { "??1exception@@UAE@XZ",       INT32(0, 0)                                                     },
//  { "??0exception@@QAE@ABV0@@Z",  INT32(0, 0)                                                     },
//  { "??1type_info@@UAE@XZ",       INT32(0, 0)                                                     },

    // ucrt
    { "__stdio_common_vfprintf",    INT32(0, syscall___stdio_common_vfprintf(memory, stack, log))   },
    { "__stdio_common_vsprintf",    INT32(0, syscall___stdio_common_vsprintf(memory, stack))        },

    // std::string
    { "??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@XZ",             INT32(0, syscall_basic_string_char_constructor(ECX, memory))                                },
    { "??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@ABV01@@Z",       INT32(1, syscall_basic_string_char_copy_constructor(ECX, memory, stack, allocator))         },
    { "??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@ABV01@II@Z",     INT32(3, syscall_basic_string_char_copy_range_constructor(ECX, memory, stack, allocator))   },
    { "??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@PBD@Z",          INT32(1, syscall_basic_string_char_cstr_constructor(ECX, memory, stack, allocator))         },
    { "??1?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@XZ",             INT32(0, syscall_basic_string_char_deconstructor(ECX, memory, allocator))                   },
    { "??4?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV01@ABV01@@Z",  INT32(1, syscall_basic_string_char_assign(ECX, memory, stack, allocator))                   },
    { "??4?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV01@PBD@Z",     INT32(1, syscall_basic_string_char_assign_cstr(ECX, memory, stack, allocator))              },
    { "??A?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAADI@Z",          INT32(1, syscall_basic_string_char_at(ECX, memory, stack))                                  },
    { "??Y?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV01@ABV01@@Z",  INT32(1, syscall_basic_string_char_append(ECX, memory, stack, allocator))                   },
    { "??Y?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV01@PBD@Z",     INT32(1, syscall_basic_string_char_append_cstr(ECX, memory, stack, allocator))              },
    { "?substr@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBE?AV12@II@Z", INT32(3, syscall_basic_string_char_substr(ECX, memory, stack, allocator))                   },

    // std::string comparsion
    { "??$?8DU?$char_traits@D@std@@V?$allocator@D@1@@std@@YA_NABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@0@PBD@Z",  INT32(0, syscall_basic_string_char_eq_cstr(memory, stack))  },
    { "??$?9DU?$char_traits@D@std@@V?$allocator@D@1@@std@@YA_NABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@0@PBD@Z",  INT32(0, syscall_basic_string_char_neq_cstr(memory, stack)) },
};

size_t syscall_windows_new(void* data, const char* path, void* image, int argc, const char* argv[], int envc, const char* envp[])
{
    if (data == nullptr)
        return 0;

    auto* cpu = (x86_i386*)data;
    auto* memory = cpu->Memory();
    auto* allocator = cpu->Allocator;
    auto* msvcrt = physical(MSVCRT*, TIB_MSVCRT);
    msvcrt->iob[0][0] = 1;
    msvcrt->iob[1][0] = 2;
    msvcrt->iob[2][0] = 3;

    auto args = (uint32_t*)allocator->allocate(sizeof(uint32_t) * argc);
    for (int i = 0; i < argc; ++i) {
        size_t length = strlen(argv[i]);
        auto arg = allocator->allocate(length + 1);
        memcpy(arg, argv[i], length);
        args[i] = virtual(int, arg);
    }

    auto envs = (uint32_t*)allocator->allocate(sizeof(uint32_t) * (envc + 1));
    for (int i = 0; i < envc; ++i) {
        size_t length = strlen(envp[i]);
        auto env = allocator->allocate(length + 1);
        memcpy(env, envp[i], length);
        envs[i] = virtual(int, env);
    }
    envs[envc] = 0;

    msvcrt->argc = argc;
    msvcrt->argv = virtual(int, args);
    msvcrt->envp = virtual(int, envs);

    auto* windows = physical(Windows*, TIB_WINDOWS);
    if (windows->image == 0) {
        windows->image = virtual(uint32_t, image);
        new (windows) Windows;
    }
    for (int i = 0; i < argc; ++i) {
        if (i) strncat(windows->commandLine, " ", 256);
        strncat(windows->commandLine, argv[i], 256);
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

size_t syscall_windows_execute(void* data, size_t index, int(*syslog)(const char*, va_list), int(*log)(const char*, va_list))
{
    index = uint32_t(-index - SYMBOL_INDEX);

    size_t count = sizeof(syscall_table) / sizeof(syscall_table[0]);
    if (index < count) {
        auto* cpu = (x86_i386*)data;
        auto& x86 = cpu->x86;
        auto& x87 = cpu->x87;
        auto* memory = cpu->Memory();
        auto* stack = (uint32_t*)(memory + cpu->Stack());
        auto* allocator = cpu->Allocator;
        auto* syscall = syscall_table[index].syscall;
        if (syslog) {
            syslog("[CALL] %s", (va_list)&syscall_table[index].name);
        }
        return syscall(cpu, x86, x87, memory, stack, allocator, syslog, log) * sizeof(uint32_t);
    }

    return 0;
}

size_t syscall_windows_symbol(const char* file, const char* name)
{
    if (file == nullptr)
        return 0;

    size_t count = sizeof(syscall_table) / sizeof(syscall_table[0]);
    for (size_t index = 0; index < count; ++index) {
        if (strcmp(syscall_table[index].name, name) == 0)
            return (uint32_t)(-index - SYMBOL_INDEX);
    }

    if (strcmp(name, "_iob") == 0)
        return TIB_MSVCRT + offsetof(MSVCRT, iob);
    if (strcmp(name, "_adjust_fdiv") == 0)
        return TIB_MSVCRT + offsetof(MSVCRT, adjust_fdiv);

    return 0;
}

const char* syscall_windows_name(size_t index)
{
    index = uint32_t(-index - SYMBOL_INDEX);

    size_t count = sizeof(syscall_table) / sizeof(syscall_table[0]);
    if (index < count) {
        return syscall_table[index].name;
    }

    if (index == TIB_MSVCRT + offsetof(MSVCRT, iob))
        return "_iob";
    if (index == TIB_MSVCRT + offsetof(MSVCRT, adjust_fdiv))
        return "_adjust_fdiv";

    return nullptr;
}

#ifdef __cplusplus
}
#endif
