//
// Created by KaeruClient on 2026/05/24.
//

#include "Addresses.hpp"
#include "libhat/fixed_string.hpp"
#include "libhat/scanner.hpp"
#include "Utils/Memory/Address.hpp"

namespace {
    template <hat::fixed_string Sig>
    Address FindPattern() {
        static constexpr auto parsed_sig = hat::compile_signature<Sig>();
        const hat::scan_result result = hat::find_pattern(parsed_sig, ".text");
        if (!result.has_result())
            throw std::runtime_error("Could not find address.");
        return Address(result.get());
    }
}

//Current Minecraft Version
#define v26_21

void Addresses::init() {
    v26_21 Keyboard_hold = FindPattern<"56 48 83 EC 70 48 8B 05 ?? ?? ?? ?? 48 31 E0 48 89 44 24 68 80 79">();
    v26_21 Keyboard_release = FindPattern<"48 83 EC 68 48 8B 05 ?? ?? ?? ?? 48 31 E0 48 89 44 24 60 80 79 10">();
    v26_21 ClientInstance_update = FindPattern<"55 56 57 53 48 81 EC E8 04 00 00 48 8D AC 24 80 00 00 00 48 C7 85 60 04 00 00 FE FF FF FF 89">();
}
#undef v26_21