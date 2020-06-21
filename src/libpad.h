#pragma once

#include "d3d9_types.h"

// Exports

__declspec(dllexport) IDirect3D9* WINAPI Direct3DCreate9(UINT SDKVersion);

__declspec(dllexport) int WINAPI D3DPERF_BeginEvent(D3DCOLOR col, LPCWSTR wszName);
__declspec(dllexport) int WINAPI D3DPERF_EndEvent(void);
__declspec(dllexport) void WINAPI D3DPERF_SetMarker(D3DCOLOR col, LPCWSTR wszName);
__declspec(dllexport) void WINAPI D3DPERF_SetRegion(D3DCOLOR col, LPCWSTR wszName);
__declspec(dllexport) BOOL WINAPI D3DPERF_QueryRepeatFrame(void);

__declspec(dllexport) void WINAPI D3DPERF_SetOptions(DWORD dwOptions);
__declspec(dllexport) DWORD WINAPI D3DPERF_GetStatus(void);
