//
// Created by KaeruClient on 2026/06/23.
//

#pragma once
#include <Feature/Module/IModule.hpp>

class TestModule final : public IModule {
public:
    TestModule();
    ~TestModule() override = default;
private:
    bool mBool   = false;
    int mInt     = 5.f;
    float mFloat = 8.9f;
};
