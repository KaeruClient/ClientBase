//
// Created by KaeruClient on 2026/05/30.
//

#pragma once
#include <Feature/Module/Setting/ISetting.hpp>
#include <algorithm>

class IntSetting final : public ISetting {
    using ValueType = int;
    static constexpr auto TYPE = SettingType::INT;
public:
    IntSetting(std::string name, std::string description, ValueType &value, const ValueType maxValue,
                 const ValueType minValue) : ISetting(TYPE, std::move(name), std::move(description)), mMaxValue(maxValue),
                                             mMinValue(minValue), mValue(value) {}
    ~IntSetting() override = default;
    [[nodiscard]] __forceinline auto getMaxValue() const noexcept -> ValueType { return mMaxValue; }
    [[nodiscard]] __forceinline auto getMinValue() const noexcept -> ValueType { return mMinValue; }
    [[nodiscard]] __forceinline auto getValue()    const noexcept -> ValueType { return mValue; }
    // ReSharper disable once CppMemberFunctionMayBeConst
    __forceinline auto setValue(const ValueType value) noexcept -> void { mValue = std::clamp(value, mMinValue, mMaxValue); }
private:
    const ValueType  mMaxValue;
    const ValueType  mMinValue;
    ValueType& mValue;
};
