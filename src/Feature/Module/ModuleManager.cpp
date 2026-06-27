//
// Created by KaeruClient on 2026/05/30.
//

#include "ModuleManager.hpp"

#include "Modules/Other/TestModule.hpp"

auto ModuleManager::init() -> void {
    registerModule<TestModule>();
}

auto ModuleManager::saveConfig(nlohmann::json& modulesJson) const -> void {
    for (const auto& entry : mModuleList) {
        nlohmann::json moduleJson;
        entry->saveConfig(moduleJson);
        modulesJson[entry->getName()] = moduleJson;
    }
}

auto ModuleManager::loadConfig(const nlohmann::json& modulesJson) const -> void {
    if (modulesJson.is_null()) return;

    for (auto& entry : mModuleList) {
        const std::string& name = entry->getName();
        if (modulesJson.contains(name))
            entry->loadConfig(modulesJson[name]);
    }
}