#pragma once

#include "../system/SystemNtdllLibrary.hpp"

#include <memory>

enum class OsVersionCondition {
    GreaterOrEqual
};

class OsVersion {
public:
    OsVersion();
    ~OsVersion();

    bool is(int major, int minor, int build, OsVersionCondition condition) const;

private:
    SystemNtdllLibrary m_ntdll;
};
