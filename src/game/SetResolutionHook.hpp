#pragma once

#include "../config/Config.hpp"
#include "../common/MinHook.hpp"
#include "../common/Process.hpp"
#include "../log/Logger.hpp"
#include <memory>

class SetResolutionHook {
public:
    SetResolutionHook(const Process& process, MinHook& minhook, const Config& config, const Logger& logger);
    ~SetResolutionHook();

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};
