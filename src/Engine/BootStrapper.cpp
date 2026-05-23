//
// Created by KaeruClient on 2026/05/19.
//

#include "BootStrapper.hpp"

#include <Windows.h>
#include <cassert>

#include <Utils/Memory/Address.hpp>

namespace BootStrapper {
    namespace detail {
        struct ThreadParameters {
            Address baseAddress;
            BootCallback callback{};
        };

        static DWORD WINAPI bootThread(LPVOID lpParameter) {
            assert(lpParameter != nullptr && "[BootStrapper] BootStrapper::bootThread's lpParameter is nullptr!");
            const auto params = static_cast<ThreadParameters*>(lpParameter);

            if (params->callback)
                params->callback(params->baseAddress);

            delete params;
            return 0;
        }
    }


    /**
     * @brief Boots the initialization callback in a separate thread to avoid Loader Lock deadlocks.
     *
     * @param baseAddr The base address of the client module.
     * @param bootCallback The callback function to be executed on the new thread.
     */
    void run(const Address baseAddr, const BootCallback bootCallback) {
        const auto hModule = reinterpret_cast<HMODULE>(baseAddr.mAddress);
        DisableThreadLibraryCalls(hModule);

        // ReSharper disable once CppDFAMemoryLeak
        const auto params = new detail::ThreadParameters{ baseAddr, bootCallback };

        /*
         * Executed under Loader Lock within DllMain.
         * Using ::CreateThread instead of std::thread to prevent deadlocks.
         */
        const auto hThread = ::CreateThread(
            nullptr,
            0,
            reinterpret_cast<LPTHREAD_START_ROUTINE>(detail::bootThread),
            reinterpret_cast<LPVOID>(params),
            0,
            nullptr
        );

        if (hThread) {
            ::CloseHandle(hThread);
            // ReSharper disable once CppDFAMemoryLeak
            return;
        }
        delete params;
    }
}
