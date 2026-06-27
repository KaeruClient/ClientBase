//
// Created by KaeruClient on 2026/05/24.
//

#include "Addresses.hpp"
#include "libhat/fixed_string.hpp"
#include "libhat/scanner.hpp"
#include "Utils/Memory/Address.hpp"

namespace {
    template <hat::fixed_string Sig>
    auto FindPattern() -> Address {
        static constexpr auto parsed_sig = hat::compile_signature<Sig>();
        const hat::scan_result result = hat::find_pattern(parsed_sig, ".text");
        if (!result.has_result())
            throw std::runtime_error("Could not find address.");
        return Address(result.get());
    }
}

// Current Minecraft Version
#define v26_30

auto Addresses::initMinimal() -> void {
    v26_30 MinecraftGame_update = FindPattern<"55 41 57 41 56 41 55 41 54 56 57 53 48 81 EC 18 04 00 00 48 8D AC 24 80 00 00 00 0F 29 BD 80 03 00 00 66">();
}

auto Addresses::init() -> void {
    v26_30 Keyboard_hold        = FindPattern<"56 48 83 EC 70 48 8B 05 ?? ?? ?? ?? 48 31 E0 48 89 44 24 68 80 79">();
    v26_30 Keyboard_release     = FindPattern<"48 83 EC 68 48 8B 05 ?? ?? ?? ?? 48 31 E0 48 89 44 24 60 80 79 10">();
}
#undef v26_30