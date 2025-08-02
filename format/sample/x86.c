__declspec(dllexport) void bsf()
{
    __asm__(".intel_syntax\n"
            "bsf cx, dx\n"
            "bsf ecx, edx\n"
            "bsf cx, [edx+0x11223344]\n"
            "bsf ecx, [edx+0x11223344]\n"
            "bsf cx, [edx*4+ebx+0x55667788]\n"
            "bsf ecx, [edx*4+ebx+0x55667788]\n");
}

__declspec(dllexport) void bsr()
{
    __asm__(".intel_syntax\n"
            "bsr cx, dx\n"
            "bsr ecx, edx\n"
            "bsr cx, [edx+0x11223344]\n"
            "bsr ecx, [edx+0x11223344]\n"
            "bsr cx, [edx*4+ebx+0x55667788]\n"
            "bsr ecx, [edx*4+ebx+0x55667788]\n");
}

__declspec(dllexport) void bt()
{
    __asm__(".intel_syntax\n"
            "bt cx, dx\n"
            "bt ecx, edx\n"
            "bt word ptr [ecx+0x11223344], dx\n"
            "bt dword ptr [ecx+0x11223344], edx\n"
            "bt word ptr [ecx*4+edx+0x55667788], 0x11\n"
            "bt dword ptr [ecx*4+edx+0x55667788], 0x22\n");
}
