//
// Created by ikasa on 2026/05/25.
//

#pragma once
#include <Memories/IHook.hpp>
#include <SDK/Game/Core/ClientInstance.hpp>

class ClientInstanceHook : public IHook {
    public:
    ClientInstanceHook();
    ~ClientInstanceHook() override = default;
private:
    static __int64 update(ClientInstance* _this, bool a2);
    static inline decltype(&update) oUpdate = nullptr;
};