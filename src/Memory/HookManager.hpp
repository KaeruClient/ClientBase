//
// Created by ikasa on 2026/05/25.
//

#pragma once
#include <memory>
#include <vector>
#include <Memory/IHook.hpp>

class HookManager {
public:
    HookManager(const HookManager&) = delete;
    HookManager& operator=(const HookManager&) = delete;
    HookManager(HookManager&&) = delete;
    HookManager& operator=(HookManager&&) = delete;
    static auto init() -> void;
    static auto shutdown() -> void;
private:
    HookManager() = default;
    ~HookManager() = default;
    [[ nodiscard ]] static auto getInstance() noexcept -> HookManager& {
        static HookManager instance;
        return instance;
    }
public:
    [[ nodiscard ]] static __forceinline auto isInitialized() noexcept -> bool {
        return getInstance().mInitialized;
    }
private:
    std::vector<std::unique_ptr<IHook>> mHooks{};
    bool mInitialized = false;
};