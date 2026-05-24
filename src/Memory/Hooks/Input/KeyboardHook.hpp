//
// Created by KaeruClient on 2026/05/24.
//

#pragma once
#include <Memory/IHook.hpp>

class KeyboardHook : public IHook {
public:
    KeyboardHook();
    ~KeyboardHook() override = default;
private:
    static void hold(std::int64_t _this, unsigned int key);
    static inline decltype(&hold) oHold = nullptr;
};
