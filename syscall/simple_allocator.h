#pragma once

#include <vector>
#include "allocator.h"

template<int MINBLOCK>
struct simple_allocator : public allocator_t {
    enum { FREED = 0xFF };
    std::vector<uint8_t> memory;
    std::vector<uint8_t> status;
    void* allocate(size_t size, size_t hint = SIZE_MAX) noexcept override {
        if (size == 0)
            return nullptr;
        uint8_t exp = std::bit_width((size + (MINBLOCK - 1)) / MINBLOCK) - 1;
        size_t block = (1 << exp);
        if (hint == SIZE_MAX)
            hint = block * MINBLOCK;
        size_t pos = hint / MINBLOCK;
        for (size_t next = pos + block, end = status.size(); next < end; next = pos + block) {
            for (size_t i = pos; i < next; ++i) {
                if (status[i] != FREED) {
                    pos = next;
                    break;
                }
            }
            if (pos == next)
                continue;
            memset(status.data() + pos, exp, block);
            return memory.data() + pos * MINBLOCK;
        }
        return nullptr;
    }
    void deallocate(void* pointer) noexcept override {
        if (pointer == nullptr)
            return;
        size_t pos = ((uint8_t*)pointer - memory.data()) / MINBLOCK;
        if (pos >= status.size())
            return;
        uint8_t exp = status[pos];
        if (exp == FREED)
            return;
        size_t block = (1 << exp);
        memset(status.data() + pos, FREED, block);
    }
    size_t size(void* pointer) const noexcept override {
        if (pointer == nullptr)
            return 0;
        size_t pos = ((uint8_t*)pointer - memory.data()) / MINBLOCK;
        if (pos >= status.size())
            return 0;
        uint8_t exp = status[pos];
        if (exp == FREED)
            return 0;
        size_t block = (1 << exp);
        return block * MINBLOCK;
    }
    void* address() noexcept override {
        return memory.data();
    }
    size_t max_size() const noexcept override {
        return memory.size();
    }
    static simple_allocator* construct(size_t size) {
        simple_allocator* allocator = new simple_allocator;
        if (allocator) {
            size = std::bit_ceil(size);
            allocator->memory.assign(size, 0);
            allocator->status.assign(size / MINBLOCK, FREED);
        }
        return allocator;
    }
};
