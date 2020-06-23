# DOA5LR Resolution Mod

A Direct3D 9 wrapper that modifies the game "Dead or Alive 5: Last Round" (Steam) to allow using custom screen resolutions.

Aspect ratios such as 21:9 and wider work without black bars, at the cost of stretched menus/HUD.

## Building

    git submodule update --init
    mkdir build && cd build
    cmake .. -G "Visual Studio 16 2019" -A Win32
    cmake --build . --config Release
