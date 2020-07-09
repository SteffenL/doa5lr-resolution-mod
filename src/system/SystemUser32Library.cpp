#include "SystemUser32Library.hpp"

#include "../common/util.hpp"

SystemUser32Library::SystemUser32Library() : User32Library{getSystemUser32LibraryPath()} {}

SystemUser32Library::~SystemUser32Library() {}

std::filesystem::path SystemUser32Library::getSystemUser32LibraryPath() {
    return util::getSystemDirectory() / "user32.dll";
}
