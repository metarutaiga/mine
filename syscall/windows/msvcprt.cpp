#include <stdint.h>
#include <string.h>
#include "syscall/allocator.h"
#include "syscall/syscall_internal.h"

template<typename T>
struct syscall_basic_string {
    uint32_t allocator;
    union {
        T buf[16];
        uint32_t ptr;
    };
    uint32_t size;
    uint32_t res;
};

#ifdef __cplusplus
extern "C" {
#endif

static size_t syscall_basic_string_char_append(size_t thiz, char* memory, struct allocator_t* allocator, const char* other_ptr, size_t other_size)
{
    auto& local = *physical(syscall_basic_string<char>*, thiz);
    size_t size = local.size + other_size;
    if (size >= sizeof(local.buf)) {
        char* ptr = nullptr;
        if (local.res >= size) {
            ptr = physical(char*, local.ptr);
        }
        else {
            ptr = (char*)allocator->allocate(size + 1);
            if (local.res >= sizeof(local.buf)) {
                memcpy(ptr, physical(char*, local.ptr), local.size);
                allocator->deallocate(physical(char*, local.ptr));
            }
            else {
                memcpy(ptr, local.buf, local.size);
            }
            local.ptr = virtual(int, ptr);
            local.res = uint32_t(allocator->size(ptr)) - 1;
        }
        memcpy(ptr + local.size, other_ptr, other_size);
        ptr[size] = 0;
    }
    else {
        memcpy(local.buf + local.size, other_ptr, other_size);
        local.buf[size] = 0;
    }
    local.size = uint32_t(size);
    return thiz;
}

size_t syscall_basic_string_char_constructor(size_t thiz, char* memory)
{
    auto& local = *physical(syscall_basic_string<char>*, thiz);
    local.allocator = 0;
    local.buf[0] = 0;
    local.size = 0;
    local.res = sizeof(local.buf) - 1;
    return thiz;
}

size_t syscall_basic_string_char_copy_constructor(size_t thiz, char* memory, const uint32_t* stack, struct allocator_t* allocator)
{
    auto& other = *physical(syscall_basic_string<char>*, stack[1]);
    auto* other_ptr = (other.res >= sizeof(other.buf)) ? physical(char*, other.ptr) : other.buf;
    syscall_basic_string_char_constructor(thiz, memory);
    return syscall_basic_string_char_append(thiz, memory, allocator, other_ptr, other.size);
}

size_t syscall_basic_string_char_copy_range_constructor(size_t thiz, char* memory, const uint32_t* stack, struct allocator_t* allocator)
{
    auto& other = *physical(syscall_basic_string<char>*, stack[1]);
    auto pos = stack[2];
    auto len = stack[3];
    auto* other_ptr = (other.res >= sizeof(other.buf)) ? physical(char*, other.ptr) : other.buf;
    syscall_basic_string_char_constructor(thiz, memory);
    return syscall_basic_string_char_append(thiz, memory, allocator, other_ptr + pos, (len == 0xFFFFFFFF) ? other.size - pos : len);
}

size_t syscall_basic_string_char_cstr_constructor(size_t thiz, char* memory, const uint32_t* stack, struct allocator_t* allocator)
{
    auto* other_ptr = physical(char*, stack[1]);
    auto other_size = strlen(other_ptr);
    syscall_basic_string_char_constructor(thiz, memory);
    return syscall_basic_string_char_append(thiz, memory, allocator, other_ptr, other_size);
}

int syscall_basic_string_char_deconstructor(size_t thiz, char* memory, struct allocator_t* allocator)
{
    auto& local = *physical(syscall_basic_string<char>*, thiz);
    if (local.res >= sizeof(local.buf)) {
        allocator->deallocate(physical(char*, local.ptr));
    }
    return 0;
}

size_t syscall_basic_string_char_assign(size_t thiz, char* memory, const uint32_t* stack, struct allocator_t* allocator)
{
    syscall_basic_string_char_deconstructor(thiz, memory, allocator);
    return syscall_basic_string_char_copy_constructor(thiz, memory, stack, allocator);
}

size_t syscall_basic_string_char_assign_cstr(size_t thiz, char* memory, const uint32_t* stack, struct allocator_t* allocator)
{
    syscall_basic_string_char_deconstructor(thiz, memory, allocator);
    return syscall_basic_string_char_cstr_constructor(thiz, memory, stack, allocator);
}

size_t syscall_basic_string_char_at(size_t thiz, char* memory, const uint32_t* stack)
{
    auto& local = *physical(syscall_basic_string<char>*, thiz);
    auto* local_ptr = (local.res >= sizeof(local.buf)) ? physical(char*, local.ptr) : local.buf;
    return virtual(size_t, &local_ptr[stack[1]]);
}

size_t syscall_basic_string_char_append(size_t thiz, char* memory, const uint32_t* stack, struct allocator_t* allocator)
{
    auto& other = *physical(syscall_basic_string<char>*, stack[1]);
    char* other_ptr = (other.res >= sizeof(other.buf)) ? physical(char*, other.ptr) : other.buf;
    return syscall_basic_string_char_append(thiz, memory, allocator, other_ptr, other.size);
}

size_t syscall_basic_string_char_append_cstr(size_t thiz, char* memory, const uint32_t* stack, struct allocator_t* allocator)
{
    auto* other_ptr = physical(char*, stack[1]);
    auto other_size = strlen(other_ptr);
    return syscall_basic_string_char_append(thiz, memory, allocator, other_ptr, other_size);
}

size_t syscall_basic_string_char_substr(size_t thiz, char* memory, const uint32_t* stack, struct allocator_t* allocator)
{
    auto& local = *physical(syscall_basic_string<char>*, thiz);
    auto other = stack[1];
    auto pos = stack[2];
    auto len = stack[3];
    char* local_ptr = (local.res >= sizeof(local.buf)) ? physical(char*, local.ptr) : local.buf;
    syscall_basic_string_char_constructor(other, memory);
    return syscall_basic_string_char_append(other, memory, allocator, local_ptr + pos, (len == 0xFFFFFFFF) ? local.size - pos : len);
}

bool syscall_basic_string_char_eq_cstr(char* memory, const uint32_t* stack)
{
    auto& local = *physical(syscall_basic_string<char>*, stack[1]);
    auto* other_ptr = physical(char*, stack[2]);
    auto* local_ptr = (local.res >= sizeof(local.buf)) ? physical(char*, local.ptr) : local.buf;
    return strcmp(local_ptr, other_ptr) == 0;
}

bool syscall_basic_string_char_neq_cstr(char* memory, const uint32_t* stack)
{
    return syscall_basic_string_char_eq_cstr(memory, stack) == false;
}

#ifdef __cplusplus
}
#endif
