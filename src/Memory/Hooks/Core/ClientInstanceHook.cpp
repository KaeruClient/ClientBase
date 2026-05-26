//
// Created by ikasa on 2026/05/25.
//

#include "ClientInstanceHook.hpp"
#include "Engine/GameThread.hpp"

#include "SDK/Mappings/Addresses.hpp"

ClientInstanceHook::ClientInstanceHook() {
    const auto targetAddress = Address(Addresses::ClientInstance_update);
    createHook<&ClientInstanceHook::update>(targetAddress, oUpdate);
}

__int64 ClientInstanceHook::update(ClientInstance* _this, bool a2) {
    return oUpdate(_this, a2);
}