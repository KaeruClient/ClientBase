#pragma once
#include <vector>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <safetyhook.hpp>
#include "fmt/format.h"
#include <Utils/Memory/Address.hpp>
#include "HookGuard.hpp"

class IHook {
public:
    IHook() = default;
    virtual ~IHook() = default;
    void reset() { mHooks.clear(); }
protected:
    template <auto detour, typename TReturn, typename... TArgs>
    void createHook(const Address address, TReturn(*&oFunc)(TArgs...)) {
        // We're developing on x64, so we don't have to worry about calling conventions. Bro!
        auto managedDetour = +[](TArgs... args) -> TReturn {
            HookGuard guard;
            return (*detour)(std::forward<TArgs>(args)...);
        };
        auto createResult = safetyhook::InlineHook::create(
            reinterpret_cast<void*>(address.mAddress),
            reinterpret_cast<void*>(managedDetour),
            safetyhook::InlineHook::Flags::StartDisabled
        );

        if (!createResult) {
            throw std::runtime_error(
                "Failed to create hook. Error Code: " + fmt::format("0x{:X}", static_cast<int>(createResult.error().type))
            );
        }

        auto hookPtr = std::make_unique<safetyhook::InlineHook>(std::move(createResult).value());

        oFunc = reinterpret_cast<TReturn(*)(TArgs...)>(hookPtr->trampoline().address());

        const auto enableResult = hookPtr->enable();
        if (!enableResult) {
            throw std::runtime_error(
                "Failed to enable hook. Error Code: " + fmt::format("0x{:X}", static_cast<int>(enableResult.error().type))
            );
        }

        mHooks.push_back(std::move(hookPtr));
    }
private:
    std::vector<std::unique_ptr<safetyhook::InlineHook>> mHooks;
};