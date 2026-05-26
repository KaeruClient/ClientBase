//
// Created by ikasa on 2026/05/26.
//

#pragma once
#include <Memory/IHook.hpp>
#include <Zycore/Internal/AtomicMSVC.h>

class FreeLibraryAndExitHook : public IHook {
public:
    FreeLibraryAndExitHook();
    ~FreeLibraryAndExitHook() override = default;
private:
    static void HookFreeLibraryAndExit(HMODULE hModule, DWORD dwExitCode);
    static inline decltype(&HookFreeLibraryAndExit) oFreeLibrary = nullptr;
};