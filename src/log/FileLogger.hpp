#pragma once

#include "Logger.hpp"

#include <filesystem>
#include <mutex>

class FileLogger : public Logger {
public:
    FileLogger(const std::filesystem::path& filePath, LogLevel logLevel);
    virtual ~FileLogger();
    virtual void doLog(const std::string& message) const override;

private:
    mutable std::mutex m_mutex;
    std::filesystem::path m_filePath;
    void* m_handle{};
};
