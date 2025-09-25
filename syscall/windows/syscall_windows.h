#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct SyscallWindows {
    size_t stack_base;
    size_t stack_limit;

    size_t(*symbol)(const char*, const char*, void*);
    void(*debugModule)(const char*, void*);

    void* image;
    int argc;
    const char* const* argv;
    int envc;
    const char* const* envp;
};

size_t syscall_windows_new(void* data, SyscallWindows* syscall_windows);
size_t syscall_windows_delete(void* data);
void syscall_windows_import(void* data, const char* file, void* image, bool execute);
size_t syscall_windows_execute(void* data, size_t index);
size_t syscall_windows_symbol(const char* file, const char* name);
const char* syscall_windows_name(size_t index);

// kernel32 - atomic
uint32_t syscall_InterlockedCompareExchange(const void* memory, const void* stack);
uint32_t syscall_InterlockedExchange(const void* memory, const void* stack);

// kernel32 - critical section
int syscall_DeleteCriticalSection(const void* memory, const void* stack, struct allocator_t* allocator);
int syscall_EnterCriticalSection(const void* memory, const void* stack);
int syscall_InitializeCriticalSection(const void* memory, const void* stack, struct allocator_t* allocator);
int syscall_LeaveCriticalSection(const void* memory, const void* stack);
int syscall_TryEnterCriticalSection(const void* memory, const void* stack);

// kernel32 - directory
uint32_t syscall_GetCurrentDirectoryA(const void* memory, const void* stack);
int syscall_SetCurrentDirectoryA(const void* memory, const void* stack);

// kernel32 - file
int syscall_CloseHandle(const void* memory, const void* stack, struct allocator_t* allocator);
size_t syscall_CreateFileA(const void* memory, const void* stack, struct allocator_t* allocator);
size_t syscall_CreateFileMappingA(const void* memory, const void* stack, struct allocator_t* allocator);
uint32_t syscall_GetFileSize(const void* memory, const void* stack);
uint32_t syscall_GetFullPathNameA(const void* memory, const void* stack);
size_t syscall_MapViewOfFile(const void* memory, const void* stack, struct allocator_t* allocator);
int syscall_UnmapViewOfFile(const void* memory, const void* stack, struct allocator_t* allocator);

// kernel32 - find
int syscall_FindClose(const void* memory, const void* stack, struct allocator_t* allocator);
int syscall_FindNextFileA(const void* memory, const void* stack);
size_t syscall_FindFirstFileA(const void* memory, const void* stack, struct allocator_t* allocator);

// kernel32 - heap
size_t syscall_HeapCreate(const void* memory, const void* stack);
bool syscall_HeapDestroy(const void* memory, const void* stack);
size_t syscall_HeapAlloc(const void* memory, const void* stack, struct allocator_t* allocator);
bool syscall_HeapFree(const void* memory, const void* stack, struct allocator_t* allocator);

// kernel32 - library
int syscall_FreeLibrary(const void* memory, const void* stack);
size_t syscall_GetModuleBaseNameA(const void* memory, const void* stack);
size_t syscall_GetModuleFileNameA(const void* memory, const void* stack);
size_t syscall_GetModuleHandleA(const void* memory, const void* stack);
size_t syscall_GetProcAddress(const void* memory, const void* stack);
size_t syscall_LoadLibraryA(const void* memory, const void* stack, void* cpu);

// kernel32 - memory
size_t syscall_LocalAlloc(const void* memory, const void* stack, struct allocator_t* allocator);
size_t syscall_LocalFree(const void* memory, const void* stack, struct allocator_t* allocator);
size_t syscall_VirtualAlloc(const void* memory, const void* stack, struct allocator_t* allocator);
int syscall_VirtualFree(const void* memory, const void* stack, struct allocator_t* allocator);

// kernel32 - system
size_t syscall_GetCommandLineA(const void* memory);
int syscall_GetCurrentProcessId();
int syscall_GetCurrentThreadId();
int syscall_GetSystemInfo(const void* memory, const void* stack);
int syscall_GetVersionExA(const void* memory, const void* stack);
int syscall_OutputDebugStringA(const void* memory, const void* stack);

// kernel32 - time
int syscall_GetSystemTime(const void* memory, const void* stack);
int syscall_GetSystemTimeAsFileTime(const void* memory, const void* stack);
int syscall_GetTickCount();
int syscall_QueryPerformanceCounter(const void* memory, const void* stack);
int syscall_QueryPerformanceFrequency(const void* memory, const void* stack);

// kernel32 - tls
uint32_t syscall_TlsAlloc(const void* memory);
bool syscall_TlsFree(const void* memory, const void* stack);
uint32_t syscall_TlsGetValue(const void* memory, const void* stack);
bool syscall_TlsSetValue(const void* memory, const void* stack);

// msvcprt
size_t syscall_basic_string_char_constructor(size_t thiz, const void* memory);
size_t syscall_basic_string_char_copy_constructor(size_t thiz, const void* memory, const void* stack, struct allocator_t* allocator);
size_t syscall_basic_string_char_copy_range_constructor(size_t thiz, const void* memory, const void* stack, struct allocator_t* allocator);
size_t syscall_basic_string_char_cstr_constructor(size_t thiz, const void* memory, const void* stack, struct allocator_t* allocator);
int syscall_basic_string_char_deconstructor(size_t thiz, const void* memory, struct allocator_t* allocator);
size_t syscall_basic_string_char_assign(size_t thiz, const void* memory, const void* stack, struct allocator_t* allocator);
size_t syscall_basic_string_char_assign_cstr(size_t thiz, const void* memory, const void* stack, struct allocator_t* allocator);
size_t syscall_basic_string_char_at(size_t thiz, const void* memory, const void* stack);
size_t syscall_basic_string_char_append(size_t thiz, const void* memory, const void* stack, struct allocator_t* allocator);
size_t syscall_basic_string_char_append_cstr(size_t thiz, const void* memory, const void* stack, struct allocator_t* allocator);
size_t syscall_basic_string_char_substr(size_t thiz, const void* memory, const void* stack, struct allocator_t* allocator);
bool syscall_basic_string_char_eq_cstr(const void* memory, const void* stack);
bool syscall_basic_string_char_neq_cstr(const void* memory, const void* stack);

// msvcrt
size_t syscall_expand(const void* stack, struct allocator_t* allocator);
int syscall_fopen_s(const void* memory, const void* stack, struct allocator_t* allocator);
size_t syscall_memcpy_s(const void* memory, const void* stack);
size_t syscall_memmove_s(const void* memory, const void* stack);
size_t syscall_msize(const void* stack, struct allocator_t* allocator);
size_t syscall_recalloc(const void* stack, struct allocator_t* allocator);
int syscall_splitpath(const void* memory, const void* stack);
size_t syscall_strcat_s(const void* memory, const void* stack);
size_t syscall_strdup(const void* memory, const void* stack, struct allocator_t* allocator);
int syscall_stricmp(const void* memory, const void* stack);
int syscall_strnicmp(const void* memory, const void* stack);
int syscall__CIatan(void* cpu);
int syscall__CIcos(void* cpu);
int syscall__CIexp(void* cpu);
int syscall__CIlog(void* cpu);
int syscall__CIpow(void* cpu);
int syscall__CIsin(void* cpu);
int syscall__CIsqrt(void* cpu);
int syscall__controlfp(const void* stack, void* cpu);
int syscall__controlfp_s(const void* memory, const void* stack, void* cpu);
int syscall__decode_pointer(const void* stack);
int syscall__encode_pointer(const void* stack);
int syscall__initterm(const void* memory, const void* stack, void* cpu);
int syscall__setjmp3(const void* memory, const void* stack, void* cpu);
size_t syscall___acrt_iob_func(const void* memory, const void* stack);
int syscall___getmainargs(const void* memory, const void* stack, struct allocator_t* allocator);
size_t syscall___iob_func(const void* memory);
size_t syscall___p__commode(const void* memory);
size_t syscall___p__fmode(const void* memory);
size_t syscall___p___initenv(const void* memory);

// ucrt
int syscall___stdio_common_vfprintf(const void* memory, const void* stack);
int syscall___stdio_common_vsprintf(const void* memory, const void* stack);

#ifdef __cplusplus
}
#endif
