#pragma once

#include "LogLevel.hpp"
#include <string>

class Logger {
public:
    Logger(LogLevel logLevel);
    virtual ~Logger();

    virtual void doLog(const std::string& message) const = 0;

    void log(LogLevel level, const std::string& message) const;
    bool isLogEnabled(LogLevel level) const;
    void info(const std::string& message) const;
    void error(const std::string& message) const;
    void debug(const std::string& message) const;
    LogLevel getLogLevel() const;
    std::string getLogLevelName(LogLevel level) const;

private:
    LogLevel m_logLevel{LogLevel::Silent};
};
