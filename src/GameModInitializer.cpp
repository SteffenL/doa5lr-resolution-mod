#include "GameModInitializer.hpp"

#include "config/LocalFileConfig.hpp"
#include "log/FileLogger.hpp"
#include "log/NullLogger.hpp"
#include "version.hpp"

std::unique_ptr<GameModInitializer> g_gameModInitializer;

GameModInitializer::GameModInitializer() {
    m_config = std::make_unique<LocalFileConfig>();
    m_logger = createLogger(*m_config);

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

std::unique_ptr<Logger> GameModInitializer::createLogger(const Config& config) {
    const auto& logFilePath = config.getLogFile();
    if (logFilePath.empty() || config.getLogLevel() == LogLevel::Silent) {
        return std::make_unique<NullLogger>();
    } else {
        return std::make_unique<FileLogger>(config.getLogFile(), config.getLogLevel());
    }
}
