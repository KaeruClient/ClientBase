//
// Created by KaeruClient on 2026/05/23.
//

#pragma once
#include <Windows.h>
#include <queue>
#include <mutex>
#include <functional>
#include <atomic>
#include <cstdint>
#include <cassert>
class GameThread {
private:
    using Task = std::move_only_function<void()>;
    GameThread() = default;

    static auto getInstance() -> GameThread& {
        static GameThread instance;
        return instance;
    }

public:
    static auto tryRegisterThread() noexcept -> void {
        auto& instance = getInstance();
        uint32_t expected = 0;
        const uint32_t current_id = ::GetCurrentThreadId();
        if (instance.mGameThreadId.compare_exchange_strong(expected, current_id, std::memory_order_release)) {
            instance.mGameThreadId.notify_all();
        }
    }
    template <typename F>
    static auto enqueue(F&& task) -> void {
        auto& instance = getInstance();
        std::lock_guard<std::mutex> lock(instance.mMutex);
        instance.mTasks.push(std::forward<F>(task));
    }

    static auto dispatch() -> void {
        auto& instance = getInstance();
        {
            assert(GameThread::isMe() &&
                "[GameThread] GameThread::dispatch() was called from a WRONG thread! Only the authorized game thread can dispatch tasks.");
        }

        std::queue<Task> localTasks;
        {
            std::lock_guard<std::mutex> lock(instance.mMutex);
            // if there are no tasks return to improve performance.
            if (instance.mTasks.empty()) return;

            // By swapping and using it, We can minimize the time lock waiting.
            localTasks.swap(instance.mTasks);
        }

        while (!localTasks.empty()) {
            auto task = std::move(localTasks.front());
            localTasks.pop();
            if (task)
                task();
        }
    }
    [[ nodiscard ]] static auto isMe() -> bool {
        const auto& instance = getInstance();
        const uint32_t cached_id = instance.mGameThreadId.load(std::memory_order_acquire);

        assert(isInitialized() &&
               "[GameThread] GameThread::isMe() was called before initialized!");

        return ::GetCurrentThreadId() == cached_id;
    }
    [[nodiscard]] static auto isInitialized() -> bool {
        const auto& instance = getInstance();
        const uint32_t cached_id = instance.mGameThreadId.load(std::memory_order_acquire);
        return cached_id != 0;
    }
    static auto waitUntilInit() -> void {
        const auto& instance = getInstance();
        uint32_t cached_id = instance.mGameThreadId.load(std::memory_order_acquire);
        while (cached_id == 0) {
            instance.mGameThreadId.wait(0, std::memory_order_acquire);
            cached_id = instance.mGameThreadId.load(std::memory_order_acquire);
        }
    }
private:
    std::queue<Task> mTasks;
    std::mutex mMutex;
    std::atomic<uint32_t> mGameThreadId{ 0 };
};