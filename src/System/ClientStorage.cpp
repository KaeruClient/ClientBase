//
// Created by KaeruClient on 2026/05/31.
//

#include "ClientStorage.hpp"
#include <winrt/Windows.Storage.h>
#pragma comment(lib, "runtimeobject")

using namespace winrt::Windows::Storage;
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
    try {
        const auto path = ApplicationData::Current().RoamingFolder().Path();
        return path.c_str();
    } catch ([[maybe_unused]] const winrt::hresult_error& e) {
        assert(false && "[ClientStorage] getRoamingFolder() was an WinRT error occurred");
    } catch ([[maybe_unused]] const std::exception& e) {
        assert(false && "[ClientStorage] getRoamingFolder() was an exception occurred.");
    }
    assert(false && "[ClientStorage] getRoamingFolder() was an unknown error occurred.");
    // ReSharper disable once CppDFAUnreachableCode
    std::unreachable();
}

auto ClientStorage::getClientPath() -> FilePath {
    return getRoamingFolder() / FilePath(CLIENT_NAME);
}