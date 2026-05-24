//
// Created by KaeruClient on 2026/05/24.
//

#include "KeyboardHook.hpp"

#include "SDK/Mappings/Addresses.hpp"
#include "Zycore/Internal/AtomicMSVC.h"

KeyboardHook::KeyboardHook() {
    const auto targetAddress = Address(Addresses::Keyboard_hold);
    createHook<&KeyboardHook::hold>(targetAddress, oHold);
}

void KeyboardHook::hold(std::int64_t _this, unsigned int key) {
    return oHold(_this, key);
}
