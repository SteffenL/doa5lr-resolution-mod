#pragma once

#include <filesystem>
#include <memory>

#include <Windows.h>

struct SteamApiExports {
    using SteamAPI_Init_t = bool (__cdecl*)();

    SteamApiExports(HMODULE dllHandle);

    SteamAPI_Init_t SteamAPI_Init{};
};

class SteamApiLibrary {
public:
    SteamApiLibrary();
    virtual ~SteamApiLibrary();
    HMODULE getDllHandle() const;
    const SteamApiExports& getExports() const;

private:
    std::unique_ptr<SteamApiExports> m_exports;
    HMODULE m_dllHandle;
};
