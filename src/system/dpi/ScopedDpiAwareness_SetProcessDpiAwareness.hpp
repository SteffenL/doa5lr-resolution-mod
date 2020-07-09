#pragma once

#include "IScopedDpiAwarenessImpl.hpp"
#include "../SystemShcoreLibrary.hpp"

#include <stdexcept>

#include <ShellScalingApi.h>

/**
 * Scoped API awareness using API available since Windows 8.1.
 */
class ScopedDpiAwareness_SetProcessDpiAwareness : public IScopedDpiAwarenessImpl {
public:
    ScopedDpiAwareness_SetProcessDpiAwareness(bool aware) {
        const auto& exports = m_library.getExports();

        if (!exports.GetProcessDpiAwareness) {
            throw std::runtime_error{"GetProcessDpiAwareness is unavailable"};
        }

        if (!exports.SetProcessDpiAwareness) {
            throw std::runtime_error{"SetProcessDpiAwareness is unavailable"};
        }

        if (exports.GetProcessDpiAwareness(GetCurrentProcess(), &m_oldAwareness) != S_OK) {
            throw std::runtime_error{"Unable to get DPI awareness using GetProcessDpiAwareness"};
        }

        auto awareness = aware ? PROCESS_PER_MONITOR_DPI_AWARE : PROCESS_DPI_UNAWARE;
        auto status = exports.SetProcessDpiAwareness(awareness);

        if (status != S_OK && status != E_ACCESSDENIED) {
            throw std::runtime_error{"Unable to set DPI awareness using SetProcessDpiAwareness"};
        }
    }

    virtual ~ScopedDpiAwareness_SetProcessDpiAwareness() {
        if (auto fn = m_library.getExports().SetProcessDpiAwareness) {
            fn(m_oldAwareness);
        }
    }

    static bool isSupported() {
        SystemShcoreLibrary library;
        if (!library.isLoaded()) {
            return false;
        }

        const auto& exports = library.getExports();
        return exports.GetProcessDpiAwareness && exports.SetProcessDpiAwareness;
    }

private:
    PROCESS_DPI_AWARENESS m_oldAwareness{};
    SystemShcoreLibrary m_library;
};
