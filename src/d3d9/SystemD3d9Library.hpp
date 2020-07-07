#pragma once

#include "D3d9Library.hpp"
#include <filesystem>

class SystemD3d9Library : public D3d9Library  {
public:
    SystemD3d9Library();
    virtual ~SystemD3d9Library();

private:
    static std::filesystem::path getSystemD3d9LibraryPath();
};
