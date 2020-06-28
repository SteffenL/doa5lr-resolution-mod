#pragma once

#include "config/Config.hpp"
#include "GameMod.hpp"
#include "log/Logger.hpp"

class GameModInitializer {
public:
    GameModInitializer();
    ~GameModInitializer();
    GameMod& getGameMod() const;
    const Logger& getLogger() const;

private:
    static std::unique_ptr<Logger> createLogger(const Config& config);

    std::unique_ptr<Config> m_config;
    std::unique_ptr<GameMod> m_gameMod;
    std::unique_ptr<Logger> m_logger;
};

extern std::unique_ptr<GameModInitializer> g_gameModInitializer;
