#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <utility>
#include "../allocator.h"
#include "../syscall.h"
#include "../syscall_internal.h"
#include "../../x86/x86_i386.h"
#include "../../x86/x86_register.inl"
#include "../../x86/x86_instruction.inl"
#include "../../x86/x87_register.inl"
#include "msvcrt.h"

#if defined(_WIN32)
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#endif

#ifdef __cplusplus
extern "C" {
#endif

size_t syscall_expand(const uint32_t* stack, struct allocator_t* allocator)
{
    auto pointer = stack[1];
    auto new_size = stack[2];
    auto memory = (char*)allocator->address();
    auto old_pointer = physical(char*, pointer);
    auto old_size = allocator->size(old_pointer);
    if (old_size >= new_size)
        return pointer;
    return 0;
}

int syscall_fopen_s(char* memory, const uint32_t* stack, struct allocator_t* allocator)
{
    auto pFile = physical(uint32_t*, stack[1]);
    if (pFile == nullptr)
        return -1;
    (*pFile) = uint32_t(syscall_fopen(stack + 1, allocator));
    if ((*pFile) == 0)
        return -1;
    return 0;
}

size_t syscall_memcpy_s(char* memory, const uint32_t* stack)
{
    auto destination = physical(char*, stack[1]);
    auto source = physical(char*, stack[3]);
    auto num = stack[4];
    memmove(destination, source, num);
    return 0;
}

size_t syscall_memmove_s(char* memory, const uint32_t* stack)
{
    auto destination = physical(char*, stack[1]);
    auto source = physical(char*, stack[3]);
    auto num = stack[4];
    memmove(destination, source, num);
    return 0;
}

size_t syscall_msize(const uint32_t* stack, struct allocator_t* allocator)
{
    auto pointer = stack[1];
    auto memory = (char*)allocator->address();
    return allocator->size(physical(char*, pointer));
}

size_t syscall_recalloc(const uint32_t* stack, struct allocator_t* allocator)
{
    auto pointer = stack[1];
    auto new_size = stack[2] * stack[3];
    auto memory = (char*)allocator->address();
    auto old_size = allocator->size(physical(char*, pointer));
    if (old_size >= new_size)
        return pointer;
    auto new_pointer = allocator->allocate(new_size);
    if (new_pointer == nullptr)
        return 0;
    memory = (char*)allocator->address();

    if (pointer) {
        memcpy(new_pointer, physical(char*, pointer), new_size < old_size ? new_size : old_size);
        allocator->deallocate(physical(char*, pointer));
    }
    if (new_size > old_size) {
        memset(memory + old_size, 0, new_size - old_size);
    }
    return virtual(size_t, new_pointer);
}

int syscall_splitpath(char* memory, const uint32_t* stack)
{
    auto path = physical(char*, stack[1]);
    auto drive = physical(char*, stack[2]);
    auto dir = physical(char*, stack[3]);
    auto fname = physical(char*, stack[4]);
    auto ext = physical(char*, stack[5]);

    int type = 2;
    size_t length = strlen(path);
    std::pair<size_t, size_t> ranges[3];
    ranges[0] = { length, length };
    ranges[1] = { length, length };
    ranges[2] = { length, length };

    for (size_t i = length - 1; i != SIZE_MAX; --i) {
        char c = path[i];
        switch (type) {
            case 2:
                if (c == '.') {
                    type = 1;
                    ranges[1].second = i;
                    ranges[2].first = i;
                    break;
                }
                if (c == '/' || c == '\\') {
                    type = 0;
                    ranges[0].second = i + 1;
                    ranges[1] = ranges[2];
                    ranges[2] = {};
                    break;
                }
                ranges[2].first = i;
                break;
            case 1:
                if (c == '/' || c == '\\') {
                    type = 0;
                    ranges[0].second = i + 1;
                    break;
                }
                ranges[1].first = i;
                break;
            case 0:
                ranges[0].first = i;
                break;
        }
    }

    ranges[0].second = ranges[0].second - ranges[0].first;
    ranges[1].second = ranges[1].second - ranges[1].first;
    ranges[2].second = ranges[2].second - ranges[2].first;

    if (drive) {
        drive[0] = 0;
    }
    if (dir) {
        dir[ranges[0].second] = 0;
        strncpy(dir, path + ranges[0].first, ranges[0].second);
    }
    if (fname) {
        fname[ranges[1].second] = 0;
        strncpy(fname, path + ranges[1].first, ranges[1].second);
    }
    if (ext) {
        ext[ranges[2].second] = 0;
        strncpy(ext, path + ranges[2].first, ranges[2].second);
    }

    auto* printf = physical(Printf*, offset_printf);
    if (printf->debugPrintf) {
        printf->debugPrintf("[CALL] %s - %s %s %s %s", "splitpath", drive, dir, fname, ext);
    }
    return 0;
}

size_t syscall_strcat_s(char* memory, const uint32_t* stack)
{
    auto destination = physical(char*, stack[1]);
    auto num = stack[2];
    auto source = physical(char*, stack[3]);
    strncat(destination, source, num);
    return stack[1];
}

size_t syscall_strdup(char* memory, const uint32_t* stack, struct allocator_t* allocator)
{
    auto str = stack[1];

    size_t size = strlen(physical(char*, str)) + 1;
    auto result = (char*)allocator->allocate(size);
    if (result == nullptr)
        return 0;
    memory = (char*)allocator->address();

    strncpy(result, physical(char*, str), size);
    return virtual(size_t, result);
}

int syscall_stricmp(char* memory, const uint32_t* stack)
{
    auto str1 = physical(char*, stack[1]);
    auto str2 = physical(char*, stack[2]);
    return strcasecmp(str1, str2);
}

int syscall_strnicmp(char* memory, const uint32_t* stack)
{
    auto str1 = physical(char*, stack[1]);
    auto str2 = physical(char*, stack[2]);
    auto num = stack[3];
    return strncasecmp(str1, str2, num);
}

int syscall__CIatan(x86_i386* cpu)
{
    auto& x87 = cpu->x87;
    ST(0) = atan(ST(0));
    return 0;
}

int syscall__CIcos(x86_i386* cpu)
{
    auto& x87 = cpu->x87;
    ST(0) = cos(ST(0));
    return 0;
}

int syscall__CIexp(x86_i386* cpu)
{
    auto& x87 = cpu->x87;
    ST(0) = exp(ST(0));
    return 0;
}

int syscall__CIlog(x86_i386* cpu)
{
    auto& x87 = cpu->x87;
    ST(0) = log(ST(0));
    return 0;
}

int syscall__CIpow(x86_i386* cpu)
{
    auto& x87 = cpu->x87;
    ST(1) = pow(ST(1), ST(0));
    TOP = TOP + 1;
    return 0;
}

int syscall__CIsin(x86_i386* cpu)
{
    auto& x87 = cpu->x87;
    ST(0) = sin(ST(0));
    return 0;
}

int syscall__CIsqrt(x86_i386* cpu)
{
    auto& x87 = cpu->x87;
    ST(0) = sqrt(ST(0));
    return 0;
}

int syscall__controlfp(const uint32_t* stack, x86_i386* cpu)
{
    auto control = stack[1];
    auto mask = stack[2];

    auto& x87 = cpu->x87;
    int old = FPUControlWord;
    FPUControlWord = (FPUControlWord & ~mask) | (control & mask);

    return old;
}

int syscall__controlfp_s(char* memory, const uint32_t* stack, x86_i386* cpu)
{
    auto oldControl = physical(int*, stack[1]);
    auto newControl = stack[2];
    auto mask = stack[3];

    auto& x87 = cpu->x87;
    if (oldControl) {
        (*oldControl) = FPUControlWord;
    }
    FPUControlWord = (FPUControlWord & ~mask) | (newControl & mask);

    return 0;
}

int syscall__clearfp(x86_i386* cpu)
{
    auto& x87 = cpu->x87;
    int old = FPUStatusWord;
    FPUStatusWord = (FPUStatusWord & ~0x8001F);
    return old;
}

int syscall__fpclass(const uint32_t* stack)
{
    auto x = double(stack[1]);
    auto _FPCLASS_SNAN = 0x0001;
    auto _FPCLASS_QNAN = 0x0002;
    auto _FPCLASS_NINF = 0x0004;
    auto _FPCLASS_NN   = 0x0008;
    auto _FPCLASS_ND   = 0x0010;
    auto _FPCLASS_NZ   = 0x0020;
    auto _FPCLASS_PZ   = 0x0040;
    auto _FPCLASS_PD   = 0x0080;
    auto _FPCLASS_PN   = 0x0100;
    auto _FPCLASS_PINF = 0x0200;
    if (__builtin_isnan(x))         return __builtin_issignaling(x) ? _FPCLASS_SNAN : _FPCLASS_QNAN;
    if (__builtin_isinf(x))         return __builtin_signbit(x)     ? _FPCLASS_NINF : _FPCLASS_PINF;
    if (__builtin_iszero(x))        return __builtin_signbit(x)     ? _FPCLASS_NZ   : _FPCLASS_PZ;
    if (__builtin_isnormal(x))      return __builtin_signbit(x)     ? _FPCLASS_NN   : _FPCLASS_PN;
    if (__builtin_issubnormal(x))   return __builtin_signbit(x)     ? _FPCLASS_ND   : _FPCLASS_PD;
    return 0;
}

int64_t syscall__ftol(x86_i386* cpu)
{
    auto& x87 = cpu->x87;
    double x = ST(0);
    TOP = TOP + 1;
    return (int64_t)x;
}

int syscall__initterm(char* memory, const uint32_t* stack, x86_i386* cpu)
{
    auto begin = physical(uint32_t*, stack[1]);
    auto end = physical(uint32_t*, stack[2]);

    auto& x86 = cpu->x86;
    for (auto function = end - 1; function >= begin; --function) {
        if (*function) {
            Push32(*function);
        }
    }
    return 0;
}

size_t syscall___acrt_iob_func(char* memory, const uint32_t* stack)
{
    auto i = stack[1];
    auto* msvcrt = physical(MSVCRT*, TIB_MSVCRT);
    return virtual(size_t, &msvcrt->iob[i]);
}

int syscall___getmainargs(char* memory, const uint32_t* stack, struct allocator_t* allocator)
{
    auto argc = physical(int*, stack[1]);
    auto argv = physical(int*, stack[2]);
    auto envp = physical(int*, stack[3]);
//  auto expand = physical(int, stack[4]);
//  auto mode = physical(int*, stack[5]);

    auto* msvcrt = physical(MSVCRT*, TIB_MSVCRT);

    *argc = msvcrt->argc;
    *argv = msvcrt->argv;
    *envp = msvcrt->envp;

    return 0;
}

size_t syscall___iob_func(char* memory)
{
    auto* msvcrt = physical(MSVCRT*, TIB_MSVCRT);
    return virtual(int, msvcrt->iob);
}

size_t syscall___p__commode(char* memory)
{
    auto* msvcrt = physical(MSVCRT*, TIB_MSVCRT);
    return virtual(size_t, &msvcrt->commode);
}

size_t syscall___p__fmode(char* memory)
{
    auto* msvcrt = physical(MSVCRT*, TIB_MSVCRT);
    return virtual(size_t, &msvcrt->fmode);
}

size_t syscall___p___initenv(char* memory)
{
    auto* msvcrt = physical(MSVCRT*, TIB_MSVCRT);
    return virtual(size_t, &msvcrt->initenv);
}

#ifdef __cplusplus
}
#endif
