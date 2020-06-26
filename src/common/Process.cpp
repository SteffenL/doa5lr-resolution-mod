#include "Process.hpp"

#include <DbgHelp.h>
#include <Shlwapi.h>

#pragma comment(lib, "DbgHelp.lib")
#pragma comment(lib, "Shlwapi.lib")

Process::Process(HMODULE handle) : m_handle{handle} {}

Process Process::current() {
    return Process{GetModuleHandleW(0)};
}

HMODULE Process::getHandle() const {
    return m_handle;
}

void Process::getBaseOfCode(uintptr_t& baseOfCode, uintptr_t& sizeOfCode) const {
    uintptr_t imageBase = reinterpret_cast<uintptr_t>(m_handle);
    auto ntHeaders = ImageNtHeader(reinterpret_cast<void*>(imageBase));
    baseOfCode = imageBase + ntHeaders->OptionalHeader.BaseOfCode;
    sizeOfCode = ntHeaders->OptionalHeader.SizeOfCode;
}

void Process::getBaseOfData(uintptr_t& baseOfData) const {
    uintptr_t imageBase = reinterpret_cast<uintptr_t>(m_handle);
    auto ntHeaders = ImageNtHeader(reinterpret_cast<void*>(imageBase));
    baseOfData = imageBase + ntHeaders->OptionalHeader.BaseOfData;
}
