#include "Process.hpp"

#include <Windows.h>
#include <DbgHelp.h>
#include <Shlwapi.h>

#pragma comment(lib, "DbgHelp.lib")
#pragma comment(lib, "Shlwapi.lib")

class Process::Impl {
public:
    Impl() : m_handle{0} {}
    Impl(HMODULE handle) : m_handle{handle} {}
    ~Impl() {}

    HMODULE getHandle() const {
        return m_handle;
    }

    void getBaseOfCode(uintptr_t& baseOfCode, uintptr_t& sizeOfCode) const {
        uintptr_t imageBase = reinterpret_cast<uintptr_t>(m_handle);
        auto ntHeaders = ImageNtHeader(reinterpret_cast<void*>(imageBase));
        baseOfCode = imageBase + ntHeaders->OptionalHeader.BaseOfCode;
        sizeOfCode = ntHeaders->OptionalHeader.SizeOfCode;
    }

    void getBaseOfData(uintptr_t& baseOfData) const {
        uintptr_t imageBase = reinterpret_cast<uintptr_t>(m_handle);
        auto ntHeaders = ImageNtHeader(reinterpret_cast<void*>(imageBase));
        baseOfData = imageBase + ntHeaders->OptionalHeader.BaseOfData;
    }

private:
    HMODULE m_handle;
};

Process::Process(void* handle) : m_impl{std::make_unique<Impl>(static_cast<HMODULE>(handle))} {}

Process::~Process() {}

Process Process::current() {
    return Process{GetModuleHandleW(0)};
}

void* Process::getHandle() const {
    return static_cast<void*>(m_impl->getHandle());
}

void Process::getBaseOfCode(uintptr_t& baseOfCode, uintptr_t& sizeOfCode) const {
    return m_impl->getBaseOfCode(baseOfCode, sizeOfCode);
}

void Process::getBaseOfData(uintptr_t& baseOfData) const {
    return m_impl->getBaseOfData(baseOfData);
}
