#pragma once

#include <Windows.h>
#include <d3d9types.h>

using IDirect3D9 = void;

struct D3d9OrigFunctions
{
    using Direct3DCreate9_t = IDirect3D9* (WINAPI*)(UINT SDKVersion);

    using D3DPERF_BeginEvent_t = int (WINAPI*)(D3DCOLOR col, LPCWSTR wszName);
    using D3DPERF_EndEvent_t = int (WINAPI*)(void);
    using D3DPERF_SetMarker_t = void (WINAPI*)(D3DCOLOR col, LPCWSTR wszName);
    using D3DPERF_SetRegion_t = void (WINAPI*)(D3DCOLOR col, LPCWSTR wszName);
    using D3DPERF_QueryRepeatFrame_t = BOOL (WINAPI*)(void);

    using D3DPERF_SetOptions_t = void (WINAPI*)(DWORD dwOptions);
    using D3DPERF_GetStatus_t = DWORD (WINAPI*)(void);

    Direct3DCreate9_t Direct3DCreate9;

    D3DPERF_BeginEvent_t D3DPERF_BeginEvent;
    D3DPERF_EndEvent_t D3DPERF_EndEvent;
    D3DPERF_SetMarker_t D3DPERF_SetMarker;
    D3DPERF_SetRegion_t D3DPERF_SetRegion;
    D3DPERF_QueryRepeatFrame_t D3DPERF_QueryRepeatFrame;

    D3DPERF_SetOptions_t D3DPERF_SetOptions;
    D3DPERF_GetStatus_t D3DPERF_GetStatus;
};
