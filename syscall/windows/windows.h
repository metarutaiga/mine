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
