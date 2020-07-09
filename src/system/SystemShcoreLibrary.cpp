#include "SystemShcoreLibrary.hpp"

#include "../common/util.hpp"

SystemShcoreLibrary::SystemShcoreLibrary() : ShcoreLibrary{getSystemShcoreLibraryPath()} {}

SystemShcoreLibrary::~SystemShcoreLibrary() {}

std::filesystem::path SystemShcoreLibrary::getSystemShcoreLibraryPath() {
    return util::getSystemDirectory() / "shcore.dll";
}
