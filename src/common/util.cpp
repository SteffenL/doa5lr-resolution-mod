#pragma once

#include "util.hpp"

#include <codecvt>
#include <vector>

#include <Windows.h>
#include <DbgHelp.h>
#include <Shlwapi.h>

#pragma comment(lib, "DbgHelp.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Version.lib")

namespace util {

// https://stackoverflow.com/a/1186465
int calculateGcd(int a, int b) {
    return (b == 0) ? a : calculateGcd(b, a % b);
}

std::filesystem::path getExecutableDir() {
    return getExecutablePath().parent_path();
}

std::filesystem::path getExecutablePath() {
    wchar_t buffer[MAX_PATH + 1]{};

    auto length = ::GetModuleFileNameW(NULL, buffer, _countof(buffer));
    if (length == 0) {
        throw std::runtime_error{"Failed to get executable path"};
    }

    buffer[MAX_PATH] = L'\0';
    return buffer;
}

std::filesystem::path getSystemDirectory() {
    wchar_t systemDir[MAX_PATH]{};
    GetSystemDirectoryW(systemDir, _countof(systemDir));
    return std::filesystem::path{systemDir};
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

} // util
