//
// Created by KaeruClient on 2026/05/26.
//

#include "MinecraftGameHook.hpp"

#include "Engine/GameThread.hpp"
#include "SDK/Mappings/Addresses.hpp"

MinecraftGameHook::MinecraftGameHook() {
    const auto targetAddress = Address(Addresses::MinecraftGame_update);
    createHook<&MinecraftGameHook::update>(targetAddress, oUpdate);
}

void MinecraftGameHook::update(MinecraftGame* _this) {
    GameThread::tryRegisterThread();
    GameThread::dispatch();
    return oUpdate(_this);
}