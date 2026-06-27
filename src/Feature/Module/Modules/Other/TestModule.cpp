//
// Created by KaeruClient on 2026/06/23.
//

#include "TestModule.hpp"
#include <windows.h>
TestModule::TestModule() : IModule(Category::OTHER, "TestModule", "A test module.") {
    addSetting<BoolSetting>({"TestBool", "A Test bool", mBool});
    addSetting<IntSetting>({"TestInt", "A Test int", mInt, 0, 20});
    addSetting<FloatSetting>({"TestFloat", "A Test float", mFloat, 0.f, 20.f});
}
