#include <stdint.h>
#include <vector>
#include "syscall.h"

static std::vector<uint64_t> convert_format_from_32bit_to_64bit(const char* format, const void* memory, const uint32_t* args)
{
    size_t args_index = 0;
    std::vector<uint64_t> output;
    for (size_t i = 0; format[i]; ++i) {
        char c = format[i];
        if (c != '%')
            continue;
        int l = 0;
        for (size_t j = i + 1; format[j]; ++j) {
            char c = format[j];
            switch (c) {
            case 'l':
                l++;
                continue;
            case 'c':
            case 'd':
            case 'i':
            case 'o':
            case 'p':
            case 'u':
            case 'x':
            case 'X':
                output.push_back((uint64_t)args[args_index++]);
                if (l < 2)
                    break;
                output.back() |= (uint64_t)args[args_index++] << 32;
                break;
            case 'a':
            case 'A':
            case 'e':
            case 'E':
            case 'f':
            case 'F':
            case 'g':
            case 'G':
                output.push_back((uint64_t)args[args_index++]);
                output.back() |= (uint64_t)args[args_index++] << 32;
                break;
            case 's':
                output.push_back((uint64_t)memory + args[args_index++]);
                break;
            case '*':
                output.push_back((uint64_t)args[args_index++]);
                continue;
            default:
                continue;
            }
            i = j - 1;
            break;
        }
    }
    return output;
}

// printf / vprintf
int syscall_printf(const void* memory, const void* stack, int(*function)(const char*, va_list))
{
    auto memory8 = (const uint8_t*)memory;
    auto stack32 = (const uint32_t*)stack;
    auto format = (const char*)(memory8 + stack32[1]);
    auto args = (const uint32_t*)(stack32 + 2);
    auto args64 = convert_format_from_32bit_to_64bit(format, memory, args);
    return function(format, (va_list)args64.data());
}

int syscall_vprintf(const void* memory, const void* stack, int(*function)(const char*, va_list))
{
    auto memory8 = (const uint8_t*)memory;
    auto stack32 = (const uint32_t*)stack;
    auto format = (const char*)(memory8 + stack32[1]);
    auto args = (const uint32_t*)(memory8 + stack32[2]);
    auto args64 = convert_format_from_32bit_to_64bit(format, memory, args);
    return function(format, (va_list)args64.data());
}

// sprintf / vsprintf
int syscall_sprintf(const void* memory, const void* stack)
{
    auto memory8 = (const uint8_t*)memory;
    auto stack32 = (const uint32_t*)stack;
    auto buffer = stack32[1] ? (char*)(memory8 + stack32[1]) : nullptr;
    auto format = (const char*)(memory8 + stack32[2]);
    auto args = (const uint32_t*)(stack32 + 3);
    auto args64 = convert_format_from_32bit_to_64bit(format, memory, args);
    return vsnprintf(buffer, UINT32_MAX, format, (va_list)args64.data());
}

int syscall_vsprintf(const void* memory, const void* stack)
{
    auto memory8 = (const uint8_t*)memory;
    auto stack32 = (const uint32_t*)stack;
    auto buffer = stack32[1] ? (char*)(memory8 + stack32[1]) : nullptr;
    auto format = (const char*)(memory8 + stack32[2]);
    auto args = (const uint32_t*)(memory8 + stack32[3]);
    auto args64 = convert_format_from_32bit_to_64bit(format, memory, args);
    return vsnprintf(buffer, UINT32_MAX, format, (va_list)args64.data());
}

// snprintf / vsnprintf
int syscall_snprintf(const void* memory, const void* stack)
{
    auto memory8 = (const uint8_t*)memory;
    auto stack32 = (const uint32_t*)stack;
    auto buffer = stack32[1] ? (char*)(memory8 + stack32[1]) : nullptr;
    auto length = stack32[2];
    auto format = (const char*)(memory8 + stack32[3]);
    auto args = (const uint32_t*)(stack32 + 4);
    auto args64 = convert_format_from_32bit_to_64bit(format, memory, args);
    return vsnprintf(buffer, length, format, (va_list)args64.data());
}

int syscall_vsnprintf(const void* memory, const void* stack)
{
    auto memory8 = (const uint8_t*)memory;
    auto stack32 = (const uint32_t*)stack;
    auto buffer = stack32[1] ? (char*)(memory8 + stack32[1]) : nullptr;
    auto length = stack32[2];
    auto format = (const char*)(memory8 + stack32[3]);
    auto args = (const uint32_t*)(memory8 + stack32[4]);
    auto args64 = convert_format_from_32bit_to_64bit(format, memory, args);
    return vsnprintf(buffer, length, format, (va_list)args64.data());
}
