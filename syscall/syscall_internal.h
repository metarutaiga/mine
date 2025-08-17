#pragma once

#define double(value)           (*(double*)&value)
#define physical(type, index)   (type)(index ? index + memory : 0)
#define virtual(type, index)    (type)(index ? (char*)index - memory : 0)
