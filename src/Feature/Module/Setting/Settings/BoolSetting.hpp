//
// Created by KaeruClient on 2026/05/30.
//

#pragma once
#include <Feature/Module/Setting/ISetting.hpp>
class BoolSetting final : public ISetting {
    using ValueType = bool;
    static constexpr auto TYPE = SettingType::BOOL;
public:
    BoolSetting(std::string name, std::string description, ValueType &value) : ISetting(TYPE, std::move(name),
                                                                               std::move(description)), mValue(value) {}
    ~BoolSetting() override = default;
    [[nodiscard]] __forceinline auto getValue()        const noexcept -> ValueType { return mValue; }
    // ReSharper disable once CppMemberFunctionMayBeConst
    __forceinline auto setValue(const ValueType value) noexcept -> void { mValue = value; }
private:
    ValueType& mValue;
};
