//
// Created by KaeruClient on 2026/06/25.
//
#include "IModule.hpp"

#include "Engine/GameThread.hpp"
auto IModule::setEnabled(const bool enabled) noexcept -> void {
    if (!GameThread::isMe()) {
        GameThread::enqueue([this, enabled] {
            this->setEnabled(enabled);
        });
        return;
    }

    if (mEnabled == enabled)
        return;

    mEnabled = enabled;
    if (mEnabled)
        onEnable();
    else
        onDisable();
}
