//
// Created by KaeruClient on 2026/05/23.
//

#pragma once
#include <atomic>
#include <cassert>

#include "Utils/Memory/Address.hpp"

class GameRuntime final {
public:
    GameRuntime(const GameRuntime&) = delete;
    GameRuntime& operator=(const GameRuntime&) = delete;
    GameRuntime(GameRuntime&&) = delete;
    GameRuntime& operator=(GameRuntime&&) = delete;

    static __forceinline auto setBaseAddress(const Address address) noexcept -> void {
        auto& instance = getInstance();
        assert(instance.mBaseAddress.mAddress == 0x0 && "[GameRuntime] Already initialized!");
        instance.mBaseAddress = address;
    }
    /**
     * @note This function is thread-safe ONLY AFTER setBaseAddress() has been called.
     *       Ensure setBaseAddress() completes on the main/boot thread before accessing this.
     */
    static __forceinline auto getBaseAddress() noexcept -> Address {
        const auto& instance = getInstance();
        assert(instance.mBaseAddress.mAddress != 0x0 && "[GameRuntime] getBaseAddress() was called before init()!");
        return instance.mBaseAddress;
    }

private:
    GameRuntime() = default;
    ~GameRuntime() = default;

    static GameRuntime& getInstance() noexcept
    {
        static GameRuntime instance;
        return instance;
    }

    Address mBaseAddress = Address(nullptr);
};
