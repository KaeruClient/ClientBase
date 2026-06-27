//
// Created by KaeruClient on 2026/05/24.
//

#include "KeyboardHook.hpp"

#include "SDK/Mappings/Addresses.hpp"
#include "Zycore/Internal/AtomicMSVC.h"

KeyboardHook::KeyboardHook() {
    createHook<&KeyboardHook::hold>(
        Addresses::Keyboard_hold,
        oHold);
    createHook<&KeyboardHook::release>(
        Addresses::Keyboard_release,
        oRelease);
}

void KeyboardHook::hold(std::int64_t _this, unsigned int key) {
    return oHold(_this, key);
}

void KeyboardHook::release(std::int64_t _this, unsigned int key) {
    return oRelease(_this, key);
}
