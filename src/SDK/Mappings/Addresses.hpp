//
// Created by KaeruClient on 2026/05/24.
//

#pragma once
#include "Utils/Memory/Address.hpp"

namespace Addresses {
    inline Address MinecraftGame_update;

    inline Address Keyboard_hold;
    inline Address Keyboard_release;

    auto initMinimal() -> void;
    auto init() -> void;
}
