#pragma once

#include <stdint.h>
#include <string>
#include <vector>

template<class C>
std::basic_string<C> syscall_convert_format_specifier(const C* format)
{
    static const C from[5] = { '%', 'I', '6', '4' };
    static const C to[4] = { '%', 'l', 'l' };

    size_t pos;
    std::basic_string<C> output = format;
    while ((pos = output.find(from)) != std::string::npos) {
        output.replace(pos, 4, to);
    }
    return output;
}

template<class C>
std::vector<uint64_t> syscall_convert_format_argument(const C* format, const void* memory, const void* stack, bool scan)
{
    auto args = (const uint32_t*)stack;
    size_t args_index = 0;
    std::vector<uint64_t> output;
    for (size_t i = 0; format[i]; ++i) {
        C c = format[i];
        if (c != '%')
            continue;
        int l = 0;
        for (size_t j = i + 1; format[j]; ++j) {
            C c = format[j];
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
                if (scan) {
                    output.push_back((uint64_t)memory + args[args_index++]);
                    break;
                }
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
                if (scan) {
                    output.push_back((uint64_t)memory + args[args_index++]);
                    break;
                }
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
