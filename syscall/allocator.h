#pragma once

#include <stdlib.h>

struct allocator_t
{
    virtual ~allocator_t() = default;
    virtual void* allocate(size_t size, size_t hint = 0) noexcept;
    virtual void deallocate(void* pointer) noexcept;
    virtual size_t size(void* pointer) const noexcept;
    virtual void* address() noexcept;
    virtual size_t max_size() const noexcept;
};
