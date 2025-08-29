#include <stdint.h>
#include <wctype.h>
#include "syscall_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

int syscall_iswalnum(const uint32_t* stack)
{
    auto c = stack[1];
    return iswalnum(c);
}

int syscall_iswalpha(const uint32_t* stack)
{
    auto c = stack[1];
    return iswalpha(c);
}

int syscall_iswblank(const uint32_t* stack)
{
    auto c = stack[1];
    return iswblank(c);
}

int syscall_iswcntrl(const uint32_t* stack)
{
    auto c = stack[1];
    return iswcntrl(c);
}

int syscall_iswctype(const uint32_t* stack)
{
    auto c = stack[1];
    auto desc = stack[2];
    return iswctype(c, desc);
}

int syscall_iswdigit(const uint32_t* stack)
{
    auto c = stack[1];
    return iswdigit(c);
}

int syscall_iswgraph(const uint32_t* stack)
{
    auto c = stack[1];
    return iswgraph(c);
}

int syscall_iswlower(const uint32_t* stack)
{
    auto c = stack[1];
    return iswlower(c);
}

int syscall_iswprint(const uint32_t* stack)
{
    auto c = stack[1];
    return iswprint(c);
}

int syscall_iswpunct(const uint32_t* stack)
{
    auto c = stack[1];
    return iswpunct(c);
}

int syscall_iswspace(const uint32_t* stack)
{
    auto c = stack[1];
    return iswspace(c);
}

int syscall_iswupper(const uint32_t* stack)
{
    auto c = stack[1];
    return iswupper(c);
}

int syscall_iswxdigit(const uint32_t* stack)
{
    auto c = stack[1];
    return iswxdigit(c);
}

int syscall_towctrans(const uint32_t* stack)
{
    auto c = stack[1];
    auto desc = stack[2];
    return towctrans(c, desc);
}

int syscall_towlower(const uint32_t* stack)
{
    auto c = stack[1];
    return towlower(c);
}

int syscall_towupper(const uint32_t* stack)
{
    auto c = stack[1];
    return towupper(c);
}

int syscall_wctrans(char* memory, const uint32_t* stack)
{
    auto property = physical(char*, stack[1]);
    return wctrans(property);
}

int syscall_wctype(char* memory, const uint32_t* stack)
{
    auto property = physical(char*, stack[1]);
    return wctype(property);
}

#ifdef __cplusplus
}
#endif
