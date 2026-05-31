//
// Created by KaeruClient on 2026/05/31.
//

#include "ClientStorage.hpp"
#include <winrt/Windows.Storage.h>
#pragma comment(lib, "runtimeobject")

using namespace winrt::Windows::Storage;
namespace fs = std::filesystem;
auto ClientStorage::init() -> void {
    mRootPath = getClientPath();
    if (!fs::exists(mRootPath)) {
        fs::create_directories(mRootPath);
    }
}

auto ClientStorage::getPath(const std::string& subPath) const -> FilePath {
    return mRootPath / subPath;
}

auto ClientStorage::getRoamingFolder() -> FilePath {
    try {
        const auto path = ApplicationData::Current().RoamingFolder().Path();
        return FilePath(path.c_str());
    } catch (const winrt::hresult_error& e) {
#ifdef _DEBUG
        assert(false && "[ClientStorage] getRoamingFolder() was an WinRT error occurred");
#endif
    } catch (const std::exception& e) {
#ifdef _DEBUG
        assert(false && "[ClientStorage] getRoamingFolder() was an exception occurred.");
#endif
    }
#ifdef _DEBUG
    assert(false && "[ClientStorage] getRoamingFolder() was an unknown error occurred.");
#endif
    std::unreachable();
}

auto ClientStorage::getClientPath() -> FilePath {
    return getRoamingFolder() / FilePath(CLIENT_NAME);
}