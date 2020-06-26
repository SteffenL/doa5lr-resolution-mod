#pragma once

#include "StreamLogger.hpp"

#include <filesystem>
#include <fstream>

class FileLogger : public StreamLogger {
public:
    FileLogger(const std::filesystem::path& filePath, LogLevel logLevel)
        : StreamLogger{std::make_unique<std::ofstream>(filePath), logLevel} {}

    virtual ~FileLogger() = default;
};
