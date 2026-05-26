//
// Created by ikasa on 2026/05/26.
//

#include "FreeLibraryAndExitHook.hpp"

#include "Runtimes/ClientRuntime.hpp"

FreeLibraryAndExitHook::FreeLibraryAndExitHook() {
    const auto address = reinterpret_cast<void*>(::FreeLibraryAndExitThread);
    createHook<&FreeLibraryAndExitHook::HookFreeLibraryAndExit>(Address(address), oFreeLibrary);
}

void FreeLibraryAndExitHook::HookFreeLibraryAndExit(HMODULE hModule, DWORD dwExitCode) {
    if (ClientRuntime::getBaseAddress().mAddress == reinterpret_cast<uintptr_t>(hModule)) {
        ClientRuntime::requestShutdown();
        return;
    }
    return oFreeLibrary(hModule, dwExitCode);
}
