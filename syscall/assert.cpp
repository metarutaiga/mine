#undef NDEBUG
#include <assert.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int syscall_assert(const uint32_t* stack)
{
    auto expression = stack[1];
    assert(expression);
    return 0;
}

#ifdef __cplusplus
}
#endif
