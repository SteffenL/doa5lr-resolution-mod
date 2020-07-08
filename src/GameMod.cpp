#include "GameMod.hpp"
#include "log/FileLogger.hpp"

#include <strsafe.h>
#include <string>

#include <Windows.h>

GameMod::GameMod(const Config& config, const Logger& logger)
    : m_process{Process::current()},
      m_config{config},
      m_logger{logger} {

    m_logger.debug("Setting DPI awareness");
    m_dpiAwareness = std::make_unique<ScopedDpiAwareness>(m_config.getDpiAware());

    m_logger.debug("Creating Steam API hook");
    m_steamApiInitHook = std::make_unique<SteamApiInitHook>(m_steamApi, m_minhook, std::bind(&GameMod::onInit, this));

    if (m_config.getFixAspectRatio()) {
        m_logger.debug("Creating aspect ratio patch");
        m_aspectRatioPatch = std::make_unique<AspectRatioPatch>(m_process, m_config);
    }

    proxyLibraries();
}

GameMod::~GameMod() {
}

const D3d9Library& GameMod::getD3d9Library() const {
    return *m_d3d9;
}

void GameMod::onInit() {
    // We need to catch errors here since we are inside a hooked function
    try {
        if (m_config.getFixViewport()) {
            m_logger.debug("Creating viewport patch");
            m_viewportPatch = std::make_unique<ViewportPatch>(m_process);
        }

        if (m_config.getEnableVideoResolution()) {
            m_logger.debug("Creating resolution hook");
            m_setResolutionHook = std::make_unique<SetResolutionHook>(m_process, m_minhook, m_config, m_logger);
        }
    } catch (const std::exception& ex) {
        m_logger.error(ex.what());
        throw;
    }
}

void GameMod::proxyLibraries() {
    const auto& proxyConfig = m_config.getProxy();

    if (proxyConfig.getD3d9().empty()) {
        m_logger.debug("Loading d3d9 from system");
        m_d3d9 = std::make_unique<SystemD3d9Library>();
    } else {
        m_logger.debug("Loading d3d9: " + proxyConfig.getD3d9().u8string());
        m_d3d9 = std::make_unique<D3d9Library>(proxyConfig.getD3d9());
    }
}
