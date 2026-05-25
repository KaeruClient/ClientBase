//
// Created by KaeruClient on 2026/05/23.
//

#pragma once
#include <atomic>
#include <cassert>

#include "Utils/Memory/Address.hpp"

class ClientRuntime final {
public:
    ClientRuntime(const ClientRuntime&) = delete;
    ClientRuntime& operator=(const ClientRuntime&) = delete;
    ClientRuntime(ClientRuntime&&) = delete;
    ClientRuntime& operator=(ClientRuntime&&) = delete;

    static __forceinline auto init(const Address address) noexcept -> void {
        auto& instance = getInstance();
        assert(instance.mBaseAddress.mAddress == 0x0 && "[ClientRuntime] Already initialized!");
        instance.mBaseAddress = address;
    }
    /**
     * @note This function is thread-safe ONLY AFTER init() has been called.
     *       Ensure init() completes on the main/boot thread before accessing this.
     */
    static __forceinline auto getBaseAddress() noexcept -> Address {
        const auto& instance = getInstance();
        assert(instance.mBaseAddress.mAddress != 0x0 && "[ClientRuntime] getBaseAddress() was called before init()!");
        return instance.mBaseAddress;
    }

    static __forceinline auto isRunning() noexcept -> bool{
        return getInstance().mIsRunning.load(std::memory_order_acquire);
    }
    static __forceinline auto requestShutdown() noexcept -> void {
        auto& instance = getInstance();
        instance.mIsRunning.store(false, std::memory_order_release);
        instance.mIsRunning.notify_all();
    }
    static __forceinline auto waitUntilExit(const Address& baseAddress) noexcept -> void {
        const auto& instance = getInstance();
        while (instance.mIsRunning.load(std::memory_order_acquire)) {
            instance.mIsRunning.wait(true, std::memory_order_relaxed);
        }
    }
private:
    ClientRuntime() = default;
    ~ClientRuntime() = default;

    static ClientRuntime& getInstance() noexcept
    {
        static ClientRuntime instance;
        return instance;
    }

    Address           mBaseAddress = Address(nullptr);
    std::atomic<bool> mIsRunning   = {true};
};
