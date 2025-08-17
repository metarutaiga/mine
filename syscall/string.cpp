#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "syscall_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

int syscall_memchr(char* memory, const uint32_t* stack)
{
    auto ptr = physical(char*, stack[1]);
    auto value = stack[2];
    auto num = stack[3];
    auto result = (char*)memchr(ptr, value, num);
    return virtual(int, result);
}

int syscall_memcmp(char* memory, const uint32_t* stack)
{
    auto ptr1 = physical(char*, stack[1]);
    auto ptr2 = physical(char*, stack[2]);
    auto num = stack[3];
    return memcmp(ptr1, ptr2, num);
}

int syscall_memcpy(char* memory, const uint32_t* stack)
{
    auto destination = physical(char*, stack[1]);
    auto source = physical(char*, stack[2]);
    auto num = stack[3];
    memcpy(destination, source, num);
    return stack[1];
}

int syscall_memmove(char* memory, const uint32_t* stack)
{
    auto destination = physical(char*, stack[1]);
    auto source = physical(char*, stack[2]);
    auto num = stack[3];
    memmove(destination, source, num);
    return stack[1];
}

int syscall_memset(char* memory, const uint32_t* stack)
{
    auto ptr = physical(char*, stack[1]);
    auto value = stack[2];
    auto num = stack[3];
    memset(ptr, value, num);
    return stack[1];
}

int syscall_strcat(char* memory, const uint32_t* stack)
{
    auto destination = physical(char*, stack[1]);
    auto source = physical(char*, stack[2]);
    strcat(destination, source);
    return stack[1];
}

int syscall_strchr(char* memory, const uint32_t* stack)
{
    auto str = physical(char*, stack[1]);
    auto character = stack[2];
    auto result = strchr(str, character);
    return virtual(int, result);
}

int syscall_strcmp(char* memory, const uint32_t* stack)
{
    auto str1 = physical(char*, stack[1]);
    auto str2 = physical(char*, stack[2]);
    return strcmp(str1, str2);
}

int syscall_strcoll(char* memory, const uint32_t* stack)
{
    auto str1 = physical(char*, stack[1]);
    auto str2 = physical(char*, stack[2]);
    return strcoll(str1, str2);
}

int syscall_strcpy(char* memory, const uint32_t* stack)
{
    auto destination = physical(char*, stack[1]);
    auto source = physical(char*, stack[2]);
    strcpy(destination, source);
    return stack[1];
}

size_t syscall_strcspn(char* memory, const uint32_t* stack)
{
    auto str1 = physical(char*, stack[1]);
    auto str2 = physical(char*, stack[2]);
    return strcspn(str1, str2);
}

int syscall_strerror(char* memory, const uint32_t* stack)
{
    auto errnum = stack[1];
    printf("%s\n", strerror(errnum));
    return 0;
}

size_t syscall_strlen(char* memory, const uint32_t* stack)
{
    auto str = physical(char*, stack[1]);
    return strlen(str);
}

int syscall_strncat(char* memory, const uint32_t* stack)
{
    auto destination = physical(char*, stack[1]);
    auto source = physical(char*, stack[2]);
    auto num = stack[3];
    strncat(destination, source, num);
    return stack[1];
}

int syscall_strncmp(char* memory, const uint32_t* stack)
{
    auto str1 = physical(char*, stack[1]);
    auto str2 = physical(char*, stack[2]);
    auto num = stack[3];
    return strncmp(str1, str2, num);
}

int syscall_strncpy(char* memory, const uint32_t* stack)
{
    auto destination = physical(char*, stack[1]);
    auto source = physical(char*, stack[2]);
    auto num = stack[3];
    strncpy(destination, source, num);
    return stack[1];
}

int syscall_strpbrk(char* memory, const uint32_t* stack)
{
    auto str1 = physical(char*, stack[1]);
    auto str2 = physical(char*, stack[2]);
    auto result = strpbrk(str1, str2);
    return virtual(int, result);
}

int syscall_strrchr(char* memory, const uint32_t* stack)
{
    auto str = physical(char*, stack[1]);
    auto character = stack[2];
    auto result = strrchr(str, character);
    return virtual(int, result);
}

size_t syscall_strspn(char* memory, const uint32_t* stack)
{
    auto str1 = physical(char*, stack[1]);
    auto str2 = physical(char*, stack[2]);
    return strspn(str1, str2);
}

int syscall_strstr(char* memory, const uint32_t* stack)
{
    auto str1 = physical(char*, stack[1]);
    auto str2 = physical(char*, stack[2]);
    auto result = strstr(str1, str2);
    return virtual(int, result);
}

int syscall_strtok(char* memory, const uint32_t* stack)
{
    auto str = physical(char*, stack[1]);
    auto delimiters = physical(char*, stack[2]);
    auto result = strtok(str, delimiters);
    return virtual(int, result);
}

size_t syscall_strxfrm(char* memory, const uint32_t* stack)
{
    auto destination = physical(char*, stack[1]);
    auto source = physical(char*, stack[2]);
    auto num = stack[3];
    return strxfrm(destination, source, num);
}

#ifdef __cplusplus
}
#endif
