//
// Created by KaeruClient on 2026/05/19.
//

#include "BootStrapper.hpp"

#include <Windows.h>
#include <cassert>
#include <memory>

#include <Utils/Memory/Address.hpp>

namespace BootStrapper {
    namespace detail {
        struct ThreadParameters {
            Address baseAddress;
            BootCallback callback{};
        };

        static auto WINAPI bootThread(const LPVOID lpParameter) -> DWORD {
            assert(lpParameter != nullptr && "[BootStrapper] BootStrapper::bootThread's lpParameter is nullptr!");
            const std::unique_ptr<ThreadParameters> params(static_cast<ThreadParameters*>(lpParameter));
            if (params->callback)
                params->callback(params->baseAddress);

            return 0;
        }
    }


    /**
     * @brief Boots the initialization callback in a separate thread to avoid Loader Lock deadlocks.
     *
     * @param baseAddr The base address of the client module.
     * @param bootCallback The callback function to be executed on the new thread.
     */
    auto run(const Address &baseAddr, const BootCallback &bootCallback) -> void {
        const auto hModule = reinterpret_cast<HMODULE>(baseAddr.mAddress);
        DisableThreadLibraryCalls(hModule);

        auto params = std::make_unique<detail::ThreadParameters>(baseAddr, bootCallback);
        /*
         * Executed under Loader Lock within DllMain.
         * Using ::CreateThread instead of std::thread to prevent deadlocks.
         */
        auto* rawParams = params.release();
        const auto hThread = ::CreateThread(
            nullptr,
            0,
            reinterpret_cast<LPTHREAD_START_ROUTINE>(detail::bootThread),
            reinterpret_cast<LPVOID>(rawParams),
            0,
            nullptr
        );

        if (hThread) {
            ::CloseHandle(hThread);
            return;
        }
        delete rawParams;
        assert(false && "[BootStrapper] Failed to create thread!");
    }
}
