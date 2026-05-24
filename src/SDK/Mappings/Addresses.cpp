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


void Addresses::init() {
    Keyboard_hold = FindPattern<"56 48 83 EC 70 48 8B 05 ?? ?? ?? ?? 48 31 E0 48 89 44 24 68 80 79">();
}