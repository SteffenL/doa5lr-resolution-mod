#pragma once

#include "util.h"

#include <Windows.h>
#include <DbgHelp.h>
#include <Shlwapi.h>

#pragma comment(lib, "DbgHelp.lib")
#pragma comment(lib, "Shlwapi.lib")

namespace util {

// https://stackoverflow.com/a/1186465
int calculateGcd(int a, int b)
{
    return (b == 0) ? a : calculateGcd(b, a % b);
}

bool search(const char* from, const char* to, const char* pattern, int patternLength, uintptr_t* resultOffset, const char* pattern2) {
    int matched = 0;
    const char* p = from;

    while (matched < patternLength && p < (to - patternLength)) {
        if (pattern2 && *(pattern2 + matched) == '?') {
            ++matched;
            continue;
        }

        if (*(p + matched) != *(pattern + matched) || p == pattern) {
            matched = 0;
            ++p;
            continue;
        }

        ++matched;
    }

    bool isMatch = matched == patternLength;

    if (isMatch && resultOffset) {
        *resultOffset = static_cast<uintptr_t>(p - from);
    }

    return isMatch;
}

bool getBaseOfCode(uintptr_t& baseOfCode, uintptr_t& sizeOfCode) {
    auto mainExe = GetModuleHandleA(NULL);
    if (!mainExe) {
        return false;
    }

    uintptr_t imageBase = reinterpret_cast<uintptr_t>(mainExe);
    auto ntHeaders = ImageNtHeader(reinterpret_cast<void*>(imageBase));
    baseOfCode = imageBase + ntHeaders->OptionalHeader.BaseOfCode;
    sizeOfCode = ntHeaders->OptionalHeader.SizeOfCode;
    return TRUE;
}

bool getBaseOfData(uintptr_t& baseOfData) {
    auto mainExe = GetModuleHandleA(NULL);
    if (!mainExe) {
        return false;
    }

    uintptr_t imageBase = reinterpret_cast<uintptr_t>(mainExe);
    auto ntHeaders = ImageNtHeader(reinterpret_cast<void*>(imageBase));
    baseOfData = imageBase + ntHeaders->OptionalHeader.BaseOfData;
    return true;
}

bool getExecutablePath(wchar_t buffer[], unsigned int length) {

    bool isOk = false;

    do {
        DWORD requiredLength = ::GetModuleFileNameW(NULL, &buffer[0], length);

        // Failed?
        if (requiredLength == 0) {
            break;
        }

        buffer[MAX_PATH - 1] = L'\0';
        isOk = true;
    } while (!isOk);

    return isOk;
}

bool getExecutableDir(wchar_t buffer[], unsigned int length)
{
    bool isOk = false;

    do {
        if (!util::getExecutablePath(buffer, length)) {
            break;
        }

        ::PathRemoveFileSpecW(buffer);

        isOk = true;
    } while (!isOk);

    return isOk;
}

} // util
