#pragma once

#include <filesystem>
#include <memory>

#include <Windows.h>

struct NtdllExports {
    using RtlVerifyVersionInfo_t = NTSTATUS (__stdcall*)(PRTL_OSVERSIONINFOEXW VersionInfo, ULONG TypeMask, ULONGLONG ConditionMask);

    NtdllExports(HMODULE dllHandle);

    RtlVerifyVersionInfo_t RtlVerifyVersionInfo{};
};

class NtdllLibrary {
public:
    NtdllLibrary(const std::filesystem::path& dllPath);
    virtual ~NtdllLibrary();
    HMODULE getDllHandle() const;
    const NtdllExports& getExports() const;

private:
    std::unique_ptr<NtdllExports> m_exports;
    HMODULE m_dllHandle;
};
