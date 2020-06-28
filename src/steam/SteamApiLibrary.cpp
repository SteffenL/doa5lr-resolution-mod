#include "SteamApiLibrary.hpp"

#include <Windows.h>

SteamApiExports::SteamApiExports(void* dllHandle) {
    this->SteamAPI_Init = reinterpret_cast<SteamAPI_Init_t>(GetProcAddress(static_cast<HMODULE>(dllHandle), "SteamAPI_Init"));
}

SteamApiLibrary::SteamApiLibrary() {
    if (auto dllHandle = GetModuleHandleW(L"steam_api.dll")) {
        m_exports = std::make_unique<SteamApiExports>(dllHandle);
        m_dllHandle = static_cast<void*>(dllHandle);
    }
}

SteamApiLibrary::~SteamApiLibrary() {
    if (m_dllHandle) {
        m_dllHandle = nullptr;
    }
}

void* SteamApiLibrary::getDllHandle() const {
    return m_dllHandle;
}

const SteamApiExports& SteamApiLibrary::getExports() const {
    return *m_exports;
}
