//
// Created by KaeruClient on 2026/05/24.
//

#pragma once
#include <atomic>
#include <thread>

class HookGuard {
public:
    static inline std::atomic<int> activeCount{ 0 };
    HookGuard() noexcept { activeCount.fetch_add(1, std::memory_order::relaxed); }
    ~HookGuard() noexcept { activeCount.fetch_sub(1, std::memory_order::acq_rel); }
    static __forceinline auto wait() noexcept -> void {
        while (activeCount.load(std::memory_order::acquire) > 0)
            std::this_thread::yield();
    }
};