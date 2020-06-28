#pragma once

#include "Logger.hpp"

class NullLogger : public Logger {
public:
    NullLogger();
    virtual ~NullLogger();
    virtual void doLog(const std::string& message) const override;
};
