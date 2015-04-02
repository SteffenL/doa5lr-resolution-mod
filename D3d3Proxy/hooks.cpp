#include "hooks.h"
#include "StandardPaths.h"
//#include "SequenceSearcher.h"

//#include <nowide/convert.hpp>

#include <vector>
#include <string>
#include <map>
#include <stdio.h>

#include <MinHook.h>
#include <DbgHelp.h>
#include <strsafe.h>

#pragma comment(lib, "DbgHelp.lib")

#define MOD_SHORT_NAME "ResMod"

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)

D3d9OrigFunctions g_origDllFunctions;
HMODULE g_origDll = NULL;
uintptr_t g_baseOfCode = 0;
uintptr_t g_sizeOfCode = 0;

//std::vector<HMODULE> g_proxyLibraries_D3d9;
std::vector<D3d9OrigFunctions::Direct3DCreate9_t> g_proxyChain_Direct3DCreate9;


int showMessageBox(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
    char text[1000] = { 0 };
    StringCchCatA(text, _countof(text), "[");
    StringCchCatA(text, _countof(text), MOD_SHORT_NAME);
    StringCchCatA(text, _countof(text), "] ");
    StringCchCatA(text, _countof(text), lpText);

    return MessageBoxA(hWnd, text, lpCaption, uType);
}

int showMessageBox(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
    wchar_t text[1000] = { 0 };
    StringCchCatW(text, _countof(text), L"[");
    StringCchCatW(text, _countof(text), WIDEN(MOD_SHORT_NAME));
    StringCchCatW(text, _countof(text), L"] ");
    StringCchCatW(text, _countof(text), lpText);

    return MessageBoxW(hWnd, text, lpCaption, uType);
}


typedef void(_stdcall* SetResolution_t)(int width, int height, int unknown1, int unknown2);
SetResolution_t SetResolutionOrig;

struct CustomVideoSettings_t
{
    unsigned int ResolutionWidth;
    unsigned int ResolutionHeight;

    CustomVideoSettings_t()
        : ResolutionWidth(0), ResolutionHeight(0)
    {
    }
};

CustomVideoSettings_t g_videoSettings;

void _stdcall SetResolutionDetour(int width, int height, int unknown1, int unknown2)
{
    width = g_videoSettings.ResolutionWidth;
    height = g_videoSettings.ResolutionHeight;

    SetResolutionOrig(width, height, unknown1, unknown2);
}


typedef HANDLE(WINAPI* CreateFileA_t)(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
CreateFileA_t CreateFileAOrig;

HANDLE WINAPI CreateFileADetour(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
    showMessageBox(0, lpFileName, "File", 0);

    return CreateFileAOrig(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}


BOOL hookGame()
{


    //
    // Create  hooks
    //


    uintptr_t SetResolution_offset;
    /*
    CPU Disasm
    Address   Hex dump                   Command                                                                                Comments
    00498A00      55                     push    ebp                                                                            ; game.00498A00(guessed Arg1,Arg2,Arg3,Arg4)
    00498A01      8BEC                   mov     ebp, esp
    00498A03      53                     push    ebx
    00498A04      8B5D 14                mov     ebx, dword ptr ss:[ebp+14]
    00498A07      56                     push    esi
    00498A08      57                     push    edi
    00498A09      8B7D 0C                mov     edi, dword ptr ss:[ebp+0C]
    00498A0C      8BC7                   mov     eax, edi
    00498A0E      0FAF45 10              imul    eax, dword ptr ss:[ebp+10]
    00498A12      33D2                   xor     edx, edx
    00498A14      F7F3                   div     ebx
    00498A16      8B75 08                mov     esi, dword ptr ss:[ebp+8]
    */

    char* sequence = "\x55\x8B\xEC\x53\x8B\x5D\x14\x56\x57\x8B\x7D\x0C\x8B\xC7\x0F\xAF\x45\x10\x33\xD2\xF7\xF3\x8B\x75\x08";
    {
        const int want = 25;
        int matched = 0;
        char* p = (char*)g_baseOfCode;
        char* pseq = sequence;
        while (matched < want && p < (char*)(g_baseOfCode + g_sizeOfCode)) {
            if (*(p + matched) == *(pseq + matched)) {
                ++matched;
            }
            else {
                matched = 0;
                ++p;
            }
        }

        if (matched != want) {
            showMessageBox(NULL, "Failed to locate SetResolution. Wrong version of the game?", NULL, MB_ICONERROR);
            ExitProcess(1);
            return FALSE;
        }

        SetResolution_offset = (uintptr_t)(p - g_baseOfCode);
    }

    //showMessageBox(NULL, "Attach", NULL, MB_ICONERROR);
    //DebugBreak();

    if (MH_CreateHook(reinterpret_cast<void*>(g_baseOfCode + SetResolution_offset), &SetResolutionDetour, reinterpret_cast<LPVOID*>(&SetResolutionOrig)) != MH_OK) {
        showMessageBox(NULL, "Failed to create hook for SetResolution.", NULL, MB_ICONERROR);
        ExitProcess(1);
        return FALSE;
    }


    //
    // Enable  hooks
    //

    if (MH_EnableHook(reinterpret_cast<void*>(g_baseOfCode + SetResolution_offset)) != MH_OK) {
        showMessageBox(NULL, "Failed to enable hook for SetResolution.", NULL, MB_ICONERROR);
        ExitProcess(1);
        return FALSE;
    }

    return TRUE;
}

typedef DWORD (WINAPI* GetPrivateProfileStringA_t)(
__in_opt LPCSTR lpAppName,
__in_opt LPCSTR lpKeyName,
__in_opt LPCSTR lpDefault,
__out_ecount_part_opt(nSize, return +1) LPSTR lpReturnedString,
__in     DWORD nSize,
__in_opt LPCSTR lpFileName
);

GetPrivateProfileStringA_t GetPrivateProfileStringAOrig;


DWORD WINAPI GetPrivateProfileStringADetour(
    __in_opt LPCSTR lpAppName,
    __in_opt LPCSTR lpKeyName,
    __in_opt LPCSTR lpDefault,
    __out_ecount_part_opt(nSize, return +1) LPSTR lpReturnedString,
    __in     DWORD nSize,
    __in_opt LPCSTR lpFileName
)
{
    MH_DisableHook(GetPrivateProfileStringA);

    hookGame();

    return GetPrivateProfileStringAOrig(lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize, lpFileName);
}


/*
OrigFunctions::Direct3DCreate9_t Direct3DCreate9Orig;

IDirect3D9* _stdcall Direct3DCreate9Detour(UINT SDKVersion)
{
    for (auto Direct3DCreate9_proxy : g_proxyChain_Direct3DCreate9) {
        if (!Direct3DCreate9_proxy(SDKVersion)) {
            continue;
        }
    }

    return Direct3DCreate9Orig(SDKVersion);
}*/


bool loadD3d9()
{
    wchar_t systemDir[MAX_PATH] = { 0 };
    GetSystemDirectoryW(systemDir, _countof(systemDir));

    wchar_t origDllFullPath[MAX_PATH] = { 0 };
    StringCchCatW(origDllFullPath, _countof(origDllFullPath), systemDir);
    StringCchCatW(origDllFullPath, _countof(origDllFullPath), L"\\d3d9.dll");

    auto origDll = LoadLibraryW(origDllFullPath);
    if (!origDll) {
        wchar_t msg[500] = { 0 };
        StringCchCatW(msg, _countof(msg), L"Failed to load libpad; make sure the original file:");
        StringCchCatW(msg, _countof(msg), origDllFullPath);
        showMessageBox(NULL, msg, NULL, MB_ICONERROR);
        return false;
    }

    g_origDll = origDll;

    g_origDllFunctions.Direct3DCreate9 = reinterpret_cast<D3d9OrigFunctions::Direct3DCreate9_t>(GetProcAddress(origDll, "Direct3DCreate9"));

    if (!g_origDllFunctions.Direct3DCreate9)
    {
        showMessageBox(NULL, "Failed to get original address of Direct3DCreate9.", NULL, MB_ICONERROR);
        return false;
    }

    return !!origDll;
}


bool loadConfig()
{
    wchar_t configFilePath[MAX_PATH];
    core::StandardPaths::GetExecutableDir(configFilePath, _countof(configFilePath));
    StringCchCatW(configFilePath, _countof(configFilePath), L"\\mod_resolution.ini");

    // Video resolution
    {
        wchar_t videoResolutionString[100];
        GetPrivateProfileStringW(L"Video", L"Resolution", NULL, videoResolutionString, _countof(videoResolutionString), configFilePath);
        if (wcslen(videoResolutionString) == 0) {
            showMessageBox(NULL, "Video resolution has not been configured.", NULL, MB_ICONERROR);
            return false;
        }

        if (swscanf_s(videoResolutionString, L"%ux%u", &g_videoSettings.ResolutionWidth, &g_videoSettings.ResolutionHeight) == EOF) {
            showMessageBox(NULL, "Failed to parse video resolutions string.", NULL, MB_ICONERROR);
            return false;
        }
    }

    // Proxy chains (D3D9)
    {
        const int maxEntries = 128;
        //wchar_t buffer[MAX_PATH * maxEntries] = { 0 };
        // buffer, _countof(buffer)
        wchar_t keyNamesBuffer[200] = { 0 };
        GetPrivateProfileStringW(L"ProxyChain_D3D9", NULL, NULL, keyNamesBuffer, _countof(keyNamesBuffer), configFilePath);

        for (wchar_t* keyName = keyNamesBuffer; *keyName != L'\0'; keyName += wcslen(keyName) + 1) {
            // Line comments (;) are automatically ignored by the Windows API function

            wchar_t libPath[MAX_PATH] = { 0 };
            GetPrivateProfileStringW(L"ProxyChain_D3D9", keyName, NULL, libPath, _countof(libPath), configFilePath);
            if (wcslen(libPath) == 0) {
                // Ignore
                continue;
            }

            auto libHandle = LoadLibraryW(libPath);
            if (!libHandle) {
                wchar_t msg[1000];
                StringCchCopyW(msg, _countof(msg), L"Failed to load library:\n");
                StringCchCatW(msg, _countof(msg), libPath);
                showMessageBox(NULL, msg, NULL, MB_ICONERROR);
                return false;
            }


            auto Direct3DCreate9_proxy = reinterpret_cast<D3d9OrigFunctions::Direct3DCreate9_t>(GetProcAddress(libHandle, "Direct3DCreate9"));
            if (!Direct3DCreate9_proxy) {
                wchar_t msg[1000];
                StringCchCopyW(msg, _countof(msg), L"Failed to get original address of Direct3DCreate9 (");
                StringCchCatW(msg, _countof(msg), keyName);
                StringCchCatW(msg, _countof(msg), L").");
                showMessageBox(NULL, msg, NULL, MB_ICONERROR);
                return false;
            }

            /*if (MH_CreateHook(Direct3DCreate9_proxy, &Direct3DCreate9Detour_proxy, reinterpret_cast<LPVOID*>(&Direct3DCreate9Orig_proxy)) != MH_OK) {
                wchar_t msg[1000];
                StringCchCopyW(msg, _countof(msg), L"Failed to create hook for Direct3DCreate9 (");
                StringCchCatW(msg, _countof(msg), keyName);
                StringCchCatW(msg, _countof(msg), L").");
                showMessageBox(NULL, msg, NULL, MB_ICONERROR);
                return false;
            }

            if (MH_EnableHook(Direct3DCreate9_proxy) != MH_OK) {
                wchar_t msg[1000];
                StringCchCopyW(msg, _countof(msg), L"Failed to enable hook for Direct3DCreate9 (");
                StringCchCatW(msg, _countof(msg), keyName);
                StringCchCatW(msg, _countof(msg), L").");
                showMessageBox(NULL, msg, NULL, MB_ICONERROR);
                return false;
            }*/

            //g_proxyLibraries_D3d9[libHandle] = Direct3DCreate9Orig_proxy;
            g_proxyChain_Direct3DCreate9.push_back(Direct3DCreate9_proxy);
        }
    }

    return true;
}

bool loadProxyChain_D3d9()
{

    return true;
}

bool loadProxyChains()
{
    if (!loadProxyChain_D3d9()) {
        return false;
    }

    return true;
}


BOOL initializeHooks()
{
    //showMessageBox(NULL, "Attach", NULL, MB_ICONERROR);

    bool isOk = false;
    do {
        if (!loadConfig()) {
            break;
        }

        if (!loadD3d9()) {
            break;
        }

        if (!loadProxyChains()) {
            break;
        }

        auto mainExe = GetModuleHandleA(NULL);
        if (!mainExe) {
            showMessageBox(NULL, "Something is wrong...", NULL, MB_ICONERROR);
            break;
        }

        if (MH_Initialize() != MH_OK) {
            showMessageBox(NULL, "Failed to initialize MinHook.", NULL, MB_ICONERROR);
            break;
        }

        uintptr_t imageBase = reinterpret_cast<uintptr_t>(mainExe);
        auto ntHeaders = ImageNtHeader(reinterpret_cast<void*>(imageBase));
        g_baseOfCode = imageBase + ntHeaders->OptionalHeader.BaseOfCode;
        g_sizeOfCode = ntHeaders->OptionalHeader.SizeOfCode;

        //
        // Create  hooks
        //

        if (MH_CreateHook(GetPrivateProfileStringA, &GetPrivateProfileStringADetour, reinterpret_cast<LPVOID*>(&GetPrivateProfileStringAOrig)) != MH_OK) {
            showMessageBox(NULL, "Failed to create hook for SetResolution.", NULL, MB_ICONERROR);
            break;
        }

        /*if (MH_CreateHook(CreateFileA, &CreateFileADetour, reinterpret_cast<LPVOID*>(&CreateFileAOrig)) != MH_OK) {
            showMessageBox(NULL, "Failed to create hook for CreateFileA.", NULL, MB_ICONERROR);
            break;
        }*/

        /*if (MH_CreateHook(g_origDllFunctions.Direct3DCreate9, &Direct3DCreate9Detour, reinterpret_cast<LPVOID*>(&Direct3DCreate9Orig)) != MH_OK) {
            showMessageBox(NULL, "Failed to create hook for Direct3DCreate9.", NULL, MB_ICONERROR);
            return false;
        }*/


        //
        // Enable  hooks
        //

        /*if (MH_EnableHook(g_origDllFunctions.Direct3DCreate9) != MH_OK) {
            showMessageBox(NULL, "Failed to enable hook for Direct3DCreate9.", NULL, MB_ICONERROR);
            return false;
        }*/

        if (MH_EnableHook(GetPrivateProfileStringA) != MH_OK) {
            showMessageBox(NULL, "Failed to enable hook for SetResolution.", NULL, MB_ICONERROR);
            break;
        }

        /*if (MH_EnableHook(CreateFileA) != MH_OK) {
            showMessageBox(NULL, "Failed to enable hook for CreateFileA.", NULL, MB_ICONERROR);
            break;
        }*/

        isOk = true;
    } while (false);

    if (!isOk) {
        ExitProcess(1);
    }

    return isOk ? TRUE : FALSE;
}

BOOL shutdownHooks()
{
    MH_Uninitialize();

    if (g_origDll && FreeLibrary(g_origDll)) {
        g_origDll = NULL;
    }

    return TRUE;
}
