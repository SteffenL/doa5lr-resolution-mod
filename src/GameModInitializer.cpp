#include "GameModInitializer.hpp"

#include "config/LocalFileConfig.hpp"
#include "log/FileLogger.hpp"
#include "version.hpp"

std::unique_ptr<GameModInitializer> g_gameModInitializer;

GameModInitializer::GameModInitializer() {
    m_config = std::make_unique<LocalFileConfig>();
    m_logger = std::make_unique<FileLogger>(m_config->getLogFile(), m_config->getLogLevel());

    m_logger->debug("Starting up");
    m_logger->info(MOD_NAME " " MOD_VERSION " by " MOD_AUTHOR);
    m_logger->info("Website: " MOD_WEBSITE);

    try {
        m_gameMod = std::make_unique<GameMod>(*m_config, *m_logger);
    } catch (const std::exception& ex) {
        m_logger->error(ex.what());
        throw;
    }

    m_logger->debug("Created game mod object");
}

GameModInitializer::~GameModInitializer() {
    m_logger->debug("Shutting down");
}

GameMod& GameModInitializer::getGameMod() const {
    return *m_gameMod;
}

const Logger& GameModInitializer::getLogger() const {
    return *m_logger;
}
