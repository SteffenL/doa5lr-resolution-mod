#include "SystemNtdllLibrary.hpp"

#include "../common/util.hpp"

SystemNtdllLibrary::SystemNtdllLibrary() : NtdllLibrary{getSystemNtdllLibraryPath()} {}

SystemNtdllLibrary::~SystemNtdllLibrary() {}

std::filesystem::path SystemNtdllLibrary::getSystemNtdllLibraryPath() {
    return util::getSystemDirectory() / "ntdll.dll";
}
