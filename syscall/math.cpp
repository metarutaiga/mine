#include <math.h>
#include <stdint.h>
#include "syscall_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

double syscall_acos(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return acos(x);
}

double syscall_acosh(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return acosh(x);
}

double syscall_asin(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return asin(x);
}

double syscall_asinh(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return asinh(x);
}

double syscall_atan(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return atan(x);
}

double syscall_atan2(const uint32_t* stack)
{
    auto y = double(stack[1]);
    auto x = double(stack[3]);
    return atan2(y, x);
}

double syscall_atanh(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return atanh(x);
}

double syscall_cbrt(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return cbrt(x);
}

double syscall_ceil(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return ceil(x);
}

double syscall_copysign(const uint32_t* stack)
{
    auto x = double(stack[1]);
    auto y = double(stack[3]);
    return copysign(x, y);
}

double syscall_cos(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return cos(x);
}

double syscall_cosh(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return cosh(x);
}

double syscall_erf(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return erf(x);
}

double syscall_erfc(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return erfc(x);
}

double syscall_exp(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return exp(x);
}

double syscall_exp2(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return exp2(x);
}

double syscall_expm1(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return expm1(x);
}

double syscall_fabs(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return fabs(x);
}

double syscall_fdim(const uint32_t* stack)
{
    auto x = double(stack[1]);
    auto y = double(stack[1]);
    return fdim(x, y);
}

double syscall_floor(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return floor(x);
}

double syscall_fma(const uint32_t* stack)
{
    auto x = double(stack[1]);
    auto y = double(stack[3]);
    auto z = double(stack[5]);
    return fma(x, y, z);
}

double syscall_fmax(const uint32_t* stack)
{
    auto x = double(stack[1]);
    auto y = double(stack[3]);
    return fmax(x, y);
}

double syscall_fmin(const uint32_t* stack)
{
    auto x = double(stack[1]);
    auto y = double(stack[3]);
    return fmin(x, y);
}

double syscall_fmod(const uint32_t* stack)
{
    auto numer = double(stack[1]);
    auto denom = double(stack[3]);
    return fmod(numer, denom);
}

int syscall_fpclassify(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return fpclassify(x);
}

double syscall_hypot(const uint32_t* stack)
{
    auto x = double(stack[1]);
    auto y = double(stack[3]);
    return hypot(x, y);
}

int syscall_ilogb(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return ilogb(x);
}

int syscall_isfinite(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return isfinite(x);
}

int syscall_isgreater(const uint32_t* stack)
{
    auto x = double(stack[1]);
    auto y = double(stack[3]);
    return isgreater(x, y);
}

int syscall_isgreaterequal(const uint32_t* stack)
{
    auto x = double(stack[1]);
    auto y = double(stack[3]);
    return isgreaterequal(x, y);
}

int syscall_isinf(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return isinf(x);
}

int syscall_isless(const uint32_t* stack)
{
    auto x = double(stack[1]);
    auto y = double(stack[3]);
    return isless(x, y);
}

int syscall_islessequal(const uint32_t* stack)
{
    auto x = double(stack[1]);
    auto y = double(stack[3]);
    return islessequal(x, y);
}

int syscall_islessgreater(const uint32_t* stack)
{
    auto x = double(stack[1]);
    auto y = double(stack[3]);
    return islessgreater(x, y);
}

int syscall_isnan(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return isnan(x);
}

int syscall_isnormal(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return isnormal(x);
}

int syscall_isunordered(const uint32_t* stack)
{
    auto x = double(stack[1]);
    auto y = double(stack[3]);
    return isunordered(x, y);
}

double syscall_frexp(char* memory, const uint32_t* stack)
{
    auto x = double(stack[1]);
    auto exp = physical(int*, stack[3]);
    return frexp(x, exp);
}

double syscall_ldexp(const uint32_t* stack)
{
    auto x = double(stack[1]);
    auto exp = stack[3];
    return ldexp(x, exp);
}

double syscall_lgamma(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return lgamma(x);
}

long long syscall_llrint(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return llrint(x);
}

long long syscall_llround(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return llround(x);
}

double syscall_log(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return log(x);
}

double syscall_log10(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return log10(x);
}

double syscall_log1p(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return log1p(x);
}

double syscall_log2(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return log2(x);
}

double syscall_logb(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return logb(x);
}

long syscall_lrint(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return lrint(x);
}

long syscall_lround(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return lround(x);
}

double syscall_modf(char* memory, const uint32_t* stack)
{
    auto x = double(stack[1]);
    auto intpart = physical(double*, stack[3]);
    return modf(x, intpart);
}

double syscall_nan(char* memory, const uint32_t* stack)
{
    auto tagp = physical(char*, stack[1]);
    return nan(tagp);
}

float syscall_nanf(char* memory, const uint32_t* stack)
{
    auto tagp = physical(char*, stack[1]);
    return nanf(tagp);
}

long double syscall_nanl(char* memory, const uint32_t* stack)
{
    auto tagp = physical(char*, stack[1]);
    return nanl(tagp);
}

double syscall_nearbyint(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return nearbyint(x);
}

double syscall_nextafter(const uint32_t* stack)
{
    auto x = double(stack[1]);
    auto y = double(stack[3]);
    return nextafter(x, y);
}

double syscall_nexttoward(const uint32_t* stack)
{
    auto x = double(stack[1]);
    auto y = double(stack[3]);
    return nexttoward(x, y);
}

double syscall_pow(const uint32_t* stack)
{
    auto base = double(stack[1]);
    auto exponent = double(stack[3]);
    return pow(base, exponent);
}

double syscall_remainder(const uint32_t* stack)
{
    auto numer = double(stack[1]);
    auto denom = double(stack[3]);
    return remainder(numer, denom);
}

double syscall_remquo(char* memory, const uint32_t* stack)
{
    auto numer = double(stack[1]);
    auto denom = double(stack[3]);
    auto quot = physical(int*, stack[5]);
    return remquo(numer, denom, quot);
}

double syscall_rint(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return rint(x);
}

double syscall_round(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return round(x);
}

double syscall_scalbln(const uint32_t* stack)
{
    auto x = double(stack[1]);
    auto n = stack[3];
    return scalbln(x, n);
}

double syscall_scalbn(const uint32_t* stack)
{
    auto x = double(stack[1]);
    auto n = stack[3];
    return scalbn(x, n);
}

int syscall_signbit(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return signbit(x);
}

double syscall_sin(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return sin(x);
}

double syscall_sinh(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return sinh(x);
}

double syscall_sqrt(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return sqrt(x);
}

double syscall_tan(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return tan(x);
}

double syscall_tanh(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return tanh(x);
}

double syscall_tgamma(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return tgamma(x);
}

double syscall_trunc(const uint32_t* stack)
{
    auto x = double(stack[1]);
    return trunc(x);
}

#ifdef __cplusplus
}
#endif
