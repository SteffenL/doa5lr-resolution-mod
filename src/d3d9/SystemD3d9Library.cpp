#include "SystemD3d9Library.hpp"

#include "../common/util.hpp"

#include <Windows.h>
#include <strsafe.h>

SystemD3d9Library::SystemD3d9Library() : D3d9Library{getSystemD3d9LibraryPath()} {}

SystemD3d9Library::~SystemD3d9Library() {}

std::filesystem::path SystemD3d9Library::getSystemD3d9LibraryPath() {
    return util::getSystemDirectory() / "d3d9.dll";
}
