#pragma once

#include "IScopedDpiAwarenessImpl.hpp"
#include "../SystemUser32Library.hpp"

#include <stdexcept>

#include <Windows.h>

class ScopedDpiAwareness_SetProcessDPIAware : public IScopedDpiAwarenessImpl {
public:
    ScopedDpiAwareness_SetProcessDPIAware(bool aware) {
        if (!aware) {
            return;
        }

        if (auto fn = m_library.getExports().SetProcessDPIAware) {
            if (!fn()) {
                throw std::runtime_error{"Unable to set DPI awareness using SetProcessDPIAware"};
            }
        } else {
            throw std::runtime_error{"SetProcessDPIAware is unavailable"};
        }
    }

    virtual ~ScopedDpiAwareness_SetProcessDPIAware() {
        // There is no way to revert the awareness
    }

    static bool isSupported() {
        SystemUser32Library library;
        if (!library.isLoaded()) {
            return false;
        }

        return !!library.getExports().SetProcessDPIAware;
    }

private:
    SystemUser32Library m_library;
};
