#include <stdint.h>
#include <setjmp.h>

int syscall_longjmp(const uint32_t* stack)
{
//  auto env = stack[1];
//  auto val = stack[2];
//  longjmp(env, val);
    return 0;
}

int syscall_setjmp(const uint32_t* stack)
{
//  auto env = stack[1];
//  setjmp(env);
    return 0;
}
