#include <locale.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "allocator.h"
#include "syscall_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

int syscall_localeconv(const char* memory)
{
    auto result = localeconv();
    return virtual(int, result);
}

int syscall_setlocale(const char* memory, const uint32_t* stack, struct allocator_t* allocator)
{
    auto category = stack[1];
    auto locale = physical(char*, stack[2]);
    auto result = setlocale(category, locale);

    auto size = strlen(result) + 1;
    auto pointer = (char*)allocator->allocate(size);
    if (pointer == nullptr)
        return 0;
    memory = (char*)allocator->address();

    strncpy(pointer, result, size);

    return virtual(int, pointer);
}

#ifdef __cplusplus
}
#endif
