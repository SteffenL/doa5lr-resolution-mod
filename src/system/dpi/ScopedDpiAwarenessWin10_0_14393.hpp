#pragma once

#include "IScopedDpiAwarenessImpl.hpp"

#include <stdexcept>

#include <Windows.h>

class ScopedDpiAwarenessWin10_0_14393 : public IScopedDpiAwarenessImpl {
public:
    ScopedDpiAwarenessWin10_0_14393(bool aware) {
        auto awareness = aware ? DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE : DPI_AWARENESS_CONTEXT_UNAWARE;

        m_oldAwareness = SetThreadDpiAwarenessContext(awareness);

        if (!m_oldAwareness) {
            throw std::runtime_error{"Unable to set process DPI awareness using API since Windows 10.0.14393"};
        }
    }

    virtual ~ScopedDpiAwarenessWin10_0_14393() {
        SetThreadDpiAwarenessContext(m_oldAwareness);
    }

private:
    DPI_AWARENESS_CONTEXT m_oldAwareness{};
};
