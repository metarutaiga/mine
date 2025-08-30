__declspec(dllexport) void f2xm1()
{
    __asm__(".intel_syntax\n"
            "f2xm1\n");
}

__declspec(dllexport) void fabs()
{
    __asm__(".intel_syntax\n"
            "fabs\n");
}

__declspec(dllexport) void fadd()
{
    __asm__(".intel_syntax\n"
            "fadd dword ptr [ecx*4+edx+0x55667788]\n"
            "fadd qword ptr [ecx*4+edx+0x55667788]\n"
            "fadd st(0), st(2)\n"
            "fadd st(2), st(0)\n"
            "faddp st(2), st(0)\n"
            "faddp\n");
}

__declspec(dllexport) void fbld()
{
    __asm__(".intel_syntax\n"
            "fbld tbyte ptr [ecx*4+edx+0x55667788]\n");
}

__declspec(dllexport) void fbstp()
{
    __asm__(".intel_syntax\n"
            "fbstp tbyte ptr [ecx*4+edx+0x55667788]\n");
}

__declspec(dllexport) void fchs()
{
    __asm__(".intel_syntax\n"
            "fchs\n");
}

__declspec(dllexport) void fclex()
{
    __asm__(".intel_syntax\n"
            "fclex\n");
}

__declspec(dllexport) void fcom()
{
    __asm__(".intel_syntax\n"
            "fcom dword ptr [ecx*4+edx+0x55667788]\n"
            "fcom qword ptr [ecx*4+edx+0x55667788]\n"
            "fcom st(2)\n"
            "fcom\n"
            "fcomp dword ptr [ecx*4+edx+0x55667788]\n"
            "fcomp qword ptr [ecx*4+edx+0x55667788]\n"
            "fcomp st(2)\n"
            "fcomp\n"
            "fcompp\n");
}

__declspec(dllexport) void fcos()
{
    __asm__(".intel_syntax\n"
            "fcos\n");
}

__declspec(dllexport) void fdecstp()
{
    __asm__(".intel_syntax\n"
            "fdecstp\n");
}

__declspec(dllexport) void fdiv()
{
    __asm__(".intel_syntax\n"
            "fdiv dword ptr [ecx*4+edx+0x55667788]\n"
            "fdiv qword ptr [ecx*4+edx+0x55667788]\n"
            "fdiv st(0), st(2)\n"
            "fdiv st(2), st(0)\n"
            "fdivp st(2), st(0)\n"
            "fdivp\n");
}

__declspec(dllexport) void fdivr()
{
    __asm__(".intel_syntax\n"
            "fdivr dword ptr [ecx*4+edx+0x55667788]\n"
            "fdivr qword ptr [ecx*4+edx+0x55667788]\n"
            "fdivr st(0), st(2)\n"
            "fdivr st(2), st(0)\n"
            "fdivrp st(2), st(0)\n"
            "fdivrp\n");
}

__declspec(dllexport) void ffree()
{
    __asm__(".intel_syntax\n"
            "ffree st(2)\n");
}

__declspec(dllexport) void fiadd()
{
    __asm__(".intel_syntax\n"
            "fiadd word ptr [ecx*4+edx+0x55667788]\n"
            "fiadd dword ptr [ecx*4+edx+0x55667788]\n");
}

__declspec(dllexport) void ficom()
{
    __asm__(".intel_syntax\n"
            "ficom word ptr [ecx*4+edx+0x55667788]\n"
            "ficom dword ptr [ecx*4+edx+0x55667788]\n"
            "ficomp word ptr [ecx*4+edx+0x55667788]\n"
            "ficomp dword ptr [ecx*4+edx+0x55667788]\n");
}

__declspec(dllexport) void fidiv()
{
    __asm__(".intel_syntax\n"
            "fidiv word ptr [ecx*4+edx+0x55667788]\n"
            "fidiv dword ptr [ecx*4+edx+0x55667788]\n");
}

__declspec(dllexport) void fidivr()
{
    __asm__(".intel_syntax\n"
            "fidivr word ptr [ecx*4+edx+0x55667788]\n"
            "fidivr dword ptr [ecx*4+edx+0x55667788]\n");
}

__declspec(dllexport) void fild()
{
    __asm__(".intel_syntax\n"
            "fild word ptr [ecx*4+edx+0x55667788]\n"
            "fild dword ptr [ecx*4+edx+0x55667788]\n");
}

__declspec(dllexport) void fimul()
{
    __asm__(".intel_syntax\n"
            "fimul word ptr [ecx*4+edx+0x55667788]\n"
            "fimul dword ptr [ecx*4+edx+0x55667788]\n");
}

__declspec(dllexport) void fincstp()
{
    __asm__(".intel_syntax\n"
            "fincstp\n");
}

__declspec(dllexport) void finit()
{
    __asm__(".intel_syntax\n"
            "finit\n");
}

__declspec(dllexport) void fist()
{
    __asm__(".intel_syntax\n"
            "fist word ptr [ecx*4+edx+0x55667788]\n"
            "fist dword ptr [ecx*4+edx+0x55667788]\n"
            "fistp word ptr [ecx*4+edx+0x55667788]\n"
            "fistp dword ptr [ecx*4+edx+0x55667788]\n"
            "fistp qword ptr [ecx*4+edx+0x55667788]\n");
}

__declspec(dllexport) void fisub()
{
    __asm__(".intel_syntax\n"
            "fisub word ptr [ecx*4+edx+0x55667788]\n"
            "fisub dword ptr [ecx*4+edx+0x55667788]\n");
}

__declspec(dllexport) void fisubr()
{
    __asm__(".intel_syntax\n"
            "fisubr word ptr [ecx*4+edx+0x55667788]\n"
            "fisubr dword ptr [ecx*4+edx+0x55667788]\n");
}

__declspec(dllexport) void fld()
{
    __asm__(".intel_syntax\n"
            "fld dword ptr [0x55667788]\n"
            "fld qword ptr [0x55667788]\n"
            "fld tbyte ptr [0x55667788]\n"
            "fld dword ptr [ecx*4+edx+0x55667788]\n"
            "fld qword ptr [ecx*4+edx+0x55667788]\n"
            "fld tbyte ptr [ecx*4+edx+0x55667788]\n"
            "fld st(2)\n");
}

__declspec(dllexport) void fld1_fldl2e_fldl2t_fldlg2_fldln2_fldpi_fldz()
{
    __asm__(".intel_syntax\n"
            "fld1\n"
            "fldl2e\n"
            "fldl2t\n"
            "fldlg2\n"
            "fldln2\n"
            "fldpi\n"
            "fldz\n");
}

__declspec(dllexport) void fldcw()
{
    __asm__(".intel_syntax\n"
            "fldcw word ptr [ecx*4+edx+0x55667788]\n");
}

__declspec(dllexport) void fmul()
{
    __asm__(".intel_syntax\n"
            "fmul dword ptr [ecx*4+edx+0x55667788]\n"
            "fmul qword ptr [ecx*4+edx+0x55667788]\n"
            "fmul st(0), st(2)\n"
            "fmul st(2), st(0)\n"
            "fmulp st(2), st(0)\n"
            "fmulp\n");
}

__declspec(dllexport) void fnop()
{
    __asm__(".intel_syntax\n"
            "fnop\n");
}

__declspec(dllexport) void fpatan()
{
    __asm__(".intel_syntax\n"
            "fpatan\n");
}

__declspec(dllexport) void fprem()
{
    __asm__(".intel_syntax\n"
            "fprem\n");
}

__declspec(dllexport) void fprem1()
{
    __asm__(".intel_syntax\n"
            "fprem1\n");
}

__declspec(dllexport) void fptan()
{
    __asm__(".intel_syntax\n"
            "fptan\n");
}

__declspec(dllexport) void frndint()
{
    __asm__(".intel_syntax\n"
            "frndint\n");
}

__declspec(dllexport) void fscale()
{
    __asm__(".intel_syntax\n"
            "fscale\n");
}

__declspec(dllexport) void fsin()
{
    __asm__(".intel_syntax\n"
            "fsin\n");
}

__declspec(dllexport) void fsincos()
{
    __asm__(".intel_syntax\n"
            "fsincos\n");
}

__declspec(dllexport) void fsqrt()
{
    __asm__(".intel_syntax\n"
            "fsqrt\n");
}

__declspec(dllexport) void fst()
{
    __asm__(".intel_syntax\n"
            "fst dword ptr [ecx*4+edx+0x55667788]\n"
            "fst qword ptr [ecx*4+edx+0x55667788]\n"
            "fst st(2)\n"
            "fstp dword ptr [ecx*4+edx+0x55667788]\n"
            "fstp qword ptr [ecx*4+edx+0x55667788]\n"
            "fstp tbyte ptr [ecx*4+edx+0x55667788]\n"
            "fstp st(2)\n");
}

__declspec(dllexport) void fstcw()
{
    __asm__(".intel_syntax\n"
            "fstcw word ptr [ecx*4+edx+0x55667788]\n");
}

__declspec(dllexport) void fstsw()
{
    __asm__(".intel_syntax\n"
            "fstsw word ptr [ecx*4+edx+0x55667788]\n"
            "fstsw ax\n");
}

__declspec(dllexport) void fsub()
{
    __asm__(".intel_syntax\n"
            "fsub dword ptr [ecx*4+edx+0x55667788]\n"
            "fsub qword ptr [ecx*4+edx+0x55667788]\n"
            "fsub st(0), st(2)\n"
            "fsub st(2), st(0)\n"
            "fsubp st(2), st(0)\n"
            "fsubp\n");
}

__declspec(dllexport) void fsubr()
{
    __asm__(".intel_syntax\n"
            "fsubr dword ptr [ecx*4+edx+0x55667788]\n"
            "fsubr qword ptr [ecx*4+edx+0x55667788]\n"
            "fsubr st(0), st(2)\n"
            "fsubr st(2), st(0)\n"
            "fsubrp st(2), st(0)\n"
            "fsubrp\n");
}

__declspec(dllexport) void ftst()
{
    __asm__(".intel_syntax\n"
            "ftst\n");
}

__declspec(dllexport) void fxam()
{
    __asm__(".intel_syntax\n"
            "fxam\n");
}

__declspec(dllexport) void fxch()
{
    __asm__(".intel_syntax\n"
            "fxch st(2)\n"
            "fxch\n");
}

__declspec(dllexport) void fxtract()
{
    __asm__(".intel_syntax\n"
            "fxtract\n");
}

__declspec(dllexport) void fyl2x()
{
    __asm__(".intel_syntax\n"
            "fyl2x\n");
}

__declspec(dllexport) void fyl2xp1()
{
    __asm__(".intel_syntax\n"
            "fyl2xp1\n");
}
