#include <stdint.h>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

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
int syscall_printf(const uint8_t* memory, const uint32_t* stack, int(*function)(const char*, va_list))
{
    auto format = (const char*)(memory + stack[1]);
    auto args = (const uint32_t*)(stack + 2);
    auto args64 = convert_format_from_32bit_to_64bit(format, memory, args);
    return function(format, (va_list)args64.data());
}

int syscall_vprintf(const uint8_t* memory, const uint32_t* stack, int(*function)(const char*, va_list))
{
    auto format = (const char*)(memory + stack[1]);
    auto args = (const uint32_t*)(memory + stack[2]);
    auto args64 = convert_format_from_32bit_to_64bit(format, memory, args);
    return function(format, (va_list)args64.data());
}

// sprintf / vsprintf
int syscall_sprintf(const uint8_t* memory, const uint32_t* stack)
{
    auto buffer = stack[1] ? (char*)(memory + stack[1]) : nullptr;
    auto format = (const char*)(memory + stack[2]);
    auto args = (const uint32_t*)(stack + 3);
    auto args64 = convert_format_from_32bit_to_64bit(format, memory, args);
    return vsnprintf(buffer, UINT32_MAX, format, (va_list)args64.data());
}

int syscall_vsprintf(const uint8_t* memory, const uint32_t* stack)
{
    auto buffer = stack[1] ? (char*)(memory + stack[1]) : nullptr;
    auto format = (const char*)(memory + stack[2]);
    auto args = (const uint32_t*)(memory + stack[3]);
    auto args64 = convert_format_from_32bit_to_64bit(format, memory, args);
    return vsnprintf(buffer, UINT32_MAX, format, (va_list)args64.data());
}

// snprintf / vsnprintf
int syscall_snprintf(const uint8_t* memory, const uint32_t* stack)
{
    auto buffer = stack[1] ? (char*)(memory + stack[1]) : nullptr;
    auto length = stack[2];
    auto format = (const char*)(memory + stack[3]);
    auto args = (const uint32_t*)(stack + 4);
    auto args64 = convert_format_from_32bit_to_64bit(format, memory, args);
    return vsnprintf(buffer, length, format, (va_list)args64.data());
}

int syscall_vsnprintf(const uint8_t* memory, const uint32_t* stack)
{
    auto buffer = stack[1] ? (char*)(memory + stack[1]) : nullptr;
    auto length = stack[2];
    auto format = (const char*)(memory + stack[3]);
    auto args = (const uint32_t*)(memory + stack[4]);
    auto args64 = convert_format_from_32bit_to_64bit(format, memory, args);
    return vsnprintf(buffer, length, format, (va_list)args64.data());
}

#ifdef __cplusplus
}
#endif
