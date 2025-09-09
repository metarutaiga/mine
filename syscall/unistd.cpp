#define _CRT_SECURE_NO_WARNINGS
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "syscall_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

int syscall_chdir(char* memory, const uint32_t* stack)
{
    auto path = physical(char*, stack[1]);

    auto directory = physical(char*, offset_directory);
    strncpy(directory, path, 260);

    return 0;
}

size_t syscall_getcwd(char* memory, const uint32_t* stack)
{
    auto buf = physical(char*, stack[1]);
    auto size = stack[2];

    auto directory = physical(char*, offset_directory);
    strncpy(buf, directory, size);

    return virtual(size_t, buf);
}

#ifdef __cplusplus
}
#endif
