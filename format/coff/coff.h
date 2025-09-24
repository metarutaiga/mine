#pragma once

#include <stdint.h>

struct COFF
{
    enum MagicNumber
    {
        /* pdp11 */
        A_MAGIC0        = 0401,     /* UNIX-rt ldp */
        A_MAGIC4        = 0405,     /* pdp11 overlay */
        A_MAGIC1        = 0407,     /* pdp11/pre System V vax executable */
        A_MAGIC2        = 0410,     /* pdp11/pre System V vax pure executable */
        A_MAGIC3        = 0411,     /* pdp11 seperate I&D */
        A_MAGIC5        = 0437,     /* pdp11 kernel overlay */
        /* ??? */
        OMAGIC          = 0404,     /* object */
        PAGEMAGIC       = 0413,     /* paged */
        REGMAGIC        = 0414,     /* region */
        LIBMAGIC        = 0443,     /* library */
        /* Intel */
        B16MAGIC        = 0502,     /* Basic-16, no TV */
        BTVMAGIC        = 0503,     /* Basic-16 with TV */
        IAPX16          = 0504,
        IAPX16TV        = 0505,
        IAPX20          = 0506,
        IAPX20TV        = 0507,
        /* x86 */
        X86MAGIC        = 0510,
        XTVMAGIC        = 0511,
        /* Intel 286 */
        I286SMAGIC      = 0512,
        I286LMAGIC      = 0522,     /* used by mc68000 (UNIX PC) and iAPX 286 */
        /* 0514, 0516 and 0517 reserved for Intel */
        /* Intel 386 */
        I386MAGIC       = 0514,
        /* i860 */
        I860MAGIC       = 0515,
        /* Motorola 68000/68008/68010/68020 */
        M68MAGIC        = 0210,
        M68TVMAGIC      = 0211,
        MC68MAGIC       = 0520,
        MC68KWRMAGIC    = 0520,     /* writeable text segments */
        MC68TVMAGIC     = 0521,
        MC68KROMAGIC    = 0521,     /* readonly shareable text segments */
        MC68KPGMAGIC    = 0522,     /* demand paged text segments */
        MC68KBCSMAGIC   = 0526,     /* Bull dpx/2 */
        APOLLOM68KMAGIC = 0627,
        /* NSC */
        NS32WRMAGIC     = 0524,     /* writeable text segments    */
        NS32ROMAGIC     = 0524,     /* readonly shareable text segments */
        NS32MDMAGIC     = 0525,     /* module table magic */
        /* IBM 370 */
        U370WRMAGIC     = 0530,     /* writeble text segments */
        U370ROMAGIC     = 0535,     /* readonly sharable text segments */
        /* 0532 and 0533 reserved for u370 */
        /* Amdahl 470/580 */
        AMDWRMAGIC      = 0531,     /* writable text segments */
        AMDROMAGIC      = 0534,     /* readonly sharable text segments */
        /* XL */
        XLMAGIC         = 0540,
        /* Motorola 88000 */
        MC88MAGIC       = 0540,     /* M88000 BCS executable */
        MC88DMAGIC      = 0541,     /* M88000 DG/UX executable */
        MC88OMAGIC      = 0555,     /* M88000 normal file */
        /* Zilog */
        Z8KMAGIC        = 0544,     /* Z8000 executable, no TV */
        Z8KTVMAGIC      = 0545,     /* Z8000 executable with TV */
        /* 0544 and 0545 reserved for Zilog */
        /* n3b */
        N3BMAGIC        = 0550,     /* 3B20 executable, no TV */
        NTVMAGIC        = 0551,     /* 3B20 executable with TV */
        /* Motorola 68020/68030/68040 */
        MC68MAGICV3     = 0554,     /* UniSoft UNIX System V/68 3.2 */
        MC68MAGICV4     = 03146,    /* AT&T UNIX System V/68 4.0 */
        /* MAC-32, 3B15, 3B5 */
        WE32MAGIC       = 0560,     /* WE 32000, no TV */
        FBOMAGIC        = 0560,     /* WE 32000, no TV */
        MTVMAGIC        = 0561,     /* WE 32000 with TV */
        RBOMAGIC        = 0562,     /* reserved for WE 32000 */
        /* VAX 11/780 and VAX 11/750 */
        VAXWRMAGIC      = 0570,     /* writeable text segments */
        VAXROMAGIC      = 0575,     /* readonly sharable text segments */
        /* Am29000 */
        SIPFBOMAGIC     = 0572,     /* Am29000 (Byte 0 is MSB) */
        SIPRBOMAGIC     = 0573,     /* Am29000 (Byte 0 is LSB) */
    };

    enum Flags
    {
        STYP_REG        = 0x0,      /* "regular" section: allocated, relocated, loaded */
        STYP_DSECT      = 0x1,      /* "dummy" section: not allocated, relocated, not loaded */
        STYP_NOLOAD     = 0x2,      /* "noload" section: allocated, relocated, not loaded */
        STYP_GROUP      = 0x4,      /* "grouped" section: formed of input sections */
        STYP_PAD        = 0x8,      /* "padding" section: not allocated, not relocated, loaded */
        STYP_COPY       = 0x10,     /* "copy" section: not allocated, not relocated, loaded */
        STYP_TEXT       = 0x20,     /* section contains text only */
        STYP_DATA       = 0x40,     /* section contains data only */
        STYP_BSS        = 0x80,     /* section contains bss only */
        STYP_ALIGN      = 0x100,    /* align section : not allocated not relocated, not loaded */
        STYP_INFO       = 0x200,    /* comment section : not allocated not relocated, not loaded */
        STYP_OVER       = 0x400,    /* overlay section : relocated not allocated or loaded */
        STYP_LIB        = 0x800,    /* for .lib section : same as INFO */
        STYP_SHADOW     = 0x1000,   /* shadow section: not a cohesive section */
    };

    struct FileHeader
    {
        uint16_t        f_magic;    /* magic number */
        uint16_t        f_nscns;    /* number of sections */
        int32_t         f_timdat;   /* time & date stamp */
        int32_t         f_symptr;   /* file pointer to symtab */
        int32_t         f_nsyms;    /* number of symtab entries */
        uint16_t        f_opthdr;   /* sizeof(optional hdr) */
        uint16_t        f_flags;    /* flags */
    };
    static_assert(sizeof(FileHeader) == 20);

    struct OptionalHeader
    {
        int16_t         magic;      /* see magic.h */
        int16_t         vstamp;     /* version stamp */
        int32_t         tsize;      /* text size in bytes, padded to FW bdry */
        int32_t         dsize;      /* initialized data */
        int32_t         bsize;      /* uninitialized data */
        int32_t         entry;      /* entry pt. */
        int32_t         text_start; /* base of text used for this file */
        int32_t         data_start; /* base of data used for this file */
    };
    static_assert(sizeof(OptionalHeader) == 28);

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
        int32_t         s_flags;    /* flags */
    };
    static_assert(sizeof(SectionHeader) == 40);

    static const char* GetMagic(uint16_t magic);
};
