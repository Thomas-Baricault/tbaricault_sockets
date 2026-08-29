/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#include "Server.hpp"


namespace tbaricault::sockets
{

    template<typename T>
    Server<T>::Server(SocketInfos infos)
        : Socket(infos)
    {
        this->_fds.push_back({this->_handle, POLLIN, 0});
        return;
    }

    template<typename T>
    void Server<T>::setBufferSize(std::size_t value) noexcept
    {
        Socket::setBufferSize(value);
        for (auto& client : this->_clients)
            client.get()->setBufferSize(value);
        return;
    }

    template<typename T>
    bool Server<T>::bind(const Address& addr) noexcept
    {
        if (this->_addr || ::bind(this->_handle, addr.data(), addr.size()) < 0)
            return (false);
        this->_addr = addr;
        return (true);
    }

    template<typename T>
    bool Server<T>::run(int timeout)
    {
#ifdef _WIN32
        int n = WSAPoll(this->_fds.data(), this->_fds.size(), timeout);
#else
        int n = poll(this->_fds.data(), this->_fds.size(), timeout);
#endif
        if (n == 0)
            return (true);
        if (n < 0)
            return (false);
        if (this->_fds.at(0).revents && this->_accept())
        {
            Client& client = *this->_clients.back().get();
            client.setBufferSize(this->_bufferSize);
            client.setBlocking(false);
            this->_handleConnect(client);
        }
        return (true);
    }

    template<typename T>
    void Server<T>::_handleConnect(T&)
    {
        return;
    }

    template<typename T>
    void Server<T>::_handleDisconnect(T& client)
    {
        for (std::size_t i = 0; i < this->_clients.size(); i++)
        {
            if (this->_clients.at(i).get() == &client)
            {
                this->_clients.erase(this->_clients.begin() + i);
                break;
            }
        }
        return;
    }

    template<typename T>
    void Server<T>::_handleMessage(Client&, std::string&)
    {
        return;
    }

}
