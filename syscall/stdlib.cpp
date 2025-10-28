#define _CRT_SECURE_NO_WARNINGS
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "allocator.h"
#include "syscall_internal.h"
#include "../mine.h"

#ifdef __cplusplus
extern "C" {
#endif

int syscall__Exit(const uint32_t* stack)
{
    auto status = stack[1];
    _Exit(status);
    return 0;
}

int syscall_abort()
{
    abort();
    return 0;
}

int syscall_abs(const uint32_t* stack)
{
    int n = stack[1];
    return abs(n);
}

int syscall_at_quick_exit(char* memory, const uint32_t* stack)
{
#if defined(_UCRT)
    return 0;
#else
    auto func = physical(void(*)(void), stack[1]);
    return at_quick_exit(func);
#endif
}

int syscall_atexit(char* memory, const uint32_t* stack)
{
    auto func = physical(void(*)(void), stack[1]);
    return atexit(func);
}

double syscall_atof(char* memory, const uint32_t* stack)
{
    auto str = physical(char*, stack[1]);
    return atof(str);
}

int syscall_atoi(char* memory, const uint32_t* stack)
{
    auto str = physical(char*, stack[1]);
    return atoi(str);
}

long syscall_atol(char* memory, const uint32_t* stack)
{
    auto str = physical(char*, stack[1]);
    return atol(str);
}

long long syscall_atoll(char* memory, const uint32_t* stack)
{
    auto str = physical(char*, stack[1]);
    return atoll(str);
}

size_t syscall_bsearch(char* memory, const uint32_t* stack)
{
    auto key = physical(void*, stack[1]);
    auto base = physical(void*, stack[2]);
    auto num = stack[3];
    auto size = stack[4];
    auto compar = physical(int(*)(const void*, const void*), stack[5]);
    auto result = bsearch(key, base, num, size, compar);
    return virtual(size_t, result);
}

size_t syscall_calloc(const uint32_t* stack, struct allocator_t* allocator)
{
    auto size = stack[1] * stack[2];
    auto pointer = allocator->allocate(size);
    if (pointer == nullptr)
        return 0;
    auto memory = allocator->address();

    memset(pointer, 0, size);
    return virtual(size_t, pointer);
}

div_t syscall_div(const uint32_t* stack)
{
    int numer = stack[1];
    int denom = stack[2];
    return div(numer, denom);
}

int syscall_exit(uint32_t* stack)
{
#if 1
    stack[0] = 0;
#else
    auto status = stack[1];
    exit(status);
#endif
    return 0;
}

int syscall_free(const uint32_t* stack, struct allocator_t* allocator)
{
    auto pointer = stack[1];
    auto memory = allocator->address();
    allocator->deallocate(physical(char*, pointer));
    return 0;
}

size_t syscall_getenv(char* memory, const uint32_t* stack)
{
    auto name = physical(char*, stack[1]);
    auto result = getenv(name);
    return virtual(size_t, result);
}

long syscall_labs(const uint32_t* stack)
{
    long n = stack[1];
    return labs(n);
}

ldiv_t syscall_ldiv(const uint32_t* stack)
{
    long numer = stack[1];
    long denom = stack[2];
    return ldiv(numer, denom);
}

long long syscall_llabs(const uint32_t* stack)
{
    long long n = *(long long*)&stack[1];
    return llabs(n);
}

lldiv_t syscall_lldiv(const uint32_t* stack)
{
    long long numer = *(long long*)&stack[1];
    long long denom = *(long long*)&stack[3];
    return lldiv(numer, denom);
}

int syscall_malloc(const uint32_t* stack, struct allocator_t* allocator)
{
    auto size = stack[1];
    auto pointer = allocator->allocate(size);
    if (pointer == nullptr)
        return 0;
    auto memory = allocator->address();

    return virtual(int, pointer);
}

int syscall_mblen(char* memory, const uint32_t* stack)
{
    auto pmb = physical(char*, stack[1]);
    auto max = stack[2];
    return mblen(pmb, max);
}

size_t syscall_mbstowcs(char* memory, const uint32_t* stack)
{
    auto dest = physical(wchar_t*, stack[1]);
    auto src = physical(char*, stack[2]);
    auto max = stack[3];
    return mbstowcs(dest, src, max);
}

int syscall_mbtowc(char* memory, const uint32_t* stack)
{
    auto pwc = physical(wchar_t*, stack[1]);
    auto pmb = physical(char*, stack[2]);
    auto max = stack[3];
    return mbtowc(pwc, pmb, max);
}

int syscall_qsort(char* memory, const uint32_t* stack, mine* cpu)
{
    auto base = physical(void*, stack[1]);
    auto num = stack[2];
    auto size = stack[3];
//  auto compar = physical(int(*)(const void*, const void*), stack[4]);

    qsort_r(base, num, size, cpu, [](void* c, const void* a, const void* b) -> int {
        auto cpu = (mine*)c;
        auto memory = cpu->Memory();
        auto stack = (uint32_t*)(memory + cpu->Stack());
        auto compar = stack[4];
        return (int)cpu->Call(10000, compar, 2, virtual(uint32_t, a), virtual(uint32_t, b));
    });
    return 0;
}

int syscall_quick_exit(const uint32_t* stack)
{
    auto status = stack[1];
    quick_exit(status);
    return 0;
}

int syscall_rand()
{
    return rand();
}

int syscall_realloc(const uint32_t* stack, struct allocator_t* allocator)
{
    auto pointer = stack[1];
    auto new_size = stack[2];
    auto memory = allocator->address();
    auto old_size = allocator->size(physical(char*, pointer));
    if (old_size >= new_size)
        return pointer;
    auto new_pointer = allocator->allocate(new_size);
    if (new_pointer == nullptr)
        return 0;
    memory = (char*)allocator->address();

    if (pointer) {
        memcpy(new_pointer, physical(char*, pointer), new_size < old_size ? new_size : old_size);
        allocator->deallocate(physical(char*, pointer));
    }
    return virtual(int, new_pointer);
}

int syscall_srand(const uint32_t* stack)
{
    auto seed = stack[1];
    srand(seed);
    return 0;
}

double syscall_strtod(char* memory, const uint32_t* stack)
{
    char* end = nullptr;
    auto str = physical(char*, stack[1]);
    auto endptr = physical(uint32_t*, stack[2]);
    auto result = strtod(str, &end);
    if (endptr) (*endptr) = virtual(uint32_t, end);
    return result;
}

float syscall_strtof(char* memory, const uint32_t* stack)
{
    char* end = nullptr;
    auto str = physical(char*, stack[1]);
    auto endptr = physical(uint32_t*, stack[2]);
    auto result = strtof(str, &end);
    if (endptr) (*endptr) = virtual(uint32_t, end);
    return result;
}

long syscall_strtol(char* memory, const uint32_t* stack)
{
    char* end = nullptr;
    auto str = physical(char*, stack[1]);
    auto endptr = physical(uint32_t*, stack[2]);
    auto base = stack[3];
    auto result = strtol(str, &end, base);
    if (endptr) (*endptr) = virtual(uint32_t, end);
    return result;
}

long double syscall_strtold(char* memory, const uint32_t* stack)
{
    char* end = nullptr;
    auto str = physical(char*, stack[1]);
    auto endptr = physical(uint32_t*, stack[2]);
    auto result = strtold(str, &end);
    if (endptr) (*endptr) = virtual(uint32_t, end);
    return result;
}

long long syscall_strtoll(char* memory, const uint32_t* stack)
{
    char* end = nullptr;
    auto str = physical(char*, stack[1]);
    auto endptr = physical(uint32_t*, stack[2]);
    auto base = stack[3];
    auto result = strtoll(str, &end, base);
    if (endptr) (*endptr) = virtual(uint32_t, end);
    return result;
}

unsigned long syscall_strtoul(char* memory, const uint32_t* stack)
{
    char* end = nullptr;
    auto str = physical(char*, stack[1]);
    auto endptr = physical(uint32_t*, stack[2]);
    auto base = stack[3];
    auto result = strtoul(str, &end, base);
    if (endptr) (*endptr) = virtual(uint32_t, end);
    return result;
}

unsigned long long syscall_strtoull(char* memory, const uint32_t* stack)
{
    char* end = nullptr;
    auto str = physical(char*, stack[1]);
    auto endptr = physical(uint32_t*, stack[2]);
    auto base = stack[3];
    auto result = strtoull(str, &end, base);
    if (endptr) (*endptr) = virtual(uint32_t, end);
    return result;
}

int syscall_system(char* memory, const uint32_t* stack)
{
    auto command = physical(char*, stack[1]);
    return system(command);
}

size_t syscall_wcstombs(char* memory, const uint32_t* stack)
{
    auto dest = physical(char*, stack[1]);
    auto src = physical(wchar_t*, stack[2]);
    auto max = stack[3];
    return wcstombs(dest, src, max);
}

int syscall_wctomb(char* memory, const uint32_t* stack)
{
    auto pmb = physical(char*, stack[1]);
    auto wc = stack[2];
    return wctomb(pmb, wc);
}

#ifdef __cplusplus
}
#endif
