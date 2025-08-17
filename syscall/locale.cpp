#include <locale.h>
#include <stdint.h>
#include "syscall_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

int syscall_localeconv(const char* memory)
{
    auto result = (char*)localeconv();
    return virtual(int, result);
}

int syscall_setlocale(const char* memory, const uint32_t* stack)
{
    auto category = stack[1];
    auto locale = memory + stack[2];
    auto result = setlocale(category, locale);
    return virtual(int, result);
}

#ifdef __cplusplus
}
#endif
