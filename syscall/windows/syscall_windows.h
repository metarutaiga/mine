#pragma once

#ifdef __cplusplus
extern "C" {
#endif

size_t syscall_windows_new(void* data, const char* path, void* image, int argc, const char* argv[], int envc, const char* envp[]);
size_t syscall_windows_debug(void* data, void(*loadLibraryCallback)(void*));
size_t syscall_windows_delete(void* data);
size_t syscall_windows_execute(void* data, size_t index, int(*syslog)(const char*, ...), int(*log)(const char*, ...));
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
int syscall_SetCurrentDirectoryA(const void* memory, const void* stack);

// kernel32 - file
int syscall_CloseHandle(const void* memory, const void* stack, struct allocator_t* allocator);
size_t syscall_CreateFileA(const void* memory, const void* stack, struct allocator_t* allocator);
size_t syscall_CreateFileMappingA(const void* memory, const void* stack, struct allocator_t* allocator);
uint32_t syscall_GetFileSize(const void* memory, const void* stack);
size_t syscall_MapViewOfFile(const void* memory, const void* stack, struct allocator_t* allocator);
int syscall_UnmapViewOfFile(const void* memory, const void* stack, struct allocator_t* allocator);

// kernel32 - find
int syscall_FindClose(const void* memory, const void* stack, struct allocator_t* allocator);
int syscall_FindNextFileA(const void* memory, const void* stack);
size_t syscall_FindFirstFileA(const void* memory, const void* stack, struct allocator_t* allocator);

// kernel32 - library
size_t syscall_GetModuleBaseNameA(const void* memory, const void* stack);
size_t syscall_GetModuleFileNameA(const void* memory, const void* stack);
size_t syscall_GetModuleHandleA(const void* memory, const void* stack);
size_t syscall_GetProcAddress(const void* memory, const void* stack, int(*log)(const char*, ...));
size_t syscall_LoadLibraryA(const void* memory, const void* stack, void* cpu, int(*log)(const char*, ...));
int syscall_FreeLibrary(const void* memory, const void* stack);

// kernel32 - system
int syscall_GetCurrentProcessId();
int syscall_GetCurrentThreadId();

// kernel32 - time
int syscall_GetSystemTimeAsFileTime(const void* memory, const void* stack);
int syscall_GetTickCount();
int syscall_QueryPerformanceCounter(const void* memory, const void* stack);

// msvcprt
size_t syscall_basic_string_char_copy_constructor(size_t thiz, uint8_t* memory, const uint32_t* stack, struct allocator_t* allocator);
size_t syscall_basic_string_char_cstr_constructor(size_t thiz, uint8_t* memory, const uint32_t* stack, struct allocator_t* allocator);
size_t syscall_basic_string_char_constructor(size_t thiz, uint8_t* memory, struct allocator_t* allocator);
size_t syscall_basic_string_char_deconstructor(size_t thiz, uint8_t* memory, struct allocator_t* allocator);

// msvcrt
size_t syscall_expand(const void* stack, struct allocator_t* allocator);
size_t syscall_memmove_s(const void* memory, const void* stack);
size_t syscall_msize(const void* stack, struct allocator_t* allocator);
size_t syscall_recalloc(const void* stack, struct allocator_t* allocator);
int syscall_splitpath(const void* memory, const void* stack);
int syscall_stricmp(const void* memory, const void* stack);
int syscall_strnicmp(const void* memory, const void* stack);
int syscall__CIatan(void* cpu);
int syscall__CIsqrt(void* cpu);
int syscall__controlfp(const void* stack, void* cpu);
int syscall__controlfp_s(const void* memory, const void* stack, void* cpu);
int syscall__decode_pointer(const void* stack);
int syscall__encode_pointer(const void* stack);
int syscall__initterm(const void* memory, const void* stack, void* cpu);
int syscall__setjmp3(const void* memory, const void* stack, void* cpu);
int syscall___getmainargs(const void* memory, const void* stack, struct allocator_t* allocator);
size_t syscall___iob_func(const void* memory);
size_t syscall___p__commode(const void* memory);
size_t syscall___p__fmode(const void* memory);
size_t syscall___p___initenv(const void* memory);

#ifdef __cplusplus
}
#endif
