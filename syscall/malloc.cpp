#include <string.h>
#include "allocator.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t syscall_calloc(const uint32_t* stack, struct allocator_t* allocator)
{
    uint32_t size = stack[1] * stack[2];
    auto memory = (uint8_t*)allocator->Base();
    auto pointer = (uint8_t*)allocator->Alloc(size);
    if (pointer == nullptr)
        return 0;
    memset(memory, 0, size);
    return (uint32_t)(pointer - memory);
}

uint32_t syscall_malloc(const uint32_t* stack, struct allocator_t* allocator)
{
    uint32_t size = stack[1];
    auto memory = (uint8_t*)allocator->Base();
    auto pointer = (uint8_t*)allocator->Alloc(size);
    if (pointer == nullptr)
        return 0;
    return (uint32_t)(pointer - memory);
}

uint32_t syscall_realloc(const uint32_t* stack, struct allocator_t* allocator)
{
    uint32_t pointer = stack[1];
    uint32_t new_size = stack[2];
    auto memory = (uint8_t*)allocator->Base();
    auto old_pointer = memory + pointer;
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
    return (uint32_t)(new_pointer - memory);
}

uint32_t syscall_free(const uint32_t* stack, struct allocator_t* allocator)
{
    uint32_t pointer = stack[1];
    auto memory = (uint8_t*)allocator->Base();
    allocator->Free(pointer + memory);
    return 0;
}

uint32_t syscall_expand(const uint32_t* stack, struct allocator_t* allocator)
{
    uint32_t pointer = stack[1];
    uint32_t new_size = stack[2];
    auto memory = (uint8_t*)allocator->Base();
    auto old_pointer = memory + pointer;
    auto old_size = allocator->Size(old_pointer);
    if (old_size < new_size)
        return 0;
    return pointer;
}

uint32_t syscall_msize(const uint32_t* stack, struct allocator_t* allocator)
{
    uint32_t pointer = stack[1];
    auto memory = (uint8_t*)allocator->Base();
    return (uint32_t)allocator->Size(pointer + memory);
}

uint32_t syscall_recalloc(const uint32_t* stack, struct allocator_t* allocator)
{
    uint32_t pointer = stack[1];
    uint32_t new_size = stack[2] * stack[3];
    auto memory = (uint8_t*)allocator->Base();
    auto old_pointer = memory + pointer;
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
        memset(memory + old_size, 0, new_size - old_size);
    }
    return (uint32_t)(new_pointer - memory);
}

#ifdef __cplusplus
}
#endif
