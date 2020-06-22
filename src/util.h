#pragma once

#include <cstdint>

namespace util {

int calculateGcd(int a, int b);
bool search(const char* from, const char* to, const char* pattern, int patternLength, uintptr_t* resultOffset, const char* pattern2 = nullptr);
bool getBaseOfCode(uintptr_t& baseOfCode, uintptr_t& sizeOfCode);
bool getBaseOfData(uintptr_t& baseOfData);
bool getExecutablePath(wchar_t buffer[], unsigned int length);
bool getExecutableDir(wchar_t buffer[], unsigned int length);

} // util
