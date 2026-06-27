//
// Created by KaeruClient on 2026/05/29.
//

#pragma once
#include <memory>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "Category.hpp"
#include "Setting/ISetting.hpp"

#include "Setting/Settings/BoolSetting.hpp"
#include "Setting/Settings/IntSetting.hpp"
#include "Setting/Settings/FloatSetting.hpp"

class IModule {
    using SettingList = std::vector<std::unique_ptr<ISetting>>;
public:
    IModule(const Category category, std::string name, std::string description, const int32_t keybind = 0x0) : mCategory(category),
        mName(std::move(name)), mDescription(std::move(description)), mKeybind(keybind) {}
    virtual ~IModule() = default;

    [[nodiscard]] __forceinline auto getCategory()    const noexcept -> Category { return mCategory; }
    [[nodiscard]] __forceinline auto getName()        const noexcept -> const std::string & { return mName; }
    [[nodiscard]] __forceinline auto getDescription() const noexcept -> const std::string & { return mDescription; }
    [[nodiscard]] __forceinline auto isEnabled()      const noexcept -> bool { return mEnabled; }
    [[nodiscard]] __forceinline auto isVisible()      const noexcept -> bool { return mVisible; }
    [[nodiscard]] __forceinline auto getKeybind()     const noexcept -> int32_t { return mKeybind; }
    [[nodiscard]] __forceinline auto getSettings()    const noexcept -> const SettingList& { return mSettings; }

    auto setEnabled(const bool enabled) noexcept -> void ;
    auto setVisible(const bool visible) noexcept -> void ;
    auto setKeybind(const int32_t keybind)  noexcept -> void ;
    template <typename T> requires std::derived_from<T, ISetting>
    auto addSetting(T setting) -> void ;

    auto saveConfig(nlohmann::json &json) const -> void;
    auto loadConfig(const nlohmann::json &json) -> void;

    virtual auto onEnable()  -> void {}
    virtual auto onDisable() -> void {}
private:
    bool        mEnabled = false;
    bool        mVisible = true;
    Category    mCategory;
    int32_t     mKeybind = 0x0;
    std::string mName;
    std::string mDescription;
    SettingList mSettings;
};

template<typename T> requires std::derived_from<T, ISetting>
auto IModule::addSetting(T setting) -> void {
    mSettings.push_back(std::make_unique<T>(std::move(setting)));
}

inline auto IModule::setVisible(const bool visible) noexcept -> void {
    mVisible = visible;
}

inline auto IModule::setKeybind(const int32_t keybind) noexcept -> void {
    mKeybind = keybind;
}

inline auto IModule::saveConfig(nlohmann::json &json) const -> void {
    json["Enabled"] = isEnabled();
    json["Visible"] = isVisible();
    json["Keybind"] = getKeybind();

    for (const auto& setting : mSettings) {
        switch (setting->getType()) {
            case SettingType::BOOL:
                json[setting->getName()] = static_cast<BoolSetting*>(setting.get())->getValue(); // NOLINT(*-pro-type-static-cast-downcast)
                break;
            case SettingType::INT:
                json[setting->getName()] = static_cast<IntSetting*>(setting.get())->getValue(); // NOLINT(*-pro-type-static-cast-downcast)
                break;
            case SettingType::FLOAT:
                json[setting->getName()] = static_cast<FloatSetting*>(setting.get())->getValue(); // NOLINT(*-pro-type-static-cast-downcast)
                break;
        }
    }
}

inline auto IModule::loadConfig(const nlohmann::json &json) -> void {
    for (auto& setting : mSettings) {
        if (!json.contains(setting->getName()))
            continue;

        switch (setting->getType()) {
            case SettingType::BOOL: {
                auto* boolSett = static_cast<BoolSetting*>(setting.get()); // NOLINT(*-pro-type-static-cast-downcast)
                boolSett->setValue(json.value(setting->getName(), boolSett->getValue()));
                break;
            }
            case SettingType::INT: {
                auto* intSett = static_cast<IntSetting*>(setting.get()); // NOLINT(*-pro-type-static-cast-downcast)
                intSett->setValue(json.value(setting->getName(), intSett->getValue()));
                break;
            }
            case SettingType::FLOAT: {
                auto* floatSett = static_cast<FloatSetting*>(setting.get()); // NOLINT(*-pro-type-static-cast-downcast)
                floatSett->setValue(json.value(setting->getName(), floatSett->getValue()));
                break;
            }
        }
    }
    setEnabled(json.value("Enabled", isEnabled()));
    mVisible = json.value("Visible", isVisible());
    mKeybind = json.value("Keybind", getKeybind());
}
