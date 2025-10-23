#pragma once

#include "simple_allocator.h"

template<unsigned int MINBLOCK>
struct extend_allocator : public simple_allocator<MINBLOCK> {
    void(*callback_function)(allocator_t*, int, void*) = nullptr;
    void* callback_data = nullptr;
    void* allocate(size_t size, size_t hint = SIZE_MAX) noexcept override {
        for (size_t retry = 0; retry < 12; ++retry) {
            void* pointer = simple_allocator<MINBLOCK>::allocate(size, hint);
            if (pointer)
                return pointer;
            if (this->memory.size() > 0xFFFFFFFF)
                break;
            this->memory.resize(this->memory.size() * 2, 0);
            this->status.resize(this->status.size() * 2, this->FREED);
            if (callback_function) {
                callback_function(this, htonl('SYNC'), callback_data);
            }
        }
        return nullptr;
    }
    void callback(void(*function)(allocator_t*, int, void*), void* data) noexcept override {
        callback_function = function;
        callback_data = data;
    }
    static extend_allocator* construct(size_t size) {
        extend_allocator* allocator = new extend_allocator;
        if (allocator) {
            size = std::bit_ceil(size);
            allocator->memory.assign(size, 0);
            allocator->status.assign(size / MINBLOCK, allocator->FREED);
        }
        return allocator;
    }
};
