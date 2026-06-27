//
// Created by ikasa on 2026/05/25.
//

#include "HookManager.hpp"

#include <cassert>

#include "Hooks/Core/MinecraftGameHook.hpp"
#include "Hooks/Input/KeyboardHook.hpp"
#include "Hooks/Windows/FreeLibraryAndExitHook.hpp"

auto HookManager::initMinimal() -> void {
    mHooks.emplace_back(std::make_unique<MinecraftGameHook>());
}

auto HookManager::initComponents() -> void {
    mHooks.emplace_back(std::make_unique<KeyboardHook>());
    mHooks.emplace_back(std::make_unique<FreeLibraryAndExitHook>());
}
