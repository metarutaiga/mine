#define _CRT_SECURE_NO_WARNINGS
#include <stdint.h>
#include <wchar.h>
#include <string>
#include <vector>
#include "syscall_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

static std::wstring convert_format_specifier(const wchar_t* format)
{
    size_t pos;
    std::wstring output = format;
    while ((pos = output.find(L"%I64")) != std::string::npos) {
        output.replace(pos, 4, L"%ll");
    }
    return output;
}

static std::vector<uint64_t> convert_format_argument(const wchar_t* format, const void* memory, const void* stack, bool scan)
{
    auto args = (const uint32_t*)stack;
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

int syscall_btowc(char* memory, const uint32_t* stack)
{
    auto c = stack[1];
    return btowc(c);
}

int syscall_fgetwc(char* memory, const uint32_t* stack)
{
    auto stream = physical(FILE**, stack[1]);
    return fgetwc(*stream);
}

int syscall_fgetws(char* memory, const uint32_t* stack)
{
    auto ws = physical(wchar_t*, stack[1]);
    auto num = stack[2];
    auto stream = physical(FILE**, stack[3]);
    auto result = fgetws(ws, num, *stream);
    return virtual(int, result);
}

int syscall_fputwc(char* memory, const uint32_t* stack)
{
    auto wc = stack[1];
    auto stream = physical(FILE**, stack[2]);
    return fputwc(wc, *stream);
}

int syscall_fputws(char* memory, const uint32_t* stack)
{
    auto ws = physical(wchar_t*, stack[1]);
    auto stream = physical(FILE**, stack[2]);
    return fputws(ws, *stream);
}

int syscall_fwide(char* memory, const uint32_t* stack)
{
    auto stream = physical(FILE**, stack[1]);
    auto mode = stack[2];
    return fwide(*stream, mode);
}

int syscall_fwprintf(char* memory, const uint32_t* stack)
{
    auto stream = physical(FILE**, stack[1]);
    auto format = physical(wchar_t*, stack[2]);
    auto args = stack + 3;
    auto format64 = convert_format_specifier(format);
    auto args64 = convert_format_argument(format, memory, args, false);
    return vfwprintf(*stream, format64.c_str(), (va_list)args64.data());
}

int syscall_fwscanf(char* memory, const uint32_t* stack)
{
    auto stream = physical(FILE**, stack[1]);
    auto format = physical(wchar_t*, stack[2]);
    auto args = stack + 3;
    auto format64 = convert_format_specifier(format);
    auto args64 = convert_format_argument(format, memory, args, true);
    return vfwscanf(*stream, format64.c_str(), (va_list)args64.data());
}

int syscall_getwc(char* memory, const uint32_t* stack)
{
    auto stream = physical(FILE**, stack[1]);
    return getwc(*stream);
}

int syscall_getwchar(char* memory, const uint32_t* stack)
{
    return getwchar();
}

size_t syscall_mbrlen(char* memory, const uint32_t* stack)
{
    auto pmb = physical(char*, stack[1]);
    auto max = stack[2];
    auto ps = physical(mbstate_t*, stack[2]);
    return mbrlen(pmb, max, ps);
}

size_t syscall_mbrtowc(char* memory, const uint32_t* stack)
{
    auto pwc = physical(wchar_t*, stack[1]);
    auto pmb = physical(char*, stack[2]);
    auto max = stack[3];
    auto ps = physical(mbstate_t*, stack[4]);
    return mbrtowc(pwc, pmb, max, ps);
}

int syscall_mbsinit(char* memory, const uint32_t* stack)
{
    auto ps = physical(mbstate_t*, stack[1]);
    return mbsinit(ps);
}

size_t syscall_mbsrtowcs(char* memory, const uint32_t* stack)
{
    auto dest = physical(wchar_t*, stack[1]);
    auto src = physical(const char**, stack[2]);
    auto max = stack[3];
    auto ps = physical(mbstate_t*, stack[4]);
    return mbsrtowcs(dest, src, max, ps);
}

int syscall_putwc(char* memory, const uint32_t* stack)
{
    auto wc = stack[1];
    auto stream = physical(FILE**, stack[2]);
    return putwc(wc, *stream);
}

int syscall_putwchar(char* memory, const uint32_t* stack)
{
    auto wc = stack[1];
    return putwchar(wc);
}

int syscall_swprintf(char* memory, const uint32_t* stack)
{
    auto ws = physical(wchar_t*, stack[1]);
    auto len = stack[2];
    auto format = physical(wchar_t*, stack[3]);
    auto args = stack + 4;
    auto format64 = convert_format_specifier(format);
    auto args64 = convert_format_argument(format, memory, args, false);
    return vswprintf(ws, len, format64.c_str(), (va_list)args64.data());
}

int syscall_swscanf(char* memory, const uint32_t* stack)
{
    auto ws = physical(wchar_t*, stack[1]);
    auto format = physical(wchar_t*, stack[2]);
    auto args = stack + 3;
    auto format64 = convert_format_specifier(format);
    auto args64 = convert_format_argument(format, memory, args, true);
    return vswscanf(ws, format64.c_str(), (va_list)args64.data());
}

int syscall_ungetwc(char* memory, const uint32_t* stack)
{
    auto wc = stack[1];
    auto stream = physical(FILE**, stack[2]);
    return ungetwc(wc, *stream);
}

int syscall_vfwprintf(char* memory, const uint32_t* stack)
{
    auto stream = physical(FILE**, stack[1]);
    auto format = physical(wchar_t*, stack[2]);
    auto args = physical(va_list, stack[3]);
    auto format64 = convert_format_specifier(format);
    auto args64 = convert_format_argument(format, memory, args, false);
    return vfwprintf(*stream, format64.c_str(), (va_list)args64.data());
}

int syscall_vfwscanf(char* memory, const uint32_t* stack)
{
    auto stream = physical(FILE**, stack[1]);
    auto format = physical(wchar_t*, stack[2]);
    auto args = physical(va_list, stack[3]);
    auto format64 = convert_format_specifier(format);
    auto args64 = convert_format_argument(format, memory, args, true);
    return vfwscanf(*stream, format64.c_str(), (va_list)args64.data());
}

int syscall_vswprintf(char* memory, const uint32_t* stack)
{
    auto ws = physical(wchar_t*, stack[1]);
    auto len = stack[2];
    auto format = physical(wchar_t*, stack[3]);
    auto args = physical(va_list, stack[4]);
    auto format64 = convert_format_specifier(format);
    auto args64 = convert_format_argument(format, memory, args, false);
    return vswprintf(ws, len, format64.c_str(), (va_list)args64.data());
}

int syscall_vswscanf(char* memory, const uint32_t* stack)
{
    auto ws = physical(wchar_t*, stack[1]);
    auto format = physical(wchar_t*, stack[2]);
    auto args = physical(va_list, stack[3]);
    auto format64 = convert_format_specifier(format);
    auto args64 = convert_format_argument(format, memory, args, true);
    return vswscanf(ws, format64.c_str(), (va_list)args64.data());
}

int syscall_vwprintf(char* memory, const uint32_t* stack)
{
    auto format = physical(wchar_t*, stack[1]);
    auto args = physical(va_list, stack[2]);
    auto format64 = convert_format_specifier(format);
    auto args64 = convert_format_argument(format, memory, args, false);
    return vwprintf(format64.c_str(), (va_list)args64.data());
}

int syscall_vwscanf(char* memory, const uint32_t* stack)
{
    auto format = physical(wchar_t*, stack[1]);
    auto args = physical(va_list, stack[2]);
    auto format64 = convert_format_specifier(format);
    auto args64 = convert_format_argument(format, memory, args, true);
    return vwscanf(format64.c_str(), (va_list)args64.data());
}

size_t syscall_wcrtomb(char* memory, const uint32_t* stack)
{
    auto pmb = physical(char*, stack[1]);
    auto wc = stack[2];
    auto ps = physical(mbstate_t*, stack[3]);
    return wcrtomb(pmb, wc, ps);
}

int syscall_wcscat(char* memory, const uint32_t* stack)
{
    auto destination = physical(wchar_t*, stack[1]);
    auto source = physical(wchar_t*, stack[2]);
    auto result = wcscat(destination, source);
    return virtual(int, result);
}

int syscall_wcschr(char* memory, const uint32_t* stack)
{
    auto ws = physical(wchar_t*, stack[1]);
    auto wc = stack[2];
    auto result = wcschr(ws, wc);
    return virtual(int, result);
}

int syscall_wcscmp(char* memory, const uint32_t* stack)
{
    auto wcs1 = physical(wchar_t*, stack[1]);
    auto wcs2 = physical(wchar_t*, stack[2]);
    return wcscmp(wcs1, wcs2);
}

int syscall_wcscoll(char* memory, const uint32_t* stack)
{
    auto wcs1 = physical(wchar_t*, stack[1]);
    auto wcs2 = physical(wchar_t*, stack[2]);
    return wcscoll(wcs1, wcs2);
}

int syscall_wcscpy(char* memory, const uint32_t* stack)
{
    auto destination = physical(wchar_t*, stack[1]);
    auto source = physical(wchar_t*, stack[2]);
    auto result = wcscpy(destination, source);
    return virtual(int, result);
}

size_t syscall_wcscspn(char* memory, const uint32_t* stack)
{
    auto wcs1 = physical(wchar_t*, stack[1]);
    auto wcs2 = physical(wchar_t*, stack[2]);
    return wcscspn(wcs1, wcs2);
}

size_t syscall_wcsftime(char* memory, const uint32_t* stack)
{
    auto ptr = physical(wchar_t*, stack[1]);
    auto maxsize = stack[2];
    auto format = physical(wchar_t*, stack[3]);
    auto timeptr = physical(struct tm*, stack[4]);
    return wcsftime(ptr, maxsize, format, timeptr);
}

size_t syscall_wcslen(char* memory, const uint32_t* stack)
{
    auto wcs = physical(wchar_t*, stack[1]);
    return wcslen(wcs);
}

int syscall_wcsncat(char* memory, const uint32_t* stack)
{
    auto destination = physical(wchar_t*, stack[1]);
    auto source = physical(wchar_t*, stack[2]);
    auto num = stack[3];
    auto result = wcsncat(destination, source, num);
    return virtual(int, result);
}

int syscall_wcsncmp(char* memory, const uint32_t* stack)
{
    auto wcs1 = physical(wchar_t*, stack[1]);
    auto wcs2 = physical(wchar_t*, stack[2]);
    auto num = stack[3];
    return wcsncmp(wcs1, wcs2, num);
}

int syscall_wcsncpy(char* memory, const uint32_t* stack)
{
    auto destination = physical(wchar_t*, stack[1]);
    auto source = physical(wchar_t*, stack[2]);
    auto num = stack[3];
    auto result = wcsncpy(destination, source, num);
    return virtual(int, result);
}

int syscall_wcspbrk(char* memory, const uint32_t* stack)
{
    auto wcs1 = physical(wchar_t*, stack[1]);
    auto wcs2 = physical(wchar_t*, stack[2]);
    auto result = wcspbrk(wcs1, wcs2);
    return virtual(int, result);
}

int syscall_wcsrchr(char* memory, const uint32_t* stack)
{
    auto ws = physical(wchar_t*, stack[1]);
    auto wc = stack[2];
    auto result = wcsrchr(ws, wc);
    return virtual(int, result);
}

size_t syscall_wcsrtombs(char* memory, const uint32_t* stack)
{
    auto dest = physical(char*, stack[1]);
    auto src = physical(const wchar_t**, stack[2]);
    auto max = stack[3];
    auto ps = physical(mbstate_t*, stack[3]);
    return wcsrtombs(dest, src, max, ps);
}

size_t syscall_wcsspn(char* memory, const uint32_t* stack)
{
    auto wcs1 = physical(wchar_t*, stack[1]);
    auto wcs2 = physical(wchar_t*, stack[2]);
    return wcsspn(wcs1, wcs2);
}

int syscall_wcsstr(char* memory, const uint32_t* stack)
{
    auto wcs1 = physical(wchar_t*, stack[1]);
    auto wcs2 = physical(wchar_t*, stack[2]);
    auto result = wcsstr(wcs1, wcs2);
    return virtual(int, result);
}

double syscall_wcstod(char* memory, const uint32_t* stack)
{
    auto str = physical(wchar_t*, stack[1]);
    auto endptr = physical(wchar_t**, stack[2]);
    return wcstod(str, endptr);
}

float syscall_wcstof(char* memory, const uint32_t* stack)
{
    auto str = physical(wchar_t*, stack[1]);
    auto endptr = physical(wchar_t**, stack[2]);
    return wcstof(str, endptr);
}

int syscall_wcstok(char* memory, const uint32_t* stack)
{
    auto wcs = physical(wchar_t*, stack[1]);
    auto delimiters = physical(wchar_t*, stack[2]);
    auto p = physical(wchar_t**, stack[3]);
    auto result = wcstok(wcs, delimiters, p);
    return virtual(int, result);
}

long syscall_wcstol(char* memory, const uint32_t* stack)
{
    auto str = physical(wchar_t*, stack[1]);
    auto endptr = physical(wchar_t**, stack[2]);
    auto base = stack[3];
    return wcstol(str, endptr, base);
}

long double syscall_wcstold(char* memory, const uint32_t* stack)
{
    auto str = physical(wchar_t*, stack[1]);
    auto endptr = physical(wchar_t**, stack[2]);
    return wcstold(str, endptr);
}

long long syscall_wcstoll(char* memory, const uint32_t* stack)
{
    auto str = physical(wchar_t*, stack[1]);
    auto endptr = physical(wchar_t**, stack[2]);
    auto base = stack[3];
    return wcstoll(str, endptr, base);
}

long syscall_wcstoul(char* memory, const uint32_t* stack)
{
    auto str = physical(wchar_t*, stack[1]);
    auto endptr = physical(wchar_t**, stack[2]);
    auto base = stack[3];
    return wcstoul(str, endptr, base);
}

long long syscall_wcstoull(char* memory, const uint32_t* stack)
{
    auto str = physical(wchar_t*, stack[1]);
    auto endptr = physical(wchar_t**, stack[2]);
    auto base = stack[3];
    return wcstoull(str, endptr, base);
}

int syscall_wcsxfrm(char* memory, const uint32_t* stack)
{
    auto destination = physical(wchar_t*, stack[1]);
    auto source = physical(wchar_t*, stack[2]);
    auto num = stack[3];
    auto result = wcsxfrm(destination, source, num);
    return virtual(int, result);
}

int syscall_wctob(char* memory, const uint32_t* stack)
{
    auto wc = stack[1];
    return wctob(wc);
}

int syscall_wmemchr(char* memory, const uint32_t* stack)
{
    auto ptr = physical(wchar_t*, stack[1]);
    auto wc = stack[2];
    auto num = stack[3];
    auto result = wmemchr(ptr, wc, num);
    return virtual(int, result);
}

int syscall_wmemcmp(char* memory, const uint32_t* stack)
{
    auto ptr1 = physical(wchar_t*, stack[1]);
    auto ptr2 = physical(wchar_t*, stack[2]);
    auto num = stack[3];
    return wmemcmp(ptr1, ptr2, num);
}

int syscall_wmemcpy(char* memory, const uint32_t* stack)
{
    auto destination = physical(wchar_t*, stack[1]);
    auto source = physical(wchar_t*, stack[2]);
    auto num = stack[3];
    auto result = wmemcpy(destination, source, num);
    return virtual(int, result);
}

int syscall_wmemmove(char* memory, const uint32_t* stack)
{
    auto destination = physical(wchar_t*, stack[1]);
    auto source = physical(wchar_t*, stack[2]);
    auto num = stack[3];
    auto result = wmemmove(destination, source, num);
    return virtual(int, result);
}

int syscall_wmemset(char* memory, const uint32_t* stack)
{
    auto ptr = physical(wchar_t*, stack[1]);
    auto wc = stack[2];
    auto num = stack[3];
    auto result = wmemset(ptr, wc, num);
    return virtual(int, result);
}

int syscall_wprintf(char* memory, const uint32_t* stack)
{
    auto format = physical(wchar_t*, stack[1]);
    auto args = stack + 2;
    auto format64 = convert_format_specifier(format);
    auto args64 = convert_format_argument(format, memory, args, false);
    return vwprintf(format64.c_str(), (va_list)args64.data());
}

int syscall_wscanf(char* memory, const uint32_t* stack)
{
    auto format = physical(wchar_t*, stack[1]);
    auto args = stack + 2;
    auto format64 = convert_format_specifier(format);
    auto args64 = convert_format_argument(format, memory, args, true);
    return wscanf(format64.c_str(), (va_list)args64.data());
}

#ifdef __cplusplus
}
#endif
