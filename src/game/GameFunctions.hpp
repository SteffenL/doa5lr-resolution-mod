#pragma once

#include <Windows.h>

struct GameFunctions {
    using SetResolution_t = void(__thiscall*)(void* self, int width, int height, int unknown1, int unknown2);

    GameFunctions(HMODULE dllHandle);

    SetResolution_t setResolution{};
};
