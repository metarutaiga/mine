#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "format/coff/pe.h"
#include "syscall/extend_allocator.h"
#include "syscall/syscall.h"
#include "syscall/windows/syscall_windows.h"
#include "x86/x86_ia32.h"

#define DEBUG 0

static int errlog(const char* format, ...)
{
    va_list va;
    va_start(va, format);
    int length = vfprintf(stderr, format, va);
    va_end(va);
    return length;
}

static int verrlog(const char* format, va_list va)
{
    return vfprintf(stderr, format, va);
}

static int debuglog(const char* format, ...)
{
#if DEBUG
    va_list va;
    va_start(va, format);
    int length = vprintf(format, va);
    va_end(va);
    printf("\n");
    return length;
#endif
    return 0;
}

static int vdebuglog(const char* format, va_list va)
{
#if DEBUG
    int length = vprintf(format, va);
    printf("\n");
    return length;
#endif
    return 0;
}

static size_t run_exception(mine* data, size_t index)
{
    size_t result = SIZE_MAX;
    if (result == SIZE_MAX) {
        result = syscall_windows_execute(data, index);
    }
    if (result == SIZE_MAX) {
        result = syscall_i386_execute(data, index);
    }
    return result;
}

static size_t get_symbol(const char* file, const char* name, void* symbol_data)
{
    size_t address = 0;
    if (address == 0) {
        address = syscall_windows_symbol(file, name);
    }
    if (address == 0) {
        address = syscall_i386_symbol(file, name);
    }
    return address;
}

int main(int argc, const char* argv[])
{
    if (argc <= 1) {
        printf("mine exe ...\n");
        return 0;
    }

    static const int allocator_size = 2 * 1024 * 1024;
    static const int stack_size = 1 * 1024 * 1024;

    mine* cpu = new x86_ia32;
    cpu->Initialize(extend_allocator<16>::construct(allocator_size), stack_size);
    cpu->Exception = run_exception;

    void* image = PE::Load(argv[1], [](size_t base, size_t size, void* userdata) {
        mine* cpu = (mine*)userdata;
        return cpu->Memory(base, size);
    }, cpu, debuglog);
    if (image) {
        Syscall syscall = {
            .path = ".",
            .argc = argc - 1,
            .argv = argv + 1,
            .printf = printf,
            .vprintf = vprintf,
            .errPrintf = errlog,
            .errVprintf = verrlog,
            .debugPrintf = debuglog,
            .debugVprintf = vdebuglog,
        };
        syscall_i386_new(cpu, &syscall);

        SyscallWindows syscall_windows = {
            .stack_base = allocator_size,
            .stack_limit = allocator_size - stack_size,
            .symbol = get_symbol,
            .image = (size_t)((uint8_t*)image - cpu->Memory()),
            .argc = argc - 1,
            .argv = argv + 1,
        };
        syscall_windows_new(cpu, &syscall_windows);
        syscall_windows_import(cpu, "main", image, true);

        // MEMORY-SYNC
        image = cpu->Memory() + syscall_windows.image;

        size_t entry = PE::Entry(image);
        if (entry) {
             cpu->Jump(entry);
             while (cpu->Step('LOOP'));
        }
    }

    delete cpu;

    return 0;
}
