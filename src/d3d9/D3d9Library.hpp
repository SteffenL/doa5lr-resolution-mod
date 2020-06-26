#pragma once

#include <filesystem>
#include <memory>

#include <d3d9.h>

struct D3d9Exports {
    using Direct3DCreate9_t = IDirect3D9* (WINAPI*)(UINT SDKVersion);
    using Direct3DCreate9Ex_t = HRESULT (WINAPI*)(UINT SDKVersion, IDirect3D9Ex** result);
    using D3DPERF_BeginEvent_t = int (WINAPI*)(D3DCOLOR col, LPCWSTR wszName);
    using D3DPERF_EndEvent_t = int (WINAPI*)();
    using D3DPERF_SetMarker_t = void (WINAPI*)(D3DCOLOR col, LPCWSTR wszName);
    using D3DPERF_SetRegion_t = void (WINAPI*)(D3DCOLOR col, LPCWSTR wszName);
    using D3DPERF_QueryRepeatFrame_t = BOOL (WINAPI*)();
    using D3DPERF_SetOptions_t = void (WINAPI*)(DWORD dwOptions);
    using D3DPERF_GetStatus_t = DWORD (WINAPI*)();

    D3d9Exports(HMODULE dllHandle);

    Direct3DCreate9_t Direct3DCreate9{};
    Direct3DCreate9Ex_t Direct3DCreate9Ex{};
    D3DPERF_BeginEvent_t D3DPERF_BeginEvent{};
    D3DPERF_EndEvent_t D3DPERF_EndEvent{};
    D3DPERF_SetMarker_t D3DPERF_SetMarker{};
    D3DPERF_SetRegion_t D3DPERF_SetRegion{};
    D3DPERF_QueryRepeatFrame_t D3DPERF_QueryRepeatFrame{};
    D3DPERF_SetOptions_t D3DPERF_SetOptions{};
    D3DPERF_GetStatus_t D3DPERF_GetStatus{};
};

class D3d9Library {
public:
    D3d9Library(const std::filesystem::path& dllPath);
    virtual ~D3d9Library();
    HMODULE getDllHandle() const;
    const D3d9Exports& getExports() const;

private:
    std::unique_ptr<D3d9Exports> m_exports;
    HMODULE m_dllHandle;
};
