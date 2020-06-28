#include "FileLogger.hpp"
#include <mutex>
#include <Windows.h>

FileLogger::FileLogger(const std::filesystem::path& filePath, LogLevel logLevel)
    : Logger{logLevel},
      m_filePath{filePath} {
    auto filePathString = filePath.wstring();

    auto handle = CreateFileW(filePathString.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle == INVALID_HANDLE_VALUE) {
        throw std::runtime_error{"Failed to open log file"};
    }

    m_handle = handle;
}

FileLogger::~FileLogger() {
    CloseHandle(m_handle);
}

void FileLogger::doLog(const std::string& message) const {
    const auto& line = message + "\n";
    auto lineC = line.c_str();
    DWORD written{};

    std::lock_guard<std::mutex> guard{m_mutex};

    WriteFile(m_handle, lineC, line.size(), &written, NULL);
}
