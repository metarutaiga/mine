#include <ctype.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int syscall_isalnum(const uint32_t* stack)
{
    auto c = stack[1];
    return isalnum(c);
}

int syscall_isalpha(const uint32_t* stack)
{
    auto c = stack[1];
    return isalpha(c);
}

int syscall_iscntrl(const uint32_t* stack)
{
    auto c = stack[1];
    return iscntrl(c);
}

int syscall_isdigit(const uint32_t* stack)
{
    auto c = stack[1];
    return isdigit(c);
}

int syscall_isgraph(const uint32_t* stack)
{
    auto c = stack[1];
    return isgraph(c);
}

int syscall_islower(const uint32_t* stack)
{
    auto c = stack[1];
    return islower(c);
}

int syscall_isprint(const uint32_t* stack)
{
    auto c = stack[1];
    return isprint(c);
}

int syscall_ispunct(const uint32_t* stack)
{
    auto c = stack[1];
    return ispunct(c);
}

int syscall_isspace(const uint32_t* stack)
{
    auto c = stack[1];
    return isspace(c);
}

int syscall_isupper(const uint32_t* stack)
{
    auto c = stack[1];
    return isupper(c);
}

int syscall_isxdigit(const uint32_t* stack)
{
    auto c = stack[1];
    return isxdigit(c);
}

int syscall_tolower(const uint32_t* stack)
{
    auto c = stack[1];
    return tolower(c);
}

int syscall_toupper(const uint32_t* stack)
{
    auto c = stack[1];
    return toupper(c);
}

#ifdef __cplusplus
}
#endif
