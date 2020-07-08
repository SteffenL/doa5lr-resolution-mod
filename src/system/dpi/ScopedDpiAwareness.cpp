#include "ScopedDpiAwareness.hpp"
#include "ScopedDpiAwarenessWin8_1.hpp"
#include "ScopedDpiAwarenessWin10_0_14393.hpp"
#include "ScopedDpiAwarenessWinLegacy.hpp"
#include "../../common/OsVersion.hpp"

ScopedDpiAwareness::ScopedDpiAwareness(bool aware) : m_impl{createImpl(aware)} {}

ScopedDpiAwareness::~ScopedDpiAwareness() {}

std::unique_ptr<IScopedDpiAwarenessImpl> ScopedDpiAwareness::createImpl(bool aware) {
    OsVersion osVersion;

    // Windows 10 version 1607
    if (osVersion.is(10, 0, 14393, OsVersionCondition::GreaterOrEqual)) {
        return std::make_unique<ScopedDpiAwarenessWin10_0_14393>(aware);
    }

    // Windows 8.1
    if (osVersion.is(6, 3, 0, OsVersionCondition::GreaterOrEqual)) {
        return std::make_unique<ScopedDpiAwarenessWin8_1>(aware);
    }

    return std::make_unique<ScopedDpiAwarenessWinLegacy>(aware);
}
