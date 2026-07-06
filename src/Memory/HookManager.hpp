//
// Created by ikasa on 2026/05/25.
//

#pragma once
#include <memory>
#include <vector>
#include <Memory/IHook.hpp>

class HookManager final {
public:
    HookManager() = default;
    ~HookManager();
    auto initMinimal() -> void;
    auto initComponents() -> void;
private:
    std::vector<std::unique_ptr<IHook>> mHooks{};
};