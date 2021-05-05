//==============================================================================
// MIPS(R) Architecture For Programmers Volume II-A: The MIPS64(R) Instruction Set Reference Manual
//
// Document Number: MD00087
// Revision 6.06
// December 15, 2016
//
// Copyright(C) Wave Computing, Inc. All rights reserved.
//==============================================================================

#pragma once

#include <stdlib.h>

#if defined(_MSC_VER)
#   if (_MSC_VER < 1800)
        typedef signed char         int8_t;
        typedef short               int16_t;
        typedef int                 int32_t;
        typedef long long           int64_t;
        typedef unsigned char       uint8_t;
        typedef unsigned short      uint16_t;
        typedef unsigned int        uint32_t;
        typedef unsigned long long  uint64_t;
#   else
#       include <stdint.h>
#   endif
#   include <crtdefs.h>
#   include <malloc.h>
#   include <intrin.h>
#   define UNUSED
    inline int __builtin_bswap32(int x)
    {
        return _byteswap_ulong(x);
    }
    inline long long __builtin_bswap64(long long x)
    {
        return _byteswap_uint64(x);
    }
    inline void __builtin_trap()
    {
        __debugbreak();
    }
    inline intptr_t __builtin_clzl(intptr_t bits)
    {
#   if defined(_M_AMD64)
        unsigned long r = 0;
        _BitScanReverse64(&r, bits);
        return r ^ 63;
#   else
        unsigned long r = 0;
        _BitScanReverse(&r, bits);
        return r ^ 31;
#   endif
    }

#   if (_MSC_VER < 1800)
#       include <math.h>
#       include <float.h>
        inline float roundf(float fValue)
        {
            return (value < 0.0f) ? ceilf(value - 0.5f) : floorf(value + 0.5f);
        }
#       ifndef round
            inline double round(double value)
            {
                return (value < 0.0) ? ceil(value - 0.5) : floor(value + 0.5);
            }
#       endif
        inline float truncf(float value)
        {
            return (value >= 0.0f) ? floorf(value) : ceilf(value);
        }
        inline double trunc(double value)
        {
            return (value >= 0.0) ? floor(value) : ceil(value);
        }
        inline float rintf(float value)
        {
            return roundf(value);
        }
        inline double rint(double value)
        {
            return round(value);
        }
        inline float fminf(float X, float Y)
        {
            return X < Y ? X : Y;
        }
        inline double fmin(double X, double Y)
        {
            return X < Y ? X : Y;
        }
        inline float fmaxf(float X, float Y)
        {
            return X > Y ? X : Y;
        }
        inline double fmax(double X, double Y)
        {
            return X > Y ? X : Y;
        }
#       ifndef isnan
            inline bool isnan(double value)
            {
                return _isnan(value) != 0;
            }
#       endif
        inline int fpclassify(float value)
        {
            return 0;   // TODO
        }
        inline int fpclassify(double value)
        {
            return 0;   // TODO
        }
#   endif
#else
#   include <stdint.h>
#   define UNUSED __attribute__((unused))
#   if defined(__APPLE__)
        inline void* _aligned_malloc(size_t size, size_t alignment)
        {
            void* ptr = NULL;
            posix_memalign(&ptr, size, alignment);
            return ptr;
        }
#   else
        inline void* _aligned_malloc(size_t size, size_t alignment)
        {
            return memalign(alignment, size);
        }
#   endif
        inline void _aligned_free(void* ptr)
        {
            free(ptr);
        }

#   if defined(__LP64__)
        typedef __int128 int128_t;
        typedef __uint128_t uint128_t;
        inline int64_t _mul128(int64_t a, int64_t b, int64_t* hi)
        {
            int128_t hilo = (int128_t)a * b;
            (*hi) = (int64_t)(hilo >> 64);
            return (int64_t)hilo;
        }
        inline uint64_t _umul128(uint64_t a, uint64_t b, uint64_t* hi)
        {
            uint128_t hilo = (uint128_t)a * b;
            (*hi) = (int64_t)(hilo >> 64);
            return (int64_t)hilo;
        }
#   endif
#endif

inline int32_t _mul64(int32_t a, int32_t b, int32_t* hi)
{
    int64_t hilo = (int64_t)a * b;
    (*hi) = (int32_t)(hilo >> 32);
    return (int32_t)hilo;
}
inline uint32_t _umul64(uint32_t a, uint32_t b, uint32_t* hi)
{
    uint64_t hilo =(uint64_t)a * b;
    (*hi) = (uint32_t)(hilo >> 32);
    return (uint32_t)hilo;
}
