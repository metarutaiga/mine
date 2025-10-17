#pragma once

#include <stdlib.h>

struct allocator_t
{
    virtual ~allocator_t() = default;
    virtual void* allocate(size_t size, size_t hint = 0) noexcept = 0;
    virtual void deallocate(void* pointer) noexcept = 0;
    virtual void* address() noexcept = 0;
    virtual size_t size(void* pointer) const noexcept = 0;
    virtual size_t max_size() const noexcept = 0;
    virtual size_t peek_size() const noexcept = 0;
    virtual size_t used_size() const noexcept = 0;
    virtual void callback(void(*function)(allocator_t*, int, void*), void* data) noexcept = 0;
};
