#define _CRT_SECURE_NO_WARNINGS
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "syscall.h"
#include "syscall_internal.h"
#include "../x86/x86_i386.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SYMBOL_INDEX 10

#define CALLBACK_ARGUMENT \
    void* memory,           \
    void* stack,            \
    x86_i386* cpu

#define DIV32(value) \
    [](CALLBACK_ARGUMENT) { x86_register& x86 = cpu->x86; \
        auto temp = value; \
        x86.regs[0].d = uint32_t(temp.quot); \
        x86.regs[2].d = uint32_t(temp.rem); \
    }
#define DIV64(value) \
    [](CALLBACK_ARGUMENT) { x86_register& x86 = cpu->x86; \
        auto stack32 = (uint32_t*)stack; \
        { \
            auto stack = stack32 + 1; \
            auto result = (lldiv_t*)(stack[0] + (size_t)memory); \
            (*result) = value; \
            x86.regs[0].d = uint32_t((size_t)result - (size_t)memory); \
        } \
    }
#define INT32(value) \
    [](CALLBACK_ARGUMENT) { x86_register& x86 = cpu->x86; auto* allocator = cpu->Allocator; (void)allocator; \
        auto temp = value; \
        x86.regs[0].d = uint32_t(temp); \
    }
#define INT64(value) \
    [](CALLBACK_ARGUMENT) { x86_register& x86 = cpu->x86; \
        auto temp = value; \
        x86.regs[0].d = uint32_t(temp); \
        x86.regs[2].d = uint32_t(temp >> 32); \
    }
#define FLT64(value) \
    [](CALLBACK_ARGUMENT) { x87_register& x87 = cpu->x87; \
        auto temp = value; \
        x87.sts[x87.status._TOP -= 1].d = temp; \
    }

#include "syscall_table.h"

size_t syscall_i386_new(void* data, Syscall* syscall)
{
    auto* cpu = (x86_i386*)data;
    auto* memory = cpu->Memory();

    auto printf = physical(Printf*, offset_printf);
    printf->printf = syscall->printf;
    printf->vprintf = syscall->vprintf;
    printf->debugPrintf = syscall->debugPrintf;
    printf->debugVprintf = syscall->debugVprintf;

    auto directory = physical(char*, offset_directory);
    directory[0] = 0;
#if defined(_WIN32)
    strncpy(directory, syscall->path, 260);
#else
    realpath(syscall->path, directory);
#endif

    auto commandLineA = physical(char*, offset_commandLineA);
    commandLineA[0] = 0;
    for (int i = 0; i < syscall->argc; ++i) {
        if (i) strncat(commandLineA, " ", 256);
        strncat(commandLineA, syscall->argv[i], 256);
    }

    auto commandLineW = physical(char16_t*, offset_commandLineW);
    commandLineW[0] = 0;
    for (int i = 0; i < 256; ++i) {
        commandLineW[i] = commandLineA[i];
    }

    return 0;
}

size_t syscall_i386_delete(void* data)
{
    auto* cpu = (x86_i386*)data;
    auto* memory = cpu->Memory();

    auto& record = *physical(std::vector<FILE*>*, offset_file);
    for (FILE*& file : record) {
        fclose(file);
    }
    record.clear();

    return 0;
}

size_t syscall_i386_execute(void* data, size_t index)
{
    index = uint32_t(-index - SYMBOL_INDEX);

    size_t count = sizeof(syscall_table) / sizeof(syscall_table[0]);
    if (index < count) {
        auto* cpu = (x86_i386*)data;
        auto* memory = cpu->Memory();
        auto* stack = memory + cpu->Stack();
        auto* syscall = syscall_table[index].syscall;
        auto* printf = physical(Printf*, offset_printf);
        if (printf->debugPrintf) {
            printf->debugPrintf("[CALL] %s", syscall_table[index].name);
        }
        syscall(memory, stack, cpu);
        return 0;
    }

    return SIZE_MAX;
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
