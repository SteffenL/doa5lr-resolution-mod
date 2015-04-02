#pragma once

#ifdef _WIN32
#include "StandardPaths_msw.h"
#else
#error Not implemented
#endif

namespace core {

#ifdef _WIN32
    typedef StandardPaths_msw StandardPaths;
#else
#error Not implemented
#endif

} // namespace
