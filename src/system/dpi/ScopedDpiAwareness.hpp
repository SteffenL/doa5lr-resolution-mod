#pragma once

#include "IScopedDpiAwarenessImpl.hpp"

#include <memory>

class ScopedDpiAwareness {
public:
    ScopedDpiAwareness(bool aware);
    ~ScopedDpiAwareness();

private:
    std::unique_ptr<IScopedDpiAwarenessImpl> m_impl;

    static std::unique_ptr<IScopedDpiAwarenessImpl> createImpl(bool aware);
};
