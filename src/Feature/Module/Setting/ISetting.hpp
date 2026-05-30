//
// Created by KaeruClient on 2026/05/30.
//

#pragma once
#include <string>
#include "SettingType.hpp"
class ISetting {
public:
    ISetting(const SettingType type, std::string name, std::string description) : mName(std::move(name)),
                                                                                  mDescription(std::move(description)),
                                                                                  mType(type) {}
    virtual ~ISetting() = default;
    [[nodiscard]] __forceinline auto getName()        const noexcept -> const std::string & { return mName; }
    [[nodiscard]] __forceinline auto getDescription() const noexcept -> const std::string & { return mDescription; }
private:
    std::string mName;
    std::string mDescription;
    SettingType mType;
};
