#pragma once

struct GameFunctions {
    using SetResolution_t = void(__thiscall*)(void* self, int width, int height, int unknown1, int unknown2);

    SetResolution_t setResolution{};
};
