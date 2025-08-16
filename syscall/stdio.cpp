#include <sys/cdefs.h>
#undef __deprecated_msg
#define __deprecated_msg(_msg)
#include <stdint.h>
#include <vector>
#include "allocator.h"

#ifdef __cplusplus
extern "C" {
#endif

#define physical(type, index)   (type)(index ? index + memory : 0)
#define virtual(type, index)    (type)(index ? (char*)index - memory : 0)

static std::vector<uint64_t> convert_format_from_32bit_to_64bit(const char* format, const void* memory, const void* stack)
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

int syscall_clearerr(char* memory, const uint32_t* stack)
{
    auto stream = physical(FILE**, stack[1]);
    clearerr(*stream);
    return 0;
}

int syscall_fclose(char* memory, const uint32_t* stack, struct allocator_t* allocator)
{
    auto stream = physical(FILE**, stack[1]);
    auto result = fclose(*stream);
    allocator->deallocate(stream);
    return result;
}

int syscall_feof(char* memory, const uint32_t* stack)
{
    auto stream = physical(FILE**, stack[1]);
    return feof(*stream);
}

int syscall_ferror(char* memory, const uint32_t* stack)
{
    auto stream = physical(FILE**, stack[1]);
    return ferror(*stream);
}

int syscall_fflush(char* memory, const uint32_t* stack)
{
    auto stream = physical(FILE**, stack[1]);
    return fflush(*stream);
}

int syscall_fgetc(char* memory, const uint32_t* stack)
{
    auto stream = physical(FILE**, stack[1]);
    return fgetc(*stream);
}

int syscall_fgetpos(char* memory, const uint32_t* stack)
{
    fpos_t temp = 0;
    auto stream = physical(FILE**, stack[1]);
    auto pos = physical(int*, stack[2]);
    auto result = fgetpos(*stream, &temp);
    (*pos) = (int)temp;
    return result;
}

int syscall_fgets(char* memory, const uint32_t* stack)
{
    auto str = physical(char*, stack[1]);
    auto num = stack[2];
    auto stream = physical(FILE**, stack[3]);
    auto result = fgets(str, num, *stream);
    return virtual(int, result);
}

int syscall_fopen(char* memory, const uint32_t* stack, struct allocator_t* allocator)
{
    auto stream = (FILE**)allocator->allocate(sizeof(FILE*));
    if (stream == nullptr)
        return 0;
    auto filename = physical(char*, stack[1]);
    auto mode = physical(char*, stack[2]);
    (*stream) = fopen(filename, mode);
    if ((*stream) == nullptr) {
        allocator->deallocate(stream);
        return 0;
    }
    return virtual(int, stream);
}

int syscall_fprintf(char* memory, const uint32_t* stack)
{
    auto stream = physical(FILE**, stack[1]);
    auto format = physical(char*, stack[2]);
    auto args = stack + 3;
    auto args64 = convert_format_from_32bit_to_64bit(format, memory, args);
    return fprintf(*stream, format, (va_list)args64.data());
}

int syscall_fputc(char* memory, const uint32_t* stack)
{
    auto character = stack[1];
    auto stream = physical(FILE**, stack[2]);
    return fputc(character, *stream);
}

int syscall_fputs(char* memory, const uint32_t* stack)
{
    auto str = physical(char*, stack[1]);
    auto stream = physical(FILE**, stack[2]);
    return fputs(str, *stream);
}

int syscall_fread(char* memory, const uint32_t* stack)
{
    auto ptr = physical(void*, stack[1]);
    auto size = stack[2];
    auto count = stack[3];
    auto stream = physical(FILE**, stack[4]);
    return (int)fread(ptr, size, count, *stream);
}

int syscall_freopen(char* memory, const uint32_t* stack)
{
    auto filename = physical(char*, stack[1]);
    auto mode = physical(char*, stack[2]);
    auto stream = physical(FILE**, stack[3]);
    (*stream) = freopen(filename, mode, *stream);
    return virtual(int, stream);
}

int syscall_fscanf(char* memory, const uint32_t* stack)
{
    auto stream = physical(FILE**, stack[1]);
    auto format = physical(char*, stack[2]);
    auto args = stack + 3;
    auto args64 = convert_format_from_32bit_to_64bit(format, memory, args);
    return vfscanf(*stream, format, (va_list)args64.data());
}

int syscall_fseek(char* memory, const uint32_t* stack)
{
    auto stream = physical(FILE**, stack[1]);
    auto offset = stack[2];
    auto origin = stack[3];
    return fseek(*stream, offset, origin);
}

int syscall_fsetpos(char* memory, const uint32_t* stack)
{
    auto stream = physical(FILE**, stack[1]);
    fpos_t pos = *physical(int*, stack[2]);
    return fsetpos(*stream, &pos);
}

int syscall_ftell(char* memory, const uint32_t* stack)
{
    auto stream = physical(FILE**, stack[1]);
    return (int)ftell(*stream);
}

int syscall_fwrite(char* memory, const uint32_t* stack)
{
    auto ptr = physical(void*, stack[1]);
    auto size = stack[2];
    auto count = stack[3];
    auto stream = physical(FILE**, stack[4]);
    return (int)fwrite(ptr, size, count, *stream);
}

int syscall_getc(char* memory, const uint32_t* stack)
{
    auto stream = physical(FILE**, stack[1]);
    return fgetc(*stream);
}

int syscall_getchar()
{
    return getchar();
}

int syscall_gets(char* memory, const uint32_t* stack)
{
    auto str = physical(char*, stack[1]);
    auto result = gets(str);
    return virtual(int, result);
}

int syscall_perror(char* memory, const uint32_t* stack)
{
    auto str = physical(char*, stack[1]);
    perror(str);
    return 0;
}

int syscall_printf(char* memory, const uint32_t* stack, int(*function)(const char*, va_list))
{
    auto format = physical(char*, stack[1]);
    auto args = stack + 2;
    auto args64 = convert_format_from_32bit_to_64bit(format, memory, args);
    return function(format, (va_list)args64.data());
}

int syscall_putc(char* memory, const uint32_t* stack)
{
    auto character = stack[1];
    auto stream = physical(FILE**, stack[2]);
    return putc(character, *stream);
}

int syscall_putchar(const uint32_t* stack)
{
    auto character = stack[1];
    return putchar(character);
}

int syscall_puts(char* memory, const uint32_t* stack)
{
    auto str = physical(char*, stack[1]);
    return puts(str);
}

int syscall_remove(char* memory, const uint32_t* stack)
{
    auto filename = physical(char*, stack[1]);
    return remove(filename);
}

int syscall_rename(char* memory, const uint32_t* stack)
{
    auto oldname = physical(char*, stack[1]);
    auto newname = physical(char*, stack[2]);
    return rename(oldname, newname);
}

int syscall_rewind(char* memory, const uint32_t* stack)
{
    auto stream = physical(FILE**, stack[1]);
    rewind(*stream);
    return 0;
}

int syscall_scanf(char* memory, const uint32_t* stack)
{
    auto format = physical(char*, stack[1]);
    auto args = stack + 2;
    auto args64 = convert_format_from_32bit_to_64bit(format, memory, args);
    return vscanf(format, (va_list)args64.data());
}

int syscall_setbuf(char* memory, const uint32_t* stack)
{
    auto stream = physical(FILE**, stack[1]);
    auto buffer = physical(char*, stack[2]);
    setbuf(*stream, buffer);
    return 0;
}

int syscall_setvbuf(char* memory, const uint32_t* stack)
{
    auto stream = physical(FILE**, stack[1]);
    auto buffer = physical(char*, stack[2]);
    auto mode = stack[3];
    auto size = stack[4];
    return setvbuf(*stream, buffer, mode, size);
}

int syscall_snprintf(char* memory, const uint32_t* stack)
{
    auto buffer = physical(char*, stack[1]);
    auto length = stack[2];
    auto format = physical(char*, stack[3]);
    auto args = stack + 4;
    auto args64 = convert_format_from_32bit_to_64bit(format, memory, args);
    return vsnprintf(buffer, length, format, (va_list)args64.data());
}

int syscall_sprintf(char* memory, const uint32_t* stack)
{
    auto buffer = physical(char*, stack[1]);
    auto format = physical(char*, stack[2]);
    auto args = stack + 3;
    auto args64 = convert_format_from_32bit_to_64bit(format, memory, args);
    return vsnprintf(buffer, UINT32_MAX, format, (va_list)args64.data());
}

int syscall_sscanf(char* memory, const uint32_t* stack)
{
    auto s = physical(char*, stack[1]);
    auto format = physical(char*, stack[2]);
    auto args = stack + 3;
    auto args64 = convert_format_from_32bit_to_64bit(format, memory, args);
    return vsscanf(s, format, (va_list)args64.data());
}

int syscall_tmpfile(char* memory, struct allocator_t* allocator)
{
    auto result = (FILE**)allocator->allocate(sizeof(FILE*));
    if (result == nullptr)
        return 0;
    (*result) = tmpfile();
    return virtual(int, result);
}

int syscall_tmpnam(char* memory, const uint32_t* stack)
{
    auto str = physical(char*, stack[1]);
    auto result = tmpnam(str);
    return virtual(int, result);
}

int syscall_ungetc(char* memory, const uint32_t* stack)
{
    auto character = stack[1];
    auto stream = physical(FILE**, stack[2]);
    return ungetc(character, *stream);
}

int syscall_vfprintf(char* memory, const uint32_t* stack)
{
    auto stream = physical(FILE**, stack[1]);
    auto format = physical(char*, stack[2]);
    auto args = physical(va_list, stack[3]);
    auto args64 = convert_format_from_32bit_to_64bit(format, memory, args);
    return vfprintf(*stream, format, (va_list)args64.data());
}

int syscall_vfscanf(char* memory, const uint32_t* stack)
{
    auto stream = physical(FILE**, stack[1]);
    auto format = physical(char*, stack[2]);
    auto args = physical(va_list, stack[3]);
    auto args64 = convert_format_from_32bit_to_64bit(format, memory, args);
    return vfscanf(*stream, format, (va_list)args64.data());
}

int syscall_vprintf(char* memory, const uint32_t* stack, int(*function)(const char*, va_list))
{
    auto format = physical(char*, stack[1]);
    auto args = physical(char*, stack[2]);
    auto args64 = convert_format_from_32bit_to_64bit(format, memory, args);
    return function(format, (va_list)args64.data());
}

int syscall_vscanf(char* memory, const uint32_t* stack)
{
    auto format = physical(char*, stack[1]);
    auto args = physical(va_list, stack[2]);
    auto args64 = convert_format_from_32bit_to_64bit(format, memory, args);
    return vscanf(format, (va_list)args64.data());
}

int syscall_vsnprintf(char* memory, const uint32_t* stack)
{
    auto buffer = physical(char*, stack[1]);
    auto length = stack[2];
    auto format = physical(char*, stack[3]);
    auto args = physical(va_list, stack[4]);
    auto args64 = convert_format_from_32bit_to_64bit(format, memory, args);
    return vsnprintf(buffer, length, format, (va_list)args64.data());
}

int syscall_vsprintf(char* memory, const uint32_t* stack)
{
    auto buffer = physical(char*, stack[1]);
    auto format = physical(char*, stack[2]);
    auto args = physical(va_list, stack[3]);
    auto args64 = convert_format_from_32bit_to_64bit(format, memory, args);
    return vsnprintf(buffer, UINT32_MAX, format, (va_list)args64.data());
}

#ifdef __cplusplus
}
#endif
