#pragma once

#include "LogLevel.hpp"
#include <string>

class Logger {
public:
    Logger(LogLevel logLevel) : m_logLevel{logLevel} {}
    virtual ~Logger() = default;

    virtual void doLog(const std::string& message) const = 0;

    void log(LogLevel level, const std::string& message) const {
        if (isLogEnabled(level)) {
            std::string s;
            s += "[";
            s += getLogLevelName(level);
            s += "] ";
            s += message;
            doLog(s);
        }
    }

    bool isLogEnabled(LogLevel level) const {
        int levelInt = static_cast<int>(level);
        int currentLevelInt = static_cast<int>(m_logLevel);
        return levelInt <= currentLevelInt;
    }

    void info(const std::string& message) const {
        log(LogLevel::Info, message);
    }

    void error(const std::string& message) const {
        log(LogLevel::Error, message);
    }

    void debug(const std::string& message) const {
        log(LogLevel::Debug, message);
    }

    LogLevel getLogLevel() const {
        return m_logLevel;
    }

    std::string getLogLevelName(LogLevel level) const {
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

private:
    LogLevel m_logLevel{LogLevel::Silent};
};
