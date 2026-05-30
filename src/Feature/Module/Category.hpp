//
// Created by KaeruClient on 2026/05/29.
//

#pragma once
#include <cstdint>
#include <string_view>

enum class Category : uint8_t {
    COMBAT,
    MOTION,
    PLAYER,
    VISUAL,
    WORLD,
    OTHER,
    CLIENT
};

inline std::string_view getCategoryName(const Category category) {
    switch (category) {
        case Category::COMBAT:   return "Combat";
        case Category::MOTION:   return "Motion";
        case Category::PLAYER:   return "Player";
        case Category::VISUAL:   return "Visual";
        case Category::WORLD:    return "World";
        case Category::OTHER:    return "Other";
        case Category::CLIENT:   return "Client";
    }
    return "Unknown";
}