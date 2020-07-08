#pragma once

#include "NtdllLibrary.hpp"
#include <filesystem>

class SystemNtdllLibrary : public NtdllLibrary  {
public:
    SystemNtdllLibrary();
    virtual ~SystemNtdllLibrary();

private:
    static std::filesystem::path getSystemNtdllLibraryPath();
};
