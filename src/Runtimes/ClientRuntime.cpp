//
// Created by KaeruClient on 2026/05/23.
//

#include "ClientRuntime.hpp"
#include <Feature/Module/ModuleManager.hpp>
#include <Config/ConfigManager.hpp>
#include <Memory/HookManager.hpp>

auto ClientRuntime::initComponents() -> void {
    auto& instance = getInstance();
    instance.mModuleManager = std::make_unique<ModuleManager>();
    instance.mConfigManager = std::make_unique<ConfigManager>();
    instance.mHookManager   = std::make_unique<HookManager>();
}

ClientRuntime::ClientRuntime() = default;
ClientRuntime::~ClientRuntime() = default;