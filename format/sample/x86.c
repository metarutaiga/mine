__declspec(dllexport) void adc()
{
    __asm__(".intel_syntax\n"
            "adc al, 0x11\n"
            "adc ax, 0x1122\n"
            "adc eax, 0x11223344\n"
            "adc byte ptr [ecx*4+edx+0x55667788], 0x11\n"
            "adc word ptr [ecx*4+edx+0x55667788], 0x1122\n"
            "adc dword ptr [ecx*4+edx+0x55667788], 0x11223344\n"
            "adc word ptr [ecx*4+edx+0x55667788], -1\n"
            "adc dword ptr [ecx*4+edx+0x55667788], -1\n"
            "adc byte ptr [ecx], dl\n"
            "adc word ptr [ecx], dx\n"
            "adc dword ptr [ecx], edx\n"
            "adc cl, byte ptr [edx]\n"
            "adc cx, word ptr [edx]\n"
            "adc ecx, dword ptr [edx]\n");
}

__declspec(dllexport) void add()
{
    __asm__(".intel_syntax\n"
            "add al, 0x11\n"
            "add ax, 0x1122\n"
            "add eax, 0x11223344\n"
            "add byte ptr [ecx*4+edx+0x55667788], 0x11\n"
            "add word ptr [ecx*4+edx+0x55667788], 0x1122\n"
            "add dword ptr [ecx*4+edx+0x55667788], 0x11223344\n"
            "add word ptr [ecx*4+edx+0x55667788], -1\n"
            "add dword ptr [ecx*4+edx+0x55667788], -1\n"
            "add byte ptr [ecx], dl\n"
            "add word ptr [ecx], dx\n"
            "add dword ptr [ecx], edx\n"
            "add cl, byte ptr [edx]\n"
            "add cx, word ptr [edx]\n"
            "add ecx, dword ptr [edx]\n");
}

__declspec(dllexport) void and()
{
    __asm__(".intel_syntax\n"
            "and al, 0x11\n"
            "and ax, 0x1122\n"
            "and eax, 0x11223344\n"
            "and byte ptr [ecx*4+edx+0x55667788], 0x11\n"
            "and word ptr [ecx*4+edx+0x55667788], 0x1122\n"
            "and dword ptr [ecx*4+edx+0x55667788], 0x11223344\n"
            "and word ptr [ecx*4+edx+0x55667788], -1\n"
            "and dword ptr [ecx*4+edx+0x55667788], -1\n"
            "and byte ptr [ecx], dl\n"
            "and word ptr [ecx], dx\n"
            "and dword ptr [ecx], edx\n"
            "and cl, byte ptr [edx]\n"
            "and cx, word ptr [edx]\n"
            "and ecx, dword ptr [edx]\n");
}

__declspec(dllexport) void bsf_bsr()
{
    __asm__(".intel_syntax\n"
            "bsf cx, dx\n"
            "bsf ecx, edx\n"
            "bsf cx, [edx*4+ebx+0x55667788]\n"
            "bsf ecx, [edx*4+ebx+0x55667788]\n"
            "bsr cx, dx\n"
            "bsr ecx, edx\n"
            "bsr cx, [edx*4+ebx+0x55667788]\n"
            "bsr ecx, [edx*4+ebx+0x55667788]\n");
}

__declspec(dllexport) void bt_btc_btr_bts()
{
    __asm__(".intel_syntax\n"
            "bt cx, dx\n"
            "bt ecx, edx\n"
            "bt word ptr [ecx*4+edx+0x55667788], 0x11\n"
            "bt dword ptr [ecx*4+edx+0x55667788], 0x22\n"
            "btc cx, dx\n"
            "btc ecx, edx\n"
            "btc word ptr [ecx*4+edx+0x55667788], 0x11\n"
            "btc dword ptr [ecx*4+edx+0x55667788], 0x22\n"
            "btr cx, dx\n"
            "btr ecx, edx\n"
            "btr word ptr [ecx*4+edx+0x55667788], 0x11\n"
            "btr dword ptr [ecx*4+edx+0x55667788], 0x22\n"
            "bts cx, dx\n"
            "bts ecx, edx\n"
            "bts word ptr [ecx*4+edx+0x55667788], 0x11\n"
            "bts dword ptr [ecx*4+edx+0x55667788], 0x22\n");
}

__declspec(dllexport) void call()
{
    __asm__(".intel_syntax\nLcall:\n"
            "call Lcall\n"
            "call ecx\n"
            "call [ecx*4+edx+0x55667788]\n");
}

__declspec(dllexport) void cbw_cwde()
{
    __asm__("cbw\n"
            "cwde\n");
}

__declspec(dllexport) void clc_cld_cli_cmc()
{
    __asm__("clc\n"
            "cld\n"
            "cli\n"
            "cmc\n");
}

__declspec(dllexport) void cmp()
{
    __asm__(".intel_syntax\n"
            "cmp al, 0x11\n"
            "cmp ax, 0x1122\n"
            "cmp eax, 0x11223344\n"
            "cmp byte ptr [ecx*4+edx+0x55667788], 0x11\n"
            "cmp word ptr [ecx*4+edx+0x55667788], 0x1122\n"
            "cmp dword ptr [ecx*4+edx+0x55667788], 0x11223344\n"
            "cmp word ptr [ecx*4+edx+0x55667788], -1\n"
            "cmp dword ptr [ecx*4+edx+0x55667788], -1\n"
            "cmp byte ptr [ecx], dl\n"
            "cmp word ptr [ecx], dx\n"
            "cmp dword ptr [ecx], edx\n"
            "cmp cl, byte ptr [edx]\n"
            "cmp cx, word ptr [edx]\n"
            "cmp ecx, dword ptr [edx]\n");
}

__declspec(dllexport) void cmpsb_cmpsw_cmpsd()
{
    __asm__(".intel_syntax\n"
            "cmpsb\n"
            "cmpsw\n"
            "cmpsd\n");
}

__declspec(dllexport) void cwd_cdq()
{
    __asm__("cwd\n"
            "cdq\n");
}

__declspec(dllexport) void dec()
{
    __asm__(".intel_syntax\n"
            "dec byte ptr [ecx*4+edx+0x55667788]\n"
            "dec word ptr [ecx*4+edx+0x55667788]\n"
            "dec dword ptr [ecx*4+edx+0x55667788]\n"
            "dec cx\n"
            "dec ecx\n");
}

__declspec(dllexport) void div()
{
    __asm__(".intel_syntax\n"
            "div byte ptr [ecx*4+edx+0x55667788]\n"
            "div word ptr [ecx*4+edx+0x55667788]\n"
            "div dword ptr [ecx*4+edx+0x55667788]\n");
}

__declspec(dllexport) void enter()
{
    __asm__(".intel_syntax\n"
            "enter 0x1122, 0x33\n");
}

__declspec(dllexport) void idiv()
{
    __asm__(".intel_syntax\n"
            "idiv byte ptr [ecx*4+edx+0x55667788]\n"
            "idiv word ptr [ecx*4+edx+0x55667788]\n"
            "idiv dword ptr [ecx*4+edx+0x55667788]\n");
}

__declspec(dllexport) void imul()
{
    __asm__(".intel_syntax\n"
            "imul byte ptr [ecx*4+edx+0x55667788]\n"
            "imul word ptr [ecx*4+edx+0x55667788]\n"
            "imul dword ptr [ecx*4+edx+0x55667788]\n"
            "imul cx, [edx*4+ebx+0x55667788]\n"
            "imul ecx, [edx*4+ebx+0x55667788]\n"
            "imul cx, [edx*4+ebx+0x55667788], -1\n"
            "imul ecx, [edx*4+ebx+0x55667788], -1\n"
            "imul cx, dx, -1\n"
            "imul ecx, edx, -1\n"
            "imul cx, [edx*4+ebx+0x55667788], 0x1122\n"
            "imul ecx, [edx*4+ebx+0x55667788], 0x11223344\n"
            "imul cx, dx, 0x1122\n"
            "imul ecx, edx, 0x11223344\n");
}

__declspec(dllexport) void inc()
{
    __asm__(".intel_syntax\n"
            "inc byte ptr [ecx*4+edx+0x55667788]\n"
            "inc word ptr [ecx*4+edx+0x55667788]\n"
            "inc dword ptr [ecx*4+edx+0x55667788]\n"
            "inc cx\n"
            "inc ecx\n");
}

__declspec(dllexport) void jcc_short()
{
    __asm__(".intel_syntax\nLjcc:\n"
            "jo Ljcc\n"
            "jno Ljcc\n"
            "jc Ljcc\n"
            "jnc Ljcc\n"
            "jz Ljcc\n"
            "jnz Ljcc\n"
            "jbe Ljcc\n"
            "ja Ljcc\n"
            "js Ljcc\n"
            "jns Ljcc\n"
            "jpe Ljcc\n"
            "jpo Ljcc\n"
            "jl Ljcc\n"
            "jge Ljcc\n"
            "jle Ljcc\n"
            "jg Ljcc\n"
            "jcxz Ljcc\n"
            "jecxz Ljcc\n"
            "ret\n"
            ".rept 128\nint 3\n.endr\n");
}

__declspec(dllexport) void jcc_near()
{
    __asm__(".intel_syntax\n"
            "jo Ljcc\n"
            "jno Ljcc\n"
            "jc Ljcc\n"
            "jnc Ljcc\n"
            "jz Ljcc\n"
            "jnz Ljcc\n"
            "jbe Ljcc\n"
            "ja Ljcc\n"
            "js Ljcc\n"
            "jns Ljcc\n"
            "jpe Ljcc\n"
            "jpo Ljcc\n"
            "jl Ljcc\n"
            "jge Ljcc\n"
            "jle Ljcc\n"
            "jg Ljcc\n");
}

__declspec(dllexport) void jmp()
{
    __asm__(".intel_syntax\nLjmp:\n"
            "jmp Ljmp\n"
            "jmp Lcall\n"
            "jmp ecx\n"
            "jmp [ecx*4+edx+0x55667788]\n");
}

__declspec(dllexport) void lahf()
{
    __asm__("lahf\n");
}

__declspec(dllexport) void lea()
{
    __asm__(".intel_syntax\n"
            "lea cx, [edx*4+ebx+0x55667788]\n"
            "lea ecx, [edx*4+ebx+0x55667788]\n");
}

__declspec(dllexport) void leave()
{
    __asm__("leave\n");
}

__declspec(dllexport) void lodsb_lodsw_lodsd()
{
    __asm__(".intel_syntax\n"
            "lodsb\n"
            "lodsw\n"
            "lodsd\n");
}

__declspec(dllexport) void loop_loopz_loopnz()
{
    __asm__(".intel_syntax\nLloop:\n"
            "loop Lloop\n"
            "loopz Lloop\n"
            "loopnz Lloop\n");
}

__declspec(dllexport) void mov()
{
    __asm__(".intel_syntax\n"
            "mov cl, [edx*4+ebx+0x55667788]\n"
            "mov cx, [edx*4+ebx+0x55667788]\n"
            "mov ecx, [edx*4+ebx+0x55667788]\n"
            "mov [ecx*4+edx+0x55667788], bl\n"
            "mov [ecx*4+edx+0x55667788], bx\n"
            "mov [ecx*4+edx+0x55667788], ebx\n"
            "mov al, [0x55667788]\n"
            "mov ax, [0x55667788]\n"
            "mov eax, [0x55667788]\n"
            "mov [0x55667788], al\n"
            "mov [0x55667788], ax\n"
            "mov [0x55667788], eax\n"
            "mov cl, 0x11\n"
            "mov cx, 0x1122\n"
            "mov ecx, 0x11223344\n"
            "mov byte ptr [ecx*4+edx+0x55667788], 0x11\n"
            "mov word ptr [ecx*4+edx+0x55667788], 0x1122\n"
            "mov dword ptr [ecx*4+edx+0x55667788], 0x11223344\n"
            "mov es, cx\n"
            "mov cs, edx\n"
            "mov cx, ss\n"
            "mov edx, ds\n");
}

__declspec(dllexport) void movsb_movsw_movsd()
{
    __asm__(".intel_syntax\n"
            "movsb\n"
            "movsw\n"
            "movsd\n");
}

__declspec(dllexport) void movsx_movzx()
{
    __asm__(".intel_syntax\n"
            "movsx cx, byte ptr [edx*4+ebx+0x55667788]\n"
            "movsx ecx, byte ptr [edx*4+ebx+0x55667788]\n"
            "movsx ecx, word ptr [edx*4+ebx+0x55667788]\n"
            "movzx cx, byte ptr [edx*4+ebx+0x55667788]\n"
            "movzx ecx, byte ptr [edx*4+ebx+0x55667788]\n"
            "movzx ecx, word ptr [edx*4+ebx+0x55667788]\n");
}

__declspec(dllexport) void mul()
{
    __asm__(".intel_syntax\n"
            "mul byte ptr [ecx*4+edx+0x55667788]\n"
            "mul word ptr [ecx*4+edx+0x55667788]\n"
            "mul dword ptr [ecx*4+edx+0x55667788]\n");
}

__declspec(dllexport) void neg()
{
    __asm__(".intel_syntax\n"
            "neg byte ptr [ecx*4+edx+0x55667788]\n"
            "neg word ptr [ecx*4+edx+0x55667788]\n"
            "neg dword ptr [ecx*4+edx+0x55667788]\n");
}

__declspec(dllexport) void nop()
{
    __asm__("nop\n");
}

__declspec(dllexport) void not()
{
    __asm__(".intel_syntax\n"
            "not byte ptr [ecx*4+edx+0x55667788]\n"
            "not word ptr [ecx*4+edx+0x55667788]\n"
            "not dword ptr [ecx*4+edx+0x55667788]\n");
}

__declspec(dllexport) void or()
{
    __asm__(".intel_syntax\n"
            "or al, 0x11\n"
            "or ax, 0x1122\n"
            "or eax, 0x11223344\n"
            "or byte ptr [ecx*4+edx+0x55667788], 0x11\n"
            "or word ptr [ecx*4+edx+0x55667788], 0x1122\n"
            "or dword ptr [ecx*4+edx+0x55667788], 0x11223344\n"
            "or word ptr [ecx*4+edx+0x55667788], -1\n"
            "or dword ptr [ecx*4+edx+0x55667788], -1\n"
            "or byte ptr [ecx], dl\n"
            "or word ptr [ecx], dx\n"
            "or dword ptr [ecx], edx\n"
            "or cl, byte ptr [edx]\n"
            "or cx, word ptr [edx]\n"
            "or ecx, dword ptr [edx]\n");
}

__declspec(dllexport) void pop()
{
    __asm__(".intel_syntax\n"
            "pop word ptr [ecx*4+edx+0x55667788]\n"
            "pop dword ptr [ecx*4+edx+0x55667788]\n"
            "pop cx\n"
            "pop ecx\n"
            "pop ss\n");
}

__declspec(dllexport) void popa_popf()
{
    __asm__("popa\n"
            "popf\n");
}

__declspec(dllexport) void push()
{
    __asm__(".intel_syntax\n"
            "push word ptr [ecx*4+edx+0x55667788]\n"
            "push dword ptr [ecx*4+edx+0x55667788]\n"
            "push cx\n"
            "push ecx\n"
            "push ss\n"
            "push 0x11\n"
            "push 0x11223344\n");
}

__declspec(dllexport) void pusha_pushf()
{
    __asm__("pusha\n"
            "pushf\n");
}

__declspec(dllexport) void rcl_rcr_rol_ror()
{
    __asm__(".intel_syntax\n"
            "rcl byte ptr [ecx*4+edx+0x55667788], 1\n"
            "rcl byte ptr [ecx*4+edx+0x55667788], cl\n"
            "rcl byte ptr [ecx*4+edx+0x55667788], 0x11\n"
            "rcl word ptr [ecx*4+edx+0x55667788], 1\n"
            "rcl word ptr [ecx*4+edx+0x55667788], cl\n"
            "rcl word ptr [ecx*4+edx+0x55667788], 0x11\n"
            "rcl dword ptr [ecx*4+edx+0x55667788], 1\n"
            "rcl dword ptr [ecx*4+edx+0x55667788], cl\n"
            "rcl dword ptr [ecx*4+edx+0x55667788], 0x11\n"
            "rcr byte ptr [ecx*4+edx+0x55667788], 1\n"
            "rcr byte ptr [ecx*4+edx+0x55667788], cl\n"
            "rcr byte ptr [ecx*4+edx+0x55667788], 0x11\n"
            "rcr word ptr [ecx*4+edx+0x55667788], 1\n"
            "rcr word ptr [ecx*4+edx+0x55667788], cl\n"
            "rcr word ptr [ecx*4+edx+0x55667788], 0x11\n"
            "rcr dword ptr [ecx*4+edx+0x55667788], 1\n"
            "rcr dword ptr [ecx*4+edx+0x55667788], cl\n"
            "rcr dword ptr [ecx*4+edx+0x55667788], 0x11\n"
            "rol byte ptr [ecx*4+edx+0x55667788], 1\n"
            "rol byte ptr [ecx*4+edx+0x55667788], cl\n"
            "rol byte ptr [ecx*4+edx+0x55667788], 0x11\n"
            "rol word ptr [ecx*4+edx+0x55667788], 1\n"
            "rol word ptr [ecx*4+edx+0x55667788], cl\n"
            "rol word ptr [ecx*4+edx+0x55667788], 0x11\n"
            "rol dword ptr [ecx*4+edx+0x55667788], 1\n"
            "rol dword ptr [ecx*4+edx+0x55667788], cl\n"
            "rol dword ptr [ecx*4+edx+0x55667788], 0x11\n"
            "ror byte ptr [ecx*4+edx+0x55667788], 1\n"
            "ror byte ptr [ecx*4+edx+0x55667788], cl\n"
            "ror byte ptr [ecx*4+edx+0x55667788], 0x11\n"
            "ror word ptr [ecx*4+edx+0x55667788], 1\n"
            "ror word ptr [ecx*4+edx+0x55667788], cl\n"
            "ror word ptr [ecx*4+edx+0x55667788], 0x11\n"
            "ror dword ptr [ecx*4+edx+0x55667788], 1\n"
            "ror dword ptr [ecx*4+edx+0x55667788], cl\n"
            "ror dword ptr [ecx*4+edx+0x55667788], 0x11\n");
}

__declspec(dllexport) void sahf()
{
    __asm__("sahf\n");
}

__declspec(dllexport) void sal_sar_shl_shr()
{
    __asm__(".intel_syntax\n"
            "sal byte ptr [ecx*4+edx+0x55667788], 1\n"
            "sal byte ptr [ecx*4+edx+0x55667788], cl\n"
            "sal byte ptr [ecx*4+edx+0x55667788], 0x11\n"
            "sal word ptr [ecx*4+edx+0x55667788], 1\n"
            "sal word ptr [ecx*4+edx+0x55667788], cl\n"
            "sal word ptr [ecx*4+edx+0x55667788], 0x11\n"
            "sal dword ptr [ecx*4+edx+0x55667788], 1\n"
            "sal dword ptr [ecx*4+edx+0x55667788], cl\n"
            "sal dword ptr [ecx*4+edx+0x55667788], 0x11\n"
            "sar byte ptr [ecx*4+edx+0x55667788], 1\n"
            "sar byte ptr [ecx*4+edx+0x55667788], cl\n"
            "sar byte ptr [ecx*4+edx+0x55667788], 0x11\n"
            "sar word ptr [ecx*4+edx+0x55667788], 1\n"
            "sar word ptr [ecx*4+edx+0x55667788], cl\n"
            "sar word ptr [ecx*4+edx+0x55667788], 0x11\n"
            "sar dword ptr [ecx*4+edx+0x55667788], 1\n"
            "sar dword ptr [ecx*4+edx+0x55667788], cl\n"
            "sar dword ptr [ecx*4+edx+0x55667788], 0x11\n"
            "shl byte ptr [ecx*4+edx+0x55667788], 1\n"
            "shl byte ptr [ecx*4+edx+0x55667788], cl\n"
            "shl byte ptr [ecx*4+edx+0x55667788], 0x11\n"
            "shl word ptr [ecx*4+edx+0x55667788], 1\n"
            "shl word ptr [ecx*4+edx+0x55667788], cl\n"
            "shl word ptr [ecx*4+edx+0x55667788], 0x11\n"
            "shl dword ptr [ecx*4+edx+0x55667788], 1\n"
            "shl dword ptr [ecx*4+edx+0x55667788], cl\n"
            "shl dword ptr [ecx*4+edx+0x55667788], 0x11\n"
            "shr byte ptr [ecx*4+edx+0x55667788], 1\n"
            "shr byte ptr [ecx*4+edx+0x55667788], cl\n"
            "shr byte ptr [ecx*4+edx+0x55667788], 0x11\n"
            "shr word ptr [ecx*4+edx+0x55667788], 1\n"
            "shr word ptr [ecx*4+edx+0x55667788], cl\n"
            "shr word ptr [ecx*4+edx+0x55667788], 0x11\n"
            "shr dword ptr [ecx*4+edx+0x55667788], 1\n"
            "shr dword ptr [ecx*4+edx+0x55667788], cl\n"
            "shr dword ptr [ecx*4+edx+0x55667788], 0x11\n");
}

__declspec(dllexport) void sbb()
{
    __asm__(".intel_syntax\n"
            "sbb al, 0x11\n"
            "sbb ax, 0x1122\n"
            "sbb eax, 0x11223344\n"
            "sbb byte ptr [ecx*4+edx+0x55667788], 0x11\n"
            "sbb word ptr [ecx*4+edx+0x55667788], 0x1122\n"
            "sbb dword ptr [ecx*4+edx+0x55667788], 0x11223344\n"
            "sbb word ptr [ecx*4+edx+0x55667788], -1\n"
            "sbb dword ptr [ecx*4+edx+0x55667788], -1\n"
            "sbb byte ptr [ecx], dl\n"
            "sbb word ptr [ecx], dx\n"
            "sbb dword ptr [ecx], edx\n"
            "sbb cl, byte ptr [edx]\n"
            "sbb cx, word ptr [edx]\n"
            "sbb ecx, dword ptr [edx]\n");
}

__declspec(dllexport) void scasb_scasw_scasd()
{
    __asm__(".intel_syntax\n"
            "scasb\n"
            "scasw\n"
            "scasd\n");
}

__declspec(dllexport) void setcc()
{
    __asm__(".intel_syntax\n"
            "seto byte ptr [ecx*4+edx+0x55667788]\n"
            "setno byte ptr [ecx*4+edx+0x55667788]\n"
            "setc byte ptr [ecx*4+edx+0x55667788]\n"
            "setnc byte ptr [ecx*4+edx+0x55667788]\n"
            "setz byte ptr [ecx*4+edx+0x55667788]\n"
            "setnz byte ptr [ecx*4+edx+0x55667788]\n"
            "setbe byte ptr [ecx*4+edx+0x55667788]\n"
            "seta byte ptr [ecx*4+edx+0x55667788]\n"
            "sets byte ptr [ecx*4+edx+0x55667788]\n"
            "setns byte ptr [ecx*4+edx+0x55667788]\n"
            "setpe byte ptr [ecx*4+edx+0x55667788]\n"
            "setpo byte ptr [ecx*4+edx+0x55667788]\n"
            "setl byte ptr [ecx*4+edx+0x55667788]\n"
            "setge byte ptr [ecx*4+edx+0x55667788]\n"
            "setle byte ptr [ecx*4+edx+0x55667788]\n"
            "setg byte ptr [ecx*4+edx+0x55667788]\n");
}

__declspec(dllexport) void shld_shrd()
{
    __asm__(".intel_syntax\n"
            "shld word ptr [ecx*4+edx+0x55667788], bx, -1\n"
            "shld dword ptr [ecx*4+edx+0x55667788], ebx, -1\n"
            "shld word ptr [ecx*4+edx+0x55667788], bx, cl\n"
            "shld dword ptr [ecx*4+edx+0x55667788], ebx, cl\n"
            "shrd word ptr [ecx*4+edx+0x55667788], bx, -1\n"
            "shrd dword ptr [ecx*4+edx+0x55667788], ebx, -1\n"
            "shrd word ptr [ecx*4+edx+0x55667788], bx, cl\n"
            "shrd dword ptr [ecx*4+edx+0x55667788], ebx, cl\n");
}

__declspec(dllexport) void stc_std_sti()
{
    __asm__("stc\n"
            "std\n"
            "sti\n");
}

__declspec(dllexport) void stosb_stosw_stosd()
{
    __asm__(".intel_syntax\n"
            "stosb\n"
            "stosw\n"
            "stosd\n");
}

__declspec(dllexport) void sub()
{
    __asm__(".intel_syntax\n"
            "sub al, 0x11\n"
            "sub ax, 0x1122\n"
            "sub eax, 0x11223344\n"
            "sub byte ptr [ecx*4+edx+0x55667788], 0x11\n"
            "sub word ptr [ecx*4+edx+0x55667788], 0x1122\n"
            "sub dword ptr [ecx*4+edx+0x55667788], 0x11223344\n"
            "sub word ptr [ecx*4+edx+0x55667788], -1\n"
            "sub dword ptr [ecx*4+edx+0x55667788], -1\n"
            "sub byte ptr [ecx], dl\n"
            "sub word ptr [ecx], dx\n"
            "sub dword ptr [ecx], edx\n"
            "sub cl, byte ptr [edx]\n"
            "sub cx, word ptr [edx]\n"
            "sub ecx, dword ptr [edx]\n");
}

__declspec(dllexport) void test()
{
    __asm__(".intel_syntax\n"
            "test al, 0x11\n"
            "test ax, 0x1122\n"
            "test eax, 0x11223344\n"
            "test byte ptr [ecx*4+edx+0x55667788], 0x11\n"
            "test word ptr [ecx*4+edx+0x55667788], 0x1122\n"
            "test dword ptr [ecx*4+edx+0x55667788], 0x11223344\n"
            "test byte ptr [ecx], dl\n"
            "test word ptr [ecx], dx\n"
            "test dword ptr [ecx], edx\n");
}

__declspec(dllexport) void xchg()
{
    __asm__(".intel_syntax\n"
            "xchg cx, ax\n"
            "xchg ecx, eax\n"
            "xchg byte ptr [ecx], dl\n"
            "xchg word ptr [ecx], dx\n"
            "xchg dword ptr [ecx], edx\n");
}

__declspec(dllexport) void xor()
{
    __asm__(".intel_syntax\n"
            "xor al, 0x11\n"
            "xor ax, 0x1122\n"
            "xor eax, 0x11223344\n"
            "xor byte ptr [ecx*4+edx+0x55667788], 0x11\n"
            "xor word ptr [ecx*4+edx+0x55667788], 0x1122\n"
            "xor dword ptr [ecx*4+edx+0x55667788], 0x11223344\n"
            "xor word ptr [ecx*4+edx+0x55667788], -1\n"
            "xor dword ptr [ecx*4+edx+0x55667788], -1\n"
            "xor byte ptr [ecx], dl\n"
            "xor word ptr [ecx], dx\n"
            "xor dword ptr [ecx], edx\n"
            "xor cl, byte ptr [edx]\n"
            "xor cx, word ptr [edx]\n"
            "xor ecx, dword ptr [edx]\n");
}
