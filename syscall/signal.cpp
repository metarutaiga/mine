#include <stdint.h>
#include <signal.h>

int syscall_raise(const uint32_t* stack)
{
    auto sig = stack[1];
    return raise(sig);
}
