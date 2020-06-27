#include "MinHook.hpp"

#include <MinHook.h>
#include <stdexcept>

Hook::Hook(void* pTarget, void* pDetour) {
    if (MH_CreateHook(pTarget, pDetour, &m_pOriginal) != MH_OK) {
        throw std::runtime_error{"Failed to create hook"};
    }

    m_pTarget = pTarget;
    m_pDetour = pDetour;
}

Hook::~Hook() {
    MH_RemoveHook(m_pTarget);
}

void Hook::enable() {
    if (MH_EnableHook(m_pTarget) != MH_OK) {
        throw std::runtime_error{"Failed to enable hook"};
    }
}

void Hook::disable() {
    if (MH_DisableHook(m_pTarget) != MH_OK) {
        throw std::runtime_error{"Failed to disable hook"};
    }
}

void* Hook::getOriginal() const {
    return m_pOriginal;
}

MinHook::MinHook() {
    if (MH_Initialize() != MH_OK) {
        throw std::runtime_error{"Failed to initialize hook library"};
    }
}

MinHook::~MinHook() {
    MH_Uninitialize();
}

Hook MinHook::create(void* pTarget, void* pDetour) {
    return Hook{pTarget, pDetour};
}
