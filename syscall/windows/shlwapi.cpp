#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../syscall_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

size_t syscall_PathFindFileNameA(const uint8_t* memory, const uint32_t* stack)
{
    auto pszPath = physical(char*, stack[1]);
    if (pszPath) {
        char* file = strrchr(pszPath, '/');
        if (file == nullptr)
            file = strrchr(pszPath, '\\');
        if (file)
            return virtual(size_t, file + 1);
    }
    return virtual(size_t, pszPath);
}

int syscall_PathRemoveExtensionA(const uint8_t* memory, const uint32_t* stack)
{
    auto pszPath = physical(char*, stack[1]);
    if (pszPath) {
        char* extension = strrchr(pszPath, '.');
        if (extension)
            extension[0] = 0;
    }
    return 0;
}

bool syscall_PathRemoveFileSpecA(const uint8_t* memory, const uint32_t* stack)
{
    auto pszPath = physical(char*, stack[1]);
    if (pszPath) {
        char* file = strrchr(pszPath, '/');
        if (file == nullptr)
            file = strrchr(pszPath, '\\');
        if (file)
            file[0] = 0;
    }
    return true;
}

#ifdef __cplusplus
}
#endif
