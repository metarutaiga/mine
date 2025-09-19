#include <stdio.h>
#include <stdint.h>
#include "format/coff/pe.h"
#include "syscall/simple_allocator.h"
#include "syscall/syscall.h"
#include "syscall/windows/syscall_windows.h"
#include "x86/x86_i386.h"

static int syslog(const char* format, ...)
{
    return 0;
}

static int vsyslog(const char* format, va_list)
{
    return 0;
}

static size_t run_exception(mine* data, size_t index)
{
    size_t result = 0;
    if (result == 0) {
        result = syscall_windows_execute(data, index);
    }
    if (result == 0) {
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
        printf("micpu exe ...\n");
        return 0;
    }

    static const int allocatorSize = 16777216;
    static const int stackSize = 65536;

    mine* cpu = new x86_i386;
    cpu->Initialize(simple_allocator<16>::construct(allocator_size), stack_size);
    cpu->Exception = run_exception;

    void* image = PE::Load(argv[1], [](size_t base, size_t size, void* userdata) {
        mine* cpu = (mine*)userdata;
        return cpu->Memory(base, size);
    }, cpu, syslog);
    if (image) {
        Syscall syscall = {
            .path = ".",
            .argc = argc - 1,
            .argv = argv + 1,
            .printf = printf,
            .vprintf = vprintf,
            .debugPrintf = syslog,
            .debugVprintf = vsyslog,
        };
        syscall_i386_new(cpu, &syscall);

        SyscallWindows syscall_windows = {
            .stack_base = allocator_size,
            .stack_limit = allocator_size - stack_size,
            .image = image,
            .symbol = get_symbol,
            .argc = argc - 1,
            .argv = argv + 1,
        };
        syscall_windows_new(cpu, &syscall_windows);
        syscall_windows_import(cpu, "main", image, true);

        size_t entry = PE::Entry(image);
        if (entry) {
             cpu->Jump(entry);
             cpu->Step('LOOP');
        }
    }

    delete cpu;

    return 0;
}
