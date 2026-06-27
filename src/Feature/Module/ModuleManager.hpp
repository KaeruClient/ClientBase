//
// Created by KaeruClient on 2026/05/30.
//

#pragma once
#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>
#include <typeindex>
#include <unordered_map>
#include <Feature/Module/IModule.hpp>
struct ModuleEntry {
    std::type_index mType;
    std::unique_ptr<IModule> mModule;
    template <typename T, typename... Args> requires std::derived_from<T, IModule>
    static auto build(Args &&... args) -> ModuleEntry {
        return {
            .mType = std::type_index(typeid(T)),
            .mModule = std::make_unique<T>(std::forward<Args>(args)...)
        };
    }
    [[nodiscard]] __forceinline auto operator->() const noexcept -> IModule* {
        return mModule.get();
    }

    [[nodiscard]] __forceinline auto operator*() const noexcept -> IModule& {
        return *mModule;
    }

    [[nodiscard]] explicit __forceinline operator IModule*() const noexcept {
        return mModule.get();
    }
};
using ModuleList = std::vector<ModuleEntry>;
class ModuleManager final {
    auto init() -> void;
    template <typename T, typename... Args> requires std::derived_from<T, IModule>
    auto registerModule(Args &&... args) -> void {
        mModuleList.emplace_back(ModuleEntry::build<T>(std::forward<Args>(args)...));
    }

    __forceinline auto sortModuleList() -> void {
        // Sort alphabetically
        std::ranges::sort(mModuleList,
                          [](const ModuleEntry& a, const ModuleEntry& b) {
                              return a->getName() < b->getName();
                          }
        );
    }
    auto buildCacheMap() -> void {
        mCacheMap.reserve(mModuleList.size());
        for (const auto& module : mModuleList)
            mCacheMap[module.mType] = static_cast<IModule*>(module);
    }
public:
    ModuleManager() {
        init();
        sortModuleList();
        buildCacheMap();
    }
    ~ModuleManager() = default;

    auto saveConfig(nlohmann::json& modulesJson) const -> void;
    auto loadConfig(const nlohmann::json& modulesJson) const -> void;

    template <typename T> requires std::derived_from<T, IModule>
    [[nodiscard]] auto getModule() const -> T& {
        const auto it = mCacheMap.find(std::type_index(typeid(T)));
#ifdef _DEBUG
        assert(it != mCacheMap.end() && "We tried to retrieve a module that does not exist. Have you added the module to init()?");
#endif
        return static_cast<T&>(*it->second);
    }
    [[nodiscard]] __forceinline auto getModuleList() const noexcept -> const ModuleList & { return mModuleList; }
private:
    using ModuleCacheMap = std::unordered_map<std::type_index, IModule*>;
    ModuleList mModuleList;
    ModuleCacheMap mCacheMap;
};
