#if defined(__APPLE__)
#include <sys/cdefs.h>
#endif
#undef __deprecated_msg
#define __deprecated_msg(_msg)
#define _CRT_SECURE_NO_WARNINGS
#include <stdint.h>
#include "allocator.h"
#include "convert_format.h"
#include "syscall_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

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

    auto& record = *physical(std::vector<FILE*>*, offset_file);
    for (FILE*& file : record) {
        if (file == (*stream)) {
            file = record.back();
            record.pop_back();
            break;
        }
    }

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
    switch ((size_t)(*stream)) {
    case 0x0:
    case 0x1:
    case 0x2:
    case 0x3:   return 0;
    default:    return fflush(*stream);
    }
    return 0;
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

size_t syscall_fgets(char* memory, const uint32_t* stack)
{
    auto str = physical(char*, stack[1]);
    auto num = stack[2];
    auto stream = physical(FILE**, stack[3]);
    auto result = fgets(str, num, *stream);
    return virtual(size_t, result);
}

size_t syscall_fopen(char* memory, const uint32_t* stack, struct allocator_t* allocator)
{
    auto stream = (FILE**)allocator->allocate(sizeof(FILE*));
    if (stream == nullptr)
        return 0;
    auto filename = physical(char*, stack[1]);
    auto mode = physical(char*, stack[2]);

    char fullpath[260];
    if (filename[0] != '/' && filename[0] != '\\') {
        auto directory = physical(char*, offset_directory);
        strncpy(fullpath, directory, 260);
        strncat(fullpath, "/", 260);
    }
    else {
        fullpath[0] = 0;
    }
    strncat(fullpath, filename, 260);
#ifndef _WIN32
    for (char& c : fullpath) {
        if (c == '\\')
            c = '/';
    }
#endif

    (*stream) = fopen(fullpath, mode);
    if ((*stream) == nullptr) {
        allocator->deallocate(stream);
        return 0;
    }

    auto& record = *physical(std::vector<FILE*>*, offset_file);
    record.push_back(*stream);

    return virtual(size_t, stream);
}

int syscall_fprintf(char* memory, const uint32_t* stack)
{
    auto printf = physical(Printf*, offset_printf);
    auto stream = physical(FILE**, stack[1]);
    auto format = physical(char*, stack[2]);
    auto args = stack + 3;
    auto format64 = syscall_convert_format_specifier(format);
    auto args64 = syscall_convert_format_argument(format, memory, args, false);
    switch ((size_t)(*stream)) {
    case 0x0:
    case 0x1:   return 0;
    case 0x45ECDFB6:
    case 0x2:
    case 0x3:   return printf->vprintf(format64.c_str(), (va_list)args64.data());
    default:    return fprintf(*stream, format64.c_str(), (va_list)args64.data());
    }
    return 0;
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

size_t syscall_fread(char* memory, const uint32_t* stack)
{
    auto ptr = physical(void*, stack[1]);
    auto size = stack[2];
    auto count = stack[3];
    auto stream = physical(FILE**, stack[4]);
    return fread(ptr, size, count, *stream);
}

size_t syscall_freopen(char* memory, const uint32_t* stack)
{
    auto filename = physical(char*, stack[1]);
    auto mode = physical(char*, stack[2]);
    auto stream = physical(FILE**, stack[3]);
    (*stream) = freopen(filename, mode, *stream);
    return virtual(size_t, stream);
}

int syscall_fscanf(char* memory, const uint32_t* stack)
{
    auto stream = physical(FILE**, stack[1]);
    auto format = physical(char*, stack[2]);
    auto args = stack + 3;
    auto format64 = syscall_convert_format_specifier(format);
    auto args64 = syscall_convert_format_argument(format, memory, args, true);
    return vfscanf(*stream, format64.c_str(), (va_list)args64.data());
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

long syscall_ftell(char* memory, const uint32_t* stack)
{
    auto stream = physical(FILE**, stack[1]);
    return ftell(*stream);
}

size_t syscall_fwrite(char* memory, const uint32_t* stack)
{
    auto ptr = physical(void*, stack[1]);
    auto size = stack[2];
    auto count = stack[3];
    auto stream = physical(FILE**, stack[4]);
    return fwrite(ptr, size, count, *stream);
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

size_t syscall_gets(char* memory, const uint32_t* stack)
{
#if defined(_UCRT)
    return 0;
#else
    auto str = physical(char*, stack[1]);
    auto result = gets(str);
    return virtual(size_t, result);
#endif
}

int syscall_perror(char* memory, const uint32_t* stack)
{
    auto str = physical(char*, stack[1]);
    perror(str);
    return 0;
}

int syscall_printf(char* memory, const uint32_t* stack)
{
    auto printf = physical(Printf*, offset_printf);
    auto format = physical(char*, stack[1]);
    auto args = stack + 2;
    auto format64 = syscall_convert_format_specifier(format);
    auto args64 = syscall_convert_format_argument(format, memory, args, false);
    return printf->vprintf(format64.c_str(), (va_list)args64.data());
}

int syscall_putc(char* memory, const uint32_t* stack)
{
    auto character = stack[1];
    auto stream = physical(FILE**, stack[2]);
    return putc(character, *stream);
}

int syscall_putchar(char* memory, const uint32_t* stack)
{
    auto printf = physical(Printf*, offset_printf);
    auto character = stack[1];
    return printf->printf("%c", character);
}

int syscall_puts(char* memory, const uint32_t* stack)
{
    auto printf = physical(Printf*, offset_printf);
    auto str = physical(char*, stack[1]);
    return printf->printf("%s\n", str);
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
    auto format64 = syscall_convert_format_specifier(format);
    auto args64 = syscall_convert_format_argument(format, memory, args, true);
    return vscanf(format64.c_str(), (va_list)args64.data());
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
    auto format64 = syscall_convert_format_specifier(format);
    auto args64 = syscall_convert_format_argument(format, memory, args, false);
    return vsnprintf(buffer, length, format64.c_str(), (va_list)args64.data());
}

int syscall_sprintf(char* memory, const uint32_t* stack)
{
    auto buffer = physical(char*, stack[1]);
    auto format = physical(char*, stack[2]);
    auto args = stack + 3;
    auto format64 = syscall_convert_format_specifier(format);
    auto args64 = syscall_convert_format_argument(format, memory, args, false);
    return vsnprintf(buffer, UINT32_MAX, format64.c_str(), (va_list)args64.data());
}

int syscall_sscanf(char* memory, const uint32_t* stack)
{
    auto s = physical(char*, stack[1]);
    auto format = physical(char*, stack[2]);
    auto args = stack + 3;
    auto format64 = syscall_convert_format_specifier(format);
    auto args64 = syscall_convert_format_argument(format, memory, args, true);
    return vsscanf(s, format64.c_str(), (va_list)args64.data());
}

size_t syscall_tmpfile(char* memory, struct allocator_t* allocator)
{
    auto result = (FILE**)allocator->allocate(sizeof(FILE*));
    if (result == nullptr)
        return 0;
    (*result) = tmpfile();
    return virtual(size_t, result);
}

size_t syscall_tmpnam(char* memory, const uint32_t* stack)
{
    auto str = physical(char*, stack[1]);
    auto result = tmpnam(str);
    return virtual(size_t, result);
}

int syscall_ungetc(char* memory, const uint32_t* stack)
{
    auto character = stack[1];
    auto stream = physical(FILE**, stack[2]);
    return ungetc(character, *stream);
}

int syscall_vfprintf(char* memory, const uint32_t* stack)
{
    auto printf = physical(Printf*, offset_printf);
    auto stream = physical(FILE**, stack[1]);
    auto format = physical(char*, stack[2]);
    auto args = physical(va_list, stack[3]);
    auto format64 = syscall_convert_format_specifier(format);
    auto args64 = syscall_convert_format_argument(format, memory, args, false);
    switch ((size_t)(*stream)) {
    case 0x0:
    case 0x1:   return 0;
    case 0x2:
    case 0x3:   return printf->vprintf(format64.c_str(), (va_list)args64.data());
    default:    return vfprintf(*stream, format64.c_str(), (va_list)args64.data());
    }
    return 0;
}

int syscall_vfscanf(char* memory, const uint32_t* stack)
{
    auto stream = physical(FILE**, stack[1]);
    auto format = physical(char*, stack[2]);
    auto args = physical(va_list, stack[3]);
    auto format64 = syscall_convert_format_specifier(format);
    auto args64 = syscall_convert_format_argument(format, memory, args, true);
    return vfscanf(*stream, format64.c_str(), (va_list)args64.data());
}

int syscall_vprintf(char* memory, const uint32_t* stack)
{
    auto printf = physical(Printf*, offset_printf);
    auto format = physical(char*, stack[1]);
    auto args = physical(char*, stack[2]);
    auto format64 = syscall_convert_format_specifier(format);
    auto args64 = syscall_convert_format_argument(format, memory, args, false);
    return printf->vprintf(format64.c_str(), (va_list)args64.data());
}

int syscall_vscanf(char* memory, const uint32_t* stack)
{
    auto format = physical(char*, stack[1]);
    auto args = physical(va_list, stack[2]);
    auto format64 = syscall_convert_format_specifier(format);
    auto args64 = syscall_convert_format_argument(format, memory, args, true);
    return vscanf(format64.c_str(), (va_list)args64.data());
}

int syscall_vsnprintf(char* memory, const uint32_t* stack)
{
    auto buffer = physical(char*, stack[1]);
    auto length = stack[2];
    auto format = physical(char*, stack[3]);
    auto args = physical(va_list, stack[4]);
    auto format64 = syscall_convert_format_specifier(format);
    auto args64 = syscall_convert_format_argument(format, memory, args, false);
    return vsnprintf(buffer, length, format64.c_str(), (va_list)args64.data());
}

int syscall_vsprintf(char* memory, const uint32_t* stack)
{
    auto buffer = physical(char*, stack[1]);
    auto format = physical(char*, stack[2]);
    auto args = physical(va_list, stack[3]);
    auto format64 = syscall_convert_format_specifier(format);
    auto args64 = syscall_convert_format_argument(format, memory, args, false);
    return vsnprintf(buffer, UINT32_MAX, format64.c_str(), (va_list)args64.data());
}

#ifdef __cplusplus
}
#endif
