#pragma once

#include <stdint.h>
#undef _C2

struct x87_register
{
    union register_t
    {
        long double l;
        double      d;
        float       f;
    };
    union control_t
    {
        uint16_t w;
        struct
        {
            uint16_t _IM:1; // INVALID OPERATION MARK
            uint16_t _DM:1; // DENORMALIZED OPERAND MARK
            uint16_t _ZM:1; // ZERO DIVIDE MARK
            uint16_t _OM:1; // OVERFLOW MARK
            uint16_t _UM:1; // UNDERFLOW MARK
            uint16_t _PM:1; // PRECISION MARK
            uint16_t _06:1;
            uint16_t _07:1;
            uint16_t _PC:2; // PRECISION CONTROL
            uint16_t _RC:2; // ROUNDING CONTROL
            uint16_t _X:1;  // INFINITY CONTORL
        };
    };
    union status_t
    {
        uint16_t w;
        struct
        {
            uint16_t _IE:1; // INVALID OPERATION EXCEPTION
            uint16_t _DE:1; // DENORMALIZED OPERAND EXCEPTION
            uint16_t _ZE:1; // ZERO DIVIDE EXCEPTION
            uint16_t _OE:1; // OVERFLOW EXCEPTION
            uint16_t _UE:1; // UNDERFLOW EXCEPTION
            uint16_t _PE:1; // PRECISION EXCEPTION
            uint16_t _SF:1; // STACK FAULT
            uint16_t _ES:1; // ERROR SUMMARY STATUS
            uint16_t _C0:1; // CONDITION CODE 0
            uint16_t _C1:1; // CONDITION CODE 1
            uint16_t _C2:1; // CONDITION CODE 2
            uint16_t _TOP:3;// TOP OF STACK POINTER
            uint16_t _C3:1; // CONDITION CODE 3
            uint16_t _B:1;  // BUSY
        };
    };
    register_t sts[8] = {};
    control_t control = {};
    status_t status = {};
};
