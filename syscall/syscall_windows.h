#pragma once

#ifdef __cplusplus
extern "C" {
#endif

size_t syscall_windows_new(void* data, const char* path, void* image);
size_t syscall_windows_debug(void* data, void(*loadLibraryCallback)(void*));
size_t syscall_windows_delete(void* data);
size_t syscall_windows_execute(void* data, size_t index, int(*syslog)(const char*, ...), int(*log)(const char*, ...));
size_t syscall_windows_symbol(const char* file, const char* name);

#ifdef __cplusplus
}
#endif
