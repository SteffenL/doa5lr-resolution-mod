#include "NtdllLibrary.hpp"

NtdllExports::NtdllExports(HMODULE dllHandle) {
    this->RtlVerifyVersionInfo = reinterpret_cast<RtlVerifyVersionInfo_t>(GetProcAddress(dllHandle, "RtlVerifyVersionInfo"));
}

NtdllLibrary::NtdllLibrary(const std::filesystem::path& dllPath) {
    if (auto dllHandle = LoadLibraryW(dllPath.wstring().c_str())) {
        m_exports = std::make_unique<NtdllExports>(dllHandle);
        m_dllHandle = dllHandle;
    }
}

NtdllLibrary::~NtdllLibrary() {
    if (m_dllHandle) {
        FreeLibrary(m_dllHandle);
        m_dllHandle = nullptr;
    }
}

HMODULE NtdllLibrary::getDllHandle() const {
    return m_dllHandle;
}

const NtdllExports& NtdllLibrary::getExports() const {
    return *m_exports;
}
