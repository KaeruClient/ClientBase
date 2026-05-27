//
// Created by KaeruClient on 2026/05/19.
//

#include "ClientRoot.hpp"
#include <Windows.h>
#include <thread>
#include <libhat/process.hpp>

#include <Utils/Memory/Address.hpp>
#include <Engine/BootStrapper.hpp>
#include "ExceptionHandler.hpp"
#include <SDK/Mappings/Addresses.hpp>
#include <Runtimes/ClientRuntime.hpp>
#include <Runtimes/GameRuntime.hpp>

#include <Memory/HookManager.hpp>

auto ClientRoot::init(const Address baseAddress) -> void {
    ClientRuntime::init(baseAddress);
    GameRuntime::init(Address(hat::process::get_process_module().address()));
    // ExceptionHandler::init(); マイクラ側の例外ももれずにキャッチしやがるから外しています
    Addresses::init();
    HookManager::init();
    // std::thread can be used. This is because we are inside the boot thread here,
    // so we don't need to worry about loader locks.

    /*
     こいつが原因でdllのハンドルが削除されないバグが起きている
     std::thread(ClientRoot::mainThread, baseAddress).detach();*/
    CreateThread(
    nullptr,
    0,
    reinterpret_cast<LPTHREAD_START_ROUTINE>(ClientRoot::mainThread),
    reinterpret_cast<LPVOID>(baseAddress.mAddress),
    0,
    nullptr
);
}
auto ClientRoot::shutdown(const Address& baseAddress) -> void {
    HookManager::shutdown();
    //ExceptionHandler::shutdown();

    assert(!HookManager::isInitialized() && "[ClientRoot] Unfortunately, HookManager is alive when unloading.");

    Sleep(100);
    FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(baseAddress.mAddress), 0);
}

auto ClientRoot::mainThread(LPVOID lpAddress) -> void {
    ClientRuntime::waitUntilExit(Address(reinterpret_cast<uintptr_t>(lpAddress)));
    ClientRoot::shutdown(Address(reinterpret_cast<uintptr_t>(lpAddress)));
}

BOOL APIENTRY DllMain(
    const HMODULE             baseAddress,
    const DWORD               fdwReason,
    [[ maybe_unused ]] LPVOID lpvReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
        BootStrapper::run(Address(baseAddress), ClientRoot::init);
    return TRUE;
}