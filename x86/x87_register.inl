#pragma once

#define PC              x87.control._PC
#define RC              x87.control._RC
#define C0              x87.status._C0
#define C1              x87.status._C1
#define C2              x87.status._C2
#define C3              x87.status._C3
#define TOP             x87.status._TOP
#define ST(i)           x87.sts[(TOP + i) & 0b111].l
#define FPUControlWord  x87.control.w
#define FPUStatusWord   x87.status.w
#define RoundNearest    0b00
#define RoundDown       0b01
#define RoundUp         0b10
#define RoundChop       0b11
