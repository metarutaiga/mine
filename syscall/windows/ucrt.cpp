#include <stdint.h>
#include <string>
#include <vector>
#include "syscall/syscall_internal.h"

std::string syscall_convert_format_specifier(const char* format);
std::vector<uint64_t> syscall_convert_format_argument(const char* format, const void* memory, const void* stack, bool scan);

#ifdef __cplusplus
extern "C" {
#endif

int syscall___stdio_common_vfprintf(uint8_t* memory, const uint32_t* stack, int(*function)(const char*, va_list))
{
    auto stream = physical(FILE**, stack[3]);
    auto format = physical(char*, stack[4]);
    auto args = physical(va_list, stack[6]);
    auto format64 = syscall_convert_format_specifier(format);
    auto args64 = syscall_convert_format_argument(format, memory, args, false);
    switch ((size_t)(*stream)) {
    case 0x0:
    case 0x1:   return 0;
    case 0x2:
    case 0x3:   return function(format64.c_str(), (va_list)args64.data());
    default:    return vfprintf(*stream, format64.c_str(), (va_list)args64.data());
    }
    return 0;
}

int syscall___stdio_common_vsprintf(uint8_t* memory, const uint32_t* stack)
{
    auto buffer = physical(char*, stack[3]);
    auto length = stack[4];
    auto format = physical(char*, stack[5]);
    auto args = physical(va_list, stack[7]);
    auto format64 = syscall_convert_format_specifier(format);
    auto args64 = syscall_convert_format_argument(format, memory, args, false);
    return vsnprintf(buffer, length, format64.c_str(), (va_list)args64.data());
}

#ifdef __cplusplus
}
#endif
