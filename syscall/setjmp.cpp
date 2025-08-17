#include <stdint.h>
#include <setjmp.h>
#include "syscall_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

int syscall_longjmp(char* memory, const uint32_t* stack)
{
    auto env = physical(int*, stack[1]);
    auto val = stack[2];
    longjmp(env, val);
    return 0;
}

int syscall_setjmp(char* memory, const uint32_t* stack)
{
    auto env = physical(int*, stack[1]);
    return setjmp(env);
}

#ifdef __cplusplus
}
#endif
