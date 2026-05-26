//
// Created by KaeruClient on 2026/05/26.
//

#pragma once
#include <Memory/IHook.hpp>
class MinecraftGame;
class MinecraftGameHook : public IHook {
public:
    MinecraftGameHook();
    ~MinecraftGameHook() override = default;
private:
    static void update(MinecraftGame* _this);
    static inline decltype(&update) oUpdate = nullptr;
};