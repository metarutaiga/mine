#pragma once

#include <vector>
#include "miCPU.h"

struct SimpleAllocator : public allocator_t {
    enum { MINBLOCK = 16 };
    std::vector<uint8_t> memory;
    std::vector<uint8_t> status;
    void* Alloc(size_t size, size_t hint = 0) override {
        if (size == 0)
            return nullptr;
        size = (size + (MINBLOCK - 1)) & ~(MINBLOCK - 1);
        uint8_t exp = std::countr_zero(size / MINBLOCK) + 1;
        size_t pos = hint / MINBLOCK;
        for (size_t next = pos + exp; next < status.size(); next = pos + exp) {
            for (size_t i = 0; i < exp; ++i) {
                if (status[pos + 1]) {
                    pos = next;
                    break;
                }
            }
            if (pos == next)
                continue;
            memset(status.data() + pos, exp, exp);
            return memory.data() + pos * MINBLOCK;
        }
        return nullptr;
    }
    void Free(void* pointer) override {
        if (pointer == nullptr)
            return;
        size_t pos = ((uint8_t*)pointer - memory.data()) / MINBLOCK;
        uint8_t exp = status[pos];
        memset(status.data() + pos, 0, exp);
    }
    size_t Size(void* pointer) override {
        if (pointer == nullptr)
            return 0;
        size_t pos = ((uint8_t*)pointer - memory.data()) / MINBLOCK;
        uint8_t exp = status[pos];
        return (1 << (exp - 1)) * MINBLOCK;
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
            size = (1 << std::countr_zero(size));
            allocator->memory.resize(size);
            allocator->status.resize(size / MINBLOCK);
        }
        return allocator;
    }
};
