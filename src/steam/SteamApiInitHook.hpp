#pragma once

#include "../common/MinHook.hpp"
#include "SteamApiLibrary.hpp"
#include <functional>

class SteamApiInitHook {
public:
    SteamApiInitHook(SteamApiLibrary& steamApi, MinHook& minhook, std::function<void()> callback)
        : m_steamApi{steamApi},
          m_initHook{minhook.create(m_steamApi.getExports().SteamAPI_Init, initDetour_)},
          m_callback(callback) {
        sm_InitDetour = std::bind(&SteamApiInitHook::initDetour, this);
        m_initHook.enable();
    }

    ~SteamApiInitHook() {}

    bool __cdecl initDetour() {
        m_initHook.disable();
        m_callback();
        auto fn = reinterpret_cast<SteamApiExports::SteamAPI_Init_t>(m_initHook.getOriginal());
        return fn();
    }

    static bool __cdecl initDetour_() {
        return sm_InitDetour();
    }

private:
    SteamApiLibrary& m_steamApi;
    Hook m_initHook;
    static inline std::function<bool()> sm_InitDetour;
    std::function<void()> m_callback;
};
