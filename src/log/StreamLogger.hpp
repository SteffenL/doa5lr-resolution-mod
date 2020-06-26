#pragma once

#include "Logger.hpp"

#include <iosfwd>
#include <mutex>

class StreamLogger : public Logger {
public:
    StreamLogger(std::unique_ptr<std::ostream> stream, LogLevel logLevel);
    virtual ~StreamLogger();
    virtual void doLog(const std::string& message) const override;

private:
    mutable std::mutex m_mutex;
    std::unique_ptr<std::ostream> m_stream;
};
