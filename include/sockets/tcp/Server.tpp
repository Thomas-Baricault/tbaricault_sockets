/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#pragma once


#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif

#include "Server.hpp"


namespace tbaricault::sockets::tcp
{

    template<typename T>
    Server<T>::Server(AddressFamily family)
        : tbaricault::sockets::Server<T>({
            family,
            SocketType::Stream,
            Protocol::TCP
        })
    {
        return;
    }

    template<typename T>
    void Server<T>::setBacklog(int value) noexcept
    {
        this->_backlog = value;
        return;
    }

    template<typename T>
    bool Server<T>::_accept()
    {
        sockaddr_storage storage;
        socklen_t len = sizeof(storage);
        Handle handle = ::accept(this->_handle, (sockaddr*)&storage, &len);
        if (handle == Invalid)
            return (false);
        this->_fds.push_back({handle, POLLIN, 0});
        this->_clients.push_back(std::make_unique<T>(T{handle, storage}));
        return (true);
    }

    template<typename T>
    bool Server<T>::bind(const Address& addr) noexcept
    {
        return (tbaricault::sockets::Server<T>::bind(addr) && ::listen(this->_handle, 5) >= 0);
    }

    template<typename T>
    bool Server<T>::run(int timeout)
    {
        if (!tbaricault::sockets::Server<T>::run(timeout))
            return (false);
        for (std::size_t i = 0; i < this->_clients.size();)
        {
            pollfd fd = this->_fds.at(i + 1);
            if (fd.revents)
            {
                std::string message;
                if (!this->_clients.at(i).get()->receive(message))
                {
                    this->_handleDisconnect(*this->_clients.at(i).get());
                    continue;
                }
                if (!message.empty())
                    this->_handleMessage(*this->_clients.at(i).get(), message);
            }
            i++;
        }
        return (true);
    }

    template<typename T>
    void Server<T>::_handleDisconnect(Client& client)
    {
        for (std::size_t i = 0; i < this->_clients.size(); i++)
        {
            if (this->_clients.at(i).get() == &client)
            {
                this->_fds.erase(this->_fds.begin() + i + 1);
                this->_clients.erase(this->_clients.begin() + i);
                break;
            }
        }
        return;
    }

}
