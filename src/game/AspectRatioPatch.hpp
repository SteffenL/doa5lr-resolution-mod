#pragma once

#include "../config/Config.hpp"
#include "../common/Process.hpp"

#include <memory>

class AspectRatioPatch {
public:
    AspectRatioPatch(const Process& process, const Config& config);
    ~AspectRatioPatch();

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};
