//
// Created by ikasa on 2026/05/25.
//

#include "HookManager.hpp"

#include <cassert>

#include "Memories/Hooks/Input/KeyboardHook.hpp"
#include "Memories/Hooks/Core/ClientInstanceHook.hpp"
#include "Memories/Hooks/Windows/FreeLibraryAndExitHook.hpp"

HookManager::HookManager() {
    mHooks.clear();
}

HookManager::~HookManager() {
    mHooks.clear();
}

void HookManager::init() {
    auto& instance = getInstance();
    assert(instance.mHooks.empty() && "[HookManager::init]: Hook Manager already initialized!");
    {
        instance.mHooks.emplace_back(std::move(std::make_unique<KeyboardHook>()));
        instance.mHooks.emplace_back(std::move(std::make_unique<ClientInstanceHook>()));
        instance.mHooks.emplace_back(std::move(std::make_unique<FreeLibraryAndExitHook>()));
    }

    instance.mInitialized = true;
}

void HookManager::shutdown() {
    auto& instance = getInstance();
    assert(instance.mInitialized && "[HookManager::shutdown]: Hook Manager is not initialized!");
    instance.mHooks.clear();
    instance.mInitialized = false;
}
