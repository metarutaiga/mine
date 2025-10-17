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
    char commandLineA[256];
    char16_t commandLineW[256];
    char directory[260];
    uint32_t image;
    char imageDirectory[260];
    std::vector<std::pair<std::string, uint32_t>> modules;
    uint32_t tls[64];
    uint32_t tlsIndex;
    size_t(*symbol)(const char*, const char*, void*);
    void(*debugModule)(const char*, void*);
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
