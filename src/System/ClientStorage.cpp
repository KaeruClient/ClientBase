//
// Created by KaeruClient on 2026/05/31.
//

#include "ClientStorage.hpp"

#include <cassert>
#include <windows.h>
#include <shlobj.h>
#pragma comment(lib, "Shell32")

namespace fs = std::filesystem;
auto ClientStorage::getPath(const FilePath& subPath) -> FilePath {
    return getInstance().mRootPath / subPath;
}

ClientStorage::ClientStorage() {
    mRootPath = getClientPath();
    if (!fs::exists(mRootPath))
        fs::create_directories(mRootPath);
}
auto ClientStorage::getRoamingFolder() -> FilePath {
    static const FilePath path = []() {
        PWSTR pszPath = nullptr;
        const HRESULT hr = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &pszPath);

        if (FAILED(hr)) {
            assert(false && "[ClientStorage] SHGetKnownFolderPath failed");
            // ReSharper disable once CppDFAUnreachableCode
            std::unreachable();
        }

        const FilePath basePath(pszPath);
        CoTaskMemFree(pszPath);

        return basePath / ".." / "Local" / "Packages" / "Microsoft.MinecraftUWP_8wekyb3d8bbwe" / "RoamingState";
    }();

    return path;
}
auto ClientStorage::getClientPath() -> FilePath {
    return getRoamingFolder() / FilePath(CLIENT_NAME);
}