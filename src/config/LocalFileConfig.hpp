#pragma once

#include "FileConfig.hpp"
#include "../common/util.hpp"

class LocalFileConfig : public FileConfig {
public:
    LocalFileConfig() : FileConfig{getConfigPath()} {}

    static std::filesystem::path getConfigPath() {
        return util::getExecutableDir() / "mod_resolution.ini";
    }
};
