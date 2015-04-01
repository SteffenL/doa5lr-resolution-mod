#pragma once

#include <string>

namespace core {

class StandardPaths_msw {
public:
    static bool GetExecutablePath(wchar_t buffer[], unsigned int length);
    static bool GetExecutableDir(wchar_t buffer[], unsigned int length);
};

} // namespace
