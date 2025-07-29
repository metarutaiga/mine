#pragma once

#include "coff.h"

struct ECOFF : public COFF
{
    enum MagicNumber
    {
        /* MIPS */
        MIPSEBMAGIC     = 0x0160,   /* objects for big-endian machines */
        MIPSELMAGIC     = 0x0162,   /* objects for little-endian machines */
        /* MIPS II */
        MIPSEBMAGIC_2   = 0x0163,   /* objects for big-endian machines */
        MIPSELMAGIC_2   = 0x0166,   /* objects for little-endian machines */
        /* MIPS III */
        MIPSEBMAGIC_3   = 0x0140,   /* objects for big-endian machines */
        MIPSELMAGIC_3   = 0x0142,   /* objects for little-endian machines */
        /* ucode */
        MIPSEBUMAGIC    = 0x0180,   /* ucode objects for big-endian machines */
        MIPSELUMAGIC    = 0x0182,   /* ucode objects for little-endian machines */
    };

    enum FileHeaderFlags
    {
        F_RELFLG            = 0x0001,   /* Relocation information stripped from file */
        F_EXEC              = 0x0002,   /* File is executable (i.e. no unresolved external references). */
        F_LNNO              = 0x0004,   /* Line numbers stripped from file. */
        F_LSYMS             = 0x0008,   /* Local symbols stripped from file. */
        F_MINMAL            = 0x0010,   /* !Minimal object file (”.m”) output of fextract. */
        F_UPDATE            = 0x0020,   /* !Fully bound update file, output of ogen. */
        F_SWABD             = 0x0040,   /* !File whose bytes were swabbed (in names). */
        F_AR16WR            = 0x0080,   /* !File has the byte ordering of an AR16WR. (e.g.11/70) machine (it was created there, or was produced by conv). */
        F_AR32WR            = 0x0100,   /* !File has the byte ordering of an AR32WR machine (e.g. vax). */
        F_AR32W             = 0x0200,   /* !File has the byte ordering of an AR32W machine (e.g. 3b,maxi,MC68000). */
        F_PATCH             = 0x0400,   /* !File contains “patch” list in Optional Header. */
        F_NODF              = 0x0400,   /* !(Minimal file only) no decision functions for replaced functions. */
        F_MIPS_NO_SHARED    = 010000,   /* Cannot be dynamically shared. */
        F_MIPS_SHARABLE     = 020000,   /* A dynamically shared object. */
        F_MIPS_CALL_SHARED  = 030000,   /* Dynamic executable. */
        F_MIPS_NO_REORG     = 040000,   /* Do not reorder sections. */
        F_MIPS_UGEN_ALLOC   = 0100000,  /* File whose procedures contain pre-allocated space on the stack that makes conversion to PIC easier. */
    };

    enum Flags
    {
        STYP_REG        = 0x00,         /* Regular section; allocated, relocated, loaded. */
        STYP_DSECT      = 0x01,         /* !Dummy; not allocated, relocated, not loaded. */
        STYP_NOLOAD     = 0x02,         /* !Noload; allocated, relocated, not loaded. */
        STYP_GROUP      = 0x04,         /* !Grouped; formed of input sections. */
        STYP_PAD        = 0x08,         /* !Padding; not allocated, not relocated, loaded. */
        STYP_COPY       = 0x10,         /* !Copy; for decision function used by field update; not allocated, not relocated, loaded; relocated, and line number entries processed normally. */
        STYP_TEXT       = 0x20,         /* Text only. */
        STYP_DATA       = 0x40,         /* Data only. */
        STYP_BSS        = 0x80,         /* Contains bss only. */
        STYP_RDATA      = 0x100,        /* Read only data only. */
        STYP_SDATA      = 0x200,        /* Small data only. */
        STYP_SBSS       = 0x400,        /* Contains small bss only. */
        STYP_UCODE      = 0x800,        /* Section contains ucode only. */
        STYP_GOT        = 0x1000,       /* Global offset table. */
        STYP_DYNAMIC    = 0x2000,       /* Dynamic linking information section. */
        STYP_DYNSYM     = 0x4000,       /* Dynamic linking symbol table. */
        STYP_REL_DYN    = 0x8000,       /* Relocation information for runtime linker. */
        STYP_DYNSTR     = 0x10000,      /* Dynamic linking strings. */
        STYP_HASH       = 0x20000,      /* Symbol has table. */
        STYP_DSOLIST    = 0x40000,      /* Dynamic shared object list table. */
        STYP_CONFLICT   = 0x100000,     /* Additional linking information. */
        STYP_FINI       = 0x1000000,    /* .fini section text. */
        STYP_COMMENT    = 0x2100000,    /* Comment section. */
        STYP_LIT8       = 0x8000000,    /* Section 8 byte literals only. */
        STYP_LIT4       = 0x10000000,   /* Section 4 byte literals only. */
        S_NRELOC_OVFL   = 0x20000000,   /* s_nreloc overflowed, the value is in r_vaddr of the first entry. */
        STYP_LIB        = 0x40000000,   /* Section contains shared library information only. */
        STYP_INIT       = 0x80000000,   /* Section initialization text only. */
    };

    static const char* GetMagic(uint16_t magic);
};
