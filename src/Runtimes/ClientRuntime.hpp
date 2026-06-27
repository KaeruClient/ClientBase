//
// Created by KaeruClient on 2026/05/23.
//

#pragma once
#include <atomic>
#include <cassert>
#include <memory>

#include "Utils/Memory/Address.hpp"

class ConfigManager;
class HookManager;
class ModuleManager;

class ClientRuntime final {
public:
    ClientRuntime(const ClientRuntime&) = delete;
    ClientRuntime& operator=(const ClientRuntime&) = delete;
    ClientRuntime(ClientRuntime&&) = delete;
    ClientRuntime& operator=(ClientRuntime&&) = delete;

    static __forceinline auto setBaseAddress(const Address address) noexcept -> void {
        auto& instance = getInstance();
        assert(instance.mBaseAddress.mAddress == 0x0 && "[ClientRuntime] Already initialized!");
        instance.mBaseAddress = address;
    }
    /**
     * @note This function is thread-safe ONLY AFTER setBaseAddress() has been called.
     *       Ensure setBaseAddress() completes on the main/boot thread before accessing this.
     */
    [[ nodiscard ]] static __forceinline auto getBaseAddress() noexcept -> Address {
        const auto& instance = getInstance();
        assert(instance.mBaseAddress.mAddress != 0x0 && "[ClientRuntime] getBaseAddress() was called before init()!");
        return instance.mBaseAddress;
    }

    [[ nodiscard ]] static __forceinline auto isRunning() noexcept -> bool {
        return getInstance().mIsRunning.load(std::memory_order_acquire);
    }
    static __forceinline auto requestShutdown() noexcept -> void {
        auto& instance = getInstance();
        instance.mIsRunning.store(false, std::memory_order_release);
        instance.mIsRunning.notify_all();
    }
    static __forceinline auto waitUntilExit() noexcept -> void {
        const auto& instance = getInstance();
        while (instance.mIsRunning.load(std::memory_order_acquire)) {
            instance.mIsRunning.wait(true, std::memory_order_relaxed);
        }
    }
    static auto initComponents() -> void;
    [[ nodiscard ]] static __forceinline auto getModuleManager() noexcept -> ModuleManager & {
        const auto& instance = getInstance();
        assert(instance.mModuleManager.get() != nullptr && "[ClientRuntime] getModuleManager() was called before initComponents()!");
        return *instance.mModuleManager;
    }

    [[ nodiscard ]] static __forceinline auto getConfigManager() noexcept -> ConfigManager & {
        const auto& instance = getInstance();
        assert(instance.mConfigManager.get() != nullptr && "[ClientRuntime] getConfigManager() was called before initComponents()!");
        return *instance.mConfigManager;
    }

    [[ nodiscard ]] static __forceinline auto getHookManager() noexcept -> HookManager & {
        const auto& instance = getInstance();
        assert(instance.mHookManager.get() != nullptr && "[ClientRuntime] getHookManager() was called before initComponents()!");
        return *instance.mHookManager;
    }


private:
    ClientRuntime();
    ~ClientRuntime();

    [[ nodiscard ]] static auto getInstance() noexcept -> ClientRuntime & {
        static ClientRuntime instance;
        return instance;
    }

    Address           mBaseAddress = Address(nullptr);
    std::atomic<bool> mIsRunning   = {true};

    std::unique_ptr<ModuleManager> mModuleManager;
    std::unique_ptr<ConfigManager> mConfigManager;
    std::unique_ptr<HookManager>   mHookManager;
};
