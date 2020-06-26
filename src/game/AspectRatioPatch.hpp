#pragma once

#include "../config/Config.hpp"
#include "../common/Process.hpp"

#include <vector>

class AspectRatioPatch {
public:
    AspectRatioPatch(const Process& process, const Config& config) {
        const auto& videoResolution = config.getVideoResolution();
        auto width = videoResolution.getWidth();
        auto height = videoResolution.getHeight();

        uintptr_t baseOfData{};
        process.getBaseOfData(baseOfData);

        // The read-only variable with the aspect ratio is referenced in multiple places and here is one of the functions
        // where it's frequently accessed.

        /*
        00DFB980 | 55                       | push ebp                                                         |
        00DFB981 | 8BEC                     | mov ebp,esp                                                      |
        00DFB983 | 51                       | push ecx                                                         |
        00DFB984 | 803D 83BFB102 00         | cmp byte ptr ds:[2B1BF83],0                                      |
        00DFB98B | 74 14                    | je game.DFB9A1                                                   |
        00DFB98D | F3:0F1005 CC6E4401       | movss xmm0,dword ptr ds:[1446ECC]                                | Aspect ratio 1.7777 (16:9)
        00DFB995 | F3:0F1145 FC             | movss dword ptr ss:[ebp-4],xmm0                                  |
        00DFB99A | D945 FC                  | fld st(0),dword ptr ss:[ebp-4]                                   |
        00DFB99D | 8BE5                     | mov esp,ebp                                                      |
        00DFB99F | 5D                       | pop ebp                                                          |
        00DFB9A0 | C3                       | ret                                                              |
        00DFB9A1 | F3:0F1005 58B24401       | movss xmm0,dword ptr ds:[144B258]                                | Aspect ratio 1.3333 (4:3)
        00DFB9A9 | F3:0F1145 FC             | movss dword ptr ss:[ebp-4],xmm0                                  |
        00DFB9AE | D945 FC                  | fld st(0),dword ptr ss:[ebp-4]                                   |
        00DFB9B1 | 8BE5                     | mov esp,ebp                                                      |
        00DFB9B3 | 5D                       | pop ebp                                                          |
        00DFB9B4 | C3                       | ret                                                              |
        */

        m_backupPtrs.emplace_back(reinterpret_cast<float*>(baseOfData + 0x50ECC)); // 16:9
        m_backupPtrs.emplace_back(reinterpret_cast<float*>(baseOfData + 0x55258)); // 4:3

        for (auto ptr : m_backupPtrs) {
            m_backupValues.emplace_back(*ptr);

            DWORD oldProtect{};
            if (!VirtualProtect(ptr, 4, PAGE_READWRITE, &oldProtect)) {
                throw std::runtime_error{"Failed to change memory access"};
            }

            *ptr = static_cast<float>(width) / static_cast<float>(height);

            if (!VirtualProtect(ptr, 4, oldProtect, &oldProtect)) {
                throw std::runtime_error{"Failed to revert memory access"};
            }
        }
    }

    ~AspectRatioPatch() {
        for (int i = 0; i < m_backupPtrs.size(); ++i) {
            auto ptr = m_backupPtrs[i];
            auto value = m_backupValues[i];
            DWORD oldProtect{};
            if (VirtualProtect(ptr, 4, PAGE_READWRITE, &oldProtect)) {
                *ptr = value;
                VirtualProtect(ptr, 4, oldProtect, &oldProtect);
            }
        }
    }

private:
    std::vector<float*> m_backupPtrs{};
    std::vector<float> m_backupValues{};
};
