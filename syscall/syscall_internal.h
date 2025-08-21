#pragma once

#define double(value)           (*(double*)&value)
#define physical(type, index)   (type)(index ? (size_t)index + (size_t)memory : 0)
#define virtual(type, index)    (type)(index ? (size_t)index - (size_t)memory : 0)
