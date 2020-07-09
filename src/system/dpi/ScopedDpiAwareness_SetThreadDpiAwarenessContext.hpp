#pragma once

#include "IScopedDpiAwarenessImpl.hpp"
#include "../SystemUser32Library.hpp"

#include <stdexcept>

#include <Windows.h>

/**
 * Scoped API awareness using API available since Windows 10 version 1607.
 */
class ScopedDpiAwareness_SetThreadDpiAwarenessContext : public IScopedDpiAwarenessImpl {
public:
    ScopedDpiAwareness_SetThreadDpiAwarenessContext(bool aware) {
        if (auto fn = m_library.getExports().SetThreadDpiAwarenessContext) {
            auto awareness = aware ? DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE : DPI_AWARENESS_CONTEXT_UNAWARE;

            m_oldAwareness = fn(awareness);

            if (!m_oldAwareness) {
                throw std::runtime_error{"Unable to set DPI awareness using SetThreadDpiAwarenessContext"};
            }
        } else {
            throw std::runtime_error{"SetThreadDpiAwarenessContext is unavailable"};
        }
    }

    virtual ~ScopedDpiAwareness_SetThreadDpiAwarenessContext() {
        if (auto fn = m_library.getExports().SetThreadDpiAwarenessContext) {
            fn(m_oldAwareness);
        }
    }

    static bool isSupported() {
        SystemUser32Library library;
        return !!library.getExports().SetThreadDpiAwarenessContext;
    }

private:
    DPI_AWARENESS_CONTEXT m_oldAwareness{};
    SystemUser32Library m_library;
};
