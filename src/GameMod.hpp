#pragma once

#include "common/MinHook.hpp"
#include "common/Process.hpp"
#include "config/Config.hpp"
#include "d3d9/D3d9Library.hpp"
#include "d3d9/SystemD3d9Library.hpp"
#include "game/AspectRatioPatch.hpp"
#include "game/SetResolutionHook.hpp"
#include "game/ViewportPatch.hpp"
#include "log/Logger.hpp"
#include "steam/SteamApiInitHook.hpp"
#include "steam/SteamApiLibrary.hpp"
#include "system/dpi/ScopedDpiAwareness.hpp"

#include <memory>

class GameMod {
public:
    GameMod(const Config& config, const Logger& logger);
    ~GameMod();

    const D3d9Library& getD3d9Library() const;
    const Logger& getLogger() const;

private:
    void onInit();
    void proxyLibraries();

    const Logger& m_logger;
    const Config& m_config;

    std::unique_ptr<D3d9Library> m_d3d9;
    MinHook m_minhook;
    Process m_process;
    SteamApiLibrary m_steamApi;

    std::unique_ptr<AspectRatioPatch> m_aspectRatioPatch;
    std::unique_ptr<ScopedDpiAwareness> m_dpiAwareness;
    std::unique_ptr<SetResolutionHook> m_setResolutionHook;
    std::unique_ptr<SteamApiInitHook> m_steamApiInitHook;
    std::unique_ptr<ViewportPatch> m_viewportPatch;
};
