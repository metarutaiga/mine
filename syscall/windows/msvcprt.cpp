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

int syscall_basic_string_char_copy_constructor(uint32_t thiz, uint8_t* memory, const uint32_t* stack, struct allocator_t* allocator) {
    auto& local = *physical(syscall_basic_string<char>*, thiz);
    auto& other = *physical(syscall_basic_string<char>*, stack[1]);
    local.allocator = other.allocator;
    local.size = other.size;
    local.res = other.res;
    if (local.res >= sizeof(local.buf)) {
        auto ptr = allocator->allocate(local.res + 1);
        memcpy(ptr, physical(char*, other.ptr), local.res + 1);
        local.ptr = virtual(int, ptr);
    }
    else {
        memcpy(local.buf, other.buf, sizeof(local.buf));
    }
    return thiz;
}

int syscall_basic_string_char_cstr_constructor(uint32_t thiz, uint8_t* memory, const uint32_t* stack, struct allocator_t* allocator) {
    auto& local = *physical(syscall_basic_string<char>*, thiz);
    auto* other = physical(char*, stack[1]);
    local.size = local.res = (uint32_t)strlen(other);
    if (local.size >= sizeof(local.buf)) {
        auto ptr = allocator->allocate(local.size + 1);
        memcpy(ptr, other, local.size + 1);
        local.ptr = virtual(int, ptr);
    }
    else {
        memcpy(local.buf, other, sizeof(local.buf));
    }
    return thiz;
}

int syscall_basic_string_char_deconstructor(uint32_t thiz, uint8_t* memory, struct allocator_t* allocator) {
    auto& local = *physical(syscall_basic_string<char>*, thiz);
    if (local.size >= sizeof(local.buf)) {
        allocator->deallocate(physical(char*, local.ptr));
    }
    return 0;
}

#ifdef __cplusplus
}
#endif
