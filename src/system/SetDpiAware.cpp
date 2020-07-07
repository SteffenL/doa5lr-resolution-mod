#include "SetDpiAware.hpp"
#include <stdexcept>
#include <Windows.h>

SetDpiAware::SetDpiAware() {
    if (!SetProcessDPIAware()) {
        throw std::runtime_error{"Unable to set process DPI awareness"};
    }
}

SetDpiAware::~SetDpiAware() {}
