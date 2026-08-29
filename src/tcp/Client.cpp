/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#include "sockets/tcp/Client.hpp"


namespace tbaricault::sockets::tcp
{

    Client::Client(AddressFamily family) noexcept
        : tbaricault::sockets::Client({
            family,
            SocketType::Stream,
            Protocol::TCP
        })
    {
        return;
    }

}
