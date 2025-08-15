#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t syscall_memchr(char* memory, const uint32_t* stack)
{
    auto ptr = memory + stack[1];
    auto value = stack[2];
    auto num = stack[3];
    auto result = (char*)memchr(ptr, value, num);
    return result ? (uint32_t)(result - memory) : 0;
}

int syscall_memcmp(char* memory, const uint32_t* stack)
{
    auto ptr1 = memory + stack[1];
    auto ptr2 = memory + stack[2];
    auto num = stack[3];
    return memcmp(ptr1, ptr2, num);
}

uint32_t syscall_memcpy(char* memory, const uint32_t* stack)
{
    auto destination = memory + stack[1];
    auto source = memory + stack[2];
    auto num = stack[3];
    memcpy(destination, source, num);
    return stack[1];
}

uint32_t syscall_memmove(char* memory, const uint32_t* stack)
{
    auto destination = memory + stack[1];
    auto source = memory + stack[2];
    auto num = stack[3];
    memmove(destination, source, num);
    return stack[1];
}

uint32_t syscall_memset(char* memory, const uint32_t* stack)
{
    auto ptr = memory + stack[1];
    auto value = stack[2];
    auto num = stack[3];
    memset(ptr, value, num);
    return stack[1];
}

uint32_t syscall_strcat(char* memory, const uint32_t* stack)
{
    auto destination = memory + stack[1];
    auto source = memory + stack[2];
    strcat(destination, source);
    return stack[1];
}

uint32_t syscall_strchr(char* memory, const uint32_t* stack)
{
    auto str = memory + stack[1];
    auto character = stack[2];
    auto result = strchr(str, character);
    return result ? (uint32_t)(result - memory) : 0;
}

int syscall_strcmp(char* memory, const uint32_t* stack)
{
    auto str1 = memory + stack[1];
    auto str2 = memory + stack[2];
    return strcmp(str1, str2);
}

int syscall_strcoll(char* memory, const uint32_t* stack)
{
    auto str1 = memory + stack[1];
    auto str2 = memory + stack[2];
    return strcoll(str1, str2);
}

uint32_t syscall_strcpy(char* memory, const uint32_t* stack)
{
    auto destination = memory + stack[1];
    auto source = memory + stack[2];
    strcpy(destination, source);
    return stack[1];
}

size_t syscall_strcspn(char* memory, const uint32_t* stack)
{
    auto str1 = memory + stack[1];
    auto str2 = memory + stack[2];
    return strcspn(str1, str2);
}

uint32_t syscall_strerror(char* memory, const uint32_t* stack)
{
    auto errnum = stack[1];
    printf("%s\n", strerror(errnum));
    return 0;
}

size_t syscall_strlen(char* memory, const uint32_t* stack)
{
    auto str = memory + stack[1];
    return strlen(str);
}

uint32_t syscall_strncat(char* memory, const uint32_t* stack)
{
    auto destination = memory + stack[1];
    auto source = memory + stack[2];
    auto num = stack[3];
    strncat(destination, source, num);
    return stack[1];
}

int syscall_strncmp(char* memory, const uint32_t* stack)
{
    auto str1 = memory + stack[1];
    auto str2 = memory + stack[2];
    auto num = stack[3];
    return strncmp(str1, str2, num);
}

uint32_t syscall_strncpy(char* memory, const uint32_t* stack)
{
    auto destination = memory + stack[1];
    auto source = memory + stack[2];
    auto num = stack[3];
    strncpy(destination, source, num);
    return stack[1];
}

uint32_t syscall_strpbrk(char* memory, const uint32_t* stack)
{
    auto str1 = memory + stack[1];
    auto str2 = memory + stack[2];
    auto result = strpbrk(str1, str2);
    return result ? (uint32_t)(result - memory) : 0;
}

uint32_t syscall_strrchr(char* memory, const uint32_t* stack)
{
    auto str = memory + stack[1];
    auto character = stack[2];
    auto result = strrchr(str, character);
    return result ? (uint32_t)(result - memory) : 0;
}

size_t syscall_strspn(char* memory, const uint32_t* stack)
{
    auto str1 = memory + stack[1];
    auto str2 = memory + stack[2];
    return strspn(str1, str2);
}

uint32_t syscall_strstr(char* memory, const uint32_t* stack)
{
    auto str1 = memory + stack[1];
    auto str2 = memory + stack[2];
    auto result = strstr(str1, str2);
    return result ? (uint32_t)(result - memory) : 0;
}

uint32_t syscall_strtok(char* memory, const uint32_t* stack)
{
    auto str = memory + stack[1];
    auto delimiters = memory + stack[2];
    auto result = strtok(str, delimiters);
    return result ? (uint32_t)(result - memory) : 0;
}

size_t syscall_strxfrm(char* memory, const uint32_t* stack)
{
    auto destination = memory + stack[1];
    auto source = memory + stack[2];
    auto num = stack[3];
    return strxfrm(destination, source, num);
}

#ifdef __cplusplus
}
#endif
