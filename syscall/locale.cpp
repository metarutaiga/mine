#include <locale.h>
#include <stdint.h>

int syscall_localeconv(const char* memory)
{
    auto result = (char*)localeconv();
    return result ? (int)(result - memory) : 0;
}

int syscall_setlocale(const char* memory, const uint32_t* stack)
{
    auto category = stack[1];
    auto locale = memory + stack[2];
    auto result = setlocale(category, locale);
    return result ? (int)(result - memory) : 0;
}
