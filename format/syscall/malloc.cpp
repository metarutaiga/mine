#include "simple_allocator.h"

uint32_t syscall_malloc(const void* stack, struct allocator_t* allocator)
{
    auto memory8 = (uint8_t*)allocator->Base();
    auto stack32 = (const uint32_t*)stack;
    uint32_t size = stack32[1];
    auto pointer = (uint8_t*)allocator->Alloc(size);
    if (pointer == nullptr)
        return 0;
    return (uint32_t)(pointer - memory8);
}

uint32_t syscall_realloc(const void* stack, struct allocator_t* allocator)
{
    auto memory8 = (uint8_t*)allocator->Base();
    auto stack32 = (const uint32_t*)stack;
    uint32_t pointer = stack32[1];
    uint32_t new_size = stack32[2];
    if (pointer == 0)
        return syscall_malloc(stack, allocator);
    auto new_pointer = (uint8_t*)allocator->Alloc(new_size);
    if (new_pointer == nullptr)
        return 0;
    auto old_pointer = memory8 + pointer;
    auto old_size = allocator->Size(old_pointer);
    memcpy(new_pointer, old_pointer, new_size < old_size ? new_size : old_size);
    allocator->Free(old_pointer);
    return (uint32_t)(new_pointer - memory8);
}

uint32_t syscall_free(const void* stack, struct allocator_t* allocator)
{
    auto memory8 = (uint8_t*)allocator->Base();
    auto stack32 = (const uint32_t*)stack;
    uint32_t pointer = stack32[1];
    allocator->Free(pointer + memory8);
    return 0;
}
