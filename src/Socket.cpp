/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#include "sockets/Socket.hpp"


namespace tbaricault::sockets
{

    Socket::Socket(Socket&& other) noexcept
        : _handle(other._handle)
        , _addr(other._addr)
        , _bufferSize(other._bufferSize)
    {
        other._handle = Invalid;
        other._addr = {};
        return;
    }

    Socket::Socket(SocketInfos infos) noexcept
        : _handle(socket(
            static_cast<int>(infos.family),
            static_cast<int>(infos.type),
            static_cast<int>(infos.protocol)
        ))
    {
        return;
    }

    Socket::~Socket()
    {
        this->close();
        return;
    }

    Socket& Socket::operator=(Socket&& other) noexcept
    {
        if (&other == this)
            return (*this);
        this->_handle = other._handle;
        this->_addr = other._addr;
        this->_bufferSize = other._bufferSize;
        other._handle = Invalid;
        other._addr = {};
        return (*this);
    }

    Socket::operator bool() const noexcept
    {
        return (this->_handle != Invalid);
    }

    const Address& Socket::getAddr() const noexcept
    {
        return (this->_addr);
    }

    void Socket::setBufferSize(std::size_t value) noexcept
    {
        this->_bufferSize = value;
        return;
    }

    void Socket::setBlocking(bool value) noexcept
    {
#ifdef _WIN32
        u_long mode = value ? 0 : 1;
        ioctlsocket(this->_handle, FIONBIO, &mode);
#else
        int flags = fcntl(this->_handle, F_GETFL, 0);
        fcntl(
            this->_handle,
            F_SETFL,
            value
                ? (flags & ~O_NONBLOCK)
                : (flags | O_NONBLOCK)
        );
#endif
        return;
    }

    void Socket::close() noexcept
    {
        if (this->_handle != Invalid)
        {
#ifdef _WIN32
            closesocket(this->_handle);
#else
            ::close(this->_handle);
#endif
            this->_handle = Invalid;
        }
        return;
    }

}
