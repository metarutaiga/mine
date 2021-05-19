//==============================================================================
// The RISC-V Instruction Set Manual
// Volume I: Unprivileged ISA
// Document Version 20191213
// December 13, 2019
//==============================================================================

#pragma once

#include <fenv.h>
#include <float.h>
#include <math.h>
#include <stdint.h>

#ifndef FP_INT_UPWARD

#if defined(__aarch64__)
#include <arm_neon.h>
#elif defined(__SSE2__) || defined(__SSE4_1__)
#include <immintrin.h>
#endif

#define FP_INT_UPWARD               0
#define FP_INT_DOWNWARD             1
#define FP_INT_TOWARDZERO           2
#define FP_INT_TONEARESTFROMZERO    3
#define FP_INT_TONEAREST            4

inline intmax_t fromfp(double x, int round, unsigned int width)
{
#if defined(__aarch64__)
    switch (round)
    {
    case FP_INT_UPWARD:             return vcvtpd_s64_f64(x);
    case FP_INT_DOWNWARD:           return vcvtmd_s64_f64(x);
    case FP_INT_TOWARDZERO:         return vcvt_s64_f64((float64x1_t&)x)[0];
    case FP_INT_TONEARESTFROMZERO:  return vcvtnd_s64_f64(x);
    case FP_INT_TONEAREST:
    default:                        return vcvtad_s64_f64(x);
    }
#elif defined(__SSE2__)
    int mode = _MM_GET_ROUNDING_MODE();
    switch (round)
    {
    case FP_INT_UPWARD:             _MM_SET_ROUNDING_MODE(_MM_ROUND_UP);            break;
    case FP_INT_DOWNWARD:           _MM_SET_ROUNDING_MODE(_MM_ROUND_DOWN);          break;
    case FP_INT_TOWARDZERO:         _MM_SET_ROUNDING_MODE(_MM_ROUND_TOWARD_ZERO);   break;
    case FP_INT_TONEARESTFROMZERO:  _MM_SET_ROUNDING_MODE(_MM_ROUND_TOWARD_ZERO);   x += 0.5;   break;
    case FP_INT_TONEAREST:
    default:                        _MM_SET_ROUNDING_MODE(_MM_ROUND_NEAREST);       break;
    }
    intmax_t value = _mm_cvtsd_si64(_mm_set_sd(x));
    int mxcsr = (_mm_getcsr() & ~_MM_ROUND_MASK) | mode;
    __asm __volatile("ldmxcsr %0" : : "m" (mxcsr));
    return value;
#elif defined(__SSE4_1__) && 0
    switch (round)
    {
    case FP_INT_UPWARD:             return _mm_cvttsd_si64(_mm_round_sd(_mm_set_sd(x), __m128d(), _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC));
    case FP_INT_DOWNWARD:           return _mm_cvttsd_si64(_mm_round_sd(_mm_set_sd(x), __m128d(), _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC));
    case FP_INT_TOWARDZERO:         return _mm_cvttsd_si64(_mm_round_sd(_mm_set_sd(x), __m128d(), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));
    case FP_INT_TONEARESTFROMZERO:  return _mm_cvttsd_si64(_mm_round_sd(_mm_set_sd(x + 0.5), __m128d(), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));
    case FP_INT_TONEAREST:
    default:                        return _mm_cvttsd_si64(_mm_round_sd(_mm_set_sd(x), __m128d(), _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC));
    }
#else
    intmax_t value = 0;
    int mode = fegetround();
    switch (round)
    {
    case FP_INT_UPWARD:             fesetround(FE_UPWARD);      break;
    case FP_INT_DOWNWARD:           fesetround(FE_DOWNWARD);    break;
    case FP_INT_TOWARDZERO:         fesetround(FE_TOWARDZERO);  break;
    case FP_INT_TONEARESTFROMZERO:  fesetround(FE_TOWARDZERO);  x += 0.5;   break;
    case FP_INT_TONEAREST:
    default:                        fesetround(FE_TONEAREST);   break;
    }
    value = lrint(x);
    fesetround(mode);
    return value;
#endif
}
inline uintmax_t ufromfp(double x, int round, unsigned int width)
{
#if defined(__aarch64__)
    switch (round)
    {
    case FP_INT_UPWARD:             return vcvtpd_u64_f64(x);
    case FP_INT_DOWNWARD:           return vcvtmd_u64_f64(x);
    case FP_INT_TOWARDZERO:         return vcvt_u64_f64((float64x1_t&)x)[0];
    case FP_INT_TONEARESTFROMZERO:  return vcvtnd_u64_f64(x);
    case FP_INT_TONEAREST:
    default:                        return vcvtad_u64_f64(x);
    }
#else
    if (x > INT64_MAX)
    {
        return fromfp(x + INT64_MIN, round, width) - INT64_MIN;
    }
    return fromfp(x, round, width);
#endif
}
inline intmax_t fromfpf(float x, int round, unsigned int width)
{
    return fromfp(x, round, width);
}
inline uintmax_t ufromfpf(float x, int round, unsigned int width)
{
    return ufromfp(x, round, width);
}

#endif
