#pragma once

#include "User32Library.hpp"
#include <filesystem>

class SystemUser32Library : public User32Library  {
public:
    SystemUser32Library();
    virtual ~SystemUser32Library();

private:
    static std::filesystem::path getSystemUser32LibraryPath();
};
