#include "simple_allocator.h"
#include "syscall.h"

uint32_t syscall_calloc(const void* stack, struct allocator_t* allocator)
{
    auto stack32 = (const uint32_t*)stack;
    uint32_t size = stack32[1] * stack32[2];
    auto memory8 = (uint8_t*)allocator->Base();
    auto pointer = (uint8_t*)allocator->Alloc(size);
    if (pointer == nullptr)
        return 0;
    memset(memory8, 0, size);
    return (uint32_t)(pointer - memory8);
}

uint32_t syscall_malloc(const void* stack, struct allocator_t* allocator)
{
    auto stack32 = (const uint32_t*)stack;
    uint32_t size = stack32[1];
    auto memory8 = (uint8_t*)allocator->Base();
    auto pointer = (uint8_t*)allocator->Alloc(size);
    if (pointer == nullptr)
        return 0;
    return (uint32_t)(pointer - memory8);
}

uint32_t syscall_realloc(const void* stack, struct allocator_t* allocator)
{
    auto stack32 = (const uint32_t*)stack;
    uint32_t pointer = stack32[1];
    uint32_t new_size = stack32[2];
    auto memory8 = (uint8_t*)allocator->Base();
    auto old_pointer = memory8 + pointer;
    auto old_size = allocator->Size(old_pointer);
    if (old_size == new_size)
        return pointer;
    auto new_pointer = (uint8_t*)allocator->Alloc(new_size);
    if (new_pointer == nullptr)
        return 0;
    if (pointer) {
        memcpy(new_pointer, old_pointer, new_size < old_size ? new_size : old_size);
        allocator->Free(old_pointer);
    }
    return (uint32_t)(new_pointer - memory8);
}

uint32_t syscall_free(const void* stack, struct allocator_t* allocator)
{
    auto stack32 = (const uint32_t*)stack;
    uint32_t pointer = stack32[1];
    auto memory8 = (uint8_t*)allocator->Base();
    allocator->Free(pointer + memory8);
    return 0;
}

uint32_t syscall_expand(const void* stack, struct allocator_t* allocator)
{
    auto stack32 = (const uint32_t*)stack;
    uint32_t pointer = stack32[1];
    uint32_t new_size = stack32[2];
    auto memory8 = (uint8_t*)allocator->Base();
    auto old_pointer = memory8 + pointer;
    auto old_size = allocator->Size(old_pointer);
    if (old_size < new_size)
        return 0;
    return pointer;
}

uint32_t syscall_msize(const void* stack, struct allocator_t* allocator)
{
    auto stack32 = (const uint32_t*)stack;
    uint32_t pointer = stack32[1];
    auto memory8 = (uint8_t*)allocator->Base();
    return (uint32_t)allocator->Size(pointer + memory8);
}

uint32_t syscall_recalloc(const void* stack, struct allocator_t* allocator)
{
    auto stack32 = (const uint32_t*)stack;
    uint32_t pointer = stack32[1];
    uint32_t new_size = stack32[2] * stack32[3];
    auto memory8 = (uint8_t*)allocator->Base();
    auto old_pointer = memory8 + pointer;
    auto old_size = allocator->Size(old_pointer);
    if (old_size == new_size)
        return pointer;
    auto new_pointer = (uint8_t*)allocator->Alloc(new_size);
    if (new_pointer == nullptr)
        return 0;
    if (pointer) {
        memcpy(new_pointer, old_pointer, new_size < old_size ? new_size : old_size);
        allocator->Free(old_pointer);
    }
    if (new_size > old_size) {
        memset(memory8 + old_size, 0, new_size - old_size);
    }
    return (uint32_t)(new_pointer - memory8);
}
