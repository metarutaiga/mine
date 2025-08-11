#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
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

bool PE::Load(const char* path,
              uint8_t*(*mmap)(size_t, size_t, void*), void* mmap_data,
              size_t(*sym)(const char*, const char*, size_t, void*), void* sym_data,
              int(*log)(const char*, ...))
{
    if (path == nullptr)
        return false;

    FILE* file = fopen(path, "rb");
    if (file == nullptr)
        return false;

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Read PE offset from MZ
    uint32_t mz[16] = {};
    fread(mz, 4, 16, file);
    uint32_t lfanew = mz[15];

    bool succeed = false;
    SectionHeader* sections = nullptr;
    switch (NULL) case NULL: {
        if (lfanew >= size || fseek(file, lfanew, SEEK_SET) != 0) {
            log("the PE offset is out of range");
            break;
        }
        int32_t signature = 0;
        if (fread(&signature, sizeof(int32_t), 1, file) != 1 || signature != 0x4550) {
            log("the signature is not PE");
            break;
        }

        // File Header
        FileHeader fileHeader = {};
        if (fread(&fileHeader, sizeof(FileHeader), 1, file) != 1) {
            log("get the PE/COFF File Header is failed");
            break;
        }
        log("%-12s : %s", "f_magic", GetMagic(fileHeader.f_magic));
        log("%-12s : %d", "f_nscns", fileHeader.f_nscns);
        log("%-12s : 0x%08x", "f_timdat", fileHeader.f_timdat);
        log("%-12s : 0x%08x", "f_symptr", fileHeader.f_symptr);
        log("%-12s : %d", "f_nsyms", fileHeader.f_nsyms);
        log("%-12s : %d", "f_opthdr", fileHeader.f_opthdr);
        log("%-12s : 0x%04x", "f_flags", fileHeader.f_flags);

        // Optional Header
        OptionalHeader optionalHeader = {};
        if (sizeof(optionalHeader) != fileHeader.f_opthdr || fread(&optionalHeader, sizeof(OptionalHeader), 1, file) != 1) {
            log("get the PE/COFF Optional Header is failed");
            break;
        }
        log("%-12s : 0x%08x", "BaseOfCode", optionalHeader.BaseOfCode);
        log("%-12s : 0x%08x", "EntryPoint", optionalHeader.AddressOfEntryPoint);
        log("%-12s : 0x%08x", "ImageBase", optionalHeader.ImageBase);

        // Section Header
        sections = new SectionHeader[fileHeader.f_nscns]{};
        if (sections == nullptr) {
            log("out of memory");
            break;
        }
        if (fread(sections, sizeof(SectionHeader), fileHeader.f_nscns, file) != fileHeader.f_nscns) {
            log("get the PE/COFF Section Header is failed");
            break;
        }
        for (uint16_t i = 0; i < fileHeader.f_nscns; ++i) {
            const SectionHeader& section = sections[i];
            log("%-12s : %d %08X %08X %s", "Section", i, section.s_vaddr, section.s_size, section.s_name);
        }

        // Load sections to memory
        log("%-12s : 0x%08x", "Base", optionalHeader.ImageBase);
        log("%-12s : 0x%x", "Size", optionalHeader.SizeOfImage);
        uint8_t* image = mmap(optionalHeader.DataDirectory[5].Size ? 0 : optionalHeader.ImageBase, optionalHeader.SizeOfImage, mmap_data);
        if (image) {
            memset(image, 0, optionalHeader.SizeOfImage);

            // Relocation
            void* memory = mmap(0, 0, mmap_data);
            uint32_t old_base = optionalHeader.ImageBase;
            uint32_t new_base = (uint32_t)(image - (uint8_t*)memory);

            // Entry
            if (optionalHeader.AddressOfEntryPoint) {
                sym(nullptr, "Entry", new_base + optionalHeader.AddressOfEntryPoint, sym_data);
            }
            else {
                sym(nullptr, "Entry", new_base + optionalHeader.BaseOfCode, sym_data);
            }

            // Section
            void* reloc = nullptr;
            for (uint16_t i = 0; i < fileHeader.f_nscns; ++i) {
                const SectionHeader& section = sections[i];
                if (section.s_size == 0)
                    continue;
                if (fseek(file, section.s_scnptr, SEEK_SET) != 0) {
                    log("fseek failed");
                    break;
                }
                void* to = nullptr;
                if (section.s_flags & (IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE)) {
                    to = image + section.s_vaddr;
                }
                if (strcmp(section.s_name, ".reloc") == 0) {
                    to = reloc = realloc(reloc, section.s_size);
                }
                if (to == nullptr)
                    continue;
                if (fread(to, section.s_size, 1, file) != 1) {
                    log("get the PE/COFF Section Image is failed");
                    break;
                }
            }

            // Export
            if (optionalHeader.DataDirectory[0].Size) {
                auto& directory = *(ExportDirectory*)(image + optionalHeader.DataDirectory[0].VirtualAddress);
                for (uint32_t i = 0; i < directory.NumberOfFunctions; ++i) {
                    auto address = *(uint32_t*)(image + directory.AddressOfFunctions + sizeof(uint32_t) * i);
                    auto pointer = *(uint32_t*)(image + directory.AddressOfNames + sizeof(uint32_t) * i);
                    auto* name = (const char*)(image + pointer);
                    sym(nullptr, name, optionalHeader.ImageBase + address, sym_data);
                }
            }

            // Import
            if (optionalHeader.DataDirectory[1].Size) {
                auto* directory = (ImportDirectory*)(image + optionalHeader.DataDirectory[1].VirtualAddress);
                while (directory->Name && directory->FirstThunk) {
                    auto file = (const char*)(image + directory->Name);
                    auto* thunk = (uint32_t*)(image + directory->FirstThunk);
                    while (*thunk) {
                        auto name = (const char*)(image + (*thunk)) + 2;
                        size_t symbol = sym(file, name, 0, sym_data);
                        memcpy(thunk, &symbol, sizeof(uint32_t));
                        thunk++;
                    }
                    directory++;
                }
            }

            // Relocation
            if (reloc) {
                Relocate(image, reloc, old_base, new_base, log);
                free(reloc);
            }
        }

        succeed = true;
        log("succeed");
    }

    delete[] sections;
    fclose(file);
    return succeed;
}

void PE::Relocate(void* image, void* reloc, uint32_t from, uint32_t to, int(*log)(const char*, ...))
{
    auto image8 = (uint8_t*)image;
    auto reloc8 = (uint8_t*)reloc;
    for (;;) {
        auto& relocation = *(ImageBaseRelocation*)reloc8;
        if (relocation.SizeOfBlock == 0)
            break;
        auto* entries = (uint16_t*)(reloc8 + sizeof(ImageBaseRelocation));
        reloc8 += relocation.SizeOfBlock;
        for (uint32_t i = 0; i < relocation.SizeOfBlock; ++i) {
            auto entry = entries[i];
            auto type = entry >> 12;
            auto offset = entry & 0x0FFF;
            auto address = relocation.VirtualAddress + offset;
            switch (type) {
            case IMAGE_REL_BASED_ABSOLUTE:
                break;
            case IMAGE_REL_BASED_HIGHLOW:
                *(uint32_t*)(image8 + address) += to - from;
                break;
            default:
                log("%02X:%04X (%08X)", type, offset, address);
                break;
            }
        }
    }
}
