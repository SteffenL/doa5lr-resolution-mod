#include "SetResolutionHook.hpp"

#include "../common/util.hpp"
#include "GameFunctions.hpp"

#include <functional>
#include <stdexcept>

class SetResolutionHook::Impl {
public:
    Impl(const Process& process, MinHook& minhook, const Config& config, const Logger& logger)
        : m_process{process},
          m_minhook{minhook},
          m_config{config},
          m_logger{logger},
          m_setResolutionHook{m_minhook.create(findSetResolution(process), setResolutionDetour_)} {
        using namespace std::placeholders;
        sm_setResolutionDetour = std::bind(&Impl::setResolutionDetour, this, _1, _2, _3, _4, _5);
        m_setResolutionHook.enable();
    }

    void setResolutionDetour(void* self, int width, int height, int unknown1, int unknown2) {
        // We need to catch errors here since we are inside a hooked function
        try {
            m_setResolutionHook.disable();
            auto fn = reinterpret_cast<GameFunctions::SetResolution_t>(m_setResolutionHook.getOriginal());

            const auto& videoResolution = m_config.getVideoResolution();
            width = videoResolution.getWidth();
            height = videoResolution.getHeight();

            auto gcd = util::calculateGcd(width, height);
            unknown1 = width / gcd;
            unknown2 = height / gcd;

            fn(self, width, height, unknown1, unknown2);
        } catch (const std::exception& ex) {
            m_logger.error(ex.what());
            throw;
        }
    }

    static void __fastcall setResolutionDetour_(void* self, void* edx_, int width, int height, int unknown1, int unknown2) {
        return sm_setResolutionDetour(self, width, height, unknown1, unknown2);
    }

    void* findSetResolution(const Process& process) const {
        uintptr_t baseOfCode{};
        uintptr_t sizeOfCode{};
        process.getBaseOfCode(baseOfCode, sizeOfCode);

        /*
        00E855F0 | 55                       | push ebp                                                         |
        00E855F1 | 8BEC                     | mov ebp,esp                                                      |
        00E855F3 | 53                       | push ebx                                                         |
        00E855F4 | 8B5D 14                  | mov ebx,dword ptr ss:[ebp+14]                                    |
        00E855F7 | 56                       | push esi                                                         |
        00E855F8 | 57                       | push edi                                                         |
        00E855F9 | 8B7D 0C                  | mov edi,dword ptr ss:[ebp+C]                                     |
        00E855FC | 8BC7                     | mov eax,edi                                                      |
        00E855FE | 0FAF45 10                | imul eax,dword ptr ss:[ebp+10]                                   |
        00E85602 | 33D2                     | xor edx,edx                                                      |
        00E85604 | F7F3                     | div ebx                                                          |
        00E85606 | 8B75 08                  | mov esi,dword ptr ss:[ebp+8]                                     |
        */

        const char* pattern = "\x55\x8B\xEC\x53\x8B\x5D\x14\x56\x57\x8B\x7D\x0C\x8B\xC7\x0F\xAF\x45\x10\x33\xD2\xF7\xF3\x8B\x75\x08";
        const int patternLength = 25;
        uintptr_t offset = 0;

        if (!util::search(reinterpret_cast<const char*>(baseOfCode), reinterpret_cast<const char*>(baseOfCode + sizeOfCode), pattern, patternLength, &offset)) {
            throw std::runtime_error{"Unable to find code for setting resolution"};
        }

        auto address = baseOfCode + offset;
        return reinterpret_cast<void*>(address);
    }

private:
    MinHook& m_minhook;
    Hook m_setResolutionHook;
    static inline std::function<void(void* self, int width, int height, int unknown1, int unknown2)> sm_setResolutionDetour;
    const Process& m_process;
    const Config& m_config;
    const Logger& m_logger;
};

SetResolutionHook::SetResolutionHook(const Process& process, MinHook& minhook, const Config& config, const Logger& logger)
    : m_impl{std::make_unique<Impl>(process, minhook, config, logger)} {}

SetResolutionHook::~SetResolutionHook() {}
