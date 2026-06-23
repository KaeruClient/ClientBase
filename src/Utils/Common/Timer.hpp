//
// Created by KaeruClient on 2026/05/22.
//

#pragma once
#include <chrono>


template <typename T>
concept is_duration = requires
{
    typename T::rep;
    typename T::period;
};


template <is_duration Unit>
class Timer {
public:
    using Clock = std::chrono::steady_clock;
    using ValueDuration = std::chrono::duration<double, typename Unit::period>;
    using TimePoint = std::chrono::time_point<Clock>;

    [[nodiscard]] Timer() noexcept { reset(); }

    void reset() noexcept { mStartTime = Clock::now(); }

    [[nodiscard]] double elapsed() const noexcept {
        return std::chrono::duration_cast<ValueDuration>(Clock::now() - mStartTime).count();
    }

    [[nodiscard]] bool has_passed(const double duration) const noexcept {
        return elapsed() >= duration;
    }

private:
    TimePoint mStartTime;
};

using TimerNanoseconds  = Timer<std::chrono::nanoseconds>;
using TimerMicroseconds = Timer<std::chrono::microseconds>;
using TimerMilliseconds = Timer<std::chrono::milliseconds>;
using TimerSeconds      = Timer<std::chrono::seconds>;

using TimerNs = TimerNanoseconds;
using TimerUs = TimerMicroseconds;
using TimerMs = TimerMilliseconds;
using TimerSec = TimerSeconds;