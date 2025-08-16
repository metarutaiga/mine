#include <math.h>
#include <stdint.h>

#define double(s, i) (*(double*)(&s[i]))

double syscall_abs(const uint32_t* stack)
{
    auto x = double(stack, 1);
    return abs(x);
}

double syscall_acos(const uint32_t* stack)
{
    auto x = double(stack, 1);
    return acos(x);
}

double syscall_asin(const uint32_t* stack)
{
    auto x = double(stack, 1);
    return asin(x);
}

double syscall_atan(const uint32_t* stack)
{
    auto x = double(stack, 1);
    return atan(x);
}

double syscall_atan2(const uint32_t* stack)
{
    auto y = double(stack, 1);
    auto x = double(stack, 3);
    return atan2(y, x);
}

double syscall_ceil(const uint32_t* stack)
{
    auto x = double(stack, 1);
    return ceil(x);
}

double syscall_cos(const uint32_t* stack)
{
    auto x = double(stack, 1);
    return cos(x);
}

double syscall_cosh(const uint32_t* stack)
{
    auto x = double(stack, 1);
    return cosh(x);
}

double syscall_exp(const uint32_t* stack)
{
    auto x = double(stack, 1);
    return exp(x);
}

double syscall_fabs(const uint32_t* stack)
{
    auto x = double(stack, 1);
    return fabs(x);
}

double syscall_floor(const uint32_t* stack)
{
    auto x = double(stack, 1);
    return floor(x);
}

double syscall_fmod(const uint32_t* stack)
{
    auto numer = double(stack, 1);
    auto denom = double(stack, 3);
    return fmod(numer, denom);
}

double syscall_frexp(char* memory, const uint32_t* stack)
{
    auto x = double(stack, 1);
    auto exp = (int*)(memory + stack[3]);
    return frexp(x, exp);
}

double syscall_ldexp(const uint32_t* stack)
{
    auto x = double(stack, 1);
    auto exp = stack[3];
    return ldexp(x, exp);
}

double syscall_log(const uint32_t* stack)
{
    auto x = double(stack, 1);
    return log(x);
}

double syscall_log10(const uint32_t* stack)
{
    auto x = double(stack, 1);
    return log10(x);
}

double syscall_modf(const uint32_t* stack)
{
    auto x = double(stack, 1);
    auto intpart = &double(stack, 3);
    return modf(x, intpart);
}

double syscall_pow(const uint32_t* stack)
{
    auto base = double(stack, 1);
    auto exponent = double(stack, 3);
    return pow(base, exponent);
}

double syscall_sin(const uint32_t* stack)
{
    auto x = double(stack, 1);
    return sin(x);
}

double syscall_sinh(const uint32_t* stack)
{
    auto x = double(stack, 1);
    return sinh(x);
}

double syscall_sqrt(const uint32_t* stack)
{
    auto x = double(stack, 1);
    return sqrt(x);
}

double syscall_tan(const uint32_t* stack)
{
    auto x = double(stack, 1);
    return tan(x);
}

double syscall_tanh(const uint32_t* stack)
{
    auto x = double(stack, 1);
    return tanh(x);
}
