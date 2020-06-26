#pragma once

#include "../common/Process.hpp"
#include "../common/util.hpp"

class ViewportPatch {
public:
    ViewportPatch(const Process& process) {
        uintptr_t baseOfCode{};
        uintptr_t sizeOfCode{};
        process.getBaseOfCode(baseOfCode, sizeOfCode);

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

        if (!util::search(reinterpret_cast<const char*>(baseOfCode), reinterpret_cast<const char*>(baseOfCode + sizeOfCode), pattern, patternLength, &offset)) {
            throw std::runtime_error{"Unable to find code for viewport"};
        }

        auto instruction = reinterpret_cast<unsigned char*>(baseOfCode + offset);

        m_backupPtr = instruction;
        m_backupValue = *instruction;

        DWORD oldProtect{};
        if (!VirtualProtect(instruction, 1, PAGE_EXECUTE_READWRITE, &oldProtect)) {
            throw std::runtime_error{"Failed to change memory access"};
        }

        *instruction = 0xeb; // jmp
        int instructionSize = 1;

        if (!VirtualProtect(instruction, instructionSize, oldProtect, &oldProtect)) {
            throw std::runtime_error{"Failed to revert memory access"};
        }

        if (!FlushInstructionCache(GetCurrentProcess(), instruction, instructionSize)) {
            throw std::runtime_error{"Failed to flush instruction cache"};
        }
    }

    ~ViewportPatch() {
        DWORD oldProtect{};
        if (VirtualProtect(m_backupPtr, 1, PAGE_READWRITE, &oldProtect)) {
            *m_backupPtr = m_backupValue;
            VirtualProtect(m_backupPtr, 1, oldProtect, &oldProtect);
        }
    }

private:
    unsigned char* m_backupPtr{};
    unsigned char m_backupValue{};
};
