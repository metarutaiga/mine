#pragma once

#include "coff.h"

struct XCOFF32 : public COFF
{
    enum MagicNumber
    {
        /* Intel 386 */
        X386MAGIC       = 0514,
        /* IBM RT */
        U800WRMAGIC     = 0630,     /* writeable text segments */
        U800ROMAGIC     = 0635,     /* readonly sharable text segments */
        U800TOCMAGIC    = 0637,     /* readonly text segments and TOC */
        /* IBM POWER and PowerPC */
        U802WRMAGIC     = 0730,     /* writeable text segments */
        U802ROMAGIC     = 0735,     /* readonly sharable text segments */
        U802TOCMAGIC    = 0737,     /* readonly text segments and TOC */
    };

    enum Flags
    {
        STYP_REG        = 0x00,     /* "regular" section */
        STYP_PAD        = 0x08,     /* "padding" section */
        STYP_TEXT       = 0x20,     /* section contains text only */
        STYP_DATA       = 0x40,     /* section contains data only */
        STYP_BSS        = 0x80,     /* section contains bss only */
        STYP_EXCEPT     = 0x0100,   /* Exception section */
        STYP_INFO       = 0x0200,   /* Comment section */
        STYP_LOADER     = 0x1000,   /* Loader section */
        STYP_DEBUG      = 0x2000,   /* Debug section */
        STYP_TYPCHK     = 0x4000,   /* Type check section */
        STYP_OVRFLO     = 0x8000,   /* Overflow section header for handling relocation and line number count overflows */
    };

    #pragma pack(push)
    #pragma pack(1)
    struct SectionHeader
    {
        char            s_name[8];  /* section name */
        int32_t         s_paddr;    /* physical address */
        int32_t         s_vaddr;    /* virtual address */
        int32_t         s_size;     /* section size */
        int32_t         s_scnptr;   /* file ptr to raw data for section */
        int32_t         s_relptr;   /* file ptr to relocation */
        int32_t         s_lnnoptr;  /* file ptr to line numbers */
        uint16_t        s_nreloc;   /* number of relocation entries */
        uint16_t        s_nlnno;    /* number of line number entries */
        int16_t         s_flags;    /* flags */
    };
    #pragma pack(pop)
    static_assert(sizeof(SectionHeader) == 38);

    static const char* GetMagic(uint16_t magic);
};
