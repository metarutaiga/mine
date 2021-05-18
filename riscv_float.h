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

#if defined(__aarch64__)
#include <arm_neon.h>
#endif

#define FP_INT_UPWARD 0
#define FP_INT_DOWNWARD 1
#define FP_INT_TOWARDZERO 2
#define FP_INT_TONEARESTFROMZERO 3
#define FP_INT_TONEAREST 4

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
#else
    intmax_t value = 0;
    int backup = fegetround();
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
    fesetround(backup);
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
    uintmax_t value = 0;
    int backup = fegetround();
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
    fesetround(backup);
    return value;
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
