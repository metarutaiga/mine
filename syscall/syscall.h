#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// printf / vprintf
int syscall_vprintf(const void* memory, const void* stack, int(*function)(const char*, va_list) = vprintf);
int syscall_printf(const void* memory, const void* stack, int(*function)(const char*, va_list) = vprintf);

// sprintf / vsprintf
int syscall_sprintf(const void* memory, const void* stack);
int syscall_vsprintf(const void* memory, const void* stack);

// snprintf / vsnprintf
int syscall_snprintf(const void* memory, const void* stack);
int syscall_vsnprintf(const void* memory, const void* stack);

// calloc / malloc / realloc / free
uint32_t syscall_calloc(const void* stack, struct allocator_t* allocator);
uint32_t syscall_malloc(const void* stack, struct allocator_t* allocator);
uint32_t syscall_realloc(const void* stack, struct allocator_t* allocator);
uint32_t syscall_free(const void* stack, struct allocator_t* allocator);
uint32_t syscall_expand(const void* stack, struct allocator_t* allocator);
uint32_t syscall_msize(const void* stack, struct allocator_t* allocator);
uint32_t syscall_recalloc(const void* stack, struct allocator_t* allocator);

// string
int syscall_memchr(const void* memory, const void* stack);
int syscall_memcmp(const void* memory, const void* stack);
int syscall_memcpy(const void* memory, const void* stack);
int syscall_memmove(const void* memory, const void* stack);
int syscall_memset(const void* memory, const void* stack);
int syscall_strcat(const void* memory, const void* stack);
int syscall_strchr(const void* memory, const void* stack);
int syscall_strcmp(const void* memory, const void* stack);
int syscall_strcoll(const void* memory, const void* stack);
int syscall_strcpy(const void* memory, const void* stack);
int syscall_strcspn(const void* memory, const void* stack);
int syscall_strerror(const void* memory, const void* stack);
int syscall_strlen(const void* memory, const void* stack);
int syscall_strncat(const void* memory, const void* stack);
int syscall_strncmp(const void* memory, const void* stack);
int syscall_strncpy(const void* memory, const void* stack);
int syscall_strpbrk(const void* memory, const void* stack);
int syscall_strrchr(const void* memory, const void* stack);
int syscall_strspn(const void* memory, const void* stack);
int syscall_strstr(const void* memory, const void* stack);
int syscall_strtok(const void* memory, const void* stack);
int syscall_strxfrm(const void* memory, const void* stack);

#ifdef __cplusplus
}
#endif
