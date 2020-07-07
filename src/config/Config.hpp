#pragma once

#include "../log/LogLevel.hpp"

#include <filesystem>
#include <stdexcept>

class VideoResolutionConfig {
public:
    VideoResolutionConfig() {}

    VideoResolutionConfig(unsigned int width, unsigned int height)
        : m_width{width},
          m_height{height} {}

    unsigned int getWidth() const {
        return m_width;
    }

    unsigned int getHeight() const {
        return m_height;
    }

private:
    unsigned int m_width{};
    unsigned int m_height{};
};

class ProxyConfig {
public:
    ProxyConfig() {}

    ProxyConfig(const std::filesystem::path& d3d9)
        : m_d3d9{d3d9} {}

    std::filesystem::path getD3d9() const {
        return m_d3d9;
    }

private:
    std::filesystem::path m_d3d9{};
};

class Config {
public:
    bool getEnableVideoResolution() const {
        return m_enableVideoResolution;
    }

    std::filesystem::path getLogFile() const {
        return m_logFile;
    }

    LogLevel getLogLevel() const {
        return m_logLevel;
    }

    bool getFixAspectRatio() const {
        return m_fixAspectRatio;
    }

    bool getDpiAware() const {
        return m_dpiAware;
    }

    bool getFixViewport() const {
        return m_fixViewport;
    }

    const ProxyConfig& getProxy() const {
        return m_proxy;
    }

    const VideoResolutionConfig& getVideoResolution() const {
        return m_videoResolution;
    }

protected:
    bool m_enableVideoResolution{};
    std::filesystem::path m_logFile;
    LogLevel m_logLevel;
    bool m_fixAspectRatio{};
    bool m_dpiAware{};
    bool m_fixViewport{};
    ProxyConfig m_proxy;
    VideoResolutionConfig m_videoResolution;
};
