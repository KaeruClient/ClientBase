//
// Created by KaeruClient on 2026/05/06.
//

#pragma once

enum struct EventPriorities {
    Immediate,
    Pre,
    High,
    Regular,
    Low,
    Post,
    Last
};

#define NES_PRIORITY_TYPE EventPriorities

// Specialize the traits template
#define NES_PRIORITY_TRAITS template<> struct event_priority_traits<NES_PRIORITY_TYPE> { using priority_type = NES_PRIORITY_TYPE; static constexpr priority_type default_value = priority_type::Regular; };

// Include the library *after* defining both of the macros
#include <nes/event_dispatcher.hpp>
namespace Event {
    typedef nes::event_dispatcher Dispatcher;
    typedef nes::event_priority Priority;
    template <typename T>
    using Holder = nes::event_holder<T>;

    [[nodiscard]] inline auto& getDispatcher() {
        static Dispatcher instance;
        return instance;
    }

    template <typename T, typename... Args>
    [[nodiscard]] __forceinline constexpr auto create(Args&&... args) -> Holder<T> {
        return nes::make_holder<T>(std::forward<Args>(args)...);
    }
    template <typename T>
    __forceinline auto fire(Holder<T>& holder) -> T& {
        getDispatcher().trigger(holder);
        return holder.ref();
    }
    template <typename T, typename... Args>
    __forceinline auto emit(Args &&... args) -> T {
        auto holder = create<T>(std::forward<Args>(args)...);
        return fire(holder);
    }

    template <typename T, auto handler, Priority p = Priority::Regular, typename C>
    __forceinline void subscribe(C* instance) {
        getDispatcher().listen<T, handler, p>(instance);
    }

    template <typename T, auto handler, typename C>
    __forceinline void unsubscribe(C* instance) {
        getDispatcher().deafen<T, handler>(instance);
    }
}