#pragma once

#include "IScopedDpiAwarenessImpl.hpp"

#include <stdexcept>

#include <Windows.h>

class ScopedDpiAwarenessWinLegacy : public IScopedDpiAwarenessImpl {
public:
    ScopedDpiAwarenessWinLegacy(bool aware) {
        if (!SetProcessDPIAware()) {
            throw std::runtime_error{"Unable to set process DPI awareness using legacy API"};
        }
    }

    virtual ~ScopedDpiAwarenessWinLegacy() {
        // There is no way to revert the awareness
    }
};
