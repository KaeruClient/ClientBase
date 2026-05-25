//
// Created by KaeruClient on 2026/05/19.
//

#pragma once
#include "../Utils/Memory/Address.hpp"

class ClientRoot final {
public:
    ClientRoot() = delete;
    static auto init(const Address baseAddress) -> void;
private:
    static auto shutdown(const Address& ) -> void;
    static auto mainThread(const Address& baseAddress) -> void;
};
