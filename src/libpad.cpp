#include "libpad.h"
#include "hooks.h"

IDirect3D9* WINAPI Direct3DCreate9(UINT SDKVersion)
{
    if (!g_proxyChain_Direct3DCreate9.empty()) {
        return g_proxyChain_Direct3DCreate9.front()(SDKVersion);
    }

    return g_origDllFunctions.Direct3DCreate9(SDKVersion);
}


int WINAPI D3DPERF_BeginEvent(D3DCOLOR col, LPCWSTR wszName)
{
    return g_origDllFunctions.D3DPERF_BeginEvent(col, wszName);
}

int WINAPI D3DPERF_EndEvent(void)
{
    return g_origDllFunctions.D3DPERF_EndEvent();
}

void WINAPI D3DPERF_SetMarker(D3DCOLOR col, LPCWSTR wszName)
{
    g_origDllFunctions.D3DPERF_SetMarker(col, wszName);
}

void WINAPI D3DPERF_SetRegion(D3DCOLOR col, LPCWSTR wszName)
{
    g_origDllFunctions.D3DPERF_SetRegion(col, wszName);
}

BOOL WINAPI D3DPERF_QueryRepeatFrame(void)
{
    return g_origDllFunctions.D3DPERF_QueryRepeatFrame();
}


void WINAPI D3DPERF_SetOptions(DWORD dwOptions)
{
    g_origDllFunctions.D3DPERF_SetOptions(dwOptions);
}

DWORD WINAPI D3DPERF_GetStatus(void)
{
    return g_origDllFunctions.D3DPERF_GetStatus();
}
