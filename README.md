# DOA5LR Resolution Mod

A Direct3D 9 wrapper that modifies the game "Dead or Alive 5: Last Round" (Steam) to allow using custom screen resolutions.

Aspect ratios such as 21:9 and wider work without black bars, at the cost of stretched menus/HUD.

## Building

    git submodule update --init
    mkdir build && cd build
    cmake .. -G "Visual Studio 16 2019" -A Win32
    cmake --build . --config Release

## License

    Copyright 2020 Steffen André Langnes

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
