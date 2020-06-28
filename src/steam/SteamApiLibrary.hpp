#pragma once

#include <filesystem>
#include <memory>

struct SteamApiExports {
    using SteamAPI_Init_t = bool (__cdecl*)();

    SteamApiExports(void* dllHandle);

    SteamAPI_Init_t SteamAPI_Init{};
};

class SteamApiLibrary {
public:
    SteamApiLibrary();
    virtual ~SteamApiLibrary();
    void* getDllHandle() const;
    const SteamApiExports& getExports() const;

private:
    std::unique_ptr<SteamApiExports> m_exports;
    void* m_dllHandle;
};
