#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mz.h"
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

void* PE::Load(const char* path, uint8_t*(*mmap)(size_t, size_t, void*), void* mmap_data, int(*log)(const char*, ...))
{
    if (path == nullptr || mmap == nullptr || log == nullptr)
        return nullptr;

    FILE* file = fopen(path, "rb");
    if (file == nullptr)
        return nullptr;

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Read PE offset from MZ
    MZ::FileHeader mz = {};
    fread(&mz, sizeof(MZ::FileHeader), 1, file);

    uint8_t* image = nullptr;
    SectionHeader* sections = nullptr;
    switch (NULL) case NULL: {
        if (mz.e_lfanew >= size || fseek(file, mz.e_lfanew, SEEK_SET) != 0) {
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
        image = mmap(optionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size ? 0 : optionalHeader.ImageBase, optionalHeader.SizeOfImage, mmap_data);
        if (image == nullptr) {
            log("out of memory");
            break;
        }
        memset(image, 0, optionalHeader.SizeOfImage);

        // Relocation
        void* memory = mmap(0, 0, mmap_data);
        uint32_t old_base = optionalHeader.ImageBase;
        uint32_t new_base = (uint32_t)(image - (uint8_t*)memory);
        if (old_base != new_base) {
            log("%-12s : 0x%08x", "Base", new_base);
            optionalHeader.ImageBase = new_base;
        }

        // Header
        memcpy(image, &signature, sizeof(int32_t));
        memcpy(image + sizeof(int32_t), &fileHeader, sizeof(FileHeader));
        memcpy(image + sizeof(int32_t) + sizeof(FileHeader), &optionalHeader, sizeof(OptionalHeader));

        // Section
        bool finish = true;
        void* reloc = nullptr;
        for (uint16_t i = 0; i < fileHeader.f_nscns; ++i) {
            const SectionHeader& section = sections[i];
            if (section.s_size == 0)
                continue;
            void* to = nullptr;
            if (section.s_flags & (IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE)) {
                to = image + section.s_vaddr;
            }
            if (strcmp(section.s_name, ".reloc") == 0) {
                to = reloc = realloc(reloc, section.s_size);
            }
            if (to == nullptr)
                continue;
            finish = false;
            if (fseek(file, section.s_scnptr, SEEK_SET) != 0) {
                log("fseek failed");
                break;
            }
            if (fread(to, section.s_size, 1, file) != 1) {
                log("get the PE/COFF Section Image is failed");
                break;
            }
            finish = true;
        }
        if (finish == false)
            break;

        // Relocation
        if (reloc) {
            Relocate(image, reloc, new_base - old_base, log);
            free(reloc);
        }

        log("succeed");
    }

    delete[] sections;
    fclose(file);
    return image;
}

size_t PE::Entry(void* image)
{
    if (image == nullptr)
        return 0;

    auto image8 = (uint8_t*)image;
    OptionalHeader& optionalHeader = *(OptionalHeader*)(image8 + sizeof(int32_t) + sizeof(FileHeader));
    if (optionalHeader.AddressOfEntryPoint) {
        return optionalHeader.ImageBase + optionalHeader.AddressOfEntryPoint;
    }

    return optionalHeader.ImageBase + optionalHeader.BaseOfCode;
}

void PE::Imports(void* image, size_t(*sym)(const char*, const char*, size_t, void*), void* sym_data, int(*log)(const char*, ...))
{
    if (image == nullptr || sym == nullptr)
        return;

    auto image8 = (uint8_t*)image;
    OptionalHeader& optionalHeader = *(OptionalHeader*)(image8 + sizeof(int32_t) + sizeof(FileHeader));
    if (optionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size) {
        auto* directory = (ImportDirectory*)(image8 + optionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
        while (directory->Name && directory->FirstThunk) {
            auto file = (const char*)(image8 + directory->Name);
            auto* ints = (uint32_t*)(image8 + directory->OriginalFirstThunk);
            auto* iats = (uint32_t*)(image8 + directory->FirstThunk);
            while (*ints) {
                auto name = (const char*)(image8 + (*ints)) + 2;
                size_t symbol = sym(file, name, 0, sym_data);
                memcpy(iats, &symbol, sizeof(uint32_t));
                if (symbol == 0) {
                    log("Symbol : [%08zX] %s.%s is not found", optionalHeader.ImageBase + (uint8_t*)iats - image8, file, name);
                }
                ints++;
                iats++;
            }
            directory++;
        }
    }
}

void PE::Exports(void* image, void(*sym)(const char*, size_t, void*), void* sym_data)
{
    if (image == nullptr || sym == nullptr)
        return;

    auto image8 = (uint8_t*)image;
    OptionalHeader& optionalHeader = *(OptionalHeader*)(image8 + sizeof(int32_t) + sizeof(FileHeader));
    if (optionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size) {
        auto& directory = *(ExportDirectory*)(image8 + optionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
        auto names = (uint32_t*)(image8 + directory.AddressOfNames);
        auto ordinals = (uint16_t*)(image8 + directory.AddressOfNameOrdinals);
        auto functions = (uint32_t*)(image8 + directory.AddressOfFunctions);
        for (uint32_t i = 0; i < directory.NumberOfNames; ++i) {
            auto name = (char*)(image8 + names[i]);
            auto address = functions[ordinals[i]];
            sym(name, optionalHeader.ImageBase + address, sym_data);
        }
    }
}

void PE::Relocate(void* image, void* reloc, size_t delta, int(*log)(const char*, ...))
{
    if (image == nullptr || reloc == nullptr || log == nullptr)
        return;

    auto image8 = (uint8_t*)image;
    auto relocation = (ImageBaseRelocation*)reloc;
    while (relocation->SizeOfBlock) {
        auto* types = (uint16_t*)relocation;
        for (uint32_t i = sizeof(ImageBaseRelocation) / sizeof(uint16_t); i < relocation->SizeOfBlock / sizeof(uint16_t); ++i) {
            auto type = types[i] >> 12;
            auto offset = types[i] & 0x0FFF;
            auto address = relocation->VirtualAddress + offset;
            switch (type) {
            case IMAGE_REL_BASED_ABSOLUTE:
                break;
            case IMAGE_REL_BASED_HIGHLOW:
                *(uint32_t*)(image8 + address) += delta;
                break;
            default:
                log("Relocate : %02X:%04X (%08X) Unknown", type, offset, address);
                break;
            }
        }
        relocation = (ImageBaseRelocation*)((uint8_t*)relocation + relocation->SizeOfBlock);
    }
}
