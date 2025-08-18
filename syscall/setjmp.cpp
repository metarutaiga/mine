#include <stdint.h>
#include <setjmp.h>
#include "syscall_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

int syscall_longjmp(char* memory, const uint32_t* stack)
{
    auto env = stack + 1;
    auto val = stack[1 + sizeof(jmp_buf) / sizeof(int)];
    longjmp(*(jmp_buf*)&env, val);
    return 0;
}

int syscall_setjmp(char* memory, const uint32_t* stack)
{
    auto env = stack + 1;
    return setjmp(*(jmp_buf*)&env);
}

#ifdef __cplusplus
}
#endif
