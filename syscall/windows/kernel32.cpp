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
#include <unistd.h>
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

#define SYSCALL_INVALID_HANDLE_VALUE size_t(int64_t(-1))

#ifdef __cplusplus
extern "C" {
#endif

// Atomic
uint32_t syscall_InterlockedCompareExchange(uint8_t* memory, const uint32_t* stack)
{
    auto Destination = physical(uint32_t*, stack[1]);
    auto ExChange = physical(uint32_t, stack[2]);
    auto Comperand = physical(uint32_t, stack[3]);
    if ((*Destination) == Comperand) {
        auto result = (*Destination);
        (*Destination) = ExChange;
        return result;
    }
    return ExChange;
}

uint32_t syscall_InterlockedExchange(uint8_t* memory, const uint32_t* stack)
{
    auto Target = physical(uint32_t*, stack[1]);
    auto Value = physical(uint32_t, stack[2]);
    auto result = (*Target);
    (*Target) = Value;
    return result;
}

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

    char* lpPathName = physical(char*, stack[1]);
    strncpy(windows->currentDirectory, lpPathName, 260);
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

size_t syscall_CreateFileA(uint8_t* memory, uint32_t* stack, struct allocator_t* allocator)
{
    auto* windows = physical(Windows*, TIB_WINDOWS);

    auto lpFileName = physical(char*, stack[1]);
    auto dwDesiredAccess = stack[2];
//  auto dwShareMode = stack[3];
//  auto lpSecurityAttributes = physical(void*, stack[4]);
    auto dwCreationDisposition = stack[5];
//  auto dwFlagsAndAttributes = stack[6];
//  auto hTemplateFile = physical(FILE**, stack[7]);

    std::string path = windows->currentDirectory;
    path += '\\';
    path += lpFileName;
#if defined(_WIN32)
#else
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

    (*handle) = fopen(path.c_str(), mode);
    if ((*handle) == nullptr) {
        allocator->deallocate(handle);
        return SYSCALL_INVALID_HANDLE_VALUE;
    }

    return virtual(size_t, handle);
}

size_t syscall_CreateFileMappingA(uint8_t* memory, uint32_t* stack, struct allocator_t* allocator)
{
    auto hFile = physical(FILE**, stack[1]);
//  auto lpFileMappingAttributes = physical(void*, stack[2]);
//  auto flProtect = stack[3];
//  auto dwMaximumSizeHigh = stack[4];
//  auto dwMaximumSizeLow = stack[5];
//  auto lpName = physical(char*, stack[6]);

    return virtual(size_t, hFile);
}

uint32_t syscall_GetFileSize(uint8_t* memory, uint32_t* stack)
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

size_t syscall_MapViewOfFile(uint8_t* memory, uint32_t* stack, struct allocator_t* allocator)
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

    fseek(file, dwFileOffsetLow, SEEK_SET);
    fread(map, 1, dwNumberOfBytesToMap, file);
    fseek(file, pos, SEEK_SET);

    return virtual(size_t, map);
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
    auto hFindFile = physical(HANDLE*, stack[1]);
    auto result = FindClose(*hFindFile);
    allocator->deallocate(hFindFile);
    return result;
#else
    auto hFindFile = physical(DIR**, stack[1]);
    auto result = closedir(*hFindFile);
    allocator->deallocate(hFindFile);
    return result ? false : true;
#endif
}

int syscall_FindNextFileA(uint8_t* memory, uint32_t* stack)
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

size_t syscall_FindFirstFileA(uint8_t* memory, uint32_t* stack, struct allocator_t* allocator)
{
#if defined(_WIN32)
    auto handle = (HANDLE*)allocator->allocate(sizeof(HANDLE));
    if (handle == nullptr)
        return SYSCALL_INVALID_HANDLE_VALUE;
    auto lpFileName = physical(char*, stack[1]);
    auto lpFindFileData = physical(WIN32_FIND_DATAA*, stack[2]);
    (*handle) = FindFirstFileA(lpFileName, lpFindFileData);
    if ((*handle) == INVALID_HANDLE_VALUE) {
        allocator->deallocate(handle);
        return SYSCALL_INVALID_HANDLE_VALUE;
    }
    return virtual(size_t, handle);
#else
    auto* windows = physical(Windows*, TIB_WINDOWS);

    auto dir = (DIR**)allocator->allocate(sizeof(DIR*) + sizeof(char) * 120);
    if (dir == nullptr)
        return SYSCALL_INVALID_HANDLE_VALUE;
    auto lpFileName = physical(char*, stack[1]);

    std::string path = windows->currentDirectory;
    path += '\\';
    path += lpFileName;
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

// Library

int syscall_FreeLibrary(uint8_t* memory, uint32_t* stack)
{
    return 0;
}

size_t syscall_GetModuleBaseNameA(uint8_t* memory, uint32_t* stack)
{
    extern size_t syscall_GetModuleFileNameA(uint8_t* memory, uint32_t* stack);
    return syscall_GetModuleFileNameA(memory, stack + 1);
}

size_t syscall_GetModuleFileNameA(uint8_t* memory, uint32_t* stack)
{
    auto* windows = physical(Windows*, TIB_WINDOWS);

    auto hModule = physical(void*, stack[1]);
    auto lpFilename = physical(char*, stack[2]);
    auto nSize = stack[3];

    if (hModule == nullptr) {
        strncpy(lpFilename, ".", nSize);
        return strlen(lpFilename);
    }

    for (auto& [name, image] : windows->modules) {
        if (image == hModule) {
            strncpy(lpFilename, name.c_str(), nSize);
            return name.size();
        }
    }

    return 0;
}

size_t syscall_GetModuleHandleA(uint8_t* memory, uint32_t* stack)
{
    auto* windows = physical(Windows*, TIB_WINDOWS);

    auto lpModuleName = physical(char*, stack[1]);
    if (lpModuleName == nullptr)
        return windows->image;

    std::string path = lpModuleName;
    auto slash = path.find_last_of("/\\");

    path = path.substr(slash + 1);
    for (auto& [name, image] : windows->modules) {
        if (strcasecmp(name.c_str(), path.c_str()) == 0)
            return virtual(int, image);
    }

    return 0;
}

size_t syscall_GetProcAddress(uint8_t* memory, uint32_t* stack, int(*log)(const char*, ...))
{
    if (stack[1] == 0 || stack[2] == 0)
        return 0;
    auto hModule = physical(void*, stack[1]);
    auto lpProcName = physical(char*, stack[2]);

    struct Data {
        const char* name;
        size_t address;
        int(*log)(const char*, ...);
    } data = { lpProcName, 0, log };

    PE::Exports(hModule, [](const char* name, size_t address, void* sym_data) {
        auto& data = *(Data*)sym_data;
        if (strcmp(data.name, name) == 0) {
            data.address = address;
            data.log("Symbol : [%08zX] %s", data.address, data.name);
        }
    }, &data);

    return data.address;
}

size_t syscall_LoadLibraryA(uint8_t* memory, uint32_t* stack, x86_i386* cpu, int(*log)(const char*, ...))
{
    auto* windows = physical(Windows*, TIB_WINDOWS);

    size_t module = syscall_GetModuleHandleA(memory, stack);
    if (module)
        return module;

    auto lpLibFileName = physical(char*, stack[1]);
    std::string path = windows->currentDirectory;
    path += '\\';
    path += lpLibFileName;
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
    windows->modules.emplace_back(path.substr(slash + 1).c_str(), image);
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

    return virtual(size_t, image);
}

// System
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
    return (int)(size_t)pthread_self();
#endif
}

// Time
int syscall_GetSystemTimeAsFileTime(uint8_t* memory, uint32_t* stack)
{
#if defined(_WIN32)
    auto lpSystemTimeAsFileTime = physical(FILETIME*, stack[1]);
    GetSystemTimeAsFileTime(lpSystemTimeAsFileTime);
#else
    auto lpSystemTimeAsFileTime = physical(uint64_t*, stack[1]);

    struct timespec ts = {};
    clock_gettime(CLOCK_REALTIME, &ts);

    (*lpSystemTimeAsFileTime) = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
#endif
    return 0;
}

int syscall_GetTickCount()
{
#if defined(_WIN32)
    return GetTickCount();
#else
    struct timespec ts = {};
    clock_gettime(CLOCK_REALTIME, &ts);

    return int(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#endif
}

int syscall_QueryPerformanceCounter(uint8_t* memory, uint32_t* stack)
{
#if defined(_WIN32)
    auto lpPerformanceCount = physical(LARGE_INTEGER*, stack[1]);
    return QueryPerformanceCounter(lpPerformanceCount);
#else
    auto lpPerformanceCount = physical(uint64_t*, stack[1]);

    struct timespec ts = {};
    clock_gettime(CLOCK_REALTIME, &ts);

    (*lpPerformanceCount) = ts.tv_sec * 1'000'000'000ull + ts.tv_nsec;
    return true;
#endif
}

#ifdef __cplusplus
}
#endif
