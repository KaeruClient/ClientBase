//
// Created by KaeruClient on 2026/05/26.
//

#include "MinecraftGameHook.hpp"

#include <Engine/GameThread.hpp>
#include <SDK/Mappings/Addresses.hpp>
#include <SDK/Game/Core/MinecraftGame.hpp>
MinecraftGameHook::MinecraftGameHook() {
    createHook<&MinecraftGameHook::update>(
        Addresses::MinecraftGame_update,
        oUpdate);
}

void MinecraftGameHook::update(MinecraftGame* _this) {
    GameThread::tryRegisterThread();
    GameThread::dispatch();
    return oUpdate(_this);
}