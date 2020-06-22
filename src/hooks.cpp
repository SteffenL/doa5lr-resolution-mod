#include "hooks.h"
#include "StandardPaths.h"

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

// https://stackoverflow.com/a/1186465
int calculateGcd(int a, int b)
{
    return (b == 0) ? a : calculateGcd(b, a % b);
}

BOOL search(const char* from, const char* to, const char* pattern, int patternLength, uintptr_t* resultOffset, const char* pattern2 = nullptr) {
    int matched = 0;
    const char* p = from;

    while (matched < patternLength && p < (to - patternLength)) {
        if (pattern2 && *(pattern2 + matched) == '?') {
            ++matched;
            continue;
        }

        if (*(p + matched) != *(pattern + matched) || p == pattern) {
            matched = 0;
            ++p;
            continue;
        }

        ++matched;
    }

    bool isMatch = matched == patternLength;

    if (isMatch && resultOffset) {
        *resultOffset = static_cast<uintptr_t>(p - from);
    }

    return isMatch;
}

bool getBaseOfCode(uintptr_t& baseOfCode, uintptr_t& sizeOfCode) {
    auto mainExe = GetModuleHandleA(NULL);
    if (!mainExe) {
        return false;
    }

    uintptr_t imageBase = reinterpret_cast<uintptr_t>(mainExe);
    auto ntHeaders = ImageNtHeader(reinterpret_cast<void*>(imageBase));
    baseOfCode = imageBase + ntHeaders->OptionalHeader.BaseOfCode;
    sizeOfCode = ntHeaders->OptionalHeader.SizeOfCode;
    return TRUE;
}

bool getBaseOfData(uintptr_t& baseOfData) {
    auto mainExe = GetModuleHandleA(NULL);
    if (!mainExe) {
        return false;
    }

    uintptr_t imageBase = reinterpret_cast<uintptr_t>(mainExe);
    auto ntHeaders = ImageNtHeader(reinterpret_cast<void*>(imageBase));
    baseOfData = imageBase + ntHeaders->OptionalHeader.BaseOfData;
    return true;
}


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

typedef void(__thiscall* SetResolution_t)(void* self, int width, int height, int unknown1, int unknown2);
SetResolution_t SetResolutionOrig;

typedef bool(__cdecl* SteamAPI_Init_t)();

SteamAPI_Init_t SteamAPI_Init;
SteamAPI_Init_t SteamAPI_InitOrig;

void __fastcall SetResolutionDetour(void* self, void* edx_, int width, int height, int unknown1, int unknown2)
{
    width = g_videoSettings.ResolutionWidth;
    height = g_videoSettings.ResolutionHeight;

    auto gcd = calculateGcd(width, height);
    unknown1 = width / gcd;
    unknown2 = height / gcd;

    SetResolutionOrig(self, width, height, unknown1, unknown2);
}

bool hookSetResolution(uintptr_t baseOfCode, uintptr_t sizeOfCode) {
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

    const char* pattern = "\x55\x8B\xEC\x53\x8B\x5D\x14\x56\x57\x8B\x7D\x0C\x8B\xC7\x0F\xAF\x45\x10\x33\xD2\xF7\xF3\x8B\x75\x08";
    const int patternLength = 25;
    uintptr_t offset = 0;

    if (!search(reinterpret_cast<const char*>(baseOfCode), reinterpret_cast<const char*>(baseOfCode + sizeOfCode), pattern, patternLength, &offset)) {
        return false;
    }

    auto SetResolution = baseOfCode + offset;

    if (MH_CreateHook(reinterpret_cast<void*>(SetResolution), &SetResolutionDetour, reinterpret_cast<LPVOID*>(&SetResolutionOrig)) != MH_OK) {
        return false;
    }

    if (MH_EnableHook(reinterpret_cast<void*>(SetResolution)) != MH_OK) {
        return false;
    }

    return true;
}

bool fixViewClipping(uintptr_t baseOfCode, uintptr_t sizeOfCode) {
    /*
    00994FFE | 74 0E                    | je game.99500E                                                   | Change to jmp to stretch image to full width
    00995000 | 84D2                     | test dl,dl                                                       |
    00995002 | 0F94C2                   | sete dl                                                          |
    00995005 | 8851 02                  | mov byte ptr ds:[ecx+2],dl                                       |
    00995008 | C641 03 01               | mov byte ptr ds:[ecx+3],1                                        |
    0099500C | EB 07                    | jmp game.995015                                                  |
    0099500E | C641 02 00               | mov byte ptr ds:[ecx+2],0                                        |
    00995012 | 8851 03                  | mov byte ptr ds:[ecx+3],dl                                       |
    00995015 | 8079 02 00               | cmp byte ptr ds:[ecx+2],0                                        |
    00995019 | 8B10                     | mov edx,dword ptr ds:[eax]                                       |
    0099501B | 8951 08                  | mov dword ptr ds:[ecx+8],edx                                     |
    0099501E | 8B40 04                  | mov eax,dword ptr ds:[eax+4]                                     |
    00995021 | 8941 0C                  | mov dword ptr ds:[ecx+C],eax                                     |
    */

    const char* pattern = "\x74\x0E\x84\xD2\x0F\x94\xC2\x88\x51\x02\xC6\x41\x03\x01\xEB\x07\xC6\x41\x02\x00\x88\x51\x03\x80\x79\x02\x00\x8B\x10\x89\x51\x08\x8B\x40\x04\x89\x41\x0C";
    const int patternLength = 25;
    uintptr_t offset = 0;

    if (!search(reinterpret_cast<const char*>(baseOfCode), reinterpret_cast<const char*>(baseOfCode + sizeOfCode), pattern, patternLength, &offset)) {
        return false;
    }

    auto instruction = reinterpret_cast<unsigned char*>(baseOfCode + offset);

    DWORD oldProtect{};
    if (!VirtualProtect(instruction, 1, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        return false;
    }

    *instruction = 0xeb; // jmp
    int instructionSize = 1;

    if (!VirtualProtect(instruction, instructionSize, oldProtect, &oldProtect)) {
        return false;
    }

    if (!FlushInstructionCache(GetCurrentProcess(), instruction, instructionSize)) {
        return false;
    }

    return true;
}

bool fixAspectRatio() {
    uintptr_t baseOfData{};
    if (!getBaseOfData(baseOfData)) {
        return false;
    }

    auto aspectRatioPtr = reinterpret_cast<float*>(baseOfData + 0x50ECC);

    DWORD oldProtect{};
    if (!VirtualProtect(aspectRatioPtr, 4, PAGE_READWRITE, &oldProtect)) {
        return false;
    }

    *aspectRatioPtr = static_cast<float>(g_videoSettings.ResolutionWidth) / static_cast<float>(g_videoSettings.ResolutionHeight);

    if (!VirtualProtect(aspectRatioPtr, 4, oldProtect, &oldProtect)) {
        return false;
    }

    return true;
}

bool g_resolutionHooked = false;
bool g_viewClippingFixed = false;

bool __cdecl SteamAPI_InitDetour()
{
    MH_DisableHook(SteamAPI_Init);

    uintptr_t baseOfCode{};
    uintptr_t sizeOfCode{};

    if (!getBaseOfCode(baseOfCode, sizeOfCode)) {
        return false;
    }

    if (!g_resolutionHooked) {
        if (hookSetResolution(baseOfCode, sizeOfCode)) {
            g_resolutionHooked = true;
        }
    }

    if (!g_viewClippingFixed) {
        if (fixViewClipping(baseOfCode, sizeOfCode)) {
            g_viewClippingFixed = true;
        }
    }

    return SteamAPI_InitOrig();
}

bool applyMainHooks() {
    auto steamApi = GetModuleHandleA("steam_api.dll");
    if (!steamApi) {
        return false;
    }

    SteamAPI_Init = reinterpret_cast<SteamAPI_Init_t>(GetProcAddress(steamApi, "SteamAPI_Init"));
    if (!SteamAPI_Init) {
        return false;
    }

    if (MH_CreateHook(SteamAPI_Init, &SteamAPI_InitDetour, reinterpret_cast<LPVOID*>(&SteamAPI_InitOrig)) != MH_OK) {
        return false;
    }

    if (MH_EnableHook(SteamAPI_Init) != MH_OK) {
        return false;
    }

    return true;
}

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

        if (MH_Initialize() != MH_OK) {
            showMessageBox(NULL, "Failed to initialize MinHook.", NULL, MB_ICONERROR);
            break;
        }

        if (!applyMainHooks()) {
            showMessageBox(NULL, "Failed to apply main hooks.", NULL, MB_ICONERROR);
            break;
        }

        if (!fixAspectRatio()) {
            showMessageBox(NULL, "Failed to fix aspect ratio.", NULL, MB_ICONERROR);
            break;
        }

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
