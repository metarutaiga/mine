#define _CRT_SECURE_NO_WARNINGS
#include <sys/stat.h>
#include <string>
#include <vector>
#include "../allocator.h"
#include "../syscall.h"
#include "../syscall_internal.h"
#include "../../format/coff/pe.h"
#include "../../x86/x86_i386.h"
#include "../../x86/x86_register.inl"
#include "../../x86/x86_instruction.inl"
#include "syscall_windows.h"
#include "msvcrt.h"
#include "windows.h"

#if defined(_WIN32)
#define strcasecmp _stricmp
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define SYMBOL_INDEX 1000

#define CALLBACK_ARGUMENT \
    x86_i386* cpu,          \
    uint8_t* memory,        \
    uint32_t* stack

#define INT32(count, value) \
    [](CALLBACK_ARGUMENT) -> size_t { x86_instruction& x86 = cpu->x86; auto* allocator = cpu->Allocator; (void)allocator; \
        auto temp = value; \
        x86.regs[0].d = uint32_t(temp); \
        return count; \
    }
#define INT64(count, value) \
    [](CALLBACK_ARGUMENT) -> size_t { x86_instruction& x86 = cpu->x86; auto* allocator = cpu->Allocator; (void)allocator; \
        auto temp = value; \
        x86.regs[0].d = uint32_t(temp); \
        x86.regs[2].d = uint32_t(temp >> 32); \
        return count; \
    }

static const struct {
    const char* name;
    size_t (*syscall)(CALLBACK_ARGUMENT);
} syscall_table[] = {

    // kernel32 - atomic
    { "InterlockedCompareExchange", INT32(3, syscall_InterlockedCompareExchange(memory, stack))     },
    { "InterlockedDecrement",       INT32(1, syscall_InterlockedDecrement(memory, stack))           },
    { "InterlockedExchange",        INT32(2, syscall_InterlockedExchange(memory, stack))            },
    { "InterlockedIncrement",       INT32(1, syscall_InterlockedIncrement(memory, stack))           },

    // kernel32 - condition variable
    { "InitializeConditionVariable",    INT32(1, 0)                                                 },
    { "SleepConditionVariableCS",       INT32(3, true)                                              },
    { "SleepConditionVariableSRW",      INT32(4, true)                                              },
    { "WakeAllConditionVariable",       INT32(1, 0)                                                 },
    { "WakeConditionVariable",          INT32(1, 0)                                                 },

    // kernel32 - critical section
    { "DeleteCriticalSection",                  INT32(1, syscall_DeleteCriticalSection(memory, stack, allocator))       },
    { "EnterCriticalSection",                   INT32(1, syscall_EnterCriticalSection(memory, stack))                   },
    { "InitializeCriticalSection",              INT32(1, syscall_InitializeCriticalSection(memory, stack, allocator))   },
    { "InitializeCriticalSectionAndSpinCount",  INT32(2, syscall_InitializeCriticalSection(memory, stack, allocator))   },
    { "LeaveCriticalSection",                   INT32(1, syscall_LeaveCriticalSection(memory, stack))                   },
    { "TryEnterCriticalSection",                INT32(1, syscall_TryEnterCriticalSection(memory, stack))                },

    // kernel32 - directory
    { "SetCurrentDirectoryA",       INT32(1, syscall_SetCurrentDirectoryA(memory, stack))           },

    // kernel32 - environment
    { "FreeEnvironmentStringsA",    INT32(1, syscall_FreeEnvironmentStringsA(memory, stack, allocator)) },
    { "FreeEnvironmentStringsW",    INT32(1, syscall_FreeEnvironmentStringsW(memory, stack, allocator)) },
    { "GetEnvironmentStrings",      INT32(0, syscall_GetEnvironmentStrings(memory, allocator))          },
    { "GetEnvironmentStringsW",     INT32(0, syscall_GetEnvironmentStringsW(memory, allocator))         },
    { "GetEnvironmentVariableA",    INT32(3, syscall_GetEnvironmentVariableA(memory, stack, allocator)) },
    { "GetEnvironmentVariableW",    INT32(3, syscall_GetEnvironmentVariableW(memory, stack, allocator)) },

    // kernel32 - exception
    { "IsDebuggerPresent",          INT32(0, false)                                                 },
    { "RtlUnwind",                  INT32(4, 0)                                                     },
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

    // kernel32 - heap
    { "GetProcessHeap",             INT32(0, 0xFFFFFFFF)                                            },
    { "HeapCreate",                 INT32(3, 0xFFFFFFFF)                                            },
    { "HeapDestroy",                INT32(1, true)                                                  },
    { "HeapAlloc",                  INT32(3, syscall_HeapAlloc(memory, stack, allocator))           },
    { "HeapFree",                   INT32(3, syscall_HeapFree(memory, stack, allocator))            },
    { "HeapSize",                   INT32(3, 0xFFFFFFFF)                                            },

    // kernel32 - library
    { "DisableThreadLibraryCalls",  INT32(1, true)                                                  },
    { "FreeLibrary",                INT32(1, syscall_FreeLibrary(memory, stack))                    },
    { "GetModuleBaseNameA",         INT32(4, syscall_GetModuleBaseNameA(memory, stack))             },
    { "GetModuleFileNameA",         INT32(3, syscall_GetModuleFileNameA(memory, stack))             },
    { "GetModuleHandleA",           INT32(1, syscall_GetModuleHandleA(memory, stack))               },
    { "GetModuleHandleW",           INT32(1, syscall_GetModuleHandleW(memory, stack))               },
    { "GetProcAddress",             INT32(2, syscall_GetProcAddress(memory, stack))                 },
    { "LoadLibraryA",               INT32(1, syscall_LoadLibraryA(memory, stack, cpu))              },

    // kernel32 - memory
    { "DecodePointer",              INT32(1, stack[1])                                              },
    { "EncodePointer",              INT32(1, stack[1])                                              },
    { "LocalAlloc",                 INT32(2, syscall_LocalAlloc(memory, stack, allocator))          },
    { "LocalFree",                  INT32(1, syscall_LocalFree(memory, stack, allocator))           },
    { "VirtualAlloc",               INT32(4, syscall_VirtualAlloc(memory, stack, allocator))        },
    { "VirtualFree",                INT32(3, syscall_VirtualFree(memory, stack, allocator))         },
    { "VirtualProtect",             INT32(4, true)                                                  },
    { "WriteProcessMemory",         INT32(5, syscall_memcpy(memory, stack + 1))                     },

    // kernel32 - profile
    { "GetProfileIntA",             INT32(3, 0)                                                     },

    // kernel32 - string
    { "GetACP",                     INT32(0, 437)                                                   },
    { "GetCPInfo",                  INT32(2, true)                                                  },
    { "GetLocaleInfoA",             INT32(4, syscall_GetLocalInfoA(memory, stack))                  },
    { "GetLocaleInfoW",             INT32(4, syscall_GetLocalInfoW(memory, stack))                  },
    { "GetStringTypeA",             INT32(4, syscall_GetStringTypeA(memory, stack))                 },
    { "GetStringTypeW",             INT32(4, syscall_GetStringTypeW(memory, stack))                 },
    { "IsValidCodePage",            INT32(1, true)                                                  },
    { "LCMapStringA",               INT32(6, syscall_LCMapStringA(memory, stack))                   },
    { "LCMapStringW",               INT32(6, syscall_LCMapStringW(memory, stack))                   },
    { "LCMapStringEx",              INT32(9, syscall_LCMapStringEx(memory, stack))                  },
    { "LCMapStringExW",             INT32(9, syscall_LCMapStringExW(memory, stack))                 },
    { "MultiByteToWideChar",        INT32(6, syscall_MultiByteToWideChar(memory, stack))            },
    { "WideCharToMultiByte",        INT32(8, syscall_WideCharToMultiByte(memory, stack))            },

    // kernel32 - system
    { "ExitProcess",                INT32(1, syscall_exit(stack))                                   },
    { "GetCommandLineA",            INT32(0, syscall_GetCommandLineA(memory))                       },
    { "GetCurrentDirectoryA",       INT32(2, syscall_GetCurrentDirectoryA(memory, stack))           },
    { "GetCurrentProcess",          INT32(0, syscall_GetCurrentProcessId())                         },
    { "GetCurrentProcessId",        INT32(0, syscall_GetCurrentProcessId())                         },
    { "GetCurrentThreadId",         INT32(0, syscall_GetCurrentThreadId())                          },
    { "GetLastError",               INT32(0, 0)                                                     },
    { "GetStartupInfoA",            INT32(1, syscall_GetStartupInfoA(memory, stack))                },
    { "GetStartupInfoW",            INT32(1, syscall_GetStartupInfoW(memory, stack))                },
    { "GetStdHandle",               INT32(1, 0xFFFFFFFF)                                            },
    { "GetSystemInfo",              INT32(1, syscall_GetSystemInfo(memory, stack))                  },
    { "GetVersion",                 INT32(0, syscall_GetVersion())                                  },
    { "GetVersionExA",              INT32(1, syscall_GetVersionExA(memory, stack))                  },
    { "IsProcessorFeaturePresent",  INT32(1, false)                                                 },
    { "OutputDebugStringA",         INT32(1, syscall_OutputDebugStringA(memory, stack))             },
    { "SetHandleCount",             INT32(1, 0)                                                     },
    { "SetLastError",               INT32(1, 0)                                                     },
    { "TerminateProcess",           INT32(2, 0)                                                     },

    // kernel32 - time
    { "GetSystemTime",              INT32(1, syscall_GetSystemTime(memory, stack))                  },
    { "GetSystemTimeAsFileTime",    INT32(1, syscall_GetSystemTimeAsFileTime(memory, stack))        },
    { "GetTickCount",               INT32(0, syscall_GetTickCount())                                },
    { "QueryPerformanceCounter",    INT32(1, syscall_QueryPerformanceCounter(memory, stack))        },
    { "QueryPerformanceFrequency",  INT32(1, syscall_QueryPerformanceFrequency(memory, stack))      },

    // kernel32 - tls / fls
    { "FlsAlloc",                   INT32(1, syscall_TlsAlloc(memory))                              },
    { "FlsFree",                    INT32(1, syscall_TlsFree(memory, stack))                        },
    { "FlsGetValue",                INT32(1, syscall_TlsGetValue(memory, stack))                    },
    { "FlsSetValue",                INT32(2, syscall_TlsSetValue(memory, stack))                    },
    { "TlsAlloc",                   INT32(0, syscall_TlsAlloc(memory))                              },
    { "TlsFree",                    INT32(1, syscall_TlsFree(memory, stack))                        },
    { "TlsGetValue",                INT32(1, syscall_TlsGetValue(memory, stack))                    },
    { "TlsSetValue",                INT32(2, syscall_TlsSetValue(memory, stack))                    },

    // kernel32 - unimplemented
//  { "CreateProcessA",             INT32(10, false)                                                },
//  { "DeleteFileA",                INT32(1, true)                                                  },
//  { "FormatMessageA",             INT32(7, 0)                                                     },
//  { "RaiseException",             INT32(4, 0)                                                     },
//  { "WaitForSingleObject",        INT32(2, 0xFFFFFFFF)                                            },

    // advapi32
    { "RegCloseKey",                INT32(1, 0)                                                     },
    { "RegOpenKeyA",                INT32(3, 2)                                                     },
    { "RegOpenKeyExA",              INT32(5, 2)                                                     },
    { "RegQueryValueExA",           INT32(6, 2)                                                     },

    // shlwapi
    { "PathFindFileNameA",          INT32(1, syscall_PathFindFileNameA(memory, stack))              },
    { "PathRemoveExtensionA",       INT32(1, syscall_PathRemoveExtensionA(memory, stack))           },
    { "PathRemoveFileSpecA",        INT32(1, syscall_PathRemoveFileSpecA(memory, stack))            },

    // msvcrt
    { "fopen_s",                    INT32(0, syscall_fopen_s(memory, stack, allocator))             },
    { "memcpy_s",                   INT32(0, syscall_memcpy_s(memory, stack))                       },
    { "memmove_s",                  INT32(0, syscall_memmove_s(memory, stack))                      },
    { "recalloc",                   INT32(0, syscall_recalloc(stack, allocator))                    },
    { "strcat_s",                   INT32(0, syscall_strcat_s(memory, stack))                       },
    { "_callnewh",                  INT32(0, 1)                                                     },
    { "_cexit",                     INT32(0, syscall_exit(stack))                                   },
    { "_CIatan",                    INT32(0, syscall__CIatan(cpu))                                  },
    { "_CIcos",                     INT32(0, syscall__CIcos(cpu))                                   },
    { "_CIexp",                     INT32(0, syscall__CIexp(cpu))                                   },
    { "_CIlog",                     INT32(0, syscall__CIlog(cpu))                                   },
    { "_CIpow",                     INT32(0, syscall__CIpow(cpu))                                   },
    { "_CIsin",                     INT32(0, syscall__CIsin(cpu))                                   },
    { "_CIsqrt",                    INT32(0, syscall__CIsqrt(cpu))                                  },
    { "_controlfp",                 INT32(0, syscall__controlfp(stack, cpu))                        },
    { "_controlfp_s",               INT32(0, syscall__controlfp_s(memory, stack, cpu))              },
    { "_crt_debugger_hook",         INT32(0, 0)                                                     },
    { "_clearfp",                   INT32(0, 0)                                                     },
    { "_c_exit",                    INT32(0, syscall_exit(stack))                                   },
    { "_decode_pointer",            INT32(0, stack[1])                                              },
    { "_encode_pointer",            INT32(0, stack[1])                                              },
    { "_expand",                    INT32(0, syscall_expand(stack, allocator))                      },
    { "_finite",                    INT32(0, syscall_isfinite(stack))                               },
    { "_flushall",                  INT32(0, 0)                                                     },
    { "_fpclass",                   INT32(0, syscall__fpclass(stack))                               },
    { "_ftol",                      INT64(0, syscall__ftol(cpu))                                    },
    { "_ftol2",                     INT64(0, syscall__ftol(cpu))                                    },
    { "_ftol2_sse",                 INT64(0, syscall__ftol(cpu))                                    },
    { "_initterm",                  INT32(0, syscall__initterm(memory, stack, cpu))                 },
    { "_initterm_e",                INT32(0, syscall__initterm(memory, stack, cpu))                 },
    { "_lock",                      INT32(0, 0)                                                     },
    { "_malloc_crt",                INT32(0, syscall_malloc(stack, allocator))                      },
    { "_msize",                     INT32(0, syscall_msize(stack, allocator))                       },
    { "_onexit",                    INT32(0, 0)                                                     },
    { "_setjmp3",                   INT32(0, syscall_setjmp(memory, stack, cpu))                    },
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
//  { "_CxxThrowException",         INT32(0, 0)                                                     },
//  { "_except_handler3",           INT32(0, 0)                                                     },
//  { "_invalid_parameter_noinfo",  INT32(0, 0)                                                     },
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
    { "__stdio_common_vfprintf",    INT32(0, syscall___stdio_common_vfprintf(memory, stack))        },
    { "__stdio_common_vsprintf",    INT32(0, syscall___stdio_common_vsprintf(memory, stack))        },

    // user32
    { "MessageBoxA",                INT32(4, syscall_puts(memory, stack + 1))                       },

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

size_t syscall_windows_new(void* data, SyscallWindows* syscall_windows)
{
    if (data == nullptr)
        return 0;

    auto* cpu = (x86_i386*)data;
    auto* memory = cpu->Memory();
    auto* allocator = cpu->Allocator;

    auto* tib = (TIB*)memory;
    tib->ExceptionList = 0;
    tib->StackBase = uint32_t(syscall_windows->stack_base);
    tib->StackLimit = uint32_t(syscall_windows->stack_limit);
    tib->Version = 0;
    tib->ArbitraryUserPointer = 0;
    tib->Self = 0;

    auto* msvcrt = physical(MSVCRT*, TIB_MSVCRT);
    msvcrt->iob[0][0] = 1;
    msvcrt->iob[1][0] = 2;
    msvcrt->iob[2][0] = 3;

    auto args = (uint32_t*)allocator->allocate(sizeof(uint32_t) * syscall_windows->argc);
    for (int i = 0; i < syscall_windows->argc; ++i) {
        size_t length = strlen(syscall_windows->argv[i]);
        auto arg = allocator->allocate(length + 1);
        memcpy(arg, syscall_windows->argv[i], length);
        args[i] = virtual(int, arg);
    }

    auto envs = (uint32_t*)allocator->allocate(sizeof(uint32_t) * (syscall_windows->envc + 1));
    for (int i = 0; i < syscall_windows->envc; ++i) {
        size_t length = strlen(syscall_windows->envp[i]);
        auto env = allocator->allocate(length + 1);
        memcpy(env, syscall_windows->envp[i], length);
        envs[i] = virtual(int, env);
    }
    envs[syscall_windows->envc] = 0;

    msvcrt->argc = syscall_windows->argc;
    msvcrt->argv = virtual(int, args);
    msvcrt->envp = virtual(int, envs);

    auto* windows = physical(Windows*, TIB_WINDOWS);
    if (windows->image == 0) {
        windows->image = virtual(uint32_t, syscall_windows->image);
        windows->symbol = syscall_windows->symbol;
        windows->debugModule = syscall_windows->debugModule;
        new (windows) Windows;
    }
    static_assert(TIB_WINDOWS + offsetof(Windows, directory) == offset_directory);
    static_assert(TIB_WINDOWS + offsetof(Windows, commandLine) == offset_commandLine);

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

void syscall_windows_import(void* data, const char* file, void* image, bool execute)
{
    auto* cpu = (x86_i386*)data;
    auto* memory = cpu->Memory();
    auto* printf = physical(Printf*, offset_printf);
    auto* windows = physical(Windows*, TIB_WINDOWS);

    struct ImportData {
        mine* cpu;
        Printf* printf;
        Windows* windows;

        static size_t Import(const char* file, const char* name, void* import_data) {
            ImportData& data = *(ImportData*)import_data;
            auto& modules = data.windows->modules;
            auto it = std::find_if(modules.begin(), modules.end(), [&](auto& pair) {
                return strcasecmp(pair.first.c_str(), file) == 0;
            });
            if (it == modules.end()) {
                struct stat st;
                std::string path = data.windows->directory;
                path += '/';
                path += file;
                if (stat(path.c_str(), &st) == 0) {
                    void* image = PE::Load(path.c_str(), [](size_t base, size_t size, void* userdata) {
                        mine* cpu = (mine*)userdata;
                        return cpu->Memory(base, size);
                    }, data.cpu, data.printf->debugPrintf);
                    if (image) {
                        size_t offset = modules.size();
                        modules.emplace_back(file, image);
                        syscall_windows_import(data.cpu, file, image, false);
                        it = modules.begin() + offset;
                    }
                }
            }
            if (it != modules.end()) {
                struct ExportData {
                    size_t address;
                    const char* name;
                    Printf* printf;
                } export_data = { .name = name, .printf = data.printf };
                PE::Exports((*it).second, [](const char* name, size_t address, void* export_data) {
                    auto& data = *(ExportData*)export_data;
                    if (strcmp(data.name, name) == 0) {
                        data.address = address;
                        if (data.printf->debugPrintf) {
                            data.printf->debugPrintf("%-12s : [%08zX] %s", "Symbol", address, name);
                        }
                    }
                }, &export_data);
                return export_data.address;
            }
            if (data.windows->symbol == nullptr)
                return 0;
            return data.windows->symbol(file, name, nullptr);
        };
    } import_data = {
        .cpu = cpu,
        .printf = printf,
        .windows = windows,
    };

    PE::Imports(image, ImportData::Import, &import_data, printf->debugPrintf);
    if (import_data.windows->debugModule && image) {
        import_data.windows->debugModule(file, image);
    }

    if (execute) {
        strncpy(windows->imageDirectory, file, sizeof(windows->imageDirectory));
#ifndef _WIN32
        for (char& c : windows->imageDirectory) {
            if (c == '/')
                c = '\\';
        }
#endif
        return;
    }

    size_t entry = PE::Entry(image);
    if (entry) {
        auto& x86 = cpu->x86;
        uint32_t ret = Pop32();
        Push32(0);
        Push32(1);
        Push32(0);
        Push32(ret);
        Push32(entry);
    }
}

size_t syscall_windows_execute(void* data, size_t index)
{
    index = uint32_t(-index - SYMBOL_INDEX);

    size_t count = sizeof(syscall_table) / sizeof(syscall_table[0]);
    if (index < count) {
        auto* cpu = (x86_i386*)data;
        auto* memory = cpu->Memory();
        auto* stack = (uint32_t*)(memory + cpu->Stack());
        auto* syscall = syscall_table[index].syscall;
        auto* printf = physical(Printf*, offset_printf);
        if (printf->debugPrintf) {
            printf->debugPrintf("[CALL] %s", syscall_table[index].name);
        }
        return syscall(cpu, memory, stack) * sizeof(uint32_t);
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
