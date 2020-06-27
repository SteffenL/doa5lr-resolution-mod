#pragma once

#include <cstdint>
#include <memory>

class Process {
public:
    Process(void* handle);
    ~Process();
    static Process current();
    void* getHandle() const;
    void getBaseOfCode(uintptr_t& baseOfCode, uintptr_t& sizeOfCode) const;
    void getBaseOfData(uintptr_t& baseOfData) const;

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};
