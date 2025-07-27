#pragma once

#include "xcoff32.h"

struct XCOFF64 : public XCOFF32
{
    enum MagicNumber
    {
        /* IBM POWER and PowerPC */
        U803TOCMAGIC    = 0747,     /* obsolete XCOFF64 - not used */
        U803XTOCMAGIC   = 0757,     /* AIX 4.3 64-bit XCOFF */
        U64_TOCMAGIC    = 0767,     /* AIX 5+ 64-bit XCOFF */
    };

    struct FileHeader
    {
        uint16_t        f_magic;    /* magic number */
        uint16_t        f_nscns;    /* number of sections */
        int32_t         f_timdat;   /* time & date stamp */
        int64_t         f_symptr;   /* file pointer to symtab */
        uint16_t        f_opthdr;   /* sizeof(optional hdr) */
        uint16_t        f_flags;    /* flags */
        int32_t         f_nsyms;    /* number of symtab entries */
    };
    static_assert(sizeof(FileHeader) == 24);

    #pragma pack(push)
    #pragma pack(1)
    struct SectionHeader
    {
        char            s_name[8];  /* section name */
        uint64_t        s_paddr;    /* physical address */
        uint64_t        s_vaddr;    /* virtual address */
        uint64_t        s_size;     /* section size */
        int64_t         s_scnptr;   /* file ptr to raw data for section */
        int64_t         s_relptr;   /* file ptr to relocation */
        int64_t         s_lnnoptr;  /* file ptr to line numbers */
        uint32_t        s_nreloc;   /* number of relocation entries */
        uint32_t        s_nlnno;    /* number of line number entries */
        int32_t         s_flags;    /* flags */
    };
    #pragma pack(pop)
    static_assert(sizeof(SectionHeader) == 68);

    static const char* GetMagic(uint16_t magic);
};
