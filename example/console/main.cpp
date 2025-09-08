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

static size_t run_exception(miCPU* data, size_t index)
{
    size_t result = 0;
    if (result == 0) {
        result = syscall_windows_execute(data, index, vsyslog, vprintf);
    }
    if (result == 0) {
        result = syscall_i386_execute(data, index, vsyslog, vprintf);
    }
    return result;
}

static size_t get_symbol(const char* file, const char* name)
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

    miCPU* cpu = new x86_i386;
    cpu->Initialize(simple_allocator<16>::construct(allocator_size), stack_size);
    cpu->Exception = run_exception;

    void* image = PE::Load(argv[1], [](size_t base, size_t size, void* userdata) {
        miCPU* cpu = (miCPU*)userdata;
        return cpu->Memory(base, size);
    }, cpu, syslog);
    if (image) {
        size_t stack_base = allocator_size;
        size_t stack_limit = allocator_size - stack_size;
        syscall_windows_new(cpu, stack_base, stack_limit, ".", image, argc - 1, argv + 1, 0, nullptr);

        PE::Imports(image, get_symbol, syslog);

        size_t entry = PE::Entry(image);
        if (entry) {
             cpu->Jump(entry);
             cpu->Run();
        }
    }

    delete cpu;

    return 0;
}
