#pragma once

#define TIB_MSVCRT 0x600

struct MSVCRT {
    uint32_t iob[3][8];
    uint32_t adjust_fdiv;
    uint32_t commode;
    uint32_t fmode;
    uint32_t initenv;
    uint32_t argc;
    uint32_t argv;
    uint32_t envp;
};

struct JUMP_BUFFER {
    uint32_t Ebp;
    uint32_t Ebx;
    uint32_t Edi;
    uint32_t Esi;
    uint32_t Esp;
    uint32_t Eip;
    uint32_t Registration;
    uint32_t TryLevel;
    uint32_t Cookie;
    uint32_t UnwindFunc;
    uint32_t UnwindData[6];
};
