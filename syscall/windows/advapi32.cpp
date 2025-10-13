#include <stdint.h>
#include <stdlib.h>
#include "../syscall_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

bool syscall_RtlGenRandom(uint8_t* memory, const uint32_t* stack)
{
    auto RandomBuffer = physical(char*, stack[1]);
    auto RandomBufferLength = stack[2];

    for (size_t i = 0; i < RandomBufferLength; ++i) {
        RandomBuffer[i] = rand();
    }

    return true;
}

#ifdef __cplusplus
}
#endif
