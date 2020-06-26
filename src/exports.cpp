#include "GameModInitializer.hpp"

IDirect3D9* WINAPI Direct3DCreate9(UINT SDKVersion) {
    auto& d3d9Library = g_gameModInitializer->getGameMod().getD3d9Library();
    return d3d9Library.getExports().Direct3DCreate9(SDKVersion);
}

HRESULT WINAPI Direct3DCreate9Ex(UINT SDKVersion, IDirect3D9Ex** result) {
    auto& d3d9Library = g_gameModInitializer->getGameMod().getD3d9Library();
    return d3d9Library.getExports().Direct3DCreate9Ex(SDKVersion, result);
}

int WINAPI D3DPERF_BeginEvent(D3DCOLOR col, LPCWSTR wszName) {
    auto& d3d9Library = g_gameModInitializer->getGameMod().getD3d9Library();
    return d3d9Library.getExports().D3DPERF_BeginEvent(col, wszName);
}

int WINAPI D3DPERF_EndEvent() {
    auto& d3d9Library = g_gameModInitializer->getGameMod().getD3d9Library();
    return d3d9Library.getExports().D3DPERF_EndEvent();
}

void WINAPI D3DPERF_SetMarker(D3DCOLOR col, LPCWSTR wszName) {
    auto& d3d9Library = g_gameModInitializer->getGameMod().getD3d9Library();
    d3d9Library.getExports().D3DPERF_SetMarker(col, wszName);
}

void WINAPI D3DPERF_SetRegion(D3DCOLOR col, LPCWSTR wszName) {
    auto& d3d9Library = g_gameModInitializer->getGameMod().getD3d9Library();
    d3d9Library.getExports().D3DPERF_SetRegion(col, wszName);
}

BOOL WINAPI D3DPERF_QueryRepeatFrame() {
    auto& d3d9Library = g_gameModInitializer->getGameMod().getD3d9Library();
    return d3d9Library.getExports().D3DPERF_QueryRepeatFrame();
}

void WINAPI D3DPERF_SetOptions(DWORD dwOptions) {
    auto& d3d9Library = g_gameModInitializer->getGameMod().getD3d9Library();
    d3d9Library.getExports().D3DPERF_SetOptions(dwOptions);
}

DWORD WINAPI D3DPERF_GetStatus() {
    auto& d3d9Library = g_gameModInitializer->getGameMod().getD3d9Library();
    return d3d9Library.getExports().D3DPERF_GetStatus();
}
