#pragma once

#include <stddef.h>
#include "coff.h"

struct PE : public COFF
{
    enum MagicNumber
    {
        IMAGE_FILE_MACHINE_UNKNOWN      = 0x0,      /* The content of this field is assumed to be applicable to any machine type */
        IMAGE_FILE_MACHINE_TARGET_HOST  = 0x1,      /* Interacts with the host and not a WOW64 guest */
        IMAGE_FILE_MACHINE_I386         = 0x14C,    /* Intel 386 or later processors and compatible processors */
        IMAGE_FILE_MACHINE_R3000BE      = 0x160,    /* MIPS I compatible 32-bit big endian */
        IMAGE_FILE_MACHINE_R3000        = 0x162,    /* MIPS I compatible 32-bit little endian */
        IMAGE_FILE_MACHINE_R4000        = 0x166,    /* MIPS III compatible 64-bit little endian */
        IMAGE_FILE_MACHINE_R10000       = 0x168,    /* MIPS IV compatible 64-bit little endian */
        IMAGE_FILE_MACHINE_WCEMIPSV2    = 0x169,    /* MIPS little-endian WCE v2 */
        IMAGE_FILE_MACHINE_MIPS16       = 0x266,    /* MIPS16 */
        IMAGE_FILE_MACHINE_MIPSFPU      = 0x366,    /* MIPS with FPU */
        IMAGE_FILE_MACHINE_MIPSFPU16    = 0x466,    /* MIPS16 with FPU */
        IMAGE_FILE_MACHINE_ALPHA        = 0x184,    /* Alpha AXP, 32-bit address space */
        IMAGE_FILE_MACHINE_ALPHA64      = 0x284,    /* Alpha 64, 64-bit address space */
        IMAGE_FILE_MACHINE_AXP64        = 0x284,    /* AXP 64 (Same as Alpha 64) */
        IMAGE_FILE_MACHINE_SH3          = 0x1A2,    /* Hitachi SH3 */
        IMAGE_FILE_MACHINE_SH3DSP       = 0x1A3,    /* Hitachi SH3 DSP */
        IMAGE_FILE_MACHINE_SH3E         = 0x1A4,    /* Hitachi SH3E */
        IMAGE_FILE_MACHINE_SH4          = 0x1A6,    /* Hitachi SH4 */
        IMAGE_FILE_MACHINE_SH5          = 0x1A8,    /* Hitachi SH5 */
        IMAGE_FILE_MACHINE_ARM          = 0x1C0,    /* ARM little endian */
        IMAGE_FILE_MACHINE_THUMB        = 0x1C2,    /* ARM Thumb little endian */
        IMAGE_FILE_MACHINE_ARMNT        = 0x1C4,    /* ARM Thumb-2 little endian */
        IMAGE_FILE_MACHINE_AM33         = 0x1D3,    /* Matsushita AM33 */
        IMAGE_FILE_MACHINE_POWERPC      = 0x1F0,    /* Power PC little endian */
        IMAGE_FILE_MACHINE_POWERPCFP    = 0x1F1,    /* Power PC with floating point support */
        IMAGE_FILE_MACHINE_POWERPCBE    = 0x1F2,    /* Power PC big endian */
        IMAGE_FILE_MACHINE_IA64         = 0x200,    /* Intel Itanium processor family */
        IMAGE_FILE_MACHINE_TRICORE      = 0x520,    /* Infineon TriCore */
        IMAGE_FILE_MACHINE_CEF          = 0xCEF,    /* CEF */
        IMAGE_FILE_MACHINE_EBC          = 0xEBC,    /* EFI byte code */
        IMAGE_FILE_MACHINE_RISCV32      = 0x5032,   /* RISC-V 32-bit address space */
        IMAGE_FILE_MACHINE_RISCV64      = 0x5064,   /* RISC-V 64-bit address space */
        IMAGE_FILE_MACHINE_RISCV128     = 0x5128,   /* RISC-V 128-bit address space */
        IMAGE_FILE_MACHINE_LOONGARCH32  = 0x6232,   /* LoongArch 32-bit processor family */
        IMAGE_FILE_MACHINE_LOONGARCH64  = 0x6264,   /* LoongArch 64-bit processor family */
        IMAGE_FILE_MACHINE_AMD64        = 0x8664,   /* x64 */
        IMAGE_FILE_MACHINE_M32R         = 0x9041,   /* Mitsubishi M32R little endian */
        IMAGE_FILE_MACHINE_ARM64        = 0xAA64,   /* ARM64 little endian */
        IMAGE_FILE_MACHINE_ARM64EC      = 0xA641,   /* ABI that enables interoperability between native ARM64 and emulated x64 code. */
        IMAGE_FILE_MACHINE_ARM64X       = 0xA64E,   /* Binary format that allows both native ARM64 and ARM64EC code to coexist in the same file. */
        IMAGE_FILE_MACHINE_CEE          = 0xC0EE,   /* Common Execution Environment */
    };

    enum FileHeaderFlags
    {
        IMAGE_FILE_RELOCS_STRIPPED          = 0x0001,   /* Image only, Windows CE, and Microsoft Windows NT and later. This indicates that the file does not contain base relocations and must therefore be loaded at its preferred base address. If the base address is not available, the loader reports an error. The default behavior of the linker is to strip base relocations from executable (EXE) files. */
        IMAGE_FILE_EXECUTABLE_IMAGE         = 0x0002,   /* Image only. This indicates that the image file is valid and can be run. If this flag is not set, it indicates a linker error. */
        IMAGE_FILE_LINE_NUMS_STRIPPED       = 0x0004,   /* COFF line numbers have been removed. This flag is deprecated and should be zero. */
        IMAGE_FILE_LOCAL_SYMS_STRIPPED      = 0x0008,   /* COFF symbol table entries for local symbols have been removed. This flag is deprecated and should be zero. */
        IMAGE_FILE_AGGRESSIVE_WS_TRIM       = 0x0010,   /* Obsolete. Aggressively trim working set. This flag is deprecated for Windows 2000 and later and must be zero. */
        IMAGE_FILE_LARGE_ADDRESS_AWARE      = 0x0020,   /* Application can handle > 2-GB addresses. */
        IMAGE_FILE_BYTES_REVERSED_LO        = 0x0080,   /* Little endian: the least significant bit (LSB) precedes the most significant bit (MSB) in memory. This flag is deprecated and should be zero. */
        IMAGE_FILE_32BIT_MACHINE            = 0x0100,   /* Machine is based on a 32-bit-word architecture. */
        IMAGE_FILE_DEBUG_STRIPPED           = 0x0200,   /* Debugging information is removed from the image file. */
        IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP  = 0x0400,   /* If the image is on removable media, fully load it and copy it to the swap file. */
        IMAGE_FILE_NET_RUN_FROM_SWAP        = 0x0800,   /* If the image is on network media, fully load it and copy it to the swap file. */
        IMAGE_FILE_SYSTEM                   = 0x1000,   /* The image file is a system file, not a user program. */
        IMAGE_FILE_DLL                      = 0x2000,   /* The image file is a dynamic-link library (DLL). Such files are considered executable files for almost all purposes, although they cannot be directly run. */
        IMAGE_FILE_UP_SYSTEM_ONLY           = 0x4000,   /* The file should be run only on a uniprocessor machine. */
        IMAGE_FILE_BYTES_REVERSED_HI        = 0x8000,   /* Big endian: the MSB precedes the LSB in memory. This flag is deprecated and should be zero. */
    };

    enum SectionHeaderFlags
    {
        IMAGE_SCN_TYPE_NO_PAD               = 0x00000008,   /* The section should not be padded to the next boundary. This flag is obsolete and is replaced by IMAGE_SCN_ALIGN_1BYTES. */
        IMAGE_SCN_CNT_CODE                  = 0x00000020,   /* The section contains executable code. */
        IMAGE_SCN_CNT_INITIALIZED_DATA      = 0x00000040,   /* The section contains initialized data. */
        IMAGE_SCN_CNT_UNINITIALIZED_DATA    = 0x00000080,   /* The section contains uninitialized data. */
        IMAGE_SCN_LNK_OTHER                 = 0x00000100,   /* Reserved. */
        IMAGE_SCN_LNK_INFO                  = 0x00000200,   /* The section contains comments or other information. This is valid only for object files. */
        IMAGE_SCN_LNK_REMOVE                = 0x00000800,   /* The section will not become part of the image. This is valid only for object files. */
        IMAGE_SCN_LNK_COMDAT                = 0x00001000,   /* The section contains COMDAT data. This is valid only for object files. */
        IMAGE_SCN_NO_DEFER_SPEC_EXC         = 0x00004000,   /* Reset speculative exceptions handling bits in the TLB entries for this section. */
        IMAGE_SCN_GPREL                     = 0x00008000,   /* The section contains data referenced through the global pointer. */
        IMAGE_SCN_MEM_PURGEABLE             = 0x00020000,   /* Reserved. */
        IMAGE_SCN_MEM_LOCKED                = 0x00040000,   /* Reserved. */
        IMAGE_SCN_MEM_PRELOAD               = 0x00080000,   /* Reserved. */
        IMAGE_SCN_ALIGN_1BYTES              = 0x00100000,   /* Align data on a 1-byte boundary. This is valid only for object files. */
        IMAGE_SCN_ALIGN_2BYTES              = 0x00200000,   /* Align data on a 2-byte boundary. This is valid only for object files. */
        IMAGE_SCN_ALIGN_4BYTES              = 0x00300000,   /* Align data on a 4-byte boundary. This is valid only for object files. */
        IMAGE_SCN_ALIGN_8BYTES              = 0x00400000,   /* Align data on a 8-byte boundary. This is valid only for object files. */
        IMAGE_SCN_ALIGN_16BYTES             = 0x00500000,   /* Align data on a 16-byte boundary. This is valid only for object files. */
        IMAGE_SCN_ALIGN_32BYTES             = 0x00600000,   /* Align data on a 32-byte boundary. This is valid only for object files. */
        IMAGE_SCN_ALIGN_64BYTES             = 0x00700000,   /* Align data on a 64-byte boundary. This is valid only for object files. */
        IMAGE_SCN_ALIGN_128BYTES            = 0x00800000,   /* Align data on a 128-byte boundary. This is valid only for object files. */
        IMAGE_SCN_ALIGN_256BYTES            = 0x00900000,   /* Align data on a 256-byte boundary. This is valid only for object files. */
        IMAGE_SCN_ALIGN_512BYTES            = 0x00A00000,   /* Align data on a 512-byte boundary. This is valid only for object files. */
        IMAGE_SCN_ALIGN_1024BYTES           = 0x00B00000,   /* Align data on a 1024-byte boundary. This is valid only for object files. */
        IMAGE_SCN_ALIGN_2048BYTES           = 0x00C00000,   /* Align data on a 2048-byte boundary. This is valid only for object files. */
        IMAGE_SCN_ALIGN_4096BYTES           = 0x00D00000,   /* Align data on a 4096-byte boundary. This is valid only for object files. */
        IMAGE_SCN_ALIGN_8192BYTES           = 0x00E00000,   /* Align data on a 8192-byte boundary. This is valid only for object files. */
        IMAGE_SCN_LNK_NRELOC_OVFL           = 0x01000000,   /* The section contains extended relocations. The count of relocations for the section exceeds the 16 bits that is reserved for it in the section header. If the NumberOfRelocations field in the section header is 0xffff, the actual relocation count is stored in the VirtualAddress field of the first relocation. It is an error if IMAGE_SCN_LNK_NRELOC_OVFL is set and there are fewer than 0xffff relocations in the section. */
        IMAGE_SCN_MEM_DISCARDABLE           = 0x02000000,   /* The section can be discarded as needed. */
        IMAGE_SCN_MEM_NOT_CACHED            = 0x04000000,   /* The section cannot be cached. */
        IMAGE_SCN_MEM_NOT_PAGED             = 0x08000000,   /* The section cannot be paged. */
        IMAGE_SCN_MEM_SHARED                = 0x10000000,   /* The section can be shared in memory. */
        IMAGE_SCN_MEM_EXECUTE               = 0x20000000,   /* The section can be executed as code. */
        IMAGE_SCN_MEM_READ                  = 0x40000000,   /* The section can be read. */
        IMAGE_SCN_MEM_WRITE                 = 0x80000000,   /* The section can be written to. */
    };

    enum ImageDirectoryTypes
    {
        IMAGE_DIRECTORY_ENTRY_EXPORT        = 0,    /* Export Directory */
        IMAGE_DIRECTORY_ENTRY_IMPORT        = 1,    /* Import Directory */
        IMAGE_DIRECTORY_ENTRY_RESOURCE      = 2,    /* Resource Directory */
        IMAGE_DIRECTORY_ENTRY_EXCEPTION     = 3,    /* Exception Directory */
        IMAGE_DIRECTORY_ENTRY_SECURITY      = 4,    /* Security Directory */
        IMAGE_DIRECTORY_ENTRY_BASERELOC     = 5,    /* Base Relocation Table */
        IMAGE_DIRECTORY_ENTRY_DEBUG         = 6,    /* Debug Directory */
        IMAGE_DIRECTORY_ENTRY_COPYRIGHT     = 7,    /* (X86 usage) */
        IMAGE_DIRECTORY_ENTRY_ARCHITECTURE  = 7,    /* Architecture Specific Data */
        IMAGE_DIRECTORY_ENTRY_GLOBALPTR     = 8,    /* RVA of GP */
        IMAGE_DIRECTORY_ENTRY_TLS           = 9,    /* TLS Directory */
        IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG   = 10,   /* Load Configuration Directory */
        IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT  = 11,   /* Bound Import Directory in headers */
        IMAGE_DIRECTORY_ENTRY_IAT           = 12,   /* Import Address Table */
        IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT  = 13,   /* Delay Load Import Descriptors */
        IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR= 14,   /* COM Runtime descriptor */
    };

    enum BaseRelocationTypes
    {
        IMAGE_REL_BASED_ABSOLUTE            = 0,    /* The base relocation is skipped. This type can be used to pad a block. */
        IMAGE_REL_BASED_HIGH                = 1,    /* The base relocation adds the high 16 bits of the difference to the 16-bit field at offset. The 16-bit field represents the high value of a 32-bit word. */
        IMAGE_REL_BASED_LOW                 = 2,    /* The base relocation adds the low 16 bits of the difference to the 16-bit field at offset. The 16-bit field represents the low half of a 32-bit word. */
        IMAGE_REL_BASED_HIGHLOW             = 3,    /* The base relocation applies all 32 bits of the difference to the 32-bit field at offset. */
        IMAGE_REL_BASED_HIGHADJ             = 4,    /* The base relocation adds the high 16 bits of the difference to the 16-bit field at offset. The 16-bit field represents the high value of a 32-bit word. The low 16 bits of the 32-bit value are stored in the 16-bit word that follows this base relocation. This means that this base relocation occupies two slots. */
        IMAGE_REL_BASED_MIPS_JMPADDR        = 5,    /* The base relocation applies to a MIPS jump instruction. */
        IMAGE_REL_BASED_ARM_MOV32           = 5,    /* The base relocation applies the 32-bit address of a symbol across a consecutive MOVW/MOVT instruction pair. */
        IMAGE_REL_BASED_RISCV_HIGH20        = 5,    /* The base relocation applies to the high 20 bits of a 32-bit absolute address. */
        IMAGE_REL_BASED_THUMB_MOV32         = 7,    /* The base relocation applies the 32-bit address of a symbol to a consecutive MOVW/MOVT instruction pair. */
        IMAGE_REL_BASED_RISCV_LOW12I        = 7,    /* The base relocation applies to the low 12 bits of a 32-bit absolute address formed in RISC-V I-type instruction format. */
        IMAGE_REL_BASED_RISCV_LOW12S        = 8,    /* The base relocation applies to the low 12 bits of a 32-bit absolute address formed in RISC-V S-type instruction format. */
        IMAGE_REL_BASED_LOONGARCH32_MARK_LA = 8,    /* The base relocation applies to a 32-bit absolute address formed in two consecutive instructions. */
        IMAGE_REL_BASED_LOONGARCH64_MARK_LA = 8,    /* The base relocation applies to a 64-bit absolute address formed in four consecutive instructions. */
        IMAGE_REL_BASED_MIPS_JMPADDR16      = 9,    /* The base relocation applies to a MIPS16 jump instruction. */
        IMAGE_REL_BASED_DIR64               = 10,   /* The base relocation applies the difference to the 64-bit field at offset. */
    };

    struct DataDirectory
    {
        uint32_t        VirtualAddress;
        uint32_t        Size;
    };

    struct OptionalHeader
    {
        uint16_t        Magic;
        uint8_t         MajorLinkerVersion;
        uint8_t         MinorLinkerVersion;
        uint32_t        SizeOfCode;
        uint32_t        SizeOfInitializedData;
        uint32_t        SizeOfUninitializedData;
        uint32_t        AddressOfEntryPoint;
        uint32_t        BaseOfCode;
        uint32_t        BaseOfData;
        uint32_t        ImageBase;
        uint32_t        SectionAlignment;
        uint32_t        FileAlignment;
        uint16_t        MajorOperatingSystemVersion;
        uint16_t        MinorOperatingSystemVersion;
        uint16_t        MajorImageVersion;
        uint16_t        MinorImageVersion;
        uint16_t        MajorSubsystemVersion;
        uint16_t        MinorSubsystemVersion;
        uint32_t        Win32VersionValue;
        uint32_t        SizeOfImage;
        uint32_t        SizeOfHeaders;
        uint32_t        CheckSum;
        uint16_t        Subsystem;
        uint16_t        DllCharacteristics;
        uint32_t        SizeOfStackReserve;
        uint32_t        SizeOfStackCommit;
        uint32_t        SizeOfHeapReserve;
        uint32_t        SizeOfHeapCommit;
        uint32_t        LoaderFlags;
        uint32_t        NumberOfRvaAndSizes;
        DataDirectory   DataDirectory[16];
    };

    struct OptionalHeader64
    {
        uint16_t        Magic;
        uint8_t         MajorLinkerVersion;
        uint8_t         MinorLinkerVersion;
        uint32_t        SizeOfCode;
        uint32_t        SizeOfInitializedData;
        uint32_t        SizeOfUninitializedData;
        uint32_t        AddressOfEntryPoint;
        uint32_t        BaseOfCode;
        uint64_t        ImageBase;
        uint32_t        SectionAlignment;
        uint32_t        FileAlignment;
        uint16_t        MajorOperatingSystemVersion;
        uint16_t        MinorOperatingSystemVersion;
        uint16_t        MajorImageVersion;
        uint16_t        MinorImageVersion;
        uint16_t        MajorSubsystemVersion;
        uint16_t        MinorSubsystemVersion;
        uint32_t        Win32VersionValue;
        uint32_t        SizeOfImage;
        uint32_t        SizeOfHeaders;
        uint32_t        CheckSum;
        uint16_t        Subsystem;
        uint16_t        DllCharacteristics;
        uint64_t        SizeOfStackReserve;
        uint64_t        SizeOfStackCommit;
        uint64_t        SizeOfHeapReserve;
        uint64_t        SizeOfHeapCommit;
        uint32_t        LoaderFlags;
        uint32_t        NumberOfRvaAndSizes;
        DataDirectory   DataDirectory[16];
    };

    struct ExportDirectory
    {
        uint32_t        Characteristics;
        uint32_t        TimeDateStamp;
        uint16_t        MajorVersion;
        uint16_t        MinorVersion;
        uint32_t        Name;
        uint32_t        Base;
        uint32_t        NumberOfFunctions;
        uint32_t        NumberOfNames;
        uint32_t        AddressOfFunctions;
        uint32_t        AddressOfNames;
        uint32_t        AddressOfNameOrdinals;
    };

    struct ImportDirectory
    {
        uint32_t        OriginalFirstThunk;
        uint32_t        TimeDateStamp;
        uint32_t        ForwarderChain;
        uint32_t        Name;
        uint32_t        FirstThunk;
    };

    struct ImageBaseRelocation
    {
        uint32_t        VirtualAddress;
        uint32_t        SizeOfBlock;
    };

    static const char* GetMagic(uint16_t magic);
    static void* Load(const char* path, uint8_t*(*mmap)(size_t, size_t, void*), void* mmap_data, int(*log)(const char*, ...));
    static size_t Entry(void* image);
    static bool SectionCode(void* image, size_t* base, size_t* address, size_t* size);
    static void Imports(void* image, size_t(*sym)(const char*, const char*, void*), void* import_data, int(*log)(const char*, ...));
    static void Exports(void* image, void(*sym)(const char*, size_t, void*), void* export_data);
    static void Relocate(void* image, void* reloc, size_t delta, int(*log)(const char*, ...));
};
