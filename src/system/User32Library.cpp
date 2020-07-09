#include "User32Library.hpp"

User32Exports::User32Exports(HMODULE dllHandle) {
    this->SetProcessDPIAware = reinterpret_cast<SetProcessDPIAware_t>(GetProcAddress(dllHandle, "SetProcessDPIAware"));
    this->SetThreadDpiAwarenessContext = reinterpret_cast<SetThreadDpiAwarenessContext_t>(GetProcAddress(dllHandle, "SetThreadDpiAwarenessContext"));
}

User32Library::User32Library(const std::filesystem::path& dllPath) {
    if (auto dllHandle = LoadLibraryW(dllPath.wstring().c_str())) {
        m_exports = std::make_unique<User32Exports>(dllHandle);
        m_dllHandle = dllHandle;
    }
}

User32Library::~User32Library() {
    if (m_dllHandle) {
        FreeLibrary(m_dllHandle);
        m_dllHandle = nullptr;
    }
}

HMODULE User32Library::getDllHandle() const {
    return m_dllHandle;
}

const User32Exports& User32Library::getExports() const {
    return *m_exports;
}
