//
// Created by KaeruClient on 2026/05/19.
//

#include "ClientRoot.hpp"
#include <Windows.h>
#include <thread>
#include <chrono>
#include <Engine/BootStrapper.hpp>
#include <Utils/Memory/Address.hpp>

auto ClientRoot::init(const Address baseAddress) -> void {

    std::thread(ClientRoot::mainThread).detach();
}
auto ClientRoot::shutdown() -> void {
}

auto ClientRoot::mainThread() -> void {
    ClientRoot::shutdown();
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