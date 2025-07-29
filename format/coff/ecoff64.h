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
        uint16_t        f_magic;    /* magic number */
        uint16_t        f_nscns;    /* number of sections */
        int32_t         f_timdat;   /* time & date stamp */
        int64_t         f_symptr;   /* file pointer to symtab */
        int32_t         f_nsyms;    /* number of symtab entries */
        uint16_t        f_opthdr;   /* sizeof(optional hdr) */
        uint16_t        f_flags;    /* flags */
    };
    static_assert(sizeof(FileHeader) == 24);

    static const char* GetMagic(uint16_t magic);
};
