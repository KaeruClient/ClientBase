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

#include "GameThread.hpp"
#include "Config/ConfigManager.hpp"


auto ClientRoot::init(const Address baseAddress) -> void {
    ClientRuntime::setBaseAddress(baseAddress);
    GameRuntime::setBaseAddress(Address(hat::process::get_process_module().address()));
    // ExceptionHandler::init(); マイクラ側の例外ももれずにキャッチしやがるから外しています

    Addresses::initMinimal();
    ClientRuntime::initComponents();
    ClientRuntime::getHookManager().initMinimal();
    GameThread::waitUntilInit();
    ClientRuntime::getConfigManager().setup();
    Addresses::init();
    // std::thread can be used. This is because we are inside the boot thread here,
    // so we don't need to worry about loader locks.

    std::thread(ClientRoot::mainThread, baseAddress).detach();
}
auto ClientRoot::shutdown(const Address baseAddress) -> void {
    //ExceptionHandler::shutdown();

    ::CreateThread(nullptr, 0, [](LPVOID const lpParam) -> DWORD {
        Sleep(100);
        FreeLibraryAndExitThread(static_cast<HMODULE>(lpParam), 0);
    }, reinterpret_cast<LPVOID>(baseAddress.mAddress), 0, nullptr);
}

auto ClientRoot::mainThread(const Address baseAddress) -> void {
    ClientRuntime::waitUntilExit();
    ClientRoot::shutdown(baseAddress);
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