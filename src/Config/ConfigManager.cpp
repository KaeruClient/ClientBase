//
// Created by KaeruClient on 2026/06/22.
//

#include "ConfigManager.hpp"
#include <Feature/Module/ModuleManager.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "Runtimes/ClientRuntime.hpp"
#include "System/ClientStorage.hpp"


ConfigManager::ConfigManager() {
    mConfigsPath = ClientStorage::getPath("Configs");
    if (!std::filesystem::exists(mConfigsPath)) {
        std::filesystem::create_directories(mConfigsPath);
    }

    mActiveProfilePath = mConfigsPath / "ActiveProfile.txt";
}

auto ConfigManager::setup() -> void {
    mCurrentConfig = fetchLastUsedConfigName();
    loadConfig(mCurrentConfig, true);
}

auto ConfigManager::loadConfig(const std::string& name, const bool create) -> void {
    const auto configPath = mConfigsPath / (name + mConfigFormat);
    const bool configExists = std::filesystem::exists(configPath);

    if (configExists || create) {
        // save previous config automatically
        if (name != mCurrentConfig) {
            saveConfig();
        }

        mCurrentConfig = name;

        if (configExists) {
            std::ifstream confFile(configPath, std::ios::binary);
            if (confFile.is_open()) {
                try {
                    mCurrentConfigObj.clear();
                    confFile >> mCurrentConfigObj;
                } catch (const json::parse_error& e) {
                    // TODO: "[ConfigManager] JSON Parse Error: {}", e.what();
                    assert(false && "[ConfigManager] JSON Parse Error");
                }
                confFile.close();
            }
        }

        if (configExists) {
            ClientRuntime::getModuleManager().loadConfig(mCurrentConfigObj["Modules"]);
        }

        if (create && !configExists) {
            saveConfig();
        }

        saveLastUsedConfigName(mCurrentConfig);

        // TODO: "[ConfigManager] Successfully loaded config: {}", name;
    } else {
        // TODO: "[ConfigManager] Could not load config: {}", name;
    }
}

auto ConfigManager::saveConfig() -> void {
    const auto configPath = mConfigsPath / (mCurrentConfig + mConfigFormat);

    try {
        std::ofstream o(configPath, std::ios::binary);
        if (!o.is_open()) return;

        ClientRuntime::getModuleManager().saveConfig(mCurrentConfigObj["Modules"]);

        mCurrentConfigObj["from"] = CLIENT_NAME;
        o << std::setw(4) << mCurrentConfigObj << std::endl;
        o.flush();
        o.close();

    } catch (const std::exception& e) {
        // TODO: "[ConfigManager] Save Error: {}", e.what();
        assert(false && "[ConfigManager] Save Error");
    }
}

auto ConfigManager::fetchLastUsedConfigName() const -> std::string {
    if (!std::filesystem::exists(mActiveProfilePath)) {
        return "default";
    }

    std::ifstream in(mActiveProfilePath);
    if (!in.is_open()) return "default";

    std::string name;
    std::getline(in, name);
    in.close();

    return name.empty() ? "default" : name;
}

auto ConfigManager::saveLastUsedConfigName(const std::string& name) const -> void {
    try {
        std::ofstream out(mActiveProfilePath, std::ios::trunc);
        if (!out.is_open()) return;

        out << name;
        out.flush();
        out.close();
    } catch (const std::exception& e) {
        // TODO: "[ConfigManager] Active Profile Save Error: ", e.what();
        assert(false && "[ConfigManager] Active Profile Save Error");
    }
}