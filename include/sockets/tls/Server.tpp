/*
 * Copyright (c) 2026-present Thomas Baricault
 *
 * SPDX-License-Identifier: MIT
 */


#pragma once


#include "Server.hpp"


namespace tbaricault::sockets::tls
{

    template<typename T>
    Server<T>::~Server() noexcept
    {
        SSL_CTX_free(this->_ctx);
        return;
    }

    template<typename T>
    bool Server<T>::setup(const std::string& certificate, const std::string& privateKey) noexcept
    {
        this->_ctx = SSL_CTX_new(TLS_server_method());
        return (
            SSL_CTX_use_certificate_file(this->_ctx, certificate.c_str(), SSL_FILETYPE_PEM) > 0 &&
            SSL_CTX_use_PrivateKey_file(this->_ctx, privateKey.c_str(), SSL_FILETYPE_PEM) > 0 &&
            SSL_CTX_check_private_key(this->_ctx)
        );
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
                if (SSL_is_init_finished(this->_clients.back().get()->_ssl))
                {
                    std::string message;
                    if (!this->_clients.at(i).get()->receive(message))
                    {
                        this->_handleDisconnect(*this->_clients.at(i).get());
                        continue;
                    }
                    else if (!message.empty())
                        this->_handleMessage(*this->_clients.at(i).get(), message);
                }
                else if (!this->_clients.back().get()->accept())
                {
                    this->_handleDisconnect(*this->_clients.at(i).get());
                    continue;
                }
            }
            i++;
        }
        return (true);
    }

    template<typename T>
    bool Server<T>::_accept()
    {
        if (!tbaricault::sockets::tcp::Server<T>::_accept())
            return (false);
        this->_clients.back().get()->setup(this->_ctx);
        return (true);
    }

}
