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
#include <Runtimes/ClientRuntime.hpp>
#include <Runtimes/GameRuntime.hpp>

#include "SDK/Mappings/Addresses.hpp"

auto ClientRoot::init(const Address baseAddress) -> void {
    ClientRuntime::init(baseAddress);
    GameRuntime::init(Address(hat::process::get_process_module().address()));
    //ExceptionHandler::init();
    Addresses::init();
    // std::thread can be used. This is because we are inside the boot thread here,
    // so we don't need to worry about loader locks.

    std::thread(ClientRoot::mainThread).detach();
}
auto ClientRoot::shutdown() -> void {
    //ExceptionHandler::shutdown();
}

auto ClientRoot::mainThread() -> void {
    ClientRuntime::waitUntilExit();
    ClientRoot::shutdown();
}

BOOL APIENTRY DllMain(
    const HMODULE             baseAddress,
    const DWORD               fdwReason,
    [[ maybe_unused ]] LPVOID lpvReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH) {
        BootStrapper::run(Address(baseAddress), ClientRoot::init);
    }
    return TRUE;
}