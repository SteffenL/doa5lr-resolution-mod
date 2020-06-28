#include "NullLogger.hpp"

NullLogger::NullLogger() : Logger{LogLevel::Silent} {}

NullLogger::~NullLogger() {}

void NullLogger::doLog(const std::string& message) const {
    // Do nothing
}
