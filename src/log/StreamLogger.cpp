#include "StreamLogger.hpp"

#include <iosfwd>
#include <mutex>
#include <ostream>

StreamLogger::StreamLogger(std::unique_ptr<std::ostream> stream, LogLevel logLevel)
    : Logger{logLevel},
      m_stream{std::move(stream)} {}

StreamLogger::~StreamLogger() {}

void StreamLogger::doLog(const std::string& message) const {
    std::lock_guard<std::mutex> guard{m_mutex};
    *m_stream << message << std::endl;
}
