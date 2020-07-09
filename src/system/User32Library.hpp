#pragma once

#include <filesystem>
#include <memory>

#include <Windows.h>

struct User32Exports {
    using SetProcessDPIAware_t = BOOL (__stdcall*)();
    using SetThreadDpiAwarenessContext_t = DPI_AWARENESS_CONTEXT (__stdcall*)(DPI_AWARENESS_CONTEXT dpiContext);

    User32Exports(HMODULE dllHandle);

    SetProcessDPIAware_t SetProcessDPIAware{};
    SetThreadDpiAwarenessContext_t SetThreadDpiAwarenessContext{};
};

class User32Library {
public:
    User32Library(const std::filesystem::path& dllPath);
    virtual ~User32Library();
    HMODULE getDllHandle() const;
    const User32Exports& getExports() const;
    bool isLoaded() const;

private:
    std::unique_ptr<User32Exports> m_exports;
    HMODULE m_dllHandle{};
};
