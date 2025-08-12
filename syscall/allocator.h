#pragma once

#include <stdlib.h>

struct allocator_t
{
	virtual ~allocator_t() = default;
	virtual void* Alloc(size_t size, size_t hint = 0);
	virtual void Free(void* pointer);
	virtual size_t Size(void* pointer);
	virtual void* Base();
	virtual size_t Space();
};
