#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "pe.h"

#define _L(x) (x)
#define _B(x) ((x >> 8) | (x << 8) & 0xFFFF)

const char* PE::GetMagic(uint16_t magic)
{
    switch (magic) {
    case _L(0x0):       return "The content of this field is assumed to be applicable to any machine type";
    case _L(0x1):       return "Interacts with the host and not a WOW64 guest";
    case _L(0x14C):     return "Intel 386 or later processors and compatible processors";
    case _L(0x160):     return "MIPS I compatible 32-bit big endian";
    case _L(0x162):     return "MIPS I compatible 32-bit little endian";
    case _L(0x166):     return "MIPS III compatible 64-bit little endian";
    case _L(0x168):     return "MIPS IV compatible 64-bit little endian";
    case _L(0x169):     return "MIPS little-endian WCE v2";
    case _L(0x266):     return "MIPS16";
    case _L(0x366):     return "MIPS with FPU";
    case _L(0x466):     return "MIPS16 with FPU";
    case _L(0x184):     return "Alpha AXP, 32-bit address space";
    case _L(0x284):     return "Alpha 64, 64-bit address space";
    case _L(0x1A2):     return "Hitachi SH3";
    case _L(0x1A3):     return "Hitachi SH3 DSP";
    case _L(0x1A4):     return "Hitachi SH3E";
    case _L(0x1A6):     return "Hitachi SH4";
    case _L(0x1A8):     return "Hitachi SH5";
    case _L(0x1C0):     return "ARM little endian";
    case _L(0x1C2):     return "ARM Thumb little endian";
    case _L(0x1C4):     return "ARM Thumb-2 little endian";
    case _L(0x1D3):     return "Matsushita AM33";
    case _L(0x1F0):     return "Power PC little endian";
    case _L(0x1F1):     return "Power PC with floating point support";
    case _L(0x1F2):     return "Power PC big endian";
    case _L(0x200):     return "Intel Itanium processor family";
    case _L(0x520):     return "Infineon TriCore";
    case _L(0xCEF):     return "CEF";
    case _L(0xEBC):     return "EFI byte code";
    case _L(0x5032):    return "RISC-V 32-bit address space";
    case _L(0x5064):    return "RISC-V 64-bit address space";
    case _L(0x5128):    return "RISC-V 128-bit address space";
    case _L(0x6232):    return "LoongArch 32-bit processor family";
    case _L(0x6264):    return "LoongArch 64-bit processor family";
    case _L(0x8664):    return "x64";
    case _L(0x9041):    return "Mitsubishi M32R little endian";
    case _L(0xAA64):    return "ARM64 little endian";
    case _L(0xA641):    return "ABI that enables interoperability between native ARM64 and emulated x64 code";
    case _L(0xA64E):    return "Binary format that allows both native ARM64 and ARM64EC code to coexist in the same file";
    case _L(0xC0EE):    return "Common Execution Environment";
    }

    return "Unknown";
}

uint32_t PE::Load(const char* path, uint8_t*(*mmap)(uint32_t, uint32_t, void*), void* userdata)
{
    if (path == nullptr)
        return 0;

    FILE* file = fopen(path, "rb");
    if (file == nullptr)
        return 0;

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Read PE offset from MZ
    uint32_t mz[16] = {};
    fread(mz, 4, 16, file);
    uint32_t lfanew = mz[15];

    uint32_t entry = 0;
    SectionHeader* sections = nullptr;
    switch (NULL) case NULL: {
        if (lfanew >= size || fseek(file, lfanew, SEEK_SET) != 0) {
            printf("the PE offset is out of range\n");
            break;
        }
        int32_t signature = 0;
        if (fread(&signature, sizeof(int32_t), 1, file) != 1 || signature != 0x4550) {
            printf("the signature is not PE\n");
            break;
        }

        // File Header
        FileHeader fileHeader = {};
        if (fread(&fileHeader, sizeof(FileHeader), 1, file) != 1) {
            printf("get the PE/COFF File Header is failed\n");
            break;
        }
        printf("%-8s : %s\n", "f_magic", GetMagic(fileHeader.f_magic));
        printf("%-8s : %d\n", "f_nscns", fileHeader.f_nscns);
        printf("%-8s : 0x%08x\n", "f_timdat", fileHeader.f_timdat);
        printf("%-8s : 0x%08x\n", "f_symptr", fileHeader.f_symptr);
        printf("%-8s : %d\n", "f_nsyms", fileHeader.f_nsyms);
        printf("%-8s : %d\n", "f_opthdr", fileHeader.f_opthdr);
        printf("%-8s : 0x%04x\n", "f_flags", fileHeader.f_flags);

        // Optional Header
        OptionalHeader optionalHeader = {};
        if (sizeof(optionalHeader) != fileHeader.f_opthdr || fread(&optionalHeader, sizeof(OptionalHeader), 1, file) != 1) {
            printf("get the PE/COFF Optional Header is failed\n");
            break;
        }

        // Section Header
        sections = new SectionHeader[fileHeader.f_nscns];
        if (sections == nullptr) {
            printf("out of memory\n");
            break;
        }
        if (fread(sections, sizeof(SectionHeader), fileHeader.f_nscns, file) != fileHeader.f_nscns) {
            printf("get the PE/COFF Section Header is failed\n");
            break;
        }
        uint32_t min = UINT32_MAX;
        uint32_t max = 0;
        for (uint16_t i = 0; i < fileHeader.f_nscns; ++i) {
            const SectionHeader& section = sections[i];
//          printf("%d : %s\n", i, section.s_name);
            if (section.s_flags & (IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE)) {
                if (min > section.s_vaddr)
                    min = section.s_vaddr;
                if (max < section.s_vaddr + section.s_size)
                    max = section.s_vaddr + section.s_size;
            }
        }
        uint32_t base = optionalHeader.ImageBase + min;
        uint32_t size = max - min;
        printf("%-8s : 0x%08x\n", "Base", base);
        printf("%-8s : 0x%x\n", "Size", size);

        // Load sections to memory
        uint8_t* memory = mmap(base, size, userdata);
        if (memory) {
            memset(memory, 0, size);
            for (uint16_t i = 0; i < fileHeader.f_nscns; ++i) {
                const SectionHeader& section = sections[i];
                if (section.s_flags & (IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE)) {
                    if (fseek(file, section.s_scnptr, SEEK_SET) != 0) {
                        printf("fseek failed\n");
                        break;
                    }
                    if (fread(memory + section.s_vaddr - min, section.s_size, 1, file) != 1) {
                        printf("get the PE/COFF Section Image is failed\n");
                        break;
                    }
                }
            }
        }

        printf("succeed\n");
        if (optionalHeader.AddressOfEntryPoint) {
            entry = optionalHeader.ImageBase + optionalHeader.AddressOfEntryPoint;
        }
        else {
            entry = optionalHeader.ImageBase + optionalHeader.BaseOfCode;
        }
    }

    delete[] sections;
    fclose(file);
    return entry;
}
