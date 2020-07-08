#pragma once

#include "IScopedDpiAwarenessImpl.hpp"

#include <stdexcept>

#include <ShellScalingApi.h>

#pragma comment(lib, "Shcore.lib")

class ScopedDpiAwarenessWin8_1 : public IScopedDpiAwarenessImpl {
public:
    ScopedDpiAwarenessWin8_1(bool aware) {
        if (GetProcessDpiAwareness(GetCurrentProcess(), &m_oldAwareness) != S_OK) {
            throw std::runtime_error{"Unable to get process DPI awareness using API since Windows 8.1"};
        }

        auto awareness = aware ? PROCESS_SYSTEM_DPI_AWARE : PROCESS_DPI_UNAWARE;
        auto status = SetProcessDpiAwareness(awareness);

        if (status != S_OK && status != E_ACCESSDENIED) {
            throw std::runtime_error{"Unable to set process DPI awareness using API since Windows 8.1"};
        }
    }

    virtual ~ScopedDpiAwarenessWin8_1() {
        SetProcessDpiAwareness(m_oldAwareness);
    }

private:
    PROCESS_DPI_AWARENESS m_oldAwareness{};
};
