#pragma once
// Copyright (c) 2024 Mike
#include <functional>
#include <vector>
#include <memory>
#include <atomic>
#include <mutex>
#include <algorithm>
#include <iostream>
#include <string_view>

#if defined(__clang__) or defined(__GNUC__)
#define NES_FUNCSIG __PRETTY_FUNCTION__
#elif _MSC_VER
#define NES_FUNCSIG __FUNCSIG__
#endif

// Optimized by KaeruClient 2026
// While listen and deafen are significantly heavier than in the original version.
// They now run without any lock-related delays.

/*
* nes - Nuvola Event System
 */

namespace nes {
    namespace detail {
        constexpr std::uint32_t fnv1a_hash(std::string_view str) {
            std::uint32_t hash = 2166136261u;
            for (std::size_t i = 0; i < str.length(); ++i) {
                hash ^= static_cast<std::uint32_t>(str[i]);
                hash *= 16777619u;
            }
            return hash;
        }

        template<typename type_t>
        struct type_info {
            static constexpr std::string_view name() {
                constexpr std::string_view sig = NES_FUNCSIG;
                constexpr auto cut = sig.substr(sig.find("[type_t = ") + 10);
                return cut.substr(0, cut.find(']'));
            }
        };

        template<typename type_t>
        struct type_hash {
            [[nodiscard]] static constexpr std::size_t value() {
                constexpr auto name = type_info<type_t>::name();
                return fnv1a_hash(name);
            }
        };

        //Utils to extract type information
        template<typename class_t = std::false_type>
        struct extract_type {
            typedef class_t type;
        };

        template<typename return_t, typename class_t, typename... args_t>
        struct extract_type<return_t (class_t::*)(args_t...)> {
            typedef class_t type;
            typedef return_t ret;
        };

        enum class standard_event_priority {
            FIRST,
            NORMAL,
            LAST
        };
    }// namespace detail

    template<typename priority_t> struct event_priority_traits { using priority_type = priority_t; };

#ifndef NES_PRIORITY_TYPE
    using event_priority = detail::standard_event_priority;
    template<> struct event_priority_traits<event_priority> { using priority_type = event_priority; static constexpr priority_type default_value = priority_type::NORMAL; };
#else
    using event_priority = NES_PRIORITY_TYPE;
#ifndef NES_PRIORITY_TRAITS
    template<> struct event_priority_traits<event_priority> { using priority_type = event_priority; static constexpr priority_type default_value = priority_type::NORMAL; };
#else
    // Define your own priority traits
    NES_PRIORITY_TRAITS;
#endif
#endif

    //Owns an event pointer, used to pass around an event
    template<typename event_t>
    struct event_holder {
        template<typename... args_t>
        explicit event_holder(args_t&&... args) : mEvent(std::forward<args_t>(args)...){};

        event_t* get() { return &mEvent; }
        event_t& ref() { return mEvent; }
        event_t* operator->() { return get(); }

    private:
        event_t mEvent;
    };

    //Creates an event holder instance
    template<typename event_t, typename... args_t>
    event_holder<event_t> make_holder(args_t&&... args) {
        return event_holder<event_t>(std::forward<args_t>(args)...);
    }

    //A type for holding the wrapper function that invokes the callback
    template<typename event_t>
    using event_wrapper = std::function<void(event_t&)>;

    template<typename event_t, typename wrapper_t = event_wrapper<event_t>>
    struct event_listener {
        using holder_t = event_holder<event_t>;

        event_listener() = delete;
        event_listener(void* instance, wrapper_t&& wrapper, const std::size_t methodHash, event_priority priority)
            : mInstance(instance), mMethod(std::move(wrapper)), mMethodHash(methodHash), mPriority(priority){};

        void invoke(holder_t& holder) const {
            mMethod(holder.ref());
        }

        void* mInstance = nullptr;
        wrapper_t mMethod{};
        std::size_t mMethodHash = 0;
        event_priority mPriority;
    };

    template<typename event_t>
    struct dispatcher {
        using type_t = event_t;
        using id_t = detail::type_hash<event_t>;
        using holder_t = event_holder<event_t>;
        using listener_t = event_listener<event_t>;
        using container_t = std::vector<listener_t>;

        std::shared_ptr<const container_t> getListeners() const {
            return mListeners.load(std::memory_order_acquire);
        }

        template<auto handler, auto priority = event_priority_traits<event_priority>::default_value, typename class_t = typename detail::extract_type<decltype(handler)>::class_t, typename wrapper_t = event_wrapper<event_t>>
        void listen(class_t* instance) {
            wrapper_t wrapper = [instance](event_t& e) {
                (instance->*handler)(e);
            };
            add_listener(instance, std::move(wrapper), detail::type_hash<decltype(handler)>::value(), priority);
        }

        template<auto priority = event_priority_traits<event_priority>::default_value, typename wrapper_t = event_wrapper<event_t>>
        void listen(auto handler) {
            wrapper_t wrapper = [handler](event_t& e) {
                handler(e);
            };
            add_listener(nullptr, std::move(wrapper), detail::type_hash<decltype(handler)>::value(), priority);
        }

        template<typename handler_t, typename class_t = typename detail::extract_type<handler_t>::class_t>
        void deafen(class_t* instance, handler_t&& handler) {
            remove_listener([&](const auto& listener) -> bool {
                return listener.mInstance == instance && listener.mMethodHash == detail::type_hash<handler_t>::value();
            });
        }

        template<typename handler_t>
        void deafen(handler_t handler) {
            remove_listener([&](const auto& listener) -> bool {
                return listener.mMethodHash == detail::type_hash<handler_t>::value();
            });
        }

        void trigger(holder_t& holder) {
            auto snapshot = mListeners.load(std::memory_order_acquire);

            for (const auto& listener : *snapshot) {
                listener.invoke(holder);
            }
        }

    private:
        std::atomic<std::shared_ptr<const container_t>> mListeners{std::make_shared<const container_t>()};
        std::mutex mWriteLock;
        void add_listener(void* instance, event_wrapper<event_t>&& wrapper, std::size_t hash, event_priority priority) {
            std::lock_guard lock(mWriteLock);
            auto current = mListeners.load(std::memory_order_relaxed);
            auto next = std::make_shared<container_t>(*current);

            next->emplace_back(instance, std::move(wrapper), hash, priority);

            std::stable_sort(next->begin(), next->end(), [](const listener_t& a, const listener_t& b) {
                return a.mPriority < b.mPriority;
            });

            mListeners.store(next, std::memory_order_release);
        }

        template<typename Predicate>
        void remove_listener(Predicate pred) {
            std::lock_guard lock(mWriteLock);
            auto current = mListeners.load(std::memory_order_relaxed);
            auto next = std::make_shared<container_t>(*current);

            if (std::erase_if(*next, pred) > 0) {
                mListeners.store(next, std::memory_order_release);
            }
        }
    };

    //The main event dispatcher, use this to listen for and dispatch events
    struct event_dispatcher {
        template<typename event_t>
        [[nodiscard]] auto& get() const {
            static dispatcher<event_t> instance;
            return instance;
        }

        template<typename event_t>
        void trigger(event_holder<event_t>& e) const {
            auto& disp = get<event_t>();
            disp.trigger(e);
        }

        template<typename event_t, auto handler, auto priority = event_priority_traits<event_priority>::default_value, typename class_t = typename detail::extract_type<decltype(handler)>::class_t>
        void listen(class_t* instance) const {
            auto& disp = get<event_t>();
            disp.template listen<handler, priority>(instance);
        }

        template<typename event_t, auto priority = event_priority_traits<event_priority>::default_value>
        void listen(auto handler) const {
            auto& disp = get<event_t>();
            disp.template listen<priority>(handler);
        }

        template<typename event_t, auto handler, typename class_t = typename detail::extract_type<decltype(handler)>::class_t>
        void deafen(class_t* instance) const {
            auto& disp = get<event_t>();
            disp.deafen(instance, static_cast<decltype(handler)>(handler));
        }

        template<typename event_t>
        void deafen(auto handler) const {
            auto& disp = get<event_t>();
            disp.deafen(handler);
        }
    };

    template<typename event_t, auto handler, auto priority = event_priority_traits<event_priority>::default_value, typename dispatcher_t = event_dispatcher>
    struct scoped_listener {
        constexpr explicit scoped_listener(dispatcher_t& disp) : mDispatcher{ disp } {
            mDispatcher.template listen<event_t, &scoped_listener::listener, priority>(this);
        }
        constexpr ~scoped_listener() {
            mDispatcher.template deafen<event_t, &scoped_listener::listener>(this);
        }
        constexpr void listener(event_t& e) const {
            handler(e);
        }
    private:
        dispatcher_t& mDispatcher;
    };
}// namespace nes