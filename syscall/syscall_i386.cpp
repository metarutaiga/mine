#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "syscall.h"
#include "x86/x86_i386.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SYMBOL_INDEX 10

#define CALLBACK_ARGUMENT \
    x86_instruction& x86,   \
    x87_instruction& x87,   \
    void* memory,           \
    void* stack,            \
    allocator_t* allocator, \
    int(*syslog)(const char*, va_list), \
    int(*log)(const char*, va_list)

#define DIV32(value) \
    [](CALLBACK_ARGUMENT) { \
        auto temp = value; \
        x86.regs[0].d = uint32_t(temp.quot); \
        x86.regs[2].d = uint32_t(temp.rem); \
    }
#define DIV64(value) \
    [](CALLBACK_ARGUMENT) { auto stack32 = (uint32_t*)stack; \
        { \
            auto stack = stack32 + 1; \
            auto result = (lldiv_t*)(stack[0] + (size_t)memory); \
            (*result) = value; \
            x86.regs[0].d = uint32_t((size_t)result - (size_t)memory); \
        } \
    }
#define INT32(value) \
    [](CALLBACK_ARGUMENT) { \
        auto temp = value; \
        x86.regs[0].d = uint32_t(temp); \
    }
#define INT64(value) \
    [](CALLBACK_ARGUMENT) { \
        auto temp = value; \
        x86.regs[0].d = uint32_t(temp); \
        x86.regs[2].d = uint32_t(temp >> 32); \
    }
#define FLT64(value) \
    [](CALLBACK_ARGUMENT) { \
        auto temp = value; \
        x87.sts[x87.status._TOP -= 1].d = temp; \
    }

#include "syscall_table.h"

size_t syscall_i386_execute(void* data, size_t index, int(*syslog)(const char*, va_list), int(*log)(const char*, va_list))
{
    index = uint32_t(-index - SYMBOL_INDEX);

    size_t count = sizeof(syscall_table) / sizeof(syscall_table[0]);
    if (index < count) {
        auto* cpu = (x86_i386*)data;
        auto& x86 = cpu->x86;
        auto& x87 = cpu->x87;
        auto* memory = cpu->Memory();
        auto* stack = memory + cpu->Stack();
        auto* allocator = cpu->Allocator;
        auto* syscall = syscall_table[index].syscall;
        if (syslog) {
            syslog("[CALL] %s", (va_list)&syscall_table[index].name);
        }
        syscall(x86, x87, memory, stack, allocator, syslog, log);
    }

    return 0;
}

size_t syscall_i386_symbol(const char* file, const char* name)
{
    if (file == nullptr)
        return 0;

    size_t count = sizeof(syscall_table) / sizeof(syscall_table[0]);
    for (size_t index = 0; index < count; ++index) {
        int shift = (name[0] == '_') ? 1 : 0;
        if (strcmp(syscall_table[index].name, name + shift) == 0)
            return (uint32_t)(-index - SYMBOL_INDEX);
    }

    return 0;
}

const char* syscall_i386_name(size_t index)
{
    index = uint32_t(-index - SYMBOL_INDEX);

    size_t count = sizeof(syscall_table) / sizeof(syscall_table[0]);
    if (index < count) {
        return syscall_table[index].name;
    }

    return nullptr;
}

#ifdef __cplusplus
}
#endif
