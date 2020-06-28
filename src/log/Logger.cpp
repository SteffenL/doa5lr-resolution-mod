#include "Logger.hpp"

Logger::Logger(LogLevel logLevel) : m_logLevel{logLevel} {}
Logger::~Logger() {}

void Logger::log(LogLevel level, const std::string& message) const {
    if (isLogEnabled(level)) {
        std::string s;
        s += "[";
        s += getLogLevelName(level);
        s += "] ";
        s += message;
        doLog(s);
    }
}

bool Logger::isLogEnabled(LogLevel level) const {
    int levelInt = static_cast<int>(level);
    int currentLevelInt = static_cast<int>(m_logLevel);
    return levelInt <= currentLevelInt;
}

void Logger::info(const std::string& message) const {
    log(LogLevel::Info, message);
}

void Logger::error(const std::string& message) const {
    log(LogLevel::Error, message);
}

void Logger::debug(const std::string& message) const {
    log(LogLevel::Debug, message);
}

LogLevel Logger::getLogLevel() const {
    return m_logLevel;
}

std::string Logger::getLogLevelName(LogLevel level) const {
    switch (level) {
        case LogLevel::Error:
            return "error";
        case LogLevel::Info:
            return "info";
        case LogLevel::Debug:
            return "debug";
    }

    return "";
}
