//
// Created by KaeruClient on 2026/05/31.
//

#pragma once
#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>

class ClientStorage final {
    using FilePath = std::filesystem::path;
    using Json = nlohmann::json;
public:
    ClientStorage() = default;
    ~ClientStorage() = default;

    auto init() -> void;
    [[nodiscard]] auto getPath(const std::string& subPath) const -> FilePath;
private:
    [[nodiscard]] static auto getRoamingFolder() -> FilePath;
    [[nodiscard]] static auto getClientPath()    -> FilePath;
    FilePath mRootPath;
};