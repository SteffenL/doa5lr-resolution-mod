#pragma once

#include "Config.hpp"
#include <filesystem>
#include <map>
#include <vector>

class FileConfig : public Config {
public:
    FileConfig(const std::filesystem::path& filePath) {
        const wchar_t* filePathC = filePath.c_str();

        // Log level
        {
            wchar_t stringValue[100]{};
            GetPrivateProfileStringW(L"Log", L"Level", NULL, stringValue, _countof(stringValue), filePathC);
            if (wcslen(stringValue) > 0) {
                unsigned int level{};
                if (swscanf_s(stringValue, L"%u", &level) == EOF) {
                    throw std::runtime_error{"Failed to parse log level config"};
                }

                m_logLevel = static_cast<LogLevel>(level);
            }
        }

        // Log file
        {
            wchar_t stringValue[MAX_PATH + 1]{};
            GetPrivateProfileStringW(L"Log", L"File", NULL, stringValue, _countof(stringValue), filePathC);
            if (wcslen(stringValue) > 0) {
                m_logFile = stringValue;
            }
        }

        // Video resolution
        {
            wchar_t stringValue[100]{};
            GetPrivateProfileStringW(L"Video", L"Resolution", NULL, stringValue, _countof(stringValue), filePathC);
            if (wcslen(stringValue) > 0) {
                unsigned int width{}, height{};
                if (swscanf_s(stringValue, L"%ux%u", &width, &height) == EOF) {
                    throw std::runtime_error{"Failed to parse video resolution config"};
                }

                m_videoResolution = VideoResolutionConfig{width, height};
                m_enableVideoResolution = true;
            }
        }

        // Aspect ratio
        {
            wchar_t stringValue[100]{};
            GetPrivateProfileStringW(L"Video", L"FixAspectRatio", NULL, stringValue, _countof(stringValue), filePathC);
            if (wcslen(stringValue) > 0) {
                unsigned int enable{};
                if (swscanf_s(stringValue, L"%u", &enable) == EOF) {
                    throw std::runtime_error{"Failed to parse aspect ratio config"};
                }

                if (enable == 1) {
                    m_fixAspectRatio = true;
                }
            }
        }

        // Viewport
        {
            wchar_t stringValue[100]{};
            GetPrivateProfileStringW(L"Video", L"FixViewport", NULL, stringValue, _countof(stringValue), filePathC);
            if (wcslen(stringValue) > 0) {
                unsigned int enable{};
                if (swscanf_s(stringValue, L"%u", &enable) == EOF) {
                    throw std::runtime_error{"Failed to parse viewport config"};
                }

                if (enable == 1) {
                    m_fixViewport = true;
                }
            }
        }

        // Proxy
        {
            std::vector<std::wstring> keys{L"d3d9"};
            std::map<std::wstring, std::filesystem::path> paths;

            for (const auto& key : keys) {
                wchar_t stringValue[MAX_PATH + 1]{};
                GetPrivateProfileStringW(L"Proxy", key.c_str(), NULL, stringValue, _countof(stringValue), filePathC);
                if (wcslen(stringValue) > 0) {
                    paths[key] = stringValue;
                }
            }

            m_proxy = ProxyConfig{paths[L"d3d9"]};
        }
    }
};
