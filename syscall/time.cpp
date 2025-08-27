#define _CRT_SECURE_NO_WARNINGS
#include <stdint.h>
#include <time.h>
#include "syscall_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

int syscall_asctime(char* memory, const uint32_t* stack)
{
    auto tm = physical(struct tm*, stack[1]);
    auto result = asctime(tm);
    return virtual(int, result);
}

clock_t syscall_clock()
{
    return clock();
}

int syscall_ctime(char* memory, const uint32_t* stack)
{
    auto timer = physical(time_t*, stack[1]);
    auto result = ctime(timer);
    return virtual(int, result);
}

double syscall_difftime(const uint32_t* stack)
{
    auto end = stack[1];
    auto beginning = stack[2];
    return difftime(end, beginning);
}

int syscall_gmtime(char* memory, const uint32_t* stack)
{
    auto timer = physical(time_t*, stack[1]);
    auto result = gmtime(timer);
    return virtual(int, result);
}

int syscall_localtime(char* memory, const uint32_t* stack)
{
    auto timer = physical(time_t*, stack[1]);
    auto result = localtime(timer);
    return virtual(int, result);
}

time_t syscall_mktime(char* memory, const uint32_t* stack)
{
    auto timeptr = physical(struct tm*, stack[1]);
    return mktime(timeptr);
}

size_t syscall_strftime(char* memory, const uint32_t* stack)
{
    auto ptr = physical(char*, stack[1]);
    auto maxsize = stack[2];
    auto format = physical(char*, stack[3]);
    auto timeptr = physical(struct tm*, stack[4]);
    return strftime(ptr, maxsize, format, timeptr);
}

time_t syscall_time(char* memory, const uint32_t* stack)
{
    auto timer = physical(time_t*, stack[1]);
    return time(timer);
}

#ifdef __cplusplus
}
#endif
