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
int syscall_CreateFileA(const void* memory, const void* stack, struct allocator_t* allocator);
int syscall_CreateFileMappingA(const void* memory, const void* stack, struct allocator_t* allocator);
int syscall_GetFileSize(const void* memory, const void* stack);
int syscall_MapViewOfFile(const void* memory, const void* stack, struct allocator_t* allocator);
int syscall_UnmapViewOfFile(const void* memory, const void* stack, struct allocator_t* allocator);

// kernel32 - find
int syscall_FindClose(const void* memory, const void* stack, struct allocator_t* allocator);
int syscall_FindNextFileA(const void* memory, const void* stack);
int syscall_FindFirstFileA(const void* memory, const void* stack, struct allocator_t* allocator);

// kernel32 - library
int syscall_GetProcAddress(const void* memory, const void* stack);
int syscall_GetModuleHandleA(const void* memory, const void* stack);
int syscall_LoadLibraryA(const void* memory, const void* stack, void* cpu, int(*log)(const char*, ...));
int syscall_FreeLibrary(const void* memory, const void* stack);

// msvcprt
int syscall_basic_string_char_copy_constructor(uint32_t thiz, uint8_t* memory, const uint32_t* stack, struct allocator_t* allocator);
int syscall_basic_string_char_cstr_constructor(uint32_t thiz, uint8_t* memory, const uint32_t* stack, struct allocator_t* allocator);
int syscall_basic_string_char_deconstructor(uint32_t thiz, uint8_t* memory, struct allocator_t* allocator);

// msvcrt
int syscall_expand(const void* stack, struct allocator_t* allocator);
int syscall_msize(const void* stack, struct allocator_t* allocator);
int syscall_recalloc(const void* stack, struct allocator_t* allocator);
int syscall_splitpath(const void* memory, const void* stack);
int syscall_stricmp(const void* memory, const void* stack);
int syscall_strnicmp(const void* memory, const void* stack);
int syscall__initterm(const void* memory, const void* stack, void* cpu);
int syscall___getmainargs(const void* memory, const void* stack, struct allocator_t* allocator);
int syscall___p__commode(const void* memory);
int syscall___p__fmode(const void* memory);
int syscall___p___initenv(const void* memory);

#ifdef __cplusplus
}
#endif
