int printf(const char* format, ...);

__attribute__((noinline))
void dump_flags(int flags, const char* name)
{
    printf("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s %04X %s\n",
        flags & 0b1000000000000000 ? "1" : ".",
        flags & 0b0100000000000000 ? "1" : ".",
        flags & 0b0010000000000000 ? "1" : ".",
        flags & 0b0001000000000000 ? "1" : ".",
        flags & 0b0000100000000000 ? "1" : ".",
        flags & 0b0000010000000000 ? "1" : ".",
        flags & 0b0000001000000000 ? "1" : ".",
        flags & 0b0000000100000000 ? "1" : ".",
        flags & 0b0000000010000000 ? "1" : ".",
        flags & 0b0000000001000000 ? "1" : ".",
        flags & 0b0000000000100000 ? "1" : ".",
        flags & 0b0000000000010000 ? "1" : ".",
        flags & 0b0000000000001000 ? "1" : ".",
        flags & 0b0000000000000100 ? "1" : ".",
        flags & 0b0000000000000010 ? "1" : ".",
        flags & 0b0000000000000001 ? "1" : ".", flags, name);
}

int adc(int a, int b)
{
    int flags;
    __asm__ __volatile__("stc\n"
                         "adcl %[a], %[b]\n"
                         "pushfd\n"
                         "pop %[flags]\n"
                         : [flags]"=r"(flags)
                         : [a]"r"(a), [b]"r"(b)
                         : "cc");
    return flags;
}

int add(int a, int b)
{
    int flags;
    __asm__ __volatile__("addl %[a], %[b]\n"
                         "pushfd\n"
                         "pop %[flags]\n"
                         : [flags]"=r"(flags)
                         : [a]"r"(a), [b]"r"(b)
                         : "cc");
    return flags;
}

int dec(int a)
{
    int flags;
    __asm__ __volatile__("dec %[a]\n"
                         "pushfd\n"
                         "pop %[flags]\n"
                         : [flags]"=r"(flags)
                         : [a]"r"(a)
                         : "cc");
    return flags;
}

int imul(int a, int b)
{
    int flags;
    __asm__ __volatile__("imul %[a], %[b]\n"
                         "pushfd\n"
                         "pop %[flags]\n"
                         : [flags]"=r"(flags)
                         : [a]"r"(a), [b]"r"(b)
                         : "cc");
    return flags;
}

int inc(int a)
{
    int flags;
    __asm__ __volatile__("inc %[a]\n"
                         "pushfd\n"
                         "pop %[flags]\n"
                         : [flags]"=r"(flags)
                         : [a]"r"(a)
                         : "cc");
    return flags;
}

int mul(int a, int b)
{
    int flags;
    __asm__ __volatile__("mov %%eax, %[a]\n"
                         "mul %[b]\n"
                         "pushfd\n"
                         "pop %[flags]\n"
                         : [flags]"=r"(flags)
                         : [a]"r"(a), [b]"r"(b)
                         : "cc");
    return flags;
}

int neg(int a)
{
    int flags;
    __asm__ __volatile__("neg %[a]\n"
                         "pushfd\n"
                         "pop %[flags]\n"
                         : [flags]"=r"(flags)
                         : [a]"r"(a)
                         : "cc");
    return flags;
}

int sbb(int a, int b)
{
    int flags;
    __asm__ __volatile__("stc\n"
                         "sbbl %[a], %[b]\n"
                         "pushfd\n"
                         "pop %[flags]\n"
                         : [flags]"=r"(flags)
                         : [a]"r"(a), [b]"r"(b)
                         : "cc");
    return flags;
}

int sub(int a, int b)
{
    int flags;
    __asm__ __volatile__("subl %[a], %[b]\n"
                         "pushfd\n"
                         "pop %[flags]\n"
                         : [flags]"=r"(flags)
                         : [a]"r"(a), [b]"r"(b)
                         : "cc");
    return flags;
}

int main(int argc, const char* argv[])
{
    printf("_N__ODITSZ_A_P_C\n");
    dump_flags(adc(0x22222222, 0x22222222), "ADC");
    dump_flags(adc(0x22222222, 0x88888888), "ADC");
    dump_flags(adc(0x88888888, 0x22222222), "ADC");
    dump_flags(adc(0x88888888, 0x88888888), "ADC");
    dump_flags(adc(0xFFFFFFFF, 0x00000000), "ADC");
    dump_flags(add(0x22222222, 0x22222222), "ADD");
    dump_flags(add(0x22222222, 0x88888888), "ADD");
    dump_flags(add(0x88888888, 0x22222222), "ADD");
    dump_flags(add(0x88888888, 0x88888888), "ADD");
    dump_flags(dec(0x00000000), "DEC");
    dump_flags(dec(0x20000000), "DEC");
    dump_flags(dec(0x80000000), "DEC");
    dump_flags(imul(0x22222222, -2), "IMUL");
    dump_flags(imul(0x22222222, -8), "IMUL");
    dump_flags(imul(0x88888888,  2), "IMUL");
    dump_flags(imul(0x88888888,  8), "IMUL");
    dump_flags(imul(0x88888888, -2), "IMUL");
    dump_flags(imul(0x88888888, -8), "IMUL");
    dump_flags(dec(0x20000000), "INC");
    dump_flags(dec(0x80000000), "INC");
    dump_flags(dec(0xFFFFFFFF), "INC");
    dump_flags(mul(0x22222222, 2), "MUL");
    dump_flags(mul(0x88888888, 8), "MUL");
    dump_flags(neg(0x00000000), "NEG");
    dump_flags(neg(0x22222222), "NEG");
    dump_flags(neg(0x88888888), "NEG");
    dump_flags(sbb(0x22222222, 0x22222222), "SBB");
    dump_flags(sbb(0x22222222, 0x88888888), "SBB");
    dump_flags(sbb(0x88888888, 0x22222222), "SBB");
    dump_flags(sbb(0x88888888, 0x88888888), "SBB");
    dump_flags(sbb(0x00000000, 0xFFFFFFFF), "SBB");
    dump_flags(add(0x22222222, 0x22222222), "SUB");
    dump_flags(add(0x22222222, 0x88888888), "SUB");
    dump_flags(add(0x88888888, 0x22222222), "SUB");
    dump_flags(add(0x88888888, 0x88888888), "SUB");
    return 0;
}
