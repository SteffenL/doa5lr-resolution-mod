#pragma once

#include <cstdint>
#include <filesystem>
#include <string>

namespace util {

int calculateGcd(int a, int b);
std::filesystem::path getExecutableDir();
std::filesystem::path getExecutablePath();
std::string getFileVersionString(const std::filesystem::path& filePath);
std::filesystem::path getSystemDirectory();
std::string narrow(const std::wstring& s);
bool search(const char* from, const char* to, const char* pattern, int patternLength, uintptr_t* resultOffset, const char* pattern2 = nullptr);

} // util
