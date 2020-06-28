#pragma once

#include "../common/Process.hpp"
#include <memory>

class ViewportPatch {
public:
    ViewportPatch(const Process& process);
    ~ViewportPatch();

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};
