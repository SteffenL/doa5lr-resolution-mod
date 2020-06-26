#include "D3d9Library.hpp"

D3d9Exports::D3d9Exports(HMODULE dllHandle) {
    this->Direct3DCreate9Ex = reinterpret_cast<Direct3DCreate9Ex_t>(GetProcAddress(dllHandle, "Direct3DCreate9Ex"));
    this->Direct3DCreate9 = reinterpret_cast<Direct3DCreate9_t>(GetProcAddress(dllHandle, "Direct3DCreate9"));
    this->D3DPERF_BeginEvent = reinterpret_cast<D3DPERF_BeginEvent_t>(GetProcAddress(dllHandle, "D3DPERF_BeginEvent"));
    this->D3DPERF_EndEvent = reinterpret_cast<D3DPERF_EndEvent_t>(GetProcAddress(dllHandle, "D3DPERF_EndEvent"));
    this->D3DPERF_SetMarker = reinterpret_cast<D3DPERF_SetMarker_t>(GetProcAddress(dllHandle, "D3DPERF_SetMarker"));
    this->D3DPERF_SetRegion = reinterpret_cast<D3DPERF_SetRegion_t>(GetProcAddress(dllHandle, "D3DPERF_SetRegion"));
    this->D3DPERF_QueryRepeatFrame = reinterpret_cast<D3DPERF_QueryRepeatFrame_t>(GetProcAddress(dllHandle, "D3DPERF_QueryRepeatFrame"));
    this->D3DPERF_SetOptions = reinterpret_cast<D3DPERF_SetOptions_t>(GetProcAddress(dllHandle, "D3DPERF_SetOptions"));
    this->D3DPERF_GetStatus = reinterpret_cast<D3DPERF_GetStatus_t>(GetProcAddress(dllHandle, "D3DPERF_GetStatus"));
}

D3d9Library::D3d9Library(const std::filesystem::path& dllPath) {
    if (auto dllHandle = LoadLibraryW(dllPath.wstring().c_str())) {
        m_exports = std::make_unique<D3d9Exports>(dllHandle);
        m_dllHandle = dllHandle;
    }
}

D3d9Library::~D3d9Library() {
    if (m_dllHandle) {
        FreeLibrary(m_dllHandle);
        m_dllHandle = nullptr;
    }
}

HMODULE D3d9Library::getDllHandle() const {
    return m_dllHandle;
}

const D3d9Exports& D3d9Library::getExports() const {
    return *m_exports;
}
