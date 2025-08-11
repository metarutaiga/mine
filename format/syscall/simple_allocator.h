#pragma once

#include <vector>
#include "miCPU.h"

template<int MINBLOCK>
struct SimpleAllocator : public allocator_t {
    enum { FREED = 0xFF };
    std::vector<uint8_t> memory;
    std::vector<uint8_t> status;
    void* Alloc(size_t size, size_t hint = SIZE_MAX) override {
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
    void Free(void* pointer) override {
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
    size_t Size(void* pointer) override {
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
    void* Base() override {
        return memory.data();
    }
    size_t Space() override {
        return memory.size();
    }
    static SimpleAllocator* Initialize(size_t size) {
        SimpleAllocator* allocator = new SimpleAllocator;
        if (allocator) {
            size = std::bit_ceil(size);
            allocator->memory.resize(size);
            allocator->status.resize(size / MINBLOCK, FREED);
        }
        return allocator;
    }
};
