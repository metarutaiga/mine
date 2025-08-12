#pragma once

struct MZ
{
    struct FileHeader
    {
        // Source : DOSSYM.ASM
        uint16_t        exe_signature;      /* must contain 4D5A (yay zibo!) */
        uint16_t        exe_len_mod_512;    /* low 9 bits of length */
        uint16_t        exe_pages;          /* number of 512b pages in file */
        uint16_t        exe_rle_count;      /* count of reloc entries */
        uint16_t        exe_par_dir;        /* number of paragraphs before image */
        uint16_t        exe_min_BSS;        /* minimum number of para of BSS */
        uint16_t        exe_max_BSS;        /* max number of para of BSS */
        uint16_t        exe_SS;             /* stack of image */
        uint16_t        exe_SP;             /* SP of image */
        uint16_t        exe_chksum;         /* checksum of file (ignored) */
        uint16_t        exe_IP;             /* IP of entry */
        uint16_t        exe_CS;             /* CS of entry */
        uint16_t        exe_rle_table;      /* byte offset of reloc table */
        uint16_t        exe_iov;            /* overlay number (0 for root) */
        uint32_t        exe_sym_tab;        /* offset of symbol table in file */

        // Source : newexe.h
        uint16_t        e_flags;            /* old exe header flags  */
        uint16_t        e_res;              /* Reserved words */
        uint16_t        e_oemid;            /* OEM identifier (for e_oeminfo) */
        uint16_t        e_oeminfo;          /* OEM information; e_oemid specific */
        uint16_t        e_res2[10];         /* Reserved words */
        uint32_t        e_lfanew;           /* File address of new exe header */
    };
    static_assert(sizeof(FileHeader) == 64);

    enum Flags
    {
        EKNOWEAS        = 0x0001,   /* e_flags - program understands EAs */
        EDOSEXTENDED    = 0x0002,   /* e_flags - program runs under DOS extender */
        EPCODE          = 0x0004,   /* e_flags - memory image constructed from PCODE */
    };
};
