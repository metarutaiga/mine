#include <stdint.h>
#include <string>
#include <vector>
#include "syscall/syscall.h"
#include "syscall/syscall_internal.h"
#include "windows.h"

#ifdef __cplusplus
extern "C" {
#endif

int syscall___stdio_common_vfprintf(char* memory, const uint32_t* stack)
{
    uint32_t redirect[4] = { 0, stack[3], stack[4], stack[6] };
    return syscall_vfprintf(memory, redirect);
}

int syscall___stdio_common_vsprintf(char* memory, const uint32_t* stack)
{
    uint32_t redirect[5] = { 0, stack[3], stack[4], stack[5], stack[7] };
    return syscall_vsnprintf(memory, redirect);
}

#ifdef __cplusplus
}
#endif
