#pragma once

#include <filesystem>
#include <memory>

#include <ShellScalingApi.h>

struct ShcoreExports {
    using GetProcessDpiAwareness_t = HRESULT (__stdcall*)(HANDLE hprocess, PROCESS_DPI_AWARENESS* value);
    using SetProcessDpiAwareness_t = HRESULT (__stdcall*)(PROCESS_DPI_AWARENESS value);

    ShcoreExports(HMODULE dllHandle);

    GetProcessDpiAwareness_t GetProcessDpiAwareness{};
    SetProcessDpiAwareness_t SetProcessDpiAwareness{};
};

class ShcoreLibrary {
public:
    ShcoreLibrary(const std::filesystem::path& dllPath);
    virtual ~ShcoreLibrary();
    HMODULE getDllHandle() const;
    const ShcoreExports& getExports() const;
    bool isLoaded() const;

private:
    std::unique_ptr<ShcoreExports> m_exports;
    HMODULE m_dllHandle;
};
