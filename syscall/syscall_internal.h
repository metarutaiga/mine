#pragma once

#define double(value)           (*(double*)&value)
#define physical(type, index)   (type)(index ? (size_t)index + (size_t)memory : 0)
#define virtual(type, index)    (type)(index ? (size_t)index - (size_t)memory : 0)

#define offset_commandLine      0x200
#define offset_directory        0x300
