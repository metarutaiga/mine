#pragma once

#include <stdarg.h>

#define double(value)           (*(double*)&value)
#define physical(type, index)   (type)((index) ? (size_t)(index) + (size_t)memory : 0)
#define virtual(type, index)    (type)((index) ? (size_t)(index) - (size_t)memory : 0)

#define offset_printf           0x100
#define offset_file             0x120
#define offset_commandLine      0x200
#define offset_directory        0x300

struct Printf {
    int(*printf)(const char*, ...);
    int(*vprintf)(const char*, va_list);
    int(*debugPrintf)(const char*, ...);
    int(*debugVprintf)(const char*, va_list);
};
