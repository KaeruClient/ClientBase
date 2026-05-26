//
// Created by ikasa on 2026/05/25.
//

#include "HookManager.hpp"

#include <cassert>

#include "Hooks/Core/MinecraftGameHook.hpp"
#include "Hooks/Input/KeyboardHook.hpp"
#include "Hooks/Core/ClientInstanceHook.hpp"
#include "Hooks/Windows/FreeLibraryAndExitHook.hpp"

void HookManager::init() {
    auto& instance = getInstance();
    auto& hooks = instance.mHooks;
    assert(hooks.empty() && "[HookManager] Already called init()!");

    // Hook Registration Process
    {
        hooks.emplace_back(std::make_unique<MinecraftGameHook>());
        hooks.emplace_back(std::make_unique<KeyboardHook>());
        hooks.emplace_back(std::make_unique<ClientInstanceHook>());
        hooks.emplace_back(std::make_unique<FreeLibraryAndExitHook>());
    }

    instance.mInitialized = true;
}

void HookManager::shutdown() {
    auto& instance = getInstance();
    assert(instance.mInitialized && "[HookManager] init() wasn't called!");
    instance.mHooks.clear();
    instance.mInitialized = false;
}
