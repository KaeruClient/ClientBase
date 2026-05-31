//
// Created by KaeruClient on 2026/05/31.
//

#pragma once
#include <filesystem>
class ClientStorage final {
    using FilePath = std::filesystem::path;
public:
    ClientStorage(const ClientStorage&) = delete;
    ClientStorage& operator=(const ClientStorage&) = delete;
    ClientStorage(ClientStorage&&) = delete;
    ClientStorage& operator=(ClientStorage&&) = delete;
    [[nodiscard]] static auto getPath(const std::string& subPath) -> FilePath;
private:
    ClientStorage();
    ~ClientStorage() = default;

    [[nodiscard]] static auto getRoamingFolder() -> FilePath;
    [[nodiscard]] static auto getClientPath()    -> FilePath;
    [[nodiscard]] static auto getInstance()      -> ClientStorage & {
        static ClientStorage instance;
        return instance;
    }
    FilePath mRootPath;
};