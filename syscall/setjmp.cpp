#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include "syscall_internal.h"
#include "../x86/x86_i386.h"
#include "../x86/x86_register.inl"
#include "../x86/x86_instruction.inl"

#ifdef __cplusplus
extern "C" {
#endif

int syscall_longjmp(char* memory, const uint32_t* stack, x86_i386* cpu)
{
    auto& x86 = cpu->x86;
    auto* env = physical(uint32_t*, stack[1]);
    EBP = env[0];
    EBX = env[1];
    EDI = env[2];
    ESI = env[3];
    ESP = env[4];
    Pop32();
    Push32(env[5]);
    return stack[2];
}

int syscall_setjmp(char* memory, const uint32_t* stack, x86_i386* cpu)
{
    auto& x86 = cpu->x86;
    auto* env = physical(uint32_t*, stack[1]);
    env[0] = EBP;
    env[1] = EBX;
    env[2] = EDI;
    env[3] = ESI;
    env[4] = ESP;
    env[5] = stack[0];
    return 0;
}

#ifdef __cplusplus
}
#endif
