#include "SteamApiLibrary.hpp"

SteamApiExports::SteamApiExports(HMODULE dllHandle) {
    this->SteamAPI_Init = reinterpret_cast<SteamAPI_Init_t>(GetProcAddress(dllHandle, "SteamAPI_Init"));
}

SteamApiLibrary::SteamApiLibrary() {
    if (auto dllHandle = GetModuleHandleW(L"steam_api.dll")) {
        m_exports = std::make_unique<SteamApiExports>(dllHandle);
        m_dllHandle = dllHandle;
    }
}

SteamApiLibrary::~SteamApiLibrary() {
    if (m_dllHandle) {
        m_dllHandle = nullptr;
    }
}

HMODULE SteamApiLibrary::getDllHandle() const {
    return m_dllHandle;
}

const SteamApiExports& SteamApiLibrary::getExports() const {
    return *m_exports;
}
