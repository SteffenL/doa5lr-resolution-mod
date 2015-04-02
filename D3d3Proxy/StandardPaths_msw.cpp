#include "StandardPaths_msw.h"

#include <Windows.h>
#include <Shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")

namespace core {

bool StandardPaths_msw::GetExecutablePath(wchar_t buffer[], unsigned int length) {

    bool isOk = false;

    do {
        DWORD requiredLength = ::GetModuleFileNameW(NULL, &buffer[0], length);

        // Failed?
        if (requiredLength == 0) {
            break;
        }

        buffer[MAX_PATH - 1] = L'\0';
        isOk = true;
    } while (!isOk);

    return isOk;
}

bool StandardPaths_msw::GetExecutableDir(wchar_t buffer[], unsigned int length)
{
    bool isOk = false;

    do {
        if (!GetExecutablePath(buffer, length)) {
            break;
        }

        ::PathRemoveFileSpecW(buffer);

        isOk = true;
    } while (!isOk);

    return isOk;
}

} // namespace