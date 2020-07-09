#pragma once

#include "ShcoreLibrary.hpp"
#include <filesystem>

class SystemShcoreLibrary : public ShcoreLibrary  {
public:
    SystemShcoreLibrary();
    virtual ~SystemShcoreLibrary();

private:
    static std::filesystem::path getSystemShcoreLibraryPath();
};
