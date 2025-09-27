#pragma once

#define TIB_MSVCRT 0xC00

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
