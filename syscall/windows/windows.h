#pragma once

#include <string>
#include <vector>

#define TIB_WINDOWS 0x200

struct TIB {
    uint32_t ExceptionList;
    uint32_t StackBase;
    uint32_t StackLimit;
    uint32_t SubSystemTib;
    uint32_t Version;
    uint32_t ArbitraryUserPointer;
    uint32_t Self;
};

struct Windows {
    uint32_t image;
    std::vector<std::pair<std::string, void*>> modules;
    void (*loadLibraryCallback)(void*);
    char commandLine[256];
    char currentDirectory[260];
};

struct SystemInfo {
    uint32_t    dwOemId;
    uint32_t    dwPageSize;
    uint32_t    lpMinimumApplicationAddress;
    uint32_t    lpMaximumApplicationAddress;
    uint32_t    dwActiveProcessorMask;
    uint32_t    dwNumberOfProcessors;
    uint32_t    dwProcessorType;
    uint32_t    dwAllocationGranularity;
    uint16_t    wProcessorLevel;
    uint16_t    wProcessorRevision;
};
