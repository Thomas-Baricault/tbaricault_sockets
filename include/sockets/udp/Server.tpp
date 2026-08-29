/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#pragma once


#include "../Address.hpp"
#include "Server.hpp"


namespace tbaricault::sockets::udp
{

    template<typename T>
    Server<T>::Server(AddressFamily family)
        : tbaricault::sockets::Server<T>({
            family,
            SocketType::Datagram,
            Protocol::UDP
        })
        , _interface({this->_handle, {}})
    {
        return;
    }

    template<typename T>
    Server<T>::~Server() noexcept
    {
        this->_interface._handle = Invalid;
        for (auto& client : this->_clients)
            client.get()->_handle = Invalid;
        return;
    }

    template<typename T>
    void Server<T>::setBufferSize(std::size_t value) noexcept
    {
        tbaricault::sockets::Server<T>::setBufferSize(value);
        this->_interface.setBufferSize(this->_bufferSize);
        return;
    }

    template<typename T>
    bool Server<T>::run(int timeout)
    {
        if (!tbaricault::sockets::Server<T>::run(timeout))
            return (false);
        if (!this->_message.empty())
        {
            this->_handleMessage(*this->_clients.back().get(), this->_message);
            this->_message.clear();
        }
        return (true);
    }

    template<typename T>
    bool Server<T>::_accept()
    {
        Address addr;
        if (!this->_interface.receive(addr, this->_message))
            return (false);
        for (auto& client : this->_clients)
        {
            if (client->getAddr() == addr)
            {
                this->_handleMessage(*client.get(), this->_message);
                this->_message.clear();
                return (false);
            }
        }
        this->_clients.push_back(std::make_unique<T>(this->_handle, addr));
        return (true);
    }

}
