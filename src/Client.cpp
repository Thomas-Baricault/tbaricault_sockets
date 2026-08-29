/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#include "sockets/Client.hpp"


namespace tbaricault::sockets
{

    Client::Client(Handle handle, const Address& addr) noexcept
        : Socket()
    {
        this->_handle = handle;
        this->_addr = addr;
        return;
    }

    bool Client::connect(const Address& addr)
    {
        if (this->_addr || ::connect(this->_handle, addr.data(), addr.size()) < 0)
            return (false);
        this->_addr = addr;
        return (true);
    }

    bool Client::send(const std::string& message) const noexcept
    {
        return (::send(this->_handle, message.c_str(), message.size(), 0) >= 0);
    }

    bool Client::receive(std::string& message) const
    {
        message.resize(this->_bufferSize);
        int bytes = recv(this->_handle, message.data(), message.size(), 0);
        if (bytes <= 0)
        {
#ifdef _WIN32
            if (bytes == 0 || WSAGetLastError() != WSAEWOULDBLOCK)
#else
            if (bytes == 0 || (errno != EAGAIN && errno != EWOULDBLOCK))
#endif
                return (false);
            bytes = 0;
        }
        message.resize(bytes);
        return (true);
    }

}
