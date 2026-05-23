//
// Created by KaeruClient on 2026/05/19.
//

#pragma once
#include "Utils/Memory/Address.hpp"

namespace BootStrapper {
    using BootCallback = void(*)(const Address baseAddress);
    void run(const Address baseAddr, BootCallback bootCallback);
}
