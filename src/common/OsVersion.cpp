#include "OsVersion.hpp"

#include <map>
#include <stdexcept>

OsVersion::OsVersion() {}

bool OsVersion::is(int major, int minor, int build, OsVersionCondition condition) const {
    static std::map<OsVersionCondition, BYTE> conditionMap{
        {OsVersionCondition::GreaterOrEqual, VER_GREATER_EQUAL}
    };

    RTL_OSVERSIONINFOEXW versionInfo{};
    versionInfo.dwOSVersionInfoSize = sizeof(versionInfo);
    versionInfo.dwMajorVersion = major;
    versionInfo.dwMinorVersion = minor;
    versionInfo.dwBuildNumber = build;

    ULONG typeMask = VER_MAJORVERSION | VER_MINORVERSION | VER_BUILDNUMBER;
    BYTE condition_ = conditionMap.at(condition);

    ULONGLONG conditionMask = 0;
    conditionMask = VerSetConditionMask(conditionMask, VER_MAJORVERSION, condition_);
    conditionMask = VerSetConditionMask(conditionMask, VER_MINORVERSION, condition_);
    conditionMask = VerSetConditionMask(conditionMask, VER_BUILDNUMBER, condition_);

    auto status = m_ntdll.getExports().RtlVerifyVersionInfo(&versionInfo, typeMask, conditionMask);
    switch (status) {
        // STATUS_SUCCESS
        case 0:
            return true;
        // STATUS_REVISION_MISMATCH
        case 0xC0000059:
            return false;
        default:
            throw std::runtime_error{"Failed to check OS version"};
    }
}

OsVersion::~OsVersion() {}
