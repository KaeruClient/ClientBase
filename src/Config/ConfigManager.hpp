//
// Created by KaeruClient on 2026/06/22.
//

#pragma once
#include <string>
#include <filesystem>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ConfigManager final {
public:
    ConfigManager();
    ~ConfigManager() = default;
    auto setup() -> void;

    auto loadConfig(const std::string& name, bool create) -> void;
    auto saveConfig() -> void;

    [[nodiscard]] auto getCurrentConfigObj() -> json& { return mCurrentConfigObj; }

private:
    std::string mCurrentConfig = "default";
    std::filesystem::path mConfigsPath;
    const std::string mConfigFormat = ".json";
    json mCurrentConfigObj;
    std::filesystem::path mActiveProfilePath;

    [[nodiscard]] auto fetchLastUsedConfigName() const -> std::string;
    auto saveLastUsedConfigName(const std::string& name) const -> void;
};