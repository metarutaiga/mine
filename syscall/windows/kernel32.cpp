#define _CRT_SECURE_NO_WARNINGS
#include <stdint.h>
#include <sys/stat.h>
#include "../allocator.h"
#include "../syscall.h"
#include "../syscall_internal.h"
#include "../../format/coff/pe.h"
#include "../../x86/x86_i386.h"
#include "../../x86/x86_register.inl"
#include "../../x86/x86_instruction.inl"
#include "windows.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define strcasecmp _stricmp
static char* strcasestr(char const* haystack, char const* needle)
{
    for (size_t length = strlen(needle); (*haystack); haystack++)
        if (_strnicmp(haystack, needle, length) == 0)
            return (char*)haystack;
    return NULL;
}
#else
#include <fnmatch.h>
#include <unistd.h>
#include <sys/dir.h>
#pragma pack(push, 1)
typedef uint64_t FILETIME;
typedef uint64_t LARGE_INTEGER;
struct OSVERSIONINFOA {
    uint32_t    dwOSVersionInfoSize;
    uint32_t    dwMajorVersion;
    uint32_t    dwMinorVersion;
    uint32_t    dwBuildNumber;
    uint32_t    dwPlatformId;
    char        szCSDVersion[128];
};
struct SYSTEMTIME {
    uint16_t    wYear;
    uint16_t    wMonth;
    uint16_t    wDayOfWeek;
    uint16_t    wDay;
    uint16_t    wHour;
    uint16_t    wMinute;
    uint16_t    wSecond;
    uint16_t    wMilliseconds;
};
struct WIN32_FIND_DATAA {
    uint32_t    dwFileAttributes;
    uint64_t    ftCreationTime;
    uint64_t    ftLastAccessTime;
    uint64_t    ftLastWriteTime;
    uint32_t    nFileSizeHigh;
    uint32_t    nFileSizeLow;
    uint32_t    dwReserved0;
    uint32_t    dwReserved1;
    char        cFileName[260];
    char        cAlternateFileName[14];
};
#pragma pack(pop)
#endif

#define SYSCALL_INVALID_HANDLE_VALUE size_t(int64_t(-1))

#ifdef __cplusplus
extern "C" {
#endif

// Atomic

uint32_t syscall_InterlockedCompareExchange(uint8_t* memory, const uint32_t* stack)
{
    auto Destination = physical(uint32_t*, stack[1]);
    auto ExChange = stack[2];
    auto Comperand = stack[3];
    auto result = (*Destination);
    if ((*Destination) == Comperand) {
        (*Destination) = ExChange;
    }
    return result;
}

uint32_t syscall_InterlockedDecrement(uint8_t* memory, const uint32_t* stack)
{
    auto Target = physical(uint32_t*, stack[1]);
    (*Target)--;
    return (*Target);
}

uint32_t syscall_InterlockedExchange(uint8_t* memory, const uint32_t* stack)
{
    auto Target = physical(uint32_t*, stack[1]);
    auto Value = stack[2];
    auto result = (*Target);
    (*Target) = Value;
    return result;
}

uint32_t syscall_InterlockedIncrement(uint8_t* memory, const uint32_t* stack)
{
    auto Target = physical(uint32_t*, stack[1]);
    (*Target)++;
    return (*Target);
}

// Directory

uint32_t syscall_GetCurrentDirectoryA(uint8_t* memory, const uint32_t* stack)
{
    auto nBufferLength = stack[1];
    auto lpBuffer = physical(char*, stack[2]);

    auto* windows = physical(Windows*, TIB_WINDOWS);
    strncpy(lpBuffer, windows->directory, nBufferLength);

    return uint32_t(strlen(lpBuffer));
}

bool syscall_SetCurrentDirectoryA(uint8_t* memory, const uint32_t* stack)
{
    char* lpPathName = physical(char*, stack[1]);

    auto* windows = physical(Windows*, TIB_WINDOWS);
    strncpy(windows->directory, lpPathName, 260);

#ifndef _WIN32
    for (char& c : windows->directory) {
        if (c == '/')
            c = '\\';
    }
#endif

    auto* printf = physical(Printf*, offset_printf);
    if (printf->debugPrintf) {
        printf->debugPrintf("[CALL] %s -> %s", "SetCurrentDirectoryA", windows->directory);
    }
    return true;
}

// Environment

bool syscall_FreeEnvironmentStringsA(uint8_t* memory, const uint32_t* stack, struct allocator_t* allocator)
{
    auto penv = physical(char*, stack[1]);
    allocator->deallocate(penv);
    return true;
}

bool syscall_FreeEnvironmentStringsW(uint8_t* memory, const uint32_t* stack, struct allocator_t* allocator)
{
    auto penv = physical(char16_t*, stack[1]);
    allocator->deallocate(penv);
    return true;
}

size_t syscall_GetEnvironmentStrings(struct allocator_t* allocator)
{
    char* empty = (char*)allocator->allocate(sizeof(char) * 2);
    if (empty == nullptr)
        return 0;
    uint8_t* memory = (uint8_t*)allocator->address();

    empty[0] = 0;
    return virtual(size_t, empty);
}

size_t syscall_GetEnvironmentStringsW(struct allocator_t* allocator)
{
    char16_t* empty = (char16_t*)allocator->allocate(sizeof(char16_t) * 2);
    if (empty == nullptr)
        return 0;
    uint8_t* memory = (uint8_t*)allocator->address();

    empty[0] = 0;
    return virtual(size_t, empty);
}

size_t syscall_GetEnvironmentVariableA(const uint32_t* stack, struct allocator_t* allocator)
{
    char* empty = (char*)allocator->allocate(sizeof(char) * 2);
    if (empty == nullptr)
        return 0;
    uint8_t* memory = (uint8_t*)allocator->address();

    empty[0] = 0;
    return virtual(size_t, empty);
}

size_t syscall_GetEnvironmentVariableW(const uint32_t* stack, struct allocator_t* allocator)
{
    char16_t* empty = (char16_t*)allocator->allocate(sizeof(char16_t) * 2);
    if (empty == nullptr)
        return 0;
    uint8_t* memory = (uint8_t*)allocator->address();

    empty[0] = 0;
    return virtual(size_t, empty);
}

// File

bool syscall_CloseHandle(uint8_t* memory, const uint32_t* stack, struct allocator_t* allocator)
{
    auto hObject = physical(FILE**, stack[1]);
    if (hObject && *hObject) {
        fclose(*hObject);

        auto& record = *physical(std::vector<FILE*>*, offset_file);
        for (FILE*& file : record) {
            if (file == (*hObject)) {
                file = record.back();
                record.pop_back();
                break;
            }
        }
    }
    allocator->deallocate(hObject);
    return true;
}

size_t syscall_CreateFileA(uint8_t* memory, const uint32_t* stack, struct allocator_t* allocator)
{
    auto lpFileName = physical(char*, stack[1]);
    auto dwDesiredAccess = stack[2];
//  auto dwShareMode = stack[3];
//  auto lpSecurityAttributes = physical(void*, stack[4]);
    auto dwCreationDisposition = stack[5];
//  auto dwFlagsAndAttributes = stack[6];
//  auto hTemplateFile = physical(FILE**, stack[7]);

    std::string path;
    if (lpFileName[0] != '/' && lpFileName[0] != '\\') {
        auto* windows = physical(Windows*, TIB_WINDOWS);
        path = windows->directory;
        path += '\\';
    }
    path += lpFileName;
#ifndef _WIN32
    for (char& c : path) {
        if (c == '\\')
            c = '/';
    }
#endif

    struct stat st;
    const char* mode = nullptr;
    switch (dwDesiredAccess & 0xC0000000) {
    default:                        return SYSCALL_INVALID_HANDLE_VALUE;
    case 0x40000000: mode = "wb";   break;
    case 0x80000000: mode = "rb";   break;
    case 0xC0000000: mode = "wb+";  break;
    }

    switch (dwCreationDisposition) {
    case 1:
        if (stat(path.c_str(), &st) == 0)
            return SYSCALL_INVALID_HANDLE_VALUE;
        break;
    case 2:
        break;
    case 3:
        if (stat(path.c_str(), &st) != 0)
            return SYSCALL_INVALID_HANDLE_VALUE;
        break;
    case 4:
        break;
    case 5:
        if (stat(path.c_str(), &st) != 0)
            return SYSCALL_INVALID_HANDLE_VALUE;
        break;
    }

    auto handle = (FILE**)allocator->allocate(sizeof(FILE*));
    if (handle == nullptr)
        return SYSCALL_INVALID_HANDLE_VALUE;
    memory = (uint8_t*)allocator->address();

    (*handle) = fopen(path.c_str(), mode);
    if ((*handle) == nullptr) {
        allocator->deallocate(handle);
        return SYSCALL_INVALID_HANDLE_VALUE;
    }

    auto& record = *physical(std::vector<FILE*>*, offset_file);
    record.push_back(*handle);

    return virtual(size_t, handle);
}

size_t syscall_CreateFileMappingA(uint8_t* memory, const uint32_t* stack, struct allocator_t* allocator)
{
    auto hFile = physical(FILE**, stack[1]);
//  auto lpFileMappingAttributes = physical(void*, stack[2]);
//  auto flProtect = stack[3];
//  auto dwMaximumSizeHigh = stack[4];
//  auto dwMaximumSizeLow = stack[5];
//  auto lpName = physical(char*, stack[6]);
    return virtual(size_t, hFile);
}

uint32_t syscall_GetFileSize(uint8_t* memory, const uint32_t* stack)
{
    auto hFile = physical(FILE**, stack[1]);
    auto lpFileSizeHigh = physical(uint32_t*, stack[2]);

    auto file = (*hFile);
    long pos = ftell(file);
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, pos, SEEK_SET);

    if (lpFileSizeHigh)
        (*lpFileSizeHigh) = uint32_t(size >> 32);

    return uint32_t(size);
}

uint32_t syscall_GetFullPathNameA(uint8_t* memory, const uint32_t* stack)
{
    auto lpFileName = physical(char*, stack[1]);
    auto nBufferLength = stack[2];
    auto lpBuffer = physical(char*, stack[3]);
    auto lpFilePart = physical(uint32_t*, stack[4]);

    if (lpBuffer == nullptr) {
        return uint32_t(strlen(lpFileName) + 1);
    }

    if (lpFilePart) {
        (*lpFilePart) = virtual(uint32_t, lpBuffer);
    }

    for (uint32_t i = 0; i < nBufferLength; ++i) {
        char c = lpFileName[i];
        lpBuffer[i] = c;
        if (c == 0)
            return i;
        if ((c == '/' || c == '\\') && lpFilePart) {
            (*lpFilePart) = virtual(uint32_t, lpBuffer + i + 1);
        }
    }
    return nBufferLength;
}

size_t syscall_MapViewOfFile(uint8_t* memory, const uint32_t* stack, struct allocator_t* allocator)
{
    auto hFileMappingObject = physical(FILE**, stack[1]);
//  auto dwDesiredAccess = stack[2];
//  auto dwFileOffsetHigh = stack[3];
    auto dwFileOffsetLow = stack[4];
    auto dwNumberOfBytesToMap = stack[5];

    auto file = (*hFileMappingObject);
    long pos = ftell(file);
    if (dwNumberOfBytesToMap == 0) {
        fseek(file, 0, SEEK_END);
        dwNumberOfBytesToMap = (uint32_t)ftell(file);
        fseek(file, pos, SEEK_SET);
    }

    auto map = allocator->allocate(dwNumberOfBytesToMap);
    if (map == nullptr)
        return 0;
    memory = (uint8_t*)allocator->address();

    fseek(file, dwFileOffsetLow, SEEK_SET);
    fread(map, 1, dwNumberOfBytesToMap, file);
    fseek(file, pos, SEEK_SET);

    return virtual(size_t, map);
}

bool syscall_UnmapViewOfFile(uint8_t* memory, const uint32_t* stack, struct allocator_t* allocator)
{
    auto lpBaseAddress = physical(void*, stack[1]);
    allocator->deallocate(lpBaseAddress);
    return true;
}

// Find

bool syscall_FindClose(uint8_t* memory, const uint32_t* stack, struct allocator_t* allocator)
{
#if defined(_WIN32)
    auto hFindFile = physical(HANDLE*, stack[1]);
    auto result = FindClose(*hFindFile);
#else
    auto hFindFile = physical(DIR**, stack[1]);
    auto result = closedir(*hFindFile);
#endif
    allocator->deallocate(hFindFile);
    return result ? false : true;
}

bool syscall_FindNextFileA(uint8_t* memory, const uint32_t* stack)
{
#if defined(_WIN32)
    auto hFindFile = physical(HANDLE*, stack[1]);
    auto lpFindFileData = physical(WIN32_FIND_DATAA*, stack[2]);
    return FindNextFileA(*hFindFile, lpFindFileData);
#else
    auto hFindFile = physical(DIR**, stack[1]);
    auto lpFindFileData = physical(WIN32_FIND_DATAA*, stack[2]);
    auto pattern = (char*)(hFindFile + 1);

    dirent* dirent = nullptr;
    while ((dirent = readdir(*hFindFile)) != nullptr) {
        if (pattern[0] && fnmatch(pattern, dirent->d_name, FNM_CASEFOLD) == 0)
            break;
    }
    if (dirent == nullptr)
        return false;

    switch (dirent->d_type) {
    case DT_DIR: lpFindFileData->dwFileAttributes = 0x010; break;
    case DT_REG: lpFindFileData->dwFileAttributes = 0x080; break;
    case DT_LNK: lpFindFileData->dwFileAttributes = 0x400; break;
    }
    lpFindFileData->ftCreationTime = 0;
    lpFindFileData->ftLastAccessTime = 0;
    lpFindFileData->ftLastWriteTime = 0;
    lpFindFileData->nFileSizeHigh = 0;
    lpFindFileData->nFileSizeLow = 0;
    strncpy(lpFindFileData->cFileName, dirent->d_name, 260);
    strncpy(lpFindFileData->cAlternateFileName, dirent->d_name, 14);

    return true;
#endif
}

size_t syscall_FindFirstFileA(uint32_t* stack, struct allocator_t* allocator)
{
#if defined(_WIN32)
    auto handle = (HANDLE*)allocator->allocate(sizeof(HANDLE));
    if (handle == nullptr)
        return SYSCALL_INVALID_HANDLE_VALUE;
    uint8_t* memory = (uint8_t*)allocator->address();

    auto lpFileName = physical(char*, stack[1]);
    auto lpFindFileData = physical(WIN32_FIND_DATAA*, stack[2]);
    (*handle) = FindFirstFileA(lpFileName, lpFindFileData);
    if ((*handle) == INVALID_HANDLE_VALUE) {
        allocator->deallocate(handle);
        return SYSCALL_INVALID_HANDLE_VALUE;
    }
    return virtual(size_t, handle);
#else
    auto dir = (DIR**)allocator->allocate(sizeof(DIR*) + sizeof(char) * 120);
    if (dir == nullptr)
        return SYSCALL_INVALID_HANDLE_VALUE;
    uint8_t* memory = (uint8_t*)allocator->address();

    auto lpFileName = physical(char*, stack[1]);

    std::string path;
    if (lpFileName[0] != '/' && lpFileName[0] != '\\') {
        auto* windows = physical(Windows*, TIB_WINDOWS);
        path = windows->directory;
        path += '\\';
    }
    path += lpFileName;
#ifndef _WIN32
    for (char& c : path) {
        if (c == '\\')
            c = '/';
    }
#endif
    auto slash = path.find_last_of("/\\");

    auto* printf = physical(Printf*, offset_printf);
    if (printf->debugPrintf) {
        printf->debugPrintf("[CALL] %s - %s", "FindFirstFileA", path.c_str());
    }

    (*dir) = opendir(path.substr(0, slash).c_str());
    if ((*dir) == nullptr) {
        allocator->deallocate(dir);
        return SYSCALL_INVALID_HANDLE_VALUE;
    }

    auto pattern = (char*)(dir + 1);
    strncpy(pattern, path.substr(slash + 1).c_str(), 120);

    stack[1] = virtual(uint32_t, dir);
    if (syscall_FindNextFileA(memory, stack) == false) {
        allocator->deallocate(dir);
        return SYSCALL_INVALID_HANDLE_VALUE;
    }
    return virtual(size_t, dir);
#endif
}

// Heap

size_t syscall_HeapAlloc(const uint32_t* stack, struct allocator_t* allocator)
{
//  auto hHeap = stack[1];
    auto dwFlags = stack[2];
    auto dwBytes = stack[3];
    void* lpMem = allocator->allocate(dwBytes);
    if (lpMem == nullptr)
        return 0;
    uint8_t* memory = (uint8_t*)allocator->address();

    if (dwFlags & 0x8) {
        memset(lpMem, 0, dwBytes);
    }
    return virtual(size_t, lpMem);
}

bool syscall_HeapFree(uint8_t* memory, const uint32_t* stack, struct allocator_t* allocator)
{
//  auto hHeap = stack[1];
//  auto dwFlags = stack[2];
    auto lpMem = physical(void*, stack[3]);
    allocator->deallocate(lpMem);
    return true;
}

size_t syscall_HeapReAlloc(uint8_t* memory, const uint32_t* stack, struct allocator_t* allocator)
{
//  auto hHeap = stack[1];
    auto dwFlags = stack[2];
    auto lpMem = stack[3];
    auto dwBytes = stack[4];
    auto dwOldBytes = allocator->size(physical(void*, lpMem));
    if (dwOldBytes >= dwBytes)
        return lpMem;
    auto lpNewMem = allocator->allocate(dwBytes);
    if (lpNewMem == nullptr)
        return 0;
    memory = (uint8_t*)allocator->address();

    if (dwFlags & 0x8) {
        memset(lpNewMem, 0, dwBytes);
    }
    if (lpMem) {
        memcpy(lpNewMem, physical(void*, lpMem), dwBytes < dwOldBytes ? dwBytes : dwOldBytes);
        allocator->deallocate(physical(void*, lpMem));
    }
    return virtual(size_t, lpNewMem);
}

// Library

int syscall_FreeLibrary(uint8_t* memory, const uint32_t* stack)
{
    return 0;
}

static size_t syscall_GetModuleFileName(uint8_t* memory, size_t module, char* lpFilename, uint32_t nSize, int nWidth, bool bBase)
{
    auto* printf = physical(Printf*, offset_printf);
    auto* windows = physical(Windows*, TIB_WINDOWS);
    void* hModule = physical(void*, module);

    const char* found = nullptr;
    if (hModule == nullptr) {
        found = windows->imageDirectory;
    }
    else {
        for (auto& [name, image] : windows->modules) {
            if (image == module) {
                found = name.c_str();
                break;
            }
        }
    }

    if (found) {
        if (bBase) {
            auto* slash = strrchr(found, '/');
            if (slash == nullptr)
                slash = strrchr(found, '\\');
            if (slash) {
                found = slash + 1;
            }
            if (printf->debugPrintf) {
                printf->debugPrintf("[CALL] %s - %08X %s", "GetModuleBaseName", hModule, found);
            }
        }
        else {
            if (printf->debugPrintf) {
                printf->debugPrintf("[CALL] %s - %08X %s", "GetModuleFileName", hModule, found);
            }
        }
        for (size_t i = 0; i < nSize; ++i) {
            char c = found[i];
            (*lpFilename++) = c;
            for (int i = 1; i < nWidth; ++i) {
                (*lpFilename++) = 0;
            }
            if (c == 0) {
                return i;
            }
        }
    }
    else {
        for (int i = 0; i < nWidth; ++i) {
            (*lpFilename++) = 0;
        }
    }
    return 0;
}

size_t syscall_GetModuleBaseNameA(uint8_t* memory, const uint32_t* stack)
{
//  auto hProcess = stack[1];
    auto hModule = stack[2];
    auto lpBaseName = physical(char*, stack[3]);
    auto nSize = stack[4];
    return syscall_GetModuleFileName(memory, hModule, lpBaseName, nSize, sizeof(char), true);
}

size_t syscall_GetModuleBaseNameW(uint8_t* memory, const uint32_t* stack)
{
//  auto hProcess = stack[1];
    auto hModule = stack[2];
    auto lpBaseName = physical(char*, stack[3]);
    auto nSize = stack[4];
    return syscall_GetModuleFileName(memory, hModule, lpBaseName, nSize, sizeof(char16_t), true);
}

size_t syscall_GetModuleFileNameA(uint8_t* memory, const uint32_t* stack)
{
    auto hModule = stack[1];
    auto lpFilename = physical(char*, stack[2]);
    auto nSize = stack[3];
    return syscall_GetModuleFileName(memory, hModule, lpFilename, nSize, sizeof(char), false);
}

size_t syscall_GetModuleFileNameW(uint8_t* memory, const uint32_t* stack)
{
    auto hModule = stack[1];
    auto lpFilename = physical(char*, stack[2]);
    auto nSize = stack[3];
    return syscall_GetModuleFileName(memory, hModule, lpFilename, nSize, sizeof(char16_t), false);
}

static size_t syscall_GetModuleHandle(uint8_t* memory, const char* lpModuleName)
{
    auto* windows = physical(Windows*, TIB_WINDOWS);

    if (lpModuleName == nullptr)
        return windows->image;

    std::string path = lpModuleName;
    auto slash = path.find_last_of("/\\");

    path = path.substr(slash + 1);
    for (auto& [name, image] : windows->modules) {
        if (strcasestr(name.c_str(), path.c_str()))
            return image;
    }

    if (strcasestr(path.c_str(), "advapi32.dll") ||
        strcasestr(path.c_str(), "kernel32.dll") ||
        strcasestr(path.c_str(), "api-ms-win-")) {
        return 0xFFFFFFFF;
    }
    return 0;
}

size_t syscall_GetModuleHandleA(uint8_t* memory, const uint32_t* stack)
{
    auto lpModuleName = physical(char*, stack[1]);
    return syscall_GetModuleHandle(memory, lpModuleName);
}

size_t syscall_GetModuleHandleW(uint8_t* memory, const uint32_t* stack)
{
    std::string name;

    auto lpModuleName = physical(char16_t*, stack[1]);
    if (lpModuleName) {
        for (char16_t c; (c = *lpModuleName); lpModuleName++) {
            name.push_back(c);
        }
    }
    return syscall_GetModuleHandle(memory, name.empty() ? nullptr : name.c_str());
}

bool syscall_GetModuleHandleExA(uint8_t* memory, const uint32_t* stack)
{
    auto lpModuleName = physical(char*, stack[2]);
    auto phModule = physical(uint32_t*, stack[3]);
    (*phModule) = uint32_t(syscall_GetModuleHandle(memory, lpModuleName));
    return (*phModule) != 0;
}

bool syscall_GetModuleHandleExW(uint8_t* memory, const uint32_t* stack)
{
    std::string name;

    auto lpModuleName = physical(char16_t*, stack[2]);
    if (lpModuleName) {
        for (char16_t c; (c = *lpModuleName); lpModuleName++) {
            name.push_back(c);
        }
    }

    auto phModule = physical(uint32_t*, stack[3]);
    (*phModule) = uint32_t(syscall_GetModuleHandle(memory, name.empty() ? nullptr : name.c_str()));
    return (*phModule) != 0;
}

size_t syscall_GetProcAddress(uint8_t* memory, const uint32_t* stack)
{
    if (stack[1] == 0 || stack[2] == 0)
        return 0;
    auto hModule = physical(void*, stack[1]);
    auto lpProcName = physical(char*, stack[2]);

    auto* printf = physical(Printf*, offset_printf);

    struct Data {
        size_t address;
        const char* name;
        int(*printf)(const char*, ...);
    } data = {
        .name = lpProcName,
        .printf = printf->debugPrintf,
    };

    if (stack[1] == 0xFFFFFFFF) {
        auto* windows = physical(Windows*, TIB_WINDOWS);
        data.address = windows->symbol("", lpProcName, nullptr);
    }
    else {
        PE::Exports(hModule, [](const char* name, size_t address, void* sym_data) {
            auto& data = *(Data*)sym_data;
            if (strcmp(data.name, name) == 0) {
                data.address = address;
                if (data.printf) {
                    data.printf("%-12s : [%08zX] %s", "Symbol", address, name);
                }
            }
        }, &data);
    }

    if (data.address == 0) {
        if (data.printf) {
            data.printf("%-12s : %s is not found", "Symbol", lpProcName);
        }
    }
    return data.address;
}

static size_t syscall_LoadLibrary(uint8_t* memory, const char* lpLibFileName, x86_i386* cpu)
{
    std::string path;
    if (lpLibFileName[0] != '/' && lpLibFileName[0] != '\\') {
        auto* windows = physical(Windows*, TIB_WINDOWS);
        path = windows->directory;
        path += '\\';
    }
    path += lpLibFileName;
#ifndef _WIN32
    for (char& c : path) {
        if (c == '\\')
            c = '/';
    }
#endif

    auto* printf = physical(Printf*, offset_printf);
    if (printf->debugPrintf) {
        printf->debugPrintf("[CALL] %s - %s", "LoadLibraryA", lpLibFileName);
    }

    void* image = PE::Load(path.c_str(), [](size_t base, size_t size, void* userdata) {
        mine* cpu = (mine*)userdata;
        return cpu->Memory(base, size);
    }, cpu, printf->debugPrintf);
    if (image == nullptr)
        return 0;

    // MEMORY-SYNC
    memory = cpu->Memory();
    size_t module = virtual(size_t, image);
    auto* windows = physical(Windows*, TIB_WINDOWS);

    // _DllMainCRTStartup - pre
    auto& x86 = cpu->x86;

    //                  _DllMainCRTStartup
    //                  _DllMainCRTStartup
    //                  _DllMainCRTStartup
    //                  hDllHandle
    //                  dwReason
    //                  lpreserved
    //                  $$$$$$$$
    //                  hDllHandle
    //                  dwReason
    //                  lpreserved
    // LoadLibraryA     LoadLibraryA
    // lpLibFileName    lpLibFileName
    uint32_t ret;

    // 58 POP EAX
    // 58 POP EAX
    // C3 RET
    // C3 RET
    ret = Pop32();
    Pop32();
    Push32(ret);
    Push32(module);
    Push32(0xC3C35858);
    uint32_t eax = ESP;
    Push32(eax);

    void syscall_windows_import(void* data, const char* file, void* image, bool execute);
    windows->modules.emplace_back(path, module);
    syscall_windows_import(cpu, path.c_str(), image, false);

    // _DllMainCRTStartup - post
    ret = Pop32();
    Push32(ret);
    Push32(ret);

    return module;
}

size_t syscall_LoadLibraryA(uint8_t* memory, const uint32_t* stack, x86_i386* cpu)
{
    size_t module = syscall_GetModuleHandleA(memory, stack);
    if (module)
        return module;

    auto lpLibFileName = physical(char*, stack[1]);
    return syscall_LoadLibrary(memory, lpLibFileName, cpu);
}

size_t syscall_LoadLibraryW(uint8_t* memory, const uint32_t* stack, x86_i386* cpu)
{
    size_t module = syscall_GetModuleHandleW(memory, stack);
    if (module)
        return module;

    std::string name;

    auto lpLibFileName = physical(char16_t*, stack[1]);
    if (lpLibFileName) {
        for (char16_t c; (c = *lpLibFileName); lpLibFileName++) {
            name.push_back(c);
        }
    }
    return syscall_LoadLibrary(memory, name.empty() ? nullptr : name.c_str(), cpu);
}

// Memory

size_t syscall_LocalAlloc(const uint32_t* stack, struct allocator_t* allocator)
{
    auto uFlags = stack[1];
    auto uBytes = stack[2];

    void* pointer = allocator->allocate(uBytes);
    if (pointer == nullptr)
        return 0;
    uint8_t* memory = (uint8_t*)allocator->address();

    if (uFlags & 0x0040) {
        memset(pointer, 0, uBytes);
    }
    return virtual(size_t, pointer);
}

size_t syscall_LocalFree(uint8_t* memory, const uint32_t* stack, struct allocator_t* allocator)
{
    auto hMem = physical(void*, stack[1]);
    allocator->deallocate(hMem);
    return 0;
}

size_t syscall_VirtualAlloc(uint8_t* memory, const uint32_t* stack, struct allocator_t* allocator)
{
    auto lpAddress = stack[1];
    auto dwSize = stack[2];
    auto flAllocationType = stack[3];
//  auto flProtect = stack[4];

    if (flAllocationType & (0x1000 | 0x2000)) {
        void* address = allocator->allocate(dwSize, lpAddress ? lpAddress : SIZE_MAX);
        memory = (uint8_t*)allocator->address();
        lpAddress = virtual(uint32_t, address);
    }

    if (lpAddress == 0)
        return 0;
    if (allocator->size(physical(void*, lpAddress)) < dwSize)
        return 0;

    if (flAllocationType & 0x80000) {
        memset(physical(void*, lpAddress), 0, dwSize);
    }
    return lpAddress;
}

bool syscall_VirtualFree(uint8_t* memory, const uint32_t* stack, struct allocator_t* allocator)
{
    auto lpAddress = physical(void*, stack[1]);
//  auto dwSize = stack[2];
    auto dwFreeType = stack[3];

    if (dwFreeType & 0x4000) {
        allocator->deallocate(lpAddress);
    }
    return true;
}

// String

int syscall_MultiByteToWideChar(uint8_t* memory, const uint32_t* stack)
{
//  auto CodePage = stack[1];
//  auto dwFlags = stack[2];
    auto lpMultiByteStr = physical(char*, stack[3]);
    auto cbMultiByte = stack[4];
    auto lpWideCharStr = physical(char16_t*, stack[5]);
    auto cchWideChar = stack[6];

    if (cbMultiByte == -1)
        cbMultiByte = (uint32_t)strlen(lpMultiByteStr) + 1;
    if (cchWideChar == 0)
        return cbMultiByte;

    if (lpWideCharStr) {
        uint32_t i = 0;
        for (i = 0; i < cbMultiByte && i < cchWideChar; ++i) {
            lpWideCharStr[i] = lpMultiByteStr[i];
        }
        return i;
    }
    return 0;
}

int syscall_WideCharToMultiByte(uint8_t* memory, const uint32_t* stack)
{
//  auto CodePage = stack[1];
//  auto dwFlags = stack[2];
    auto lpWideCharStr = physical(char16_t*, stack[3]);
    auto cchWideChar = stack[4];
    auto lpMultiByteStr = physical(char*, stack[5]);
    auto cbMultiByte = stack[6];
//  auto lpDefaultChar = stack[7];
//  auto lpUsedDefaultChar = stack[8];

    if (cchWideChar == -1)
        cchWideChar = (uint32_t)std::char_traits<char16_t>::length(lpWideCharStr) + 1;
    if (cbMultiByte == 0)
        return cchWideChar;

    if (lpMultiByteStr) {
        uint32_t i = 0;
        for (i = 0; i < cbMultiByte && i < cchWideChar; ++i) {
            lpMultiByteStr[i] = lpWideCharStr[i];
        }
        return i;
    }
    return 0;
}

// System

size_t syscall_GetCommandLineA(uint8_t* memory)
{
    auto* windows = physical(Windows*, TIB_WINDOWS);
    return virtual(size_t, windows->commandLineA);
}

size_t syscall_GetCommandLineW(uint8_t* memory)
{
    auto* windows = physical(Windows*, TIB_WINDOWS);
    return virtual(size_t, windows->commandLineW);
}

int syscall_GetCurrentProcessId()
{
#if defined(_WIN32)
    return GetCurrentProcessId();
#else
    return getpid();
#endif
}

int syscall_GetCurrentThreadId()
{
#if defined(_WIN32)
    return GetCurrentThreadId();
#else
    return getpid();
#endif
}

int syscall_GetSystemInfo(uint8_t* memory, const uint32_t* stack)
{
    auto* tib = (TIB*)memory;
    auto lpSystemInfo = physical(SystemInfo*, stack[1]);
    lpSystemInfo->dwOemId = 0;
    lpSystemInfo->dwPageSize = 4096;
    lpSystemInfo->lpMinimumApplicationAddress = 4096;
    lpSystemInfo->lpMaximumApplicationAddress = tib->StackLimit;
    lpSystemInfo->dwActiveProcessorMask = 1;
    lpSystemInfo->dwNumberOfProcessors = 1;
    lpSystemInfo->dwProcessorType = 386;
    lpSystemInfo->dwAllocationGranularity = 65536;
    lpSystemInfo->wProcessorLevel = 3;
    lpSystemInfo->wProcessorRevision = 0;
    return 0;
}

int syscall_GetStartupInfoA(uint8_t* memory, const uint32_t* stack)
{
    auto lpStartupInfo = physical(int32_t*, stack[1]);
    lpStartupInfo[1] = 0;   // lpReserved
    lpStartupInfo[2] = 0;   // lpDesktop
    lpStartupInfo[3] = 0;   // lpTitle
    lpStartupInfo[4] = 0;   // dwX
    lpStartupInfo[5] = 0;   // dwY
    lpStartupInfo[6] = 0;   // dwXSize
    lpStartupInfo[7] = 0;   // dwYSize
    lpStartupInfo[8] = 0;   // dwXCountChars
    lpStartupInfo[9] = 0;   // dwYCountChars
    lpStartupInfo[10] = 0;  // dwFillAttribute
    lpStartupInfo[11] = 0;  // dwFlags
    lpStartupInfo[12] = 0;  // wShowWindow / cbReserved2
    lpStartupInfo[13] = 0;  // lpReserved2
    lpStartupInfo[14] = 0;  // hStdInput
    lpStartupInfo[15] = 0;  // hStdOutput
    lpStartupInfo[16] = 0;  // hStdError
    return 0;
}

int syscall_GetStartupInfoW(uint8_t* memory, const uint32_t* stack)
{
    auto lpStartupInfo = physical(int32_t*, stack[1]);
    lpStartupInfo[1] = 0;   // lpReserved
    lpStartupInfo[2] = 0;   // lpDesktop
    lpStartupInfo[3] = 0;   // lpTitle
    lpStartupInfo[4] = 0;   // dwX
    lpStartupInfo[5] = 0;   // dwY
    lpStartupInfo[6] = 0;   // dwXSize
    lpStartupInfo[7] = 0;   // dwYSize
    lpStartupInfo[8] = 0;   // dwXCountChars
    lpStartupInfo[9] = 0;   // dwYCountChars
    lpStartupInfo[10] = 0;  // dwFillAttribute
    lpStartupInfo[11] = 0;  // dwFlags
    lpStartupInfo[12] = 0;  // wShowWindow / cbReserved2
    lpStartupInfo[13] = 0;  // lpReserved2
    lpStartupInfo[14] = 0;  // hStdInput
    lpStartupInfo[15] = 0;  // hStdOutput
    lpStartupInfo[16] = 0;  // hStdError
    return 0;
}

int syscall_GetVersion()
{
    return 5;
}

bool syscall_GetVersionExA(uint8_t* memory, const uint32_t* stack)
{
    auto lpVersionInformation = physical(OSVERSIONINFOA*, stack[1]);
    lpVersionInformation->dwMajorVersion = 5;
    lpVersionInformation->dwMinorVersion = 0;
    lpVersionInformation->dwBuildNumber = 0;
    lpVersionInformation->dwPlatformId = 1;
    lpVersionInformation->szCSDVersion[0] = 0;
    return true;
}

bool syscall_InitOnceExecuteOnce(uint8_t* memory, const uint32_t* stack, x86_i386* cpu)
{
    auto& x86 = cpu->x86;

    auto ret = Pop32();
    auto InitOnce = Pop32();
    auto InitFn = Pop32();
    auto Parameter = Pop32();
    auto Context = Pop32();

    Push32(Context);
    Push32(Parameter);
    Push32(InitOnce);
    Push32(ret);
    Push32(Context);
    Push32(Parameter);
    Push32(InitFn);
    Push32(InitOnce);
    Push32(InitFn);

    return true;
}

int syscall_OutputDebugStringA(uint8_t* memory, const uint32_t* stack)
{
    auto lpOutputString = physical(char*, stack[1]);

    auto* printf = physical(Printf*, offset_printf);
    if (printf->debugPrintf) {
        printf->debugPrintf("%s", lpOutputString);
    }
    return 0;
}

// Time

int syscall_GetSystemTime(uint8_t* memory, const uint32_t* stack)
{
    auto lpSystemTime = physical(SYSTEMTIME*, stack[1]);
#if defined(_WIN32)
    GetSystemTime(lpSystemTime);
#else
    time_t t = time(nullptr);
    struct tm* tm = gmtime(&t);

    lpSystemTime->wYear = tm->tm_year + 1900;
    lpSystemTime->wMonth = tm->tm_mon + 1;
    lpSystemTime->wDayOfWeek = tm->tm_wday;
    lpSystemTime->wDay = tm->tm_mday;
    lpSystemTime->wHour = tm->tm_hour;
    lpSystemTime->wMinute = tm->tm_min;
    lpSystemTime->wSecond = tm->tm_sec;
    lpSystemTime->wMilliseconds = 0;
#endif
    return 0;
}

int syscall_GetSystemTimeAsFileTime(uint8_t* memory, const uint32_t* stack)
{
    auto lpSystemTimeAsFileTime = physical(FILETIME*, stack[1]);
#if defined(_WIN32)
    GetSystemTimeAsFileTime(lpSystemTimeAsFileTime);
#else
    struct timespec ts = {};
    clock_gettime(CLOCK_REALTIME, &ts);

    (*lpSystemTimeAsFileTime) = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
#endif
    return 0;
}

uint64_t syscall_GetTickCount64()
{
#if defined(_WIN32)
    return GetTickCount64();
#else
    struct timespec ts = {};
    clock_gettime(CLOCK_REALTIME, &ts);

    return uint64_t(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#endif
}

bool syscall_QueryPerformanceCounter(uint8_t* memory, const uint32_t* stack)
{
    auto lpPerformanceCount = physical(LARGE_INTEGER*, stack[1]);
#if defined(_WIN32)
    return QueryPerformanceCounter(lpPerformanceCount);
#else
    struct timespec ts = {};
    clock_gettime(CLOCK_REALTIME, &ts);

    (*lpPerformanceCount) = ts.tv_sec * 1'000'000'000ull + ts.tv_nsec;
    return true;
#endif
}

bool syscall_QueryPerformanceFrequency(uint8_t* memory, const uint32_t* stack)
{
    auto lpFrequency = physical(LARGE_INTEGER*, stack[1]);
#if defined(_WIN32)
    return QueryPerformanceFrequency(lpFrequency);
#else
    (*lpFrequency) = 1'000'000'000ull;
    return true;
#endif
}

// TLS

uint32_t syscall_TlsAlloc(uint8_t* memory)
{
    auto* windows = physical(Windows*, TIB_WINDOWS);
    auto& tls = windows->tls[windows->tlsIndex++];
    tls = 0;
    return virtual(uint32_t, &tls);
}

bool syscall_TlsFree(uint8_t* memory, const uint32_t* stack)
{
    return true;
}

uint32_t syscall_TlsGetValue(uint8_t* memory, const uint32_t* stack)
{
    auto& tls = *physical(uint32_t*, stack[1]);
    return tls;
}

bool syscall_TlsSetValue(uint8_t* memory, const uint32_t* stack)
{
    auto& tls = *physical(uint32_t*, stack[1]);
    tls = stack[2];
    return true;
}

#ifdef __cplusplus
}
#endif
