//
// Created by KaeruClient on 2026/05/20.
//

#pragma once
#include <cstdint>

class Address final {
public:
    std::uintptr_t mAddress = 0;

    Address() noexcept = default;
    explicit Address(const std::uintptr_t address) noexcept : mAddress(address) {}
    explicit Address(const void* address) noexcept : mAddress(reinterpret_cast<std::uintptr_t>(address)) {}

    template <typename T>
    [[nodiscard]] __forceinline auto as() const noexcept -> T {
        return reinterpret_cast<T>(mAddress);
    }

    [[nodiscard]] auto resolveRip(const std::int32_t offset, const std::int32_t instructionLength) const noexcept -> Address {
        if (!mAddress) return Address(nullptr);
        const auto relativeOffset = *reinterpret_cast<const std::int32_t*>(mAddress + offset);
        return Address(mAddress + relativeOffset + instructionLength);
    }

    [[nodiscard]] constexpr explicit operator bool() const noexcept { return mAddress != 0; }
};