#pragma once

// printf / vprintf
int syscall_vprintf(const void* memory, const void* stack, int(*function)(const char*, va_list) = vprintf);
int syscall_printf(const void* memory, const void* stack, int(*function)(const char*, va_list) = vprintf);

// sprintf / vsprintf
int syscall_sprintf(const void* memory, const void* stack);
int syscall_vsprintf(const void* memory, const void* stack);

// snprintf / vsnprintf
int syscall_snprintf(const void* memory, const void* stack);
int syscall_vsnprintf(const void* memory, const void* stack);
