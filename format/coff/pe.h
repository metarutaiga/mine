#pragma once

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

    static const char* GetMagic(uint16_t magic);
};
