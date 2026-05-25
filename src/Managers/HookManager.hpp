//
// Created by ikasa on 2026/05/25.
//

#pragma once
#include <memory>
#include <vector>
#include <Memories/IHook.hpp>

class HookManager {
private:
    std::vector<std::unique_ptr<IHook>> mHooks;
    bool mInitialized = false;
private:
    static auto& getInstance() {
        static HookManager instance;
        return instance;
    }
public:
    HookManager();
    ~HookManager();

    static void init();
    static void shutdown();
    static bool isInitialized() {
        return getInstance().mInitialized;
    };
};