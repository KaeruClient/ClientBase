//
// Created by ikasa on 2026/05/26.
//

#include "FreeLibraryAndExitHook.hpp"

#include "Runtimes/ClientRuntime.hpp"

FreeLibraryAndExitHook::FreeLibraryAndExitHook() {
    createHook<&FreeLibraryAndExitHook::HookFreeLibraryAndExit>(Address(reinterpret_cast<void*>(::FreeLibraryAndExitThread)), oFreeLibary);
}

void FreeLibraryAndExitHook::HookFreeLibraryAndExit(HMODULE hModule, DWORD dwExitCode) {
    if (ClientRuntime::getBaseAddress().mAddress == reinterpret_cast<uintptr_t>(hModule)) {
        ClientRuntime::requestShutdown();
        return;
    }
    return oFreeLibary(hModule, dwExitCode);
}
