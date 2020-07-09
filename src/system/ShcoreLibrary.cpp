#include "ShcoreLibrary.hpp"

ShcoreExports::ShcoreExports(HMODULE dllHandle) {
    if (!dllHandle) {
        return;
    }

    this->GetProcessDpiAwareness = reinterpret_cast<GetProcessDpiAwareness_t>(GetProcAddress(dllHandle, "GetProcessDpiAwareness"));
    this->SetProcessDpiAwareness = reinterpret_cast<SetProcessDpiAwareness_t>(GetProcAddress(dllHandle, "SetProcessDpiAwareness"));
}

ShcoreLibrary::ShcoreLibrary(const std::filesystem::path& dllPath) {
    if (auto dllHandle = LoadLibraryW(dllPath.wstring().c_str())) {
        m_exports = std::make_unique<ShcoreExports>(dllHandle);
        m_dllHandle = dllHandle;
    }
}

ShcoreLibrary::~ShcoreLibrary() {
    if (m_dllHandle) {
        FreeLibrary(m_dllHandle);
        m_dllHandle = nullptr;
    }
}

HMODULE ShcoreLibrary::getDllHandle() const {
    return m_dllHandle;
}

const ShcoreExports& ShcoreLibrary::getExports() const {
    return *m_exports;
}

bool ShcoreLibrary::isLoaded() const {
    return m_exports && m_dllHandle;
}
