//
// Created by KaeruClient on 2026/05/29.
//

#pragma once
#include <memory>
#include <string>
#include <vector>

#include <Feature/Module/Category.hpp>
#include <Feature/Module/Setting/ISetting.hpp>

class IModule {
public:
    IModule(const Category category, std::string name, std::string description, const int keybind = 0x0) : mCategory(category),
        mName(std::move(name)), mDescription(std::move(description)), mKeybind(keybind) {}
    virtual ~IModule() = default;

    [[nodiscard]] __forceinline auto getCategory()    const noexcept -> Category { return mCategory; }
    [[nodiscard]] __forceinline auto getName()        const noexcept -> const std::string & { return mName; }
    [[nodiscard]] __forceinline auto getDescription() const noexcept -> const std::string & { return mDescription; }
    [[nodiscard]] __forceinline auto isEnabled()      const noexcept -> bool { return mEnabled; }
    [[nodiscard]] __forceinline auto isVisible()      const noexcept -> bool { return mVisible; }
    [[nodiscard]] __forceinline auto getKeybind()     const noexcept -> int { return mKeybind; }
    [[nodiscard]] __forceinline auto getSettings()    const noexcept -> const std::vector<std::unique_ptr<ISetting>>& { return mSettings; }
    __forceinline auto setEnabled(const bool enabled) noexcept -> void {
        if (mEnabled == enabled)
            return;
        mEnabled = enabled;
        if (mEnabled)
            onEnable();
        else
            onDisable();
    }
    __forceinline auto setVisible(const bool visible) noexcept -> void { mVisible = visible; }
    __forceinline auto setKeybind(const int keybind)  noexcept -> void { mKeybind = keybind; }
    __forceinline auto addSetting(std::unique_ptr<ISetting> setting) -> void { mSettings.push_back(std::move(setting)); }

    virtual auto onEnable()  -> void {}
    virtual auto onDisable() -> void {}
private:
    Category    mCategory;
    std::string mName;
    std::string mDescription;
    bool mEnabled = false;
    bool mVisible = true;
    int  mKeybind = 0x0;
    std::vector<std::unique_ptr<ISetting>> mSettings;
};
