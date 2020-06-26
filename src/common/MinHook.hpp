#pragma once

#include <MinHook.h>
#include <stdexcept>

class Hook {
public:
    Hook(LPVOID pTarget, LPVOID pDetour) {
        if (MH_CreateHook(pTarget, pDetour, &m_pOriginal) != MH_OK) {
            throw std::runtime_error{"Failed to create hook"};
        }

        m_pTarget = pTarget;
        m_pDetour = pDetour;
    }

    ~Hook() {
        MH_RemoveHook(m_pTarget);
    }

    void enable() {
        if (MH_EnableHook(m_pTarget) != MH_OK) {
            throw std::runtime_error{"Failed to enable hook"};
        }
    }

    void disable() {
        if (MH_DisableHook(m_pTarget) != MH_OK) {
            throw std::runtime_error{"Failed to disable hook"};
        }
    }

    LPVOID getOriginal() const {
        return m_pOriginal;
    }

private:
    LPVOID m_pTarget{};
    LPVOID m_pDetour{};
    LPVOID m_pOriginal{};
};

class MinHook {
public:
    MinHook() {
        if (MH_Initialize() != MH_OK) {
            throw std::runtime_error{"Failed to initialize hook library"};
        }
    }

    ~MinHook() {
        MH_Uninitialize();
    }

    Hook create(LPVOID pTarget, LPVOID pDetour) {
        return Hook{pTarget, pDetour};
    }
};
