#include "ScopedDpiAwareness.hpp"
#include "ScopedDpiAwareness_SetProcessDPIAware.hpp"
#include "ScopedDpiAwareness_SetProcessDpiAwareness.hpp"
#include "ScopedDpiAwareness_SetThreadDpiAwarenessContext.hpp"
#include "../../common/OsVersion.hpp"

#include <stdexcept>

ScopedDpiAwareness::ScopedDpiAwareness(bool aware) : m_impl{createImpl(aware)} {}

ScopedDpiAwareness::~ScopedDpiAwareness() {}

std::unique_ptr<IScopedDpiAwarenessImpl> ScopedDpiAwareness::createImpl(bool aware) {
    if (ScopedDpiAwareness_SetThreadDpiAwarenessContext::isSupported()) {
        return std::make_unique<ScopedDpiAwareness_SetThreadDpiAwarenessContext>(aware);
    }

    if (ScopedDpiAwareness_SetProcessDpiAwareness::isSupported()) {
        return std::make_unique<ScopedDpiAwareness_SetProcessDpiAwareness>(aware);
    }

    if (ScopedDpiAwareness_SetProcessDPIAware::isSupported()) {
        return std::make_unique<ScopedDpiAwareness_SetProcessDPIAware>(aware);
    }

    // We support minimum Windows 7 and DPI awareness is supported as far back as Windows Vista so throwing an exception here is OK
    throw std::runtime_error{"DPI awareness API is unavailable on this operating system"};
}
