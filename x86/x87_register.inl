#pragma once

#define PC              x87.control._PC
#define RC              x87.control._RC
#define TOP             x87.status._TOP
#define ST(i)           x87.sts[(TOP + i) & 0b111].l
#define FPUControlWord  x87.control.w
#define FPUStatusWord   x87.status.w
