/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#include "sockets/udp/Client.hpp"


namespace tbaricault::sockets::udp
{

    Client::Client(AddressFamily family) noexcept
        : tbaricault::sockets::Client({
            family,
            SocketType::Datagram,
            Protocol::UDP
        })
    {
        return;
    }

    bool Client::connect(const Address& addr)
    {
        this->_addr = {};
        return (tbaricault::sockets::Client::connect(addr));
    }

    bool Client::send(const Address& addr, const std::string& message) const noexcept
    {
        if (!addr || (this->_addr && addr != this->_addr))
            return (false);
        return (::sendto(this->_handle, message.c_str(), message.size(), 0, addr.data(), addr.size()) >= 0);
    }

    bool Client::receive(Address& addr, std::string& message) const
    {
        if (this->_addr && addr != this->_addr)
            return (false);
        message.resize(this->_bufferSize);
        sockaddr_storage storage;
        socklen_t len = sizeof(storage);
        int bytes = recvfrom(this->_handle, message.data(), message.size(), 0, reinterpret_cast<sockaddr*>(&storage), &len);
        if (bytes <= 0)
        {
#ifdef _WIN32
            if (bytes == 0 || WSAGetLastError() != WSAEWOULDBLOCK)
#else
            if (bytes == 0 || errno != EAGAIN && errno != EWOULDBLOCK)
#endif
                return (false);
        }
        message.resize(bytes);
        addr = Address(storage);
        return (true);
    }

}
