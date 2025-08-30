#define _CRT_SECURE_NO_WARNINGS
#include <stdint.h>
#include <sys/stat.h>
#include "windows.h"
#include "syscall/allocator.h"
#include "syscall/syscall.h"
#include "syscall/syscall_internal.h"
#include "format/coff/pe.h"
#include "x86/x86_i386.h"
#include "x86/x86_register.inl"
#include "x86/x86_instruction.inl"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define strcasecmp _stricmp
#else
#include <fnmatch.h>
#include <pthread.h>
#include <sys/dir.h>
static_assert(true);
#pragma pack(push, 1)
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

#ifdef __cplusplus
extern "C" {
#endif

// Critical Section

int syscall_DeleteCriticalSection(uint8_t* memory, const uint32_t* stack, struct allocator_t* allocator)
{
#if defined(_WIN32)
    auto lpCriticalSection = physical(CRITICAL_SECTION**, stack[1]);
    DeleteCriticalSection(*lpCriticalSection);
#else
    auto lpCriticalSection = physical(pthread_mutex_t**, stack[1]);
    pthread_mutex_destroy(*lpCriticalSection);
#endif
    allocator->deallocate(lpCriticalSection);
    return 0;
}

int syscall_EnterCriticalSection(uint8_t* memory, const uint32_t* stack)
{
#if defined(_WIN32)
    auto lpCriticalSection = physical(CRITICAL_SECTION**, stack[1]);
    EnterCriticalSection(*lpCriticalSection);
#else
    auto lpCriticalSection = physical(pthread_mutex_t**, stack[1]);
    pthread_mutex_lock(*lpCriticalSection);
#endif
    return 0;
}

int syscall_InitializeCriticalSection(uint8_t* memory, const uint32_t* stack, struct allocator_t* allocator)
{
#if defined(_WIN32)
    auto lpCriticalSection = physical(CRITICAL_SECTION**, stack[1]);

    auto mutex = (CRITICAL_SECTION*)allocator->allocate(sizeof(CRITICAL_SECTION));
    if (mutex == nullptr)
        return 0;

    InitializeCriticalSection(mutex);
#else
    auto lpCriticalSection = physical(pthread_mutex_t**, stack[1]);

    auto mutex = (pthread_mutex_t*)allocator->allocate(sizeof(pthread_mutex_t));
    if (mutex == nullptr)
        return 0;

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(mutex, &attr);
#endif
    (*lpCriticalSection) = mutex;
    return 0;
}

int syscall_LeaveCriticalSection(uint8_t* memory, const uint32_t* stack)
{
#if defined(_WIN32)
    auto lpCriticalSection = physical(CRITICAL_SECTION**, stack[1]);
    LeaveCriticalSection(*lpCriticalSection);
#else
    auto lpCriticalSection = physical(pthread_mutex_t**, stack[1]);
    pthread_mutex_unlock(*lpCriticalSection);
#endif
    return 0;
}

int syscall_TryEnterCriticalSection(uint8_t* memory, const uint32_t* stack)
{
#if defined(_WIN32)
    auto lpCriticalSection = physical(CRITICAL_SECTION**, stack[1]);
    return TryEnterCriticalSection(*lpCriticalSection);
#else
    auto lpCriticalSection = physical(pthread_mutex_t**, stack[1]);
    return pthread_mutex_trylock(*lpCriticalSection) ? false : true;
#endif
}

// Directory

int syscall_SetCurrentDirectoryA(uint8_t* memory, uint32_t* stack)
{
    auto* windows = physical(Windows*, TIB_WINDOWS);
    char* pathName = physical(char*, stack[1]);
    strncpy(windows->currentDirectory, pathName, 260);
#if defined(_WIN32)
    return true;
#else
    for (char& c : windows->currentDirectory) {
        if (c == '\\')
            c = '/';
    }
    return true;
#endif
}

// File

int syscall_CloseHandle(uint8_t* memory, uint32_t* stack, struct allocator_t* allocator)
{
    auto hObject = physical(FILE**, stack[1]);
    if (hObject && *hObject) {
        fclose(*hObject);
    }
    allocator->deallocate(hObject);
    return true;
}

int syscall_CreateFileA(uint8_t* memory, uint32_t* stack, struct allocator_t* allocator)
{
    auto fileName = physical(char*, stack[1]);
    auto desiredAccess = stack[2];
//  auto shareMode = stack[3];
//  auto securityAttributes = physical(void*, stack[4]);
    auto creationDisposition = stack[5];
//  auto flagsAndAttributes = stack[6];
//  auto templateFile = physical(FILE**, stack[7]);

    auto* windows = physical(Windows*, TIB_WINDOWS);

    std::string path = windows->currentDirectory;
    path += '\\';
    path += fileName;
#if defined(_WIN32)
#else
    for (char& c : path) {
        if (c == '\\')
            c = '/';
    }
#endif

    struct stat st;
    const char* mode = nullptr;
    switch (desiredAccess & 0xC0000000) {
    default:                        return 0xFFFFFFFF;
    case 0x40000000: mode = "wb";   break;
    case 0x80000000: mode = "rb";   break;
    case 0xC0000000: mode = "wb+";  break;
    }

    switch (creationDisposition) {
    case 1:
        if (stat(path.c_str(), &st) == 0)
            return 0xFFFFFFFF;
        break;
    case 2:
        break;
    case 3:
        if (stat(path.c_str(), &st) != 0)
            return 0xFFFFFFFF;
        break;
    case 4:
        break;
    case 5:
        if (stat(path.c_str(), &st) != 0)
            return 0xFFFFFFFF;
        break;
    }

    auto handle = (FILE**)allocator->allocate(sizeof(FILE*));
    if (handle == nullptr)
        return 0xFFFFFFFF;

    (*handle) = fopen(path.c_str(), mode);
    if ((*handle) == nullptr) {
        allocator->deallocate(handle);
        return 0xFFFFFFFF;
    }

    return virtual(int, handle);
}

int syscall_CreateFileMappingA(uint8_t* memory, uint32_t* stack, struct allocator_t* allocator)
{
    auto hFile = physical(FILE**, stack[1]);
//  auto lpFileMappingAttributes = physical(void*, stack[2]);
//  auto flProtect = stack[3];
//  auto dwMaximumSizeHigh = stack[4];
//  auto dwMaximumSizeLow = stack[5];
//  auto lpName = physical(char*, stack[6]);

    return virtual(int, hFile);
}

int syscall_GetFileSize(uint8_t* memory, uint32_t* stack)
{
    auto hFile = physical(FILE**, stack[1]);
    auto lpFileSizeHigh = physical(int*, stack[2]);

    auto file = (*hFile);
    long pos = ftell(file);
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, pos, SEEK_SET);

    if (lpFileSizeHigh)
        (*lpFileSizeHigh) = int(size >> 32);

    return virtual(int, size);
}

int syscall_MapViewOfFile(uint8_t* memory, uint32_t* stack, struct allocator_t* allocator)
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
        return NULL;

    fseek(file, dwFileOffsetLow, SEEK_SET);
    fread(map, 1, dwNumberOfBytesToMap, file);
    fseek(file, pos, SEEK_SET);

    return virtual(int, map);
}

int syscall_UnmapViewOfFile(uint8_t* memory, uint32_t* stack, struct allocator_t* allocator)
{
    auto lpBaseAddress = physical(void*, stack[1]);
    allocator->deallocate(lpBaseAddress);
    return true;
}

// Find

int syscall_FindClose(uint8_t* memory, uint32_t* stack, struct allocator_t* allocator)
{
#if defined(_WIN32)
    auto handle = physical(HANDLE*, stack[1]);
    auto result = FindClose(*handle);
    allocator->deallocate(handle);
    return result;
#else
    auto dir = physical(DIR**, stack[1]);
    auto result = closedir(*dir);
    allocator->deallocate(dir);
    return result ? false : true;
#endif
}

int syscall_FindNextFileA(uint8_t* memory, uint32_t* stack)
{
#if defined(_WIN32)
    auto handle = physical(HANDLE*, stack[1]);
    auto findFileData = physical(WIN32_FIND_DATAA*, stack[2]);
    return FindNextFileA(*handle, findFileData);
#else
    auto dir = physical(DIR**, stack[1]);
    auto findFileData = physical(WIN32_FIND_DATAA*, stack[2]);
    auto pattern = (char*)(dir + 1);

    dirent* dirent = nullptr;
    while ((dirent = readdir(*dir)) != nullptr) {
        if (pattern[0] && fnmatch(pattern, dirent->d_name, FNM_CASEFOLD) == 0)
            break;
    }
    if (dirent == nullptr)
        return false;

    switch (dirent->d_type) {
    case DT_DIR:    findFileData->dwFileAttributes = 0x010; break;
    case DT_REG:    findFileData->dwFileAttributes = 0x080; break;
    case DT_LNK:    findFileData->dwFileAttributes = 0x400; break;
    }
    findFileData->ftCreationTime = 0;
    findFileData->ftLastAccessTime = 0;
    findFileData->ftLastWriteTime = 0;
    findFileData->nFileSizeHigh = 0;
    findFileData->nFileSizeLow = 0;
    strncpy(findFileData->cFileName, dirent->d_name, 260);
    strncpy(findFileData->cAlternateFileName, dirent->d_name, 14);

    return true;
#endif
}

int syscall_FindFirstFileA(uint8_t* memory, uint32_t* stack, struct allocator_t* allocator)
{
#if defined(_WIN32)
    auto handle = (HANDLE*)allocator->allocate(sizeof(HANDLE));
    if (handle == nullptr)
        return 0xFFFFFFFF;
    auto name = physical(char*, stack[1]);
    auto findFileData = physical(WIN32_FIND_DATAA*, stack[2]);
    (*handle) = FindFirstFileA(name, findFileData);
    if ((*handle) == INVALID_HANDLE_VALUE) {
        allocator->deallocate(handle);
        return 0xFFFFFFFF;
    }
    return virtual(int, handle);
#else
    auto dir = (DIR**)allocator->allocate(sizeof(DIR*) + sizeof(char) * 120);
    if (dir == nullptr)
        return 0xFFFFFFFF;
    auto* windows = physical(Windows*, TIB_WINDOWS);
    auto name = physical(char*, stack[1]);

    std::string path = windows->currentDirectory;
    path += '\\';
    path += name;
#if defined(_WIN32)
#else
    for (char& c : path) {
        if (c == '\\')
            c = '/';
    }
#endif
    auto slash = path.find_last_of("/\\");

    (*dir) = opendir(path.substr(0, slash).c_str());
    if ((*dir) == nullptr) {
        allocator->deallocate(dir);
        return 0xFFFFFFFF;
    }

    auto pattern = (char*)(dir + 1);
    strncpy(pattern, path.substr(slash + 1).c_str(), 120);

    stack[1] = virtual(int, dir);
    if (syscall_FindNextFileA(memory, stack) == false) {
        allocator->deallocate(dir);
        return 0xFFFFFFFF;
    }

    return virtual(int, dir);
#endif
}

// Library

int syscall_FreeLibrary(uint8_t* memory, uint32_t* stack)
{
    return 0;
}

int syscall_GetModuleHandleA(uint8_t* memory, uint32_t* stack)
{
    auto* windows = physical(Windows*, TIB_WINDOWS);
    auto& modules = windows->modules;
    auto name = physical(char*, stack[1]);

    if (name == nullptr)
        return windows->image;

    std::string path = name;
    auto slash = path.find_last_of("/\\");

    path = path.substr(slash + 1);
    for (auto& [name, image] : modules) {
        if (strcasecmp(name.c_str(), path.c_str()) == 0)
            return virtual(int, image);
    }

    return 0;
}

int syscall_GetProcAddress(uint8_t* memory, uint32_t* stack)
{
    if (stack[1] == 0 || stack[2] == 0)
        return 0;
    auto image = physical(void*, stack[1]);
    auto function = physical(char*, stack[2]);

    struct Data {
        const char* name;
        size_t address;
    } data = { function };

    PE::Exports(image, [](const char* name, size_t address, void* sym_data) {
        auto& data = *(Data*)sym_data;
        if (strcmp(data.name, name) == 0)
            data.address = address;
    }, &data);

    return (int)data.address;
}

int syscall_LoadLibraryA(uint8_t* memory, uint32_t* stack, x86_i386* cpu, int(*log)(const char*, ...))
{
    int module = syscall_GetModuleHandleA(memory, stack);
    if (module)
        return module;

    auto* windows = physical(Windows*, TIB_WINDOWS);
    auto& modules = windows->modules;
    auto name = physical(char*, stack[1]);

    std::string path = windows->currentDirectory;
    path += '\\';
    path += name;
#if defined(_WIN32)
#else
    for (char& c : path) {
        if (c == '\\')
            c = '/';
    }
#endif
    auto slash = path.find_last_of("/\\");

    void* image = PE::Load(path.c_str(), [](size_t base, size_t size, void* userdata) {
        miCPU* cpu = (miCPU*)userdata;
        return cpu->Memory(base, size);
    }, cpu, log);
    PE::Imports(image, [](const char* file, const char* name) {
        extern size_t syscall_windows_symbol(const char* file, const char* name);
        extern size_t syscall_i386_symbol(const char* file, const char* name);
        size_t address = 0;
        if (address == 0)
            address = syscall_windows_symbol(file, name);
        if (address == 0)
            address = syscall_i386_symbol(file, name);
        return address;
    }, log);
    modules.emplace_back(path.substr(slash + 1).c_str(), image);
    if (windows->loadLibraryCallback) {
        windows->loadLibraryCallback(image);
    }

    // _DllMainCRTStartup
    size_t entry = PE::Entry(image);
    if (entry) {
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

        // 58 POP EAX
        // 58 POP EAX
        // C3 RET
        // C3 RET
        uint32_t ret = Pop32();
        Pop32();
        Push32(ret);
        Push32(virtual(int, image));
        Push32(0xC3C35858);
        uint32_t eax = ESP;
        Push32(0);
        Push32(2);
        Push32(0);
        Push32(eax);
        Push32(0);
        Push32(1);
        Push32(0);
        Push32(entry);
        Push32(entry);
        Push32(entry);
    }

    return virtual(int, image);
}

#ifdef __cplusplus
}
#endif
