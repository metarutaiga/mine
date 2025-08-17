#include <stdint.h>
#include <signal.h>

#ifdef __cplusplus
extern "C" {
#endif

int syscall_raise(const uint32_t* stack)
{
    auto sig = stack[1];
    return raise(sig);
}

#ifdef __cplusplus
}
#endif
