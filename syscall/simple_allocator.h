#pragma once

#include <bit>
#include <vector>
#include "allocator.h"

template<unsigned int MINBLOCK>
struct simple_allocator : public allocator_t {
    enum { HEAD = 0x80, FREED = 0xFF };
    std::vector<uint8_t> memory;
    std::vector<uint8_t> status;
    void* allocate(size_t size, size_t hint = SIZE_MAX) noexcept override {
        if (size == 0)
            size = 1;
        uint8_t exp = std::bit_width(std::bit_ceil(size) / MINBLOCK);
        if (exp > 0)
            exp = exp - 1;
        size_t block = (size + MINBLOCK - 1) / MINBLOCK;
        size_t power_block = (1 << exp);
        size_t next_block = power_block;
        if (next_block > 1048576 / MINBLOCK)
            next_block = 1048576 / MINBLOCK;
        if (hint == SIZE_MAX)
            hint = power_block * MINBLOCK;
        size_t pos = hint / MINBLOCK;
        for (size_t next = pos + block, end = status.size(); next <= end; next = pos + block) {
            for (size_t i = pos; i < next; ++i) {
                if (status[i] != FREED) {
                    pos = next;
                    break;
                }
            }
            if (pos == next) {
                pos = pos + next_block - block;
                continue;
            }
            status[pos] = exp | HEAD;
            memset(status.data() + pos + 1, exp, block - 1);
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
        if ((exp & HEAD) == 0)
            return;
        exp = exp & ~HEAD;
        status[pos] = FREED;
        size_t block = (1 << exp);
        for (auto it = status.data() + pos + 1, end = status.data() + pos + block; it < end; ++it) {
            if ((*it) != exp)
                break;
            (*it) = FREED;
        }
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
        if ((exp & HEAD) == 0)
            return 0;
        exp = exp & ~HEAD;
        size_t count = 1;
        size_t block = (1 << exp);
        for (auto it = status.data() + pos + 1, end = status.data() + pos + block; it < end; ++it) {
            if ((*it) != exp)
                break;
            count++;
        }
        return count * MINBLOCK;
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
