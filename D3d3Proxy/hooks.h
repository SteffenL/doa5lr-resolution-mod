#pragma once

#include "d3d9_types.h"
#include <vector>
#include <Windows.h>

extern D3d9OrigFunctions g_origDllFunctions;
extern HMODULE g_origDll;
extern std::vector<D3d9OrigFunctions::Direct3DCreate9_t> g_proxyChain_Direct3DCreate9;

BOOL initializeHooks();
BOOL shutdownHooks();
