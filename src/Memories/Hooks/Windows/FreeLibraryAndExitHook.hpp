//
// Created by ikasa on 2026/05/26.
//

#pragma once
#include <Memories/IHook.hpp>
#include <Zycore/Internal/AtomicMSVC.h>

class FreeLibraryAndExitHook : public IHook {
public:
    FreeLibraryAndExitHook();
    ~FreeLibraryAndExitHook() override = default;
private:
    static void HookFreeLibraryAndExit(HMODULE hModule, DWORD dwExitCode);
    static inline decltype(&HookFreeLibraryAndExit) oFreeLibary = nullptr;
};