#pragma once

#include <string>
#include <vector>

#define TIB_WINDOWS 0x700

struct Windows {
    uint32_t image;
    std::vector<std::pair<std::string, void*>> modules;
    void (*loadLibraryCallback)(void*);
    char commandLine[256];
    char currentDirectory[260];
};
