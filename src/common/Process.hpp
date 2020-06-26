#pragma once

#include <cstdint>
#include <Windows.h>

class Process {
public:
    Process(HMODULE handle);
    static Process current();
    HMODULE getHandle() const;
    void getBaseOfCode(uintptr_t& baseOfCode, uintptr_t& sizeOfCode) const;
    void getBaseOfData(uintptr_t& baseOfData) const;

private:
    HMODULE m_handle;
};
