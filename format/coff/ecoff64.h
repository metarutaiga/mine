#pragma once

#include "ecoff.h"

struct ECOFF64 : public ECOFF
{
    enum MagicNumber
    {
        /* Alpha */
        ALPHAMAGIC      = 0603,     /* object file */
        ALPHAMAGICZ     = 0610,     /* compressed object file */
        ALPHAUMAGIC     = 0617,     /* ucode object file */
    };

    enum FileHeaderFlags
    {
        F_RELFLG            = 0x0001,   /* Relocation information stripped from file */
        F_EXEC              = 0x0002,   /* File is executable (that is, no unresolved external references) */
        F_LNNO              = 0x0004,   /* Line numbers stripped from file */
        F_LSYMS             = 0x0008,   /* Local symbols stripped from file */
        F_NO_SHARED         = 0x0010,   /* Object file cannot be used to create a shared library */
        F_NO_CALL_SHARED    = 0x0020,   /* Object file cannot be used to create a -call_shared executable file */
        F_LOMAP             = 0x0040,   /* Allows an executable file to be loaded at an address less than VM_MIN_ADDRESS */
        F_AR16WR            = 0x0080,   /* File has the byte ordering of an AR16WR machine (for example, PDP-11/70) */
        F_AR32WR            = 0x0100,   /* File has the byte ordering of an AR32WR machine (for example, VAX) */
        F_AR32W             = 0x0200,   /* File has the byte ordering of an AR32W machine (for example, 3b, maxi, MC68000) */
        F_PATCH             = 0x0400,   /* File contains ‘‘patch’’ list in optional header */
        F_NODF              = 0x0400,   /* (Minimal file only.) No decision functions for replaced functions */
        F_MIPS_NO_SHARED    = 0x1000,   /* Cannot be dynamically shared */
        F_MIPS_SHARABLE     = 0x2000,   /* A dynamically shared object */
        F_MIPS_CALL_SHARED  = 0x3000,   /* Dynamic executable file */
        F_MIPS_NO_REORG     = 0x4000,   /* Do not reorder sections */
        F_MIPS_NO_REMOVE    = 0x8000,   /* Do not remove nops */
    };

    enum Flags
    {
        STYP_REG        = 0x00,         /* Regular section: allocated, relocated, loaded */
        STYP_DSECT      = 0x01,         /* Dummy section: not allocated, relocated, not loaded */
        STYP_NOLOAD     = 0x02,         /* Noload section: allocated, relocated, not loaded */
        STYP_GROUP      = 0x04,         /* Grouped section: formed of input sections */
        STYP_PAD        = 0x08,         /* Padding section: not allocated, not relocated, loaded */
        STYP_COPY       = 0x10,         /* Copy section (for decision function used by field update): not allocated, not relocated, loaded */
        STYP_TEXT       = 0x20,         /* Text only */
        STYP_DATA       = 0x40,         /* Data only */
        STYP_BSS        = 0x80,         /* Bss only */
        STYP_RDATA      = 0x100,        /* Read-only data only */
        STYP_SDATA      = 0x200,        /* Small data only */
        STYP_SBSS       = 0x400,        /* Small bss only */
        STYP_UCODE      = 0x800,        /* Ucode only */
        STYP_GOT        = 0x1000,       /* Global offset table */
        STYP_DYNAMIC    = 0x2000,       /* Dynamic linking information */
        STYP_DYNSYM     = 0x4000,       /* Dynamic linking symbol table */
        STYP_REL_DYN    = 0x8000,       /* Dynamic relocation information */
        STYP_DYNSTR     = 0x10000,      /* Dynamic linking symbol table */
        STYP_HASH       = 0x20000,      /* Dynamic symbol hash table */
        STYP_MSYM       = 0x80000,      /* Additional dynamic linking symbol table */
        STYP_CONFLICT   = 0x100000,     /* Additional dynamic linking information */
        STYP_REGINFO    = 0x200000,     /* Register usage information */
        STYP_FINI       = 0x01000000,   /* .fini section text */
        STYP_COMMENT    = 0x02000000,   /* Comment section */
        STYP_RCONST     = 0x02200000,   /* Read-only constants */
        STYP_XDATA      = 0x02400000,   /* Exception scope table */
        STYP_PDATA      = 0x02800000,   /* Exception procedure table */
        STYP_LITA       = 0x04000000,   /* Address literals only */
        STYP_LIT8       = 0x08000000,   /* 8-byte literals only */
        STYP_LIT4       = 0x10000000,   /* 4-byte literals only */
        S_NRELOC_OVFL   = 0x20000000,   /* s_nrelocoverflowed, the value is in r_vaddrof the first entry */
        STYP_INIT       = 0x80000000,   /* Section initialization text only */
    };

    struct FileHeader
    {
        uint16_t        f_magic;    /* File magic number. Used for identification. */
        uint16_t        f_nscns;    /* Number of section headers in the object file. */
        int32_t         f_timdat;   /* Time and date stamp. */
        int64_t         f_symptr;   /* File offset to symbolic header. */
        int32_t         f_nsyms;    /* Size of symbolic header (in bytes). */
        uint16_t        f_opthdr;   /* Size of a.out header (in bytes). */
        uint16_t        f_flags;    /* Flags that describe the object file. */
    };
    static_assert(sizeof(FileHeader) == 24);

    struct OptionalHeader
    {
        int16_t         magic;      /* Object-file magic numbers */
        int16_t         vstamp;     /* Object file version stamp. */
        int16_t         bldrev;     /* Revision of system build tools. */
        int16_t         padcell;
        int64_t         tsize;      /* Text segment size (in bytes) padded to 16-byte boundary; set to zero if there is no text segment. */
        int64_t         dsize;      /* Data segment size (in bytes) padded to 16-byte boundary; set to zero if there is no data segment. */
        int64_t         bsize;      /* Bss segment size (in bytes) padded to 16-byte boundary; set to zero if there is no bss segment. */
        int64_t         entry;      /* Virtual address of program entry point. */
        int64_t         text_start; /* Base address of text segments. */
        int64_t         data_start; /* Base address of data segments. */
        int64_t         bss_start;  /* Base address of bss segments. */
        int32_t         gprmask;
        int16_t         fprmask;
        int64_t         gp_value;   /* The initial GP (Global Pointer) value used for this object. */
    };
    static_assert(sizeof(OptionalHeader) == 80);

    struct SectionHeader
    {
        char            s_name[8];  /* Section name */
        int64_t         s_paddr;    /* Base virtual address of section in the image. */
        int64_t         s_vaddr;    /* Base virtual address of a loadable section in the image. */
        int64_t         s_size;     /* Section size rounded to 16-byte multiple. */
        int64_t         s_scnptr;   /* File offset to beginning of raw data for the section. */
        int64_t         s_relptr;   /* File offset to relocations for the section */
        int64_t         s_lnnoptr;  /* In .lita section header */
        uint16_t        s_nreloc;   /* Number of relocation entries; 0xffff if number of entries overflows size of this field */
        uint16_t        s_nlnno;
        int32_t         s_flags;    /* Flags identifying the section */
    };
    static_assert(sizeof(SectionHeader) == 64);

    static const char* GetMagic(uint16_t magic);
};
