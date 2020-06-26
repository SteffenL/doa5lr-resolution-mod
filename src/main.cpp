#include "GameModInitializer.hpp"
#include <Windows.h>

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    try {
        switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hinstDLL);
            g_gameModInitializer = std::make_unique<GameModInitializer>();
            break;

        case DLL_PROCESS_DETACH:
            g_gameModInitializer.reset();
            break;
        }

        return TRUE;
    } catch (const std::exception& ex) {
        return FALSE;
    }
}
